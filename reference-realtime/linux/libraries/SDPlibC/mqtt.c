#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>

/* For version 3 of the MQTT protocol */
#include "mqtt.h"
#include "cJSON.h"

#define PROTOCOL_NAME       "MQIsdp"    // 
#define PROTOCOL_VERSION    3U          // version 3.0 of MQTT
#define CLEAN_SESSION       (1U<<1)
#define KEEPALIVE           30U         // specified in seconds
#define MESSAGE_ID          1U // not used by QoS 0 - value must be > 0

/* Macros for accessing the MSB and LSB of a uint16_t */
#define MSB(A) ((uint8_t)((A & 0xFF00) >> 8))
#define LSB(A) ((uint8_t)(A & 0x00FF))


#define SET_MESSAGE(M) ((uint8_t)(M << 4))
#define GET_MESSAGE(M) ((uint8_t)(M >> 4))

typedef enum {
    CONNECT = 1,
    CONNACK,
    PUBLISH,
    PUBACK,
    PUBREC,
    PUBREL,
    PUBCOMP,
    SUBSCRIBE,
    SUBACK,
    UNSUBSCRIBE,
    UNSUBACK,
    PINGREQ,
    PINGRESP,
    DISCONNECT 
} connect_msg_t;

typedef enum {
    Connection_Accepted,
    Connection_Refused_unacceptable_protocol_version,
    Connection_Refused_identifier_rejected,
    Connection_Refused_server_unavailable,
    Connection_Refused_bad_username_or_password,
    Connection_Refused_not_authorized
} connack_msg_t;


struct mqtt_broker_handle
{
	int                 socket;
	struct sockaddr_in  socket_address;
	uint16_t            port;
	char                hostname[16];  // based on xxx.xxx.xxx.xxx format
	char                clientid[24];  // max 23 charaters long
	bool                connected;
    size_t              topic;
    uint16_t            pubMsgID;
    uint16_t            subMsgID;
};

typedef struct fixed_header_t
{
    uint16_t     retain             : 1;
    uint16_t     Qos                : 2;
    uint16_t     DUP                : 1;
    uint16_t     connect_msg_t      : 4;
    uint16_t     remaining_length   : 8;    
}fixed_header_t;



