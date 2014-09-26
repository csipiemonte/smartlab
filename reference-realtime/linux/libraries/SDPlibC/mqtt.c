#include "mqtt.h"

//struct and callback for the message
message__state_t state_message;
message_state_cb_t state_callback_message;

SenderMqtt newSenderMqtt(char* _ip, char* _port, char* _client, char* _topic)
{
        SenderMqtt senderMqtt;
        senderMqtt.ip = _ip;
        senderMqtt.port = _port;
        senderMqtt.client = _client;
        senderMqtt.topic = _topic;
        return senderMqtt;
}

mqtt_broker_handle_t * mqtt_connect(const char* client, const char * server_ip, uint32_t port, char* userName, char* password)
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
            printf("\n\n");

            if ((broker->socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
               printf("failed to connect: could not create socket\n");
               free(broker);            
               return 0;
           }
        
        // create the stuff we need to connect
           broker->connected = false;
           broker->socket_address.sin_family = AF_INET;
           broker->socket_address.sin_port = htons(broker->port); // converts the unsigned short from host byte order to network byte order
           broker->socket_address.sin_addr.s_addr = inet_addr(broker->hostname);
   
           if ((connect(broker->socket, (struct sockaddr *)&broker->socket_address, sizeof(broker->socket_address))) < 0) {
               fprintf(stderr,"failed to connect: to server socket\n");
               free(broker);
               return 0;
           }
   
           uint8_t var_header[] = {
               MSB(strlen(PROTOCOL_NAME)),// but 0 as messages must be < 127
               strlen(PROTOCOL_NAME),     // LSB(strlen(PROTOCOL_NAME)) is redundant 
               'M','Q','I','s','d','p',
               PROTOCOL_VERSION,
               CLEAN_SESSION,
               MSB(KEEPALIVE),
               LSB(KEEPALIVE)
           };
            memset(broker->clientid,0,sizeof(broker->clientid));
            broker->clientid[0]=0x00;
            broker->clientid[1]=strlen(client);
            strcat(broker->clientid+2, client);

            broker->clientid[strlen(client)+2]=0x00;
            broker->clientid[strlen(client)+3]=strlen(userName);
            strcat(broker->clientid+4+strlen(client),userName);


            broker->clientid[strlen(client)+strlen(userName)+4]=0x00;
            broker->clientid[strlen(client)+strlen(userName)+5]=strlen(password);
            strcat(broker->clientid+6+strlen(client)+strlen(userName),password);

            uint8_t payload[2+strlen(broker->clientid)];
            payload[0] = 0;
            payload[1] = strlen(broker->clientid);//+strlen(userName)+strlen(password)-1;
	
            memcpy(&payload[2],broker->clientid,strlen(broker->clientid));
            uint8_t fixed_header[] = { SET_MESSAGE(CONNECT), sizeof(var_header)+sizeof(payload) };
            uint8_t packet[sizeof(fixed_header)+sizeof(var_header)+sizeof(broker->clientid)];
            memset(packet,0,sizeof(packet));
            memcpy(packet,fixed_header,sizeof(fixed_header));
            memcpy(packet+sizeof(fixed_header),var_header,sizeof(var_header));
            memcpy(packet+sizeof(fixed_header)+sizeof(var_header),broker->clientid,30);
       
	   
	    printf("lunghezza client=%s =>%d\n",client, strlen(client));
	    printf("lunghezza username=%s =>%d\n",userName,strlen(userName));
	    printf("lunghezza password=%s =>%d\n",password,strlen(password));
	    printf("lunghezza broker->clientid=%s =>%d\n",broker->clientid,sizeof(broker->clientid));
            packet[1]=0x2a;//strlen(client)+strlen(userName)+strlen(password)+26;
            packet[11]=0xc2;
            packet[12]=0x00;
            packet[13]=0x3c;

	    int ic;
	    for(ic=0;ic<sizeof(packet);ic++)
	        printf("%2x",packet[ic]);
	    printf("\n");	
	    for(ic=0;ic<sizeof(packet);ic++)
	        printf("%c",packet[ic]);
	    printf("\n");	    
	    
        // send Connect message
            if (send(broker->socket, packet, sizeof(packet), 0) < sizeof(packet)) {
                close(broker->socket);
                free(broker);
                return 0;
            }
            else
                printf("connesso\n\n\n");
        
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


        printf("valore del topic=%s lungo=%d\n",topic,strlen(topic));

        uint8_t utf_topic[2+strlen(topic)+1]; // 2 for message size + 1 for QoS
    
        // set up topic payload
        utf_topic[0] = 0;                       // MSB(strlen(topic));
        utf_topic[1] = LSB(strlen(topic));
        memcpy((char *)&utf_topic[2], topic, strlen(topic));
        utf_topic[sizeof(utf_topic)-1] = qos; 

        uint8_t fixed_header[] = { SET_MESSAGE(SUBSCRIBE), sizeof(var_header)+sizeof(utf_topic)};
//      fixed_header_t  fixed_header = { .QoS = 0, .connect_msg_t = SUBSCRIBE, .remaining_length = sizeof(var_header)+strlen(utf_topic) };

        uint8_t packet[sizeof(fixed_header)+sizeof(var_header)+sizeof(utf_topic)];
    
        memset(packet, 0, sizeof(packet));
        memcpy(packet, &fixed_header, sizeof(fixed_header));
        memcpy(packet+sizeof(fixed_header), var_header, sizeof(var_header));
        memcpy(packet+sizeof(fixed_header)+sizeof(var_header), utf_topic, sizeof(utf_topic));
        int ic=0;
        packet[0]=0x82;

        printf("\n");
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
   // broker->topic = strlen(topic);
        //fprintf("valore del topic=%s",broker->topic);
        struct timeval tv;
        tv.tv_sec = 30;  
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
      
            uint8_t utf_topic[2+strlen(topic)]; // original 2+ 2 for message size QoS-0 does not have msg ID
   
            //printf("messaggio[%d] minore max\n",strlen(msg));
            if(strlen(msg)<128){
                utf_topic[0] = 0x00;// MSB(strlen(topic));
                utf_topic[1] = LSB(strlen(topic));
            }else{
                utf_topic[0] = 0x00;// MSB(strlen(topic));
                utf_topic[1] = strlen(topic);
            }

            memcpy((char *)&utf_topic[2], topic, strlen(topic));

            uint8_t fixed_header[] = { SET_MESSAGE(PUBLISH)|(qos << 1), sizeof(utf_topic)+strlen(msg)};    
            uint8_t fixed_header_long[] = { SET_MESSAGE(PUBLISH)|(qos << 1), sizeof(utf_topic)+strlen(msg), 0x01}; //indica la lunghezza del messaggio   
	    //uint8_t fixed_header_long2[] = { SET_MESSAGE(PUBLISH)|(qos << 1), sizeof(utf_topic)+strlen(msg), 0x01};
            uint8_t packet[sizeof(fixed_header)+sizeof(utf_topic)+strlen(msg)+2];
            memset(packet, 0, sizeof(packet));

            int ic=0;

            //printf("lunghezza di fixheader=%d e lunghezza di topic=%d\n",sizeof(fixed_header),sizeof(utf_topic));
 /*           for(ic=0;ic<strlen(msg);ic++)
                  printf("%2x",msg[ic]);
            printf("\n");
            for(ic=0;ic<sizeof(utf_topic);ic++)
                  printf("%2x",utf_topic[ic]);
            printf("\n");
*/
            if(strlen(msg)<128){
                memcpy(packet, &fixed_header, sizeof(fixed_header));
                memcpy(packet+sizeof(fixed_header), utf_topic, sizeof(utf_topic));
                memcpy(packet+sizeof(fixed_header)+sizeof(utf_topic), msg, strlen(msg));
            }if(strlen(msg)>256){
                memcpy(packet, &fixed_header_long, sizeof(fixed_header_long));
                memcpy(packet+sizeof(fixed_header_long), utf_topic, sizeof(utf_topic));
                memcpy(packet+sizeof(fixed_header_long)+sizeof(utf_topic), msg, strlen(msg));	    
                packet[1]=((strlen(msg)+strlen(topic)+2)%128)|0x80;//0x91;//strlen(msg)+strlen(topic)+3;//x and ff
		packet[2]=(strlen(msg)+strlen(topic)+2)/128;//0x03;//x and ff
                packet[4]=strlen(topic);
            }else{
                memcpy(packet, &fixed_header_long, sizeof(fixed_header_long));
                memcpy(packet+sizeof(fixed_header_long), utf_topic, sizeof(utf_topic));
                memcpy(packet+sizeof(fixed_header_long)+sizeof(utf_topic), msg, strlen(msg));	    
                packet[1]=strlen(msg)+strlen(topic)+3;
                packet[4]=strlen(topic);
            }
           /* printf("PACKETPACKET lungo=%d:\n",strlen(msg)+strlen(topic)+3);
            for(ic=0;ic<sizeof(packet);ic++)
                  printf("%02x",packet[ic]);*/
            printf("\n");
            if (send(broker->socket, packet, sizeof(packet), 0) < sizeof(packet)) {
                return -1;
            } 
            sleep(1);	        
        }
        else {/*//QoSo==1
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
                else{
                    puts("failed to publisg at QoS1");
                    return -1;
                }
            } 
        */}
        return 1;
}

