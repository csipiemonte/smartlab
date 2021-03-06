#include "mqttSecure.h"

#define MSGMODE_NONE 0
#define MSGMODE_CMD 1
#define MSGMODE_STDIN_LINE 2
#define MSGMODE_STDIN_FILE 3
#define MSGMODE_FILE 4
#define MSGMODE_NULL 5

#define STATUS_CONNECTING 0
#define STATUS_CONNACK_RECVD 1
#define STATUS_WAITING 2
static int mode = MSGMODE_NONE;
static int status = STATUS_CONNECTING;
static int mid_sent = 0;
static int last_mid = -1;
static int last_mid_sent = -1;
static bool disconnect_sent = true;
char *topic;
int qos;
char* userName;
char* password;
//struct and callback for the message
message__mqtt_t state_message;
message_mqtt_cb_t state_callback_message;
bool connected;

SSLMqtt newSSLMqtt(char* _cafile, char* _capath, char* _certfile, char* _keyfile)
{
        SSLMqtt sslMqtt;
        sslMqtt.cafile = _cafile;
        sslMqtt.capath = _capath;
        sslMqtt.certfile = _certfile;
        sslMqtt.keyfile = _keyfile;
        return sslMqtt;
}

void my_message_callback_sll(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
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


void my_subscribe_username_sll(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
        mosquitto_username_pw_set(mosq, userName, password);
}

void my_sub_connect_callback_ssl(struct mosquitto *mosq, void *obj, int result)
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

void my_pub_connect_callback_ssl(struct mosquitto *mosq, void *userdata, int result)
{
        int rc = MOSQ_ERR_SUCCESS;
        bool quiet = true;
        status = STATUS_CONNACK_RECVD;
}

void my_subscribe_callback_sll(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
        int i;
        printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
        for(i=1; i<qos_count; i++){
            printf(", %d", granted_qos[i]);
        }
        printf("\n");
}

void my_log_callback_sll(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
        /* Pring all log messages regardless of level. */
        printf("%s\n", str);
}

void my_publish_callback_sll(struct mosquitto *mosq, void *obj, int mid)
{
        last_mid_sent = mid;
        if(mode == MSGMODE_STDIN_LINE){
            if(mid == last_mid){
                mosquitto_disconnect(mosq);
                disconnect_sent = true;
            }
        }else if(disconnect_sent == false){
            mosquitto_disconnect(mosq);
            disconnect_sent = true;
        }
}

void my_disconnect_callback_sll(struct mosquitto *mosq, void *obj, int rc)
{
        connected = false;
}


int mqtt_subscribe_ssl(ClientMqtt sender, SSLMqtt sslMqtt, char* _userName, char* _password){
        char id[30];
        int i;
        printf( "INIT.\n");
        char *host = sender.ip;
        char *client = sender.client;
        int port = atoi(sender.port);//8000;
        topic = sender.topic;
        userName = _userName;
        password = _password;
        char *cafile = sslMqtt.cafile;
        char *capath = sslMqtt.capath;
        char *certfile = sslMqtt.certfile;
        char *keyfile = sslMqtt.keyfile;             

        int keepalive = 60;
        bool clean_session = true;
        struct mosquitto *mosq ;

        mosquitto_lib_init();

        mosq = mosquitto_new(client, clean_session, NULL);
        if(!mosq){
                printf("Error: Impossible create struct mosquitto.\n");
                return ERROR_MOSQUITTO;
        }
//         mosquitto_log_callback_set(mosq, my_log_callback);
        mosquitto_username_pw_set(mosq, userName, password) ;
        mosquitto_tls_set(mosq, cafile, capath, certfile, keyfile, NULL);
        mosquitto_connect_callback_set(mosq, my_sub_connect_callback_ssl);
        mosquitto_subscribe_callback_set(mosq, my_subscribe_callback_sll);
        mosquitto_message_callback_set(mosq, my_message_callback_sll);
	
        if(mosquitto_connect(mosq, host, port, keepalive)){
                fprintf(stderr, "Unable to connect.\n");
                return 1;
        }

        int lp;

        mosquitto_loop_forever(mosq, -1, 1);       
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return SUBSCRIBE_CORRECT;
}

int mqtt_publish_ssl(ClientMqtt sender, SSLMqtt sslMqtt, char *_message, char* _userName, char* _password){
        char id[30];
        int i;
        fprintf(stderr, "INIT.\n");
        char *host = sender.ip;
        char *client = sender.client;
        int port = atoi(sender.port);//8000;
        printf("valore della porta=%d client=%s\n",port,client);
        topic = sender.topic;
        qos = sender.qos;	
        char *cafile = sslMqtt.cafile;
        char *capath = sslMqtt.capath;
        char *certfile = sslMqtt.certfile;
        char *keyfile = sslMqtt.keyfile; 
        userName = _userName;
        password = _password;      
        int keepalive = 60;
        int rc;
        const void *message = _message;
        bool clean_session = true;
        struct mosquitto *mosq ;

        mosquitto_lib_init();
        printf("MOSQUITTO NEW.\n");
        mosq = mosquitto_new(client, clean_session, NULL);
        if(!mosq){
                printf("Error: Impossible create struct mosquitto.\n");
                return ERROR_MOSQUITTO;
        }


        int max_inflight = 20;
        mosquitto_max_inflight_messages_set(mosq, max_inflight);

        mosquitto_disconnect_callback_set(mosq, my_disconnect_callback_sll);
        
	
        mosquitto_username_pw_set(mosq, userName, password);//
        mosquitto_tls_set(mosq, cafile, capath, certfile, keyfile, NULL);
        mosquitto_connect_callback_set(mosq, my_pub_connect_callback_ssl);
        mosquitto_publish_callback_set(mosq, my_publish_callback_sll);
        mosquitto_message_callback_set(mosq, my_message_callback_sll);

        if(mosquitto_connect(mosq, host, port, keepalive)){
                fprintf(stderr, "Unable to connect.\n");
                return 1;
        }

        if(rc == MOSQ_ERR_ERRNO){
                fprintf(stderr, "Unable to connect.\n");
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
//         free(message);
        printf("esito della publish=%d topic=%s qos=%d\n",p,topic,qos);
        mosquitto_disconnect(mosq);

        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        printf("invio terminato\n");
        return PUBLISH_CORRECT;
}        