mqtt_broker_handle_t * mqtt_connect(const char* client, const char * server_ip, uint32_t port)
{
    mqtt_broker_handle_t *broker = (mqtt_broker_handle_t *)calloc(sizeof(mqtt_broker_handle_t), 1) ;
    if(broker != 0) {
        // check connection strings are within bounds
        if ( (strlen(client)+1 > sizeof(broker->clientid)) || (strlen(server_ip)+1 > sizeof(broker->hostname))) {
            fprintf(stderr,"failed to connect: client or server exceed limits\n");
            free(broker);
            return 0;  // strings too large
        }
        
        broker->port = port;
        strcpy(broker->hostname, server_ip);
        strcpy(broker->clientid, client);
        
        if ((broker->socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            fprintf(stderr,"failed to connect: could not create socket\n");
            free(broker);            
            return 0;
        }
        
        // create the stuff we need to connect
        broker->connected = false;
        broker->socket_address.sin_family = AF_INET;
        broker->socket_address.sin_port = htons(broker->port); // converts the unsigned short from host byte order to network byte order
        broker->socket_address.sin_addr.s_addr = inet_addr(broker->hostname);
        
        // connect
        if ((connect(broker->socket, (struct sockaddr *)&broker->socket_address, sizeof(broker->socket_address))) < 0) {
            fprintf(stderr,"failed to connect: to server socket\n");
            free(broker);
            return 0;
        }
        
        // variable header
        uint8_t var_header[] = {
            0,                         // MSB(strlen(PROTOCOL_NAME)) but 0 as messages must be < 127
            strlen(PROTOCOL_NAME),     // LSB(strlen(PROTOCOL_NAME)) is redundant 
            'M','Q','I','s','d','p',
            PROTOCOL_VERSION,
            CLEAN_SESSION,
            MSB(KEEPALIVE),
            LSB(KEEPALIVE)
        };

        // set up payload for connect message
        uint8_t payload[2+strlen(broker->clientid)];
        payload[0] = 0;
        payload[1] = strlen(broker->clientid);
        memcpy(&payload[2],broker->clientid,strlen(broker->clientid));

        // fixed header: 2 bytes, big endian
        uint8_t fixed_header[] = { SET_MESSAGE(CONNECT), sizeof(var_header)+sizeof(payload) };
//      fixed_header_t  fixed_header = { .QoS = 0, .connect_msg_t = CONNECT, .remaining_length = sizeof(var_header)+strlen(broker->clientid) };
        
        uint8_t packet[sizeof(fixed_header)+sizeof(var_header)+sizeof(payload)];
        
        memset(packet,0,sizeof(packet));
        memcpy(packet,fixed_header,sizeof(fixed_header));
        memcpy(packet+sizeof(fixed_header),var_header,sizeof(var_header));
        memcpy(packet+sizeof(fixed_header)+sizeof(var_header),payload,sizeof(payload));
        
        // send Connect message
        if (send(broker->socket, packet, sizeof(packet), 0) < sizeof(packet)) {
            close(broker->socket);
            free(broker);
            return 0;
        }
        
        uint8_t buffer[4];
        long sz = recv(broker->socket, buffer, sizeof(buffer), 0);  // wait for CONNACK
        //        printf("buffer size is %ld\n",sz);
        //        printf("%2x:%2x:%2x:%2x\n",(uint8_t)buffer[0],(uint8_t)buffer[1],(uint8_t)buffer[2],(uint8_t)buffer[3]);
        if( (GET_MESSAGE(buffer[0]) == CONNACK) && ((sz-2)==buffer[1]) && (buffer[3] == Connection_Accepted) ) {
            printf("Connected to MQTT Server at %s:%4d\n",server_ip, port );
        }
        else
        {
            fprintf(stderr,"failed to connect with error: %d\n", buffer[3]);
            close(broker->socket);
            free(broker);
            return 0;
        }
        
        // set connected flag
        broker->connected = true;
        
    }
	
	return broker;
}




int mqtt_subscribe(mqtt_broker_handle_t *broker, const char *topic, QoS qos)
{
	if (!broker->connected) {
        return -1;
	}
	
	uint8_t var_header[] = { MSB(MESSAGE_ID), LSB(MESSAGE_ID) };	// appended to end of PUBLISH message
    
	// utf topic
	uint8_t utf_topic[2+strlen(topic)+1]; // 2 for message size + 1 for QoS
    
    // set up topic payload
	utf_topic[0] = 0;                       // MSB(strlen(topic));
	utf_topic[1] = LSB(strlen(topic));
    memcpy((char *)&utf_topic[2], topic, strlen(topic));
	utf_topic[sizeof(utf_topic)-1] = qos; 
    
    uint8_t fixed_header[] = { SET_MESSAGE(SUBSCRIBE), sizeof(var_header)+sizeof(utf_topic)};
//    fixed_header_t  fixed_header = { .QoS = 0, .connect_msg_t = SUBSCRIBE, .remaining_length = sizeof(var_header)+strlen(utf_topic) };
	
	uint8_t packet[sizeof(fixed_header)+sizeof(var_header)+sizeof(utf_topic)];
    
	memset(packet, 0, sizeof(packet));
	memcpy(packet, &fixed_header, sizeof(fixed_header));
	memcpy(packet+sizeof(fixed_header), var_header, sizeof(var_header));
	memcpy(packet+sizeof(fixed_header)+sizeof(var_header), utf_topic, sizeof(utf_topic));
	
	if (send(broker->socket, packet, sizeof(packet), 0) < sizeof(packet)) {
        puts("failed to send subscribe message");
		return -1;
    }
    
    uint8_t buffer[5];
    long sz = recv(broker->socket, buffer, sizeof(buffer), 0);  // wait for SUBACK
    
        printf("buffer size is %ld\n",sz);
        printf("%2x:%2x:%2x:%2x:%2x\n",(uint8_t)buffer[0],(uint8_t)buffer[1],(uint8_t)buffer[2],(uint8_t)buffer[3],(uint8_t)buffer[4]);
    
    if((GET_MESSAGE(buffer[0]) == SUBACK) && ((sz-2) == buffer[1])&&(buffer[2] == MSB(MESSAGE_ID)) &&  (buffer[3] == LSB(MESSAGE_ID)) ) {
        printf("Subscribed to MQTT Service %s with QoS %d\n", topic, buffer[4]);
    }
    else
    {
        puts("failed to subscribe");
        return -1;
    }
    broker->topic = strlen(topic);
    struct timeval tv;
    
    tv.tv_sec = 30;  /* 30 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    
    setsockopt(broker->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	return 1;
}

int SetSocketTimeout(int connectSocket, int milliseconds)
{
    struct timeval tv;

    tv.tv_sec = milliseconds / 1000 ;
    tv.tv_usec = ( milliseconds % 1000) * 1000  ;

    return setsockopt (connectSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof tv);
}


int mqtt_publish(mqtt_broker_handle_t *broker, const char *topic, const char *msg, QoS qos)
{
	if (!broker->connected) {
        return -1;
	}
    if(qos > QoS2) {
        return -1;
    }
	
    if (qos == QoS0) {  
        // utf topic
      
        uint8_t utf_topic[2+strlen(topic)]; // 2 for message size QoS-0 does not have msg ID
        //const char* msg2 = "{\"version\": \"1.1c\", \"url\": \"http://<194.116.5.164/data.zip\",\"hash\": \"3ea4c8f57b985435d2f2ca9b5ff99e8a\"}";//, \"Size\": \"101310\",\"Time\": \"2014-05-12T06:27:36Z\", \"Priority\": \"Critical\", \"Requirement\": \"1.0\"}";
        // set up topic payload
	cJSON *root;	/* declare a few. */

	/* Here we construct some JSON standards, from the JSON site. */

	root=cJSON_CreateObject();	
	cJSON_AddItemToObject(root, "version", cJSON_CreateString("1.1c"));
	cJSON_AddItemToObject(root, "url", cJSON_CreateString("http://194.116.5.164/data.txt"));
	cJSON_AddItemToObject(root, "hash", cJSON_CreateString("3ea4c8f57b985435d2f2ca9b5ff99e8a"));
	const char* msg2 = cJSON_Print(root);
        if(strlen(msg2)<111){

	
	}else{

	}
        
	  printf("messaggio[%d] minore max\n",strlen(msg2));
	  
	  //uint8_t utf_topic[2+strlen(topic)]; 
	  utf_topic[0] =  MSB(strlen(topic));
	  utf_topic[1] = LSB(strlen(topic));	  
	  memcpy((char *)&utf_topic[2], topic, strlen(topic));
	  uint8_t fixed_header[] = { SET_MESSAGE(PUBLISH)|(qos << 1), sizeof(utf_topic)+strlen(msg2)};    
	  uint8_t packet[sizeof(fixed_header)+sizeof(utf_topic)+strlen(msg2)];
          memset(packet, 0, sizeof(packet));
	  int ic=0;
	  printf("lunghezza di fixheader=%d e lunghezza di topic=%d\n",sizeof(fixed_header),sizeof(utf_topic));
	  for(ic=0;ic<sizeof(fixed_header);ic++)
	    printf("%c",fixed_header[ic]);
	  printf("\n");
	  for(ic=0;ic<sizeof(utf_topic);ic++)
	    printf("%c",utf_topic[ic]);
	  printf("\n");
	  memcpy(packet, &fixed_header, sizeof(fixed_header));
	  memcpy(packet+sizeof(fixed_header), utf_topic, sizeof(utf_topic));
	  memcpy(packet+sizeof(fixed_header)+sizeof(utf_topic), msg2, strlen(msg2));

	
	//packet[1]=0x7E;	  
//  	for(ic=0;ic<sizeof(packet);ic++)
//  	    printf("%c",packet[ic]);
// 	printf("\n\n\n\n");
// 	
// 	char pakmsg[]="0%input/smartlabasdfghjklpoiuytrewasdfghjklpoiuytrewasdfghjklpoiuytrewasdfghjklpoiuytrewasdfghjklpoiuytrewasdfghjklpoiuytrew25ss\0";
// 	uint8_t pak[strlen(pakmsg)];
// 	memset(pak, 0, sizeof(pak));
// 	memcpy(pak, pakmsg, strlen(pakmsg));
	
	for(ic=0;ic<sizeof(packet);ic++)
	    printf("%c",packet[ic]);	
	printf("\nlunghezza messaggio=%d\n",sizeof(utf_topic)+strlen(msg2));
        if (send(broker->socket, packet, sizeof(packet), 0) < sizeof(packet)) {
	      return -1;
        } 
	sleep(1);	        
	
// 	for(ic=0;ic<sizeof(pak);ic++)
// 	    printf("%c",pak[ic]);	
// 	printf("\nlunghezza messaggio=%d\n",strlen(pak));
//         if (send(broker->socket, pak, sizeof(pak), 0) < sizeof(pak)) {
// 	      return -1;
//         } 
// 	sleep(1);	


	//int numSend=send(broker->socket, packet, sizeof(packet), 0);
	//printf("\n\nmsg:\n%s\n lunghezza=%d\n",msg,numSend);        
	//if(numSend<sizeof(packet))
	//      return -1;
    }
    else {
        broker->pubMsgID++;
        // utf topic
        uint8_t utf_topic[2+strlen(topic)+2]; // 2 extra for message size > QoS0 for msg ID
        
        // set up topic payload
        utf_topic[0] = 0;                       // MSB(strlen(topic));
        utf_topic[1] = LSB(strlen(topic));
        memcpy((char *)&utf_topic[2], topic, strlen(topic));
        utf_topic[sizeof(utf_topic)-2] = 0;//MSB(broker->pubMsgID);
        utf_topic[sizeof(utf_topic)-1] = LSB(broker->pubMsgID);
        
        uint8_t fixed_header[] = { SET_MESSAGE(PUBLISH)|(qos << 1), sizeof(utf_topic)+strlen(msg)};
        
        uint8_t packet[sizeof(fixed_header)+sizeof(utf_topic)+strlen(msg)];
        
        memset(packet, 0, sizeof(packet));
        memcpy(packet, &fixed_header, sizeof(fixed_header));
        memcpy(packet+sizeof(fixed_header), utf_topic, sizeof(utf_topic));
        memcpy(packet+sizeof(fixed_header)+sizeof(utf_topic), msg, strlen(msg));
        
	printf("msg:\n%s\n lungo=%d\n",msg,strlen(msg));
	int ic=0;
		for(ic=0;ic<sizeof(packet);ic++)
	    printf("%c",packet[ic]);
	
	packet[1]=0x7E;
	
	printf("\n");
        if (send(broker->socket, packet, sizeof(packet), 0) < sizeof(packet)) {
            return -1;
        }
        if(qos == QoS1){
            // expect PUBACK with MessageID
            uint8_t buffer[4];
            long sz = recv(broker->socket, buffer, sizeof(buffer), 0);  // wait for SUBACK
            
            //    printf("buffer size is %ld\n",sz);
            //    printf("%2x:%2x:%2x:%2x:%2x\n",(uint8_t)buffer[0],(uint8_t)buffer[1],(uint8_t)buffer[2],(uint8_t)buffer[3],(uint8_t)buffer[4]);
            
            if((GET_MESSAGE(buffer[0]) == PUBACK) && ((sz-2) == buffer[1]) && (buffer[2] == MSB(broker->pubMsgID)) &&  (buffer[3] == LSB(broker->pubMsgID)) ) {
                printf("Published to MQTT Service %s with QoS1\n", topic);
            }
            else
            {
                puts("failed to publisg at QoS1");
                return -1;
            }
        }
        
    }
    
	return 1;
}

