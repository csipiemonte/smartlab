#include "observation.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include <string.h>

char* getTime(char *buffer){
        time_t curtime;
        struct tm *timeTF;
//         char *buffer=malloc(30*sizeof(char));
        time(&curtime);
        timeTF = gmtime(&curtime);
        strftime(buffer,21,"%Y-%m-%dT%H:%M:%SZ",timeTF);
        return buffer;
}

char* getNextTime(char *buffer ,int seconds){
        time_t curtime;
        struct tm *timeTF;
//         char *buffer=malloc(30*sizeof(char));
        //time(&curtime) + seconds;
	curtime = time (NULL) + seconds;
        timeTF = localtime( &curtime);
        strftime(buffer,21,"%Y-%m-%dT%H:%M:%SZ",timeTF);
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

Value newValue(char *time){
        Value val;
//      if(strlen(time)>0)
//          val.time = time;
//      else{
        val.time = getTime(time);   
//         }
        val.contComponents = 0;
        val.validity = "unknown";
        val.components = (struct Component*)malloc(100*sizeof*val.components);
        return val;
}

Value newValueWithTime(char *time){
        Value val;
        val.time = time;
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


Component newComponentValue(char *name, double value/*char* value*/){
        Component comp;
        comp.sensor_name = name;
        comp.sensor_value = value;
        comp.stringValue = "";
        comp.type='f';
        return comp;
}

Component newComponentDefaultValue( double value/*char* value*/){
        Component comp;
        comp.sensor_name = "c0";
        comp.sensor_value = value;
        comp.stringValue = "";
        comp.type='f';
        return comp;
}

Component newComponentString(char *name, char* stringVal){
        Component comp;
        comp.sensor_name = name;
        comp.sensor_value = 0;
        comp.stringValue = stringVal;
        comp.type='s';
        return comp;
}

Component newComponentDefaultString( char* stringVal){
        Component comp;
        comp.sensor_name = "c0";
        comp.sensor_value = 0;
        comp.stringValue = stringVal;
        comp.type='s';
        return comp;
}

Component newComponentBoolean(char* name, int boolVal)
{
        Component comp;
        comp.sensor_name = name;
        comp.sensor_value = boolVal;
        comp.stringValue = "";
        comp.type='b';
        return comp;
}

Component newComponentDefaultBoolean( int boolVal){
        Component comp;
        comp.sensor_name = "c0";
        comp.sensor_value = boolVal;
        comp.stringValue = "";
        comp.type='b';
        return comp;
}
char* toJson(Observation observation, char *sendMessage){
	//printf("JSON completo con contValues=%d e contComponents=%d\n", observation.contValues, observation.mValues->contComponents);;
//         char *sendMessage;
//         sendMessage = malloc(1000*sizeof(char));
        int lValue=0;

        sprintf(sendMessage,"{\"stream\":\"%s\",",observation.mStream.idStream);
        sprintf(sendMessage,"%s\"sensor\":\"%s\",",sendMessage, observation.mSensor.idSensor);
        sprintf(sendMessage,"%s\"values\": [",sendMessage);

        while(lValue<observation.contValues){
            sprintf(sendMessage,"%s{\"time\":\"%s\",\"components\":{",sendMessage,observation.mValues[lValue].time);
            int lComp = 0;
            while(lComp<observation.mValues[lValue].contComponents){
                sprintf(sendMessage,"%s\"%s\":",sendMessage, observation.mValues[lValue].components[lComp].sensor_name);
                if(sendMessage,observation.mValues[lValue].components[lComp].type == 'f')
                    sprintf(sendMessage,"%s%.2lf",sendMessage,observation.mValues[lValue].components[lComp].sensor_value);
                else if(sendMessage,observation.mValues[lValue].components[lComp].type == 's')
                         sprintf(sendMessage,"%s\"%s\"",sendMessage,observation.mValues[lValue].components[lComp].stringValue);
                else if(sendMessage,observation.mValues[lValue].components[lComp].type == 'b'){
                         if(observation.mValues[lValue].components[lComp].sensor_value==0)
                             sprintf(sendMessage,"%s\"false\"",sendMessage,observation.mValues[lValue].components[lComp].stringValue);
                         else
                             sprintf(sendMessage,"%s\"true\"",sendMessage,observation.mValues[lValue].components[lComp].stringValue);
                }

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