/**
 * @file mqttSecure.h
 * @author Leonardo Sileo
 * @date 15 Settembre 2014
 * @brief This library adds function for realize a subscribe or a publiscer MQTT with SSL. This library uses mosquitto library.
 * It provides functionality for connecting to a topic, publishing and receiving messages MQTT. 
 * 
 */

#include "clientMqtt.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>



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
} SSLMqtt;

/**
 * @brief Create a SSLMQTT
 * 
 * Create a new sender MQTT for send the messages or riceive
 * 
 * @param _ip address ip of the server
 * @param _port port of the server
 * @param _client the name of client MQTT
 * @param _topic the topic used 
 * @return the sender
 */
SSLMqtt newSSLMqtt(char* _cafile, char* _capath, char* _certfile, char* _keyfile);

/**
 * @brief Subscribe a client with SSL
 * 
 * Subscribe a client with SSL. 
 * 
 * @param sender the sender MQTT
 * @param sslMqtt the SSL parametres
 * @param userName the name for the authentication
 * @param password the password for the authentication 
 * 
 * @return Return 1 for publish succed, -1 for error
 */
int mqtt_subscribe_ssl(ClientMqtt sender, SSLMqtt sslMqtt, char* userName, char* password);

/**
 * @brief Pubscribe a client with SSL
 * 
 * Pubscribe a client with SSL. 
 * 
 * @param sender the sender MQTT
 * @param sslMqtt the SSL parametres
 * @param userName the name for the authentication
 * @param password the password for the authentication 
 * 
 * @return Return 1 for publish succed, -1 for error
 */
int mqtt_publish_ssl(ClientMqtt sender, SSLMqtt sslMqtt, char *_message, char* userName, char* password);