void mqtt_disconnect(mqtt_broker_handle_t *broker)
{
    uint8_t fixed_header[] = { SET_MESSAGE(DISCONNECT), 0};
    if (send(broker->socket, fixed_header, sizeof(fixed_header), 0)< sizeof(fixed_header)) {
        puts("failed to disconnect");
    }
}

SenderMqtt newSenderMqtt(char* _ip, char* _port, char* _client, char* _topic)
{
  SenderMqtt senderMqtt;
  senderMqtt.ip = _ip;
  senderMqtt.port = _port;
  senderMqtt.client = _client;
  senderMqtt.topic = _topic;
  return senderMqtt;
}

int sendMessageMqtt(SenderMqtt sender, char *message)
{
  mqtt_broker_handle_t *broker = mqtt_connect(sender.client, sender.ip, 1883);

    
    if(broker == 0) {
        puts("Failed to connect");
        exit(1);
    }
 
    //char msg[10000];
    int  i = 0;
    for(i = 1; i <= 1; ++i) {
       // sprintf(msg, message);

        if(mqtt_publish(broker, sender.topic, message, QoS0) == -1) {
            printf("publish failed\n");
        }
        else {
            printf("Sent messages\n");
        }
        sleep(1);
    }
    mqtt_disconnect(broker);
}

