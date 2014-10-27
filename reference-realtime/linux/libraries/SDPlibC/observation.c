#include "observation.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include <string.h>

char* getTime(){
        time_t curtime;
        struct tm *timeTF;
        char *buffer=malloc(30*sizeof(char));
        time(&curtime);
        timeTF = localtime( &curtime );
        strftime(buffer,30,"%Y-%m-%dT%XZ",timeTF);
        return buffer;
}

Observation newObservation(char *idStream, char *idSensor){
        Observation obs;
        obs.mStream.idStream = idStream;
        obs.mSensor.idSensor = idSensor;
        obs.contValues = 0;
        obs.mValues = (struct Value*)malloc(100*sizeof*obs.mValues);
        return obs;
}


Observation observationAddValue(Observation obs, Value value){
        obs.mValues[obs.contValues] = value;
        obs.contValues++;
        return obs;
}

Value newValue(char* time){
        Value val;
        if(strlen(time)>0)
            val.time = time;
        else
            val.time = getTime();
        val.contComponents = 0;
        val.validity = "unknown";
        val.components = (struct Component*)malloc(100*sizeof*val.components);
        return val;
}

Value addComponetsAtValue(Value val, Component component){
        val.components[val.contComponents] = component;
        val.contComponents++;
        return val;
}


Component newComponent(char *name, float value/*char* value*/){
        Component comp;
        comp.sensor_name = name;
        comp.sensor_value = value;
        return comp;
}

Component newComponentDefault( float value/*char* value*/){
        Component comp;
        comp.sensor_name = "0";
        comp.sensor_value = value;
        return comp;
}

char* toJson(Observation observation){
	//printf("JSON completo con contValues=%d e contComponents=%d\n", observation.contValues, observation.mValues->contComponents);;
        char *sendMessage;
        sendMessage = malloc(1000*sizeof(char));
        int lValue=0;

        sprintf(sendMessage,"{\"stream\":\"%s\",",observation.mStream.idStream);
        sprintf(sendMessage,"%s\"sensor\":\"%s\",",sendMessage, observation.mSensor.idSensor);
        sprintf(sendMessage,"%s\"values\": [",sendMessage);

        while(lValue<observation.contValues){
            sprintf(sendMessage,"%s{\"time\":\"%s\",\"components\":{",sendMessage,observation.mValues[lValue].time);
            int lComp = 0;
            while(lComp<observation.mValues[lValue].contComponents){
                sprintf(sendMessage,"%s\"%s\":%d",sendMessage, 
                observation.mValues[lValue].components[lComp].sensor_name,observation.mValues[lValue].components[lComp].sensor_value);
                lComp++;
                if(lComp==observation.mValues[lValue].contComponents){
                    sprintf(sendMessage,"%s },\"validaty\":\"%s\"}",sendMessage,observation.mValues[lValue].validity);
                }
                else{
                    sprintf(sendMessage,"%s ,",sendMessage);
                }
            }

            lValue++;
            if(lValue==observation.contValues){
                sprintf(sendMessage,"%s ]",sendMessage);
            }
            else{
                sprintf(sendMessage,"%s ,",sendMessage);
            }		  
        }
        sprintf(sendMessage,"%s}",sendMessage);
//         printf("valore del messaggio:\n%s\n\n",sendMessage);
        return sendMessage;
}