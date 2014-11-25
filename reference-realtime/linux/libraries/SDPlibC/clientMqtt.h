/**
 * @file clientMqtt.h
 * @author Leonardo Sileo
 * @date 15 Settembre 2014
 * @brief This library adds function for realize a subscribe or a publiscer MQTT with SSL. This library uses mosquitto library.
 * It provides functionality for connecting to a topic, publishing and receiving messages MQTT. 
 * 
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define ERROR_MOSQUITTO -1
#define ERROR_CONNECT -2
#define PUBLISH_CORRECT 1
#define SUBSCRIBE_CORRECT 1
/**
 * @brief Struct for the ClientMqtt
 *
 * 
 */
typedef struct 
{
char *port; /**<The port of the server */
char *ip;  /**< Address IP of the server */
char *client;  /**The client MQTT */
char *topic; /** The topic for MQTT*/
int qos; /** The quality of service*/
} ClientMqtt;

/**
 * @brief Create a client MQTT
 * 
 * Create a new client MQTT for send the messages or riceive
 * 
 * @param _ip address ip of the server
 * @param _port port of the server
 * @param _client the name of client MQTT
 * @param _topic the topic used 
 * @parma _qos the quality of service
 * 
 * @return the sender
 */
ClientMqtt newClientMqtt(char* _ip, char* _port, char* _client, char* _topic, int _qos);

/**
 * @brief Struct for the message usage for the callback
 *
 * 
 * 
 */
typedef struct {

int auth_sts; /**<Status of message (not used) */
char message[512]; /**<The message received */
} message__mqtt_t;

/**
 * @brief Callback function for message notification.
 * 
 * This callback is used for signaling a new MQTT message received
 *
 * @param message_state	A read-only message__state_t structure
 */

typedef void (*message_mqtt_cb_t)( const message__mqtt_t *message_state);

/**
 * @brief Set the callback for message state updates.
 *
 * @param cb	A funcion pointer or NULL to disable callbacks.
 * @return	Zero on success or NULL on error.
 */
extern int message_mqtt_state_cb( message_mqtt_cb_t cb);

/**
 * @brief Subscribe a client with SSL
 * 
 * Subscribe a client with SSL. 
 * 
 * @param sender the sender MQTT
 * @param userName the name for the authentication
 * @param password the password for the authentication 
 * 
 * @return Return 1 for publish succed, -1 for error
 */
int client_subscribe(ClientMqtt sender, char* userName, char* password);

/**
 * @brief Pubscribe a client with SSL
 * 
 * Pubscribe a client with SSL. 
 * 
 * @param sender the sender MQTT
 * @param userName the name for the authentication
 * @param password the password for the authentication 
 * 
 * @return Return 1 for publish succed, -1 for error
 */
int client_publish(ClientMqtt sender, char *_message, char* userName, char* password);