void mqtt_display_message(mqtt_broker_handle_t *broker, int (*print)(int))
{
    uint8_t buffer[512];
    char bufferJson[512];
    SetSocketTimeout(broker->socket, 30000);
    memset(bufferJson,0,512);
    while(1) {
        // wait for next message
        long sz = recv(broker->socket, buffer, sizeof(buffer), 0);
        //printf("message size is %ld\n",sz);        
        // if more than ack - i.e. data > 0
        if (sz == 0)
        {

           printf("\nSocket EOF\n");

           close(broker->socket);
           broker->socket = 0;
           return;
        }

        if(sz < 0)
        {
           printf("\nSocket recv returned %ld, errno %d %s\n",sz,errno, strerror(errno));

           close(broker->socket); 
           broker->socket = 0;
           exit(0);
        }

        if(sz > 0) {
            //printf("message size is %ld\n",sz);
            if( GET_MESSAGE(buffer[0]) == PUBLISH) {                
                //printf("Got PUBLISH message with size %d\n", (uint8_t)buffer[1]);
                uint32_t topicSize = (buffer[2]<<8) + buffer[3];
                //printf("topic size is %d\n", topicSize);
                //for(int loop = 0; loop < topicSize; ++loop) {
                //    putchar(buffer[4+loop]);
                //}
                //putchar('\n');
                unsigned long i = 4+topicSize;
                if (((buffer[0]>>1) & 0x03) > QoS0) {
                    uint32_t messageId = (buffer[4+topicSize] << 8) + buffer[4+topicSize+1];
                    //printf("Message ID is %d\n", messageId);
                    i += 2; // 2 extra for msgID
                    // if QoS1 the send PUBACK with message ID
                    uint8_t puback[4] = { SET_MESSAGE(PUBACK), 2, buffer[4+topicSize], buffer[4+topicSize+1] };
                    if (send(broker->socket, puback, sizeof(puback), 0) < sizeof(puback)) {
                        puts("failed to PUBACK");
                        return;
                    }
                }
                int l=0;
                for ( ; i < sz; ++i) { 
		    
                    print(buffer[i]);
		    bufferJson[l] = buffer[i];
		    l++;
		    
                }
                printf("\n");
		printf("\n");
		printf("\n");
		
		//for (l=0;l<50;l++)
		//  sprintf(bufferJson[l], "%02x", buffer[l]);
		
		  printf("MESSAGGIO:\n%s",bufferJson);
		printf("\n");
		parseJsonMessage(bufferJson);
               // return;
            }
        }
        usleep(500);
    }
    //return buffer;
}

