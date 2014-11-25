#include "observation.h"
#include "senderHttp.h"
#include "cJSON.h"
#include "clientMqtt.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Struct for the configure state
 *
 * 
 */
typedef struct {
        int sleep_time;
        int temp_sleep_time;     
} config_state_t;

/**
 * @brief Struct for the message of status
 *
 * 
 */
typedef struct {
        char *nextSend;
        char *lastSend;
        char *timeNow;   
        char *id;  
} send_status_t;

/**
 * @brief Parsing the message from server for extract the time
 * 
 * Parsing the message send to server for for extract the time
 *
 * @param data the message
 * @param state_t the configure state of client
 * 
 * @return the new time
 */
int getTimeConfig(cJSON *data);

/**
 * @brief Send the status of client 
 * 
 * Send the status of client at server
 *
 * @param _sender the sender MQTT
 * @param status_t the state of client
 */
void sendStatus(ClientMqtt _sender, send_status_t status_t);

/**
 * @brief Parsing the message from server
 * 
 * Parsing the message send to server for status or configuration
 *
 * @param json the message
 * @param state_t the state of client
 * @param _sender the sender MQTT
 * @param status_t the state of client
 *
 */
// config_state_t getValueTo(cJSON *json,config_state_t state_t,ClientMqtt _sender, send_status_t status_t);
void getValueTo(cJSON *json,config_state_t state_t,ClientMqtt _sender, send_status_t status_t);