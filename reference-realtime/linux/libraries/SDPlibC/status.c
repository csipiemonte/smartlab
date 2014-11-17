#include "status.h"

void parseWConfig(cJSON *data, config_state_t state_t){
        cJSON *sec = cJSON_GetObjectItem( data, "sec" );
        cJSON *csv = cJSON_GetObjectItem( data, "csv" );
        char *token = strtok(cJSON_Print(csv),"\"");
        char *pchToken[2];
        int i=0;
        char *token2 = strtok(token,";");
        while (token2 != NULL){
          pchToken[i]=token2;
          token2 = strtok (NULL, ";");
          if(i==1)
              state_t.temp_sleep_time = atoi(pchToken[i]);

          printf("valore del token[%d]=%s\n",i, pchToken[i]);
          i++;
        }
}

void sendStatus(SenderMqtt _sender, send_status_t status_t){
       cJSON *root = cJSON_CreateObject();
       cJSON_AddStringToObject(root,"id",status_t.id);
       cJSON_AddStringToObject(root,"time",getTime());
       cJSON_AddStringToObject(root,"last",status_t.lastSend);
       cJSON_AddStringToObject(root,"next",status_t.nextSend);
       printf("valore del json status=%s\n",cJSON_Print(root));
       SenderMqtt sender = newSenderMqtt( _sender.ip, _sender.port, _sender.client, _sender.topic);//
       sendMessageMqtt(sender, cJSON_Print(root), "", "");
}

void getValueTo(cJSON *json,config_state_t state_t,SenderMqtt _sender, send_status_t status_t){

        cJSON *jsonTo;      
        cJSON *jsonMsg;
        cJSON *jsonData;
        char *to;
        char *msg;
        char *data;

        if (!json){
            printf("Error for json before: [%s]\n",cJSON_GetErrorPtr());
            return;
        }
        else{
            jsonTo= cJSON_GetObjectItem( json, "to" );
            if(!jsonTo){
                printf("Error for jsonTo before: [%s]\n",cJSON_GetErrorPtr());
                return;
            }else{
                to = strtok (cJSON_Print(jsonTo),"\"");
                printf("valore di to=%s\n",to);
            }

            jsonMsg= cJSON_GetObjectItem( json, "msg" );
            if(!jsonMsg){
                printf("Error for jsonMsg before: [%s]\n",cJSON_GetErrorPtr());
                return;
            }else{
                msg = strtok (cJSON_Print(jsonMsg),"\"");
                printf("valore di msg=%s\n",msg);
            }
            
            if(!strcmp(msg, "status")){
                printf("mando status\n");
                sendStatus(_sender, status_t);
            }else{
                    if(!strcmp(msg, "config")){
                    printf("ricevuto config\n");
                    state_t.sleep_time = state_t.temp_sleep_time;
                }else{
                    if(!strcmp(msg, "wconfig")){
                        printf("ricevuto wconfig\n");
                        jsonData= cJSON_GetObjectItem( json, "data" );
                        if(!jsonData){
                            printf("Error for jsonData before: [%s]\n",cJSON_GetErrorPtr());
                            return;
                        }else{
                            data = cJSON_Print(jsonData);
                            printf("valore di data=%s\n",data);
                            parseWConfig(jsonData, state_t);
                        }
                    }else
                    printf("messaggio sconosciuto");
                }
            }

        }
}