//sistemare da qui in poi i tab con 4 spazi
void mqtt_disconnect(mqtt_broker_handle_t *broker)
{
        uint8_t fixed_header[] = { SET_MESSAGE(DISCONNECT), 0};
        if (send(broker->socket, fixed_header, sizeof(fixed_header), 0)< sizeof(fixed_header)) {
            puts("failed to disconnect");
        }
}



int sendMessageMqtt(SenderMqtt sender, char *message, char* userName, char* password)
{
        mqtt_broker_handle_t *broker = mqtt_connect(sender.client, sender.ip, 1883, userName, password);

    
        if(broker == 0) {
            puts("Failed to connect");
            exit(1);
        }
 
        //char msg[10000];
        int  i = 0;
        for(i = 1; i <= 1; ++i) {
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
    
int message_register_state_cb( message_state_cb_t cb){

        state_callback_message=cb;//(&state_nfc);
        //printf("richiamata callback\n");
}

void mqtt_display_message(mqtt_broker_handle_t *broker, int (*print)(int))
{
        uint8_t buffer[512];
        char bufferJson[512];
        //printf("valore del topic=%s\n",broker->topic);
        SetSocketTimeout(broker->socket, 30000);
        memset(bufferJson,0,512);
        while(1) {
            // wait for next message
            long sz = recv(broker->socket, buffer, sizeof(buffer), 0);
            if (sz == 0){
                printf("\nSocket EOF\n");
                close(broker->socket);
                broker->socket = 0;
                return;
            }

            if(sz < 0){
                printf("\nSocket recv returned %ld, errno %d %s\n",sz,errno, strerror(errno));
                close(broker->socket); 
                broker->socket = 0;
                exit(0);
            }

            if(sz > 0) {
                int l=0;
                printf("\n");
            
                if( GET_MESSAGE(buffer[0]) == PUBLISH) {                
                //printf("Got PUBLISH message with size %d\n", (uint8_t)buffer[1]);
                    uint32_t topicSize;
                    if(sz>128)
                        topicSize   = buffer[4]+1;
                    else
                        topicSize   = (buffer[2]<<8) + buffer[3];

                    unsigned long i = 4+topicSize;
                    if (((buffer[0]>>1) & 0x03) > QoS0) {
                        uint32_t messageId = (buffer[4+topicSize] << 8) + buffer[4+topicSize+1];
                        i += 2; // 2 extra for msgID
                    
                        uint8_t puback[4] = { SET_MESSAGE(PUBACK), 2, buffer[4+topicSize], buffer[4+topicSize+1] };
                        if (send(broker->socket, puback, sizeof(puback), 0) < sizeof(puback)) {
                            puts("failed to PUBACK");
                            return;
                        }
                    }
                    int l=0;
                    for ( ; i < sz; ++i) {
                        //print(buffer[i]);
                        bufferJson[l] = buffer[i];
                        l++;
                    }
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
		    int cp=0;
		    
		    state_message.auth_sts=0;
		    memset(state_message.message,0,512);
		    for(cp=0; cp < strlen(bufferJson); cp++)
		        state_message.message[cp]=bufferJson[cp];
		    state_callback_message(&state_message);	
                    //parseJsonMessage(bufferJson);
                }
            }
                usleep(500);
        }
        return ;
}

