#include "clientMqtt.h"


#define STATUS_CONNECTING 0
#define STATUS_CONNACK_RECVD 1
#define STATUS_WAITING 2
static int status = STATUS_CONNECTING;
static int mid_sent = 0;
static int last_mid = -1;
static int last_mid_sent = -1;
static bool disconnect_sent = true;
char *topic;
char* userName;
char* password;
int qos;
static bool connected = true;
const void *message;
long msglen = 0;
int retain = 0;

//struct and callback for the message
message__mqtt_t state_message;
message_mqtt_cb_t state_callback_message;

ClientMqtt newClientMqtt(char* _ip, char* _port, char* _client, char* _topic, int _qos)
{
        ClientMqtt clientMqtt;
        clientMqtt.ip = _ip;
        clientMqtt.port = _port;
        clientMqtt.client = _client;
        clientMqtt.topic = _topic;
	clientMqtt.qos = _qos;
        return clientMqtt;
}

int message_mqtt_state_cb( message_mqtt_cb_t cb){

        state_callback_message=cb;
}

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
      //printf("callback  my_message_callback\n");
        char bufferJson[512];
        if(message->payloadlen){
            state_message.auth_sts=0;
            memset(state_message.message,0,512);
            int cp = 0;
            sprintf(state_message.message, "%s", message->payload);          
            state_callback_message(&state_message);
        }else{
            printf("%s (null)\n", message->topic);
        }
        fflush(stdout);
}

void my_subscribe_username(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
        mosquitto_username_pw_set(mosq, userName, password);	
}


void my_sub_connect_callback(struct mosquitto *mosq, void *obj, int result)
{
        int i;
        struct ClientMqtt *ud;
        ud = (struct ClientMqtt *)obj;

        if(!result){
            for(i=0; i<1; i++){
                mosquitto_subscribe(mosq, NULL, topic, qos);
            }
        }else{
            if(result  ){
                fprintf(stderr, "%s\n", mosquitto_connack_string(result));
            }
        }
}

void my_pub_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
        int rc = MOSQ_ERR_SUCCESS;
        bool quiet = true;
        status = STATUS_CONNACK_RECVD;
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
        int i;
        printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
        for(i=1; i<qos_count; i++){
            printf(", %d", granted_qos[i]);
        }
        printf("\n");
}

void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
        printf("%s\n", str);
}

void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
      //printf("richiamato publish callback\n");
        connected = false;
        last_mid_sent = mid;

        if(mid == last_mid){
            mosquitto_disconnect(mosq);
            disconnect_sent = true;
        }
        else if(disconnect_sent == false){
            mosquitto_disconnect(mosq);
            disconnect_sent = true;
        }
}

void my_disconnect_callback(struct mosquitto *mosq, void *obj, int rc)
{
        connected = false;
}


int client_subscribe(ClientMqtt sender, char* _userName, char* _password){
        char id[30];
        int i;
        char *host = sender.ip;
        char *client = sender.client;
        int port = atoi(sender.port);//8000;
        topic = sender.topic;
        qos = sender.qos;
        userName = _userName;
        password = _password;
        int keepalive = 60;
        bool clean_session = false;
        struct mosquitto *mosq ;

        mosquitto_lib_init();

        mosq = mosquitto_new(client, clean_session, &sender);
        if(!mosq){
                printf("Error: Impossible create struct mosquitto.\n");
                return ERROR_MOSQUITTO;
        }
        mosquitto_username_pw_set(mosq, userName, password) ;
        mosquitto_connect_callback_set(mosq, my_sub_connect_callback);
        mosquitto_message_callback_set(mosq, my_message_callback);	
        mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);

        if(mosquitto_connect(mosq, host, port, keepalive)){
                fprintf(stderr, "Unable to connect.\n");
                return ERROR_CONNECT;
        }

        int lp;

        mosquitto_loop_forever(mosq, -1, 1);       
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return SUBSCRIBE_CORRECT;
}

int client_publish(ClientMqtt sender, char *_message, char* _userName, char* _password){
        char id[30];
        int i;
        char *host = sender.ip;
        char *client = sender.client;
        int port = atoi(sender.port);//8000;
        topic = sender.topic;
        qos = sender.qos;
        userName = _userName;
        password = _password;      
        int keepalive = 60;
        connected = true;

        message = _message;
        msglen = strlen(message);
        retain = 1;
        bool clean_session = true;
        struct mosquitto *mosq ;

        mosquitto_lib_init();
        mosq = mosquitto_new(client, true, NULL);
        if(!mosq){
                printf("Error: Impossible create struct mosquitto.\n");
                return ERROR_MOSQUITTO;
        }
        mosquitto_log_callback_set(mosq, my_log_callback);
        mosquitto_username_pw_set(mosq, userName, password);//	
        int max_inflight = 20;
        mosquitto_max_inflight_messages_set(mosq, max_inflight);
        mosquitto_connect_callback_set(mosq, my_pub_connect_callback);
        mosquitto_disconnect_callback_set(mosq, my_disconnect_callback);
        mosquitto_publish_callback_set(mosq, my_publish_callback);

        int rc = mosquitto_connect(mosq, host, port, keepalive);
            if(rc == MOSQ_ERR_ERRNO){
                printf("Error to connect\n");
                return ERROR_CONNECT;
            }
        int lp=1;
        int p = -1;
        int rc1=1;
        do{
            if(p!=0){
                p = mosquitto_publish( mosq, &lp, topic,strlen(message),(uint8_t *)message, qos, 0);
                if(p){
                    printf( "Error: Publish returned %d, disconnecting.\n", p);
                    mosquitto_disconnect(mosq);
                }
            }
            
            rc1 = mosquitto_loop(mosq, -1, 1);
            usleep(100000);
        }while(rc1 == MOSQ_ERR_SUCCESS && connected);	    
	
        mosquitto_loop_stop(mosq, false);
        printf("Publish at the topic=%s with qos=%d\n",topic,qos);
        mosquitto_disconnect(mosq);

        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        printf("Publish End\n");
        return PUBLISH_CORRECT;
}        