void parseJsonMessage(char *bufferJson)
{
    //printf("stampa il valore del bufferJson=%s\n ",bufferJson);
    	char *out;
	cJSON *json;
	cJSON *jsonObj;
	
	json=cJSON_Parse(bufferJson);
	if (!json) 
	  {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		jsonObj= cJSON_GetObjectItem( json, "url" );
		out=cJSON_Print(jsonObj);
		printf("valore oggetto %s\n",out);
		//free(out);
	}
    char sendline[1000], recvline[1000];
char* ptr;

size_t n;
char *OutResponse;
/// Form request http://194.116.5.164/data.zip
// curl -i -H "Accept: application/json" -H "Content-Type: application/json" -X GET http://127.0.0.1/data.txt
snprintf(sendline, 10000, 
     "GET http://194.116.5.164/data.txt HTTP/1.0\r\n"  // POST or GET, both tested and works. Both HTTP 1.0 HTTP 1.1 works, but sometimes 
     //"Host: http://194.116.5.164\r\n"     // but sometimes HTTP 1.0 works better in localhost type
     "Content-type: application/json\r\n"
     "Accept: application/json\r\n"
     "Content-Length: 200");
    /* "Content-length: %d\r\n\r\n"
     "%s\r\n"*/ /*,page, host,/* (unsigned int)strlen(poststr), poststr);*/
    int confd;
    char cout;
    struct sockaddr_in servaddr;
    int sockfd = 0;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
      printf("Errore nell'invio del messaggio. Socket non aperto\n");
      return;
 }
    else
        printf(" Socket aperto ora provoi ad inviare messaggio:%s\n",sendline);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi("80"));
    inet_pton(AF_INET, "194.116.5.164", &servaddr.sin_addr);    
    

    confd = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if(confd<0)
      printf("Errore nell'invio del messaggio. Connessione non stabilita\n");      

/// Write the request
if (write(sockfd, sendline, strlen(sendline))>= 0) 
{
printf("nessun errore nella scrittura\n");
    /// Read the response
    while ((n = read(sockfd, recvline, 100000)) > 0) 
    {
        recvline[n] = '\0';

        if(fputs(recvline,stdout) == EOF) { printf("fputs erros"); }
        /// Remove the trailing chars
        ptr = strstr(recvline, "\r\n\r\n");

        // check len for OutResponse here ?
        snprintf(OutResponse, 10,"%s", ptr);
    }   
 
    printf("valore della risposta=%s\n",OutResponse);
}
else
  printf("errore nella scrittura\n");


printf("carico il firmware\n");

char command[100];
memset(command,0,100);
strcat(command,"avrdude -p m2560 -P /dev/ttyACM0 -c stk500v2 -b 115200 -U flash:w:");
char *substr = malloc (20*sizeof(char));
memset(substr,0,20);
substr = strndup(out+22, strlen(out)-1);//substring(out, 23, strlen(out)-2);

strcat(command,substr);
printf("valore del comando:%s strlen=%d\n",substr,strlen(out));
free(out);

}


