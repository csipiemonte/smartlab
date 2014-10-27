#include "mqttSecure.h"

char *topic;
char* userName;
char* password;
message__state_t state_message;
message_state_cb_t state_callback_message;

SLLMqtt newSLLMqtt(char* _cafile, char* _capath, char* _certfile, char* _keyfile)
{
        SLLMqtt sllMqtt;
        sllMqtt.cafile = _cafile;
        sllMqtt.capath = _capath;
        sllMqtt.certfile = _certfile;
        sllMqtt.keyfile = _keyfile;
        return sllMqtt;
}

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
        char bufferJson[512];
        if(message->payloadlen){
//             printf("%s \n", message->payload);//, message->payload);
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
        mosquitto_username_pw_set(mosq, userName, password);//"sandbox","sandbox$1");	
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
        int i;
        if(!result){
        /* Subscribe to broker information topics on successful connect. */
            mosquitto_username_pw_set(mosq, userName, password);//"sandbox","sandbox$1");
            int res= mosquitto_subscribe(mosq, NULL, topic , 0); ///*"input/sandbox"*/
            printf("username=%s password=%s topic=%s\n",userName, password, topic);
            if(res==MOSQ_ERR_SUCCESS)
                printf("Connect ok\n");
            else
                printf("Connect failed\n"); 
        }else{
            fprintf(stderr, "Connect failed\n");
        }
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

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
        /* Pring all log messages regardless of level. */
        printf("%s\n", str);
}

void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
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
void my_disconnect_callback(struct mosquitto *mosq, void *obj, int rc)
{
        connected = false;
}


int mqtt_subscribe_sll(SenderMqtt sender, SLLMqtt sllMqtt, char* _userName, char* _password){
        char id[30];
        int i;
        printf( "INIT.\n");
        char *host = sender.ip;
        char *client = sender.client;
        int port = atoi(sender.port);//8000;
        printf("valore della porta=%d\n",port);
        topic = sender.topic;
        userName = _userName;
        password = _password;
        char *cafile = sllMqtt.cafile;
        char *capath = sllMqtt.capath;
        char *certfile = sllMqtt.certfile;
        char *keyfile = sllMqtt.keyfile;             

        int keepalive = 60;
        bool clean_session = true;
        struct mosquitto *mosq ;

        mosquitto_lib_init();
        printf("MOSQUITTO NEW.\n");
        mosq = mosquitto_new(client, clean_session, NULL);
        if(!mosq){
                printf("Error: Out of memory.\n");
                return 1;
        }
//         mosquitto_log_callback_set(mosq, my_log_callback);
        mosquitto_username_pw_set(mosq, userName, password ) ;
        mosquitto_tls_set(mosq, cafile, capath, certfile, keyfile, NULL);
        mosquitto_connect_callback_set(mosq, my_connect_callback);
        
        mosquitto_message_callback_set(mosq, my_message_callback);
        mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);
        
        if(mosquitto_connect(mosq, host, port, keepalive)){
                fprintf(stderr, "Unable to connect.\n");
                return 1;
        }

        int lp;

        mosquitto_loop_forever(mosq, -1, 1);       
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 0;
}

int mqtt_pubscribe_sll(SenderMqtt sender, SLLMqtt sllMqtt, char *_message, char* _userName, char* _password){
        char id[30];
        int i;
        fprintf(stderr, "INIT.\n");
        char *host = sender.ip;
        char *client = sender.client;
        int port = atoi(sender.port);//8000;
        printf("valore della porta=%d client=%s\n",port,client);
        topic = sender.topic;
        char *cafile = sllMqtt.cafile;
        char *capath = sllMqtt.capath;
        char *certfile = sllMqtt.certfile;
        char *keyfile = sllMqtt.keyfile; 
        userName = _userName;
        password = _password;      
        int keepalive = 60;

        const void *message = _message;
        bool clean_session = true;
        struct mosquitto *mosq ;

        mosquitto_lib_init();
        printf("MOSQUITTO NEW.\n");
        mosq = mosquitto_new(client, clean_session, NULL);
        if(!mosq){
                fprintf(stderr, "Error: Out of memory.\n");
                return 1;
        }
        mosquitto_username_pw_set(mosq, userName, password);//
        mosquitto_tls_set(mosq, cafile, capath, certfile, keyfile, NULL);
        mosquitto_connect_callback_set(mosq, my_connect_callback);
        mosquitto_disconnect_callback_set(mosq, my_disconnect_callback);
        mosquitto_message_callback_set(mosq, my_message_callback);
        mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);


        
        if(mosquitto_connect(mosq, host, port, keepalive)){
                fprintf(stderr, "Unable to connect.\n");
                return 1;
        }

        int lp;
        int p = mosquitto_publish( mosq, &lp, topic,strlen(message),(uint8_t *)message,0,true);

       // mosquitto_loop_forever(mosq, -1, 1);
       // while(!mosquitto_loop(mosq,-1, 1)){
       // }
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 0;
}        