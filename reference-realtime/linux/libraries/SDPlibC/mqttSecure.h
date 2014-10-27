/**
 * @file mqttSecure.h
 * @author Leonardo Sileo
 * @date 15 Settembre 2014
 * @brief This library adds function for realize a subscribe or a publiscer MQTT with SLL. This library uses mosquitto library.
 * It provides functionality for connecting to a topic, publishing and receiving messages MQTT. 
 * 
 */

#include "mqtt.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

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
bool connected;

/**
 * @brief Struct for the SenderMqtt
 *
 * 
 */
typedef struct 
{
    char *cafile;
    char *capath;
    char *certfile;
    char *keyfile;
} SLLMqtt;

/**
 * @brief Create a SLLMQTT
 * 
 * Create a new sender MQTT for send the messages or riceive
 * 
 * @param _ip address ip of the server
 * @param _port port of the server
 * @param _client the name of client MQTT
 * @param _topic the topic used 
 * @return the sender
 */
SLLMqtt newSLLMqtt(char* _cafile, char* _capath, char* _certfile, char* _keyfile);

/**
 * @brief Subscribe a client with SLL
 * 
 * Subscribe a client with SLL. 
 * 
 * @param sender the sender MQTT
 * @param sllMqtt the SLL parametres
 * @param userName the name for the authentication
 * @param password the password for the authentication 
 * 
 * @return Return 1 for publish succed, -1 for error
 */
int mqtt_subscribe_sll(SenderMqtt sender, SLLMqtt sllMqtt, char* userName, char* password);

/**
 * @brief Pubscribe a client with SLL
 * 
 * Pubscribe a client with SLL. 
 * 
 * @param sender the sender MQTT
 * @param sllMqtt the SLL parametres
 * @param userName the name for the authentication
 * @param password the password for the authentication 
 * 
 * @return Return 1 for publish succed, -1 for error
 */
int mqtt_pubscribe_sll(SenderMqtt sender, SLLMqtt sllMqtt, char *_message, char* userName, char* password);
