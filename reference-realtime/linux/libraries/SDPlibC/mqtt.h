/**
 * @file mqtt.h
 * @author Leonardo Sileo
 * @date 15 Settembre 2014
 * @brief Allows you create a MQTT subscribe or publisher
 * 
 */
#ifndef mqtt_h
#define mqtt_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#define PROTOCOL_NAME       "MQIsdp"    // 
#define PROTOCOL_VERSION    3U          // version 3.0 of MQTT
#define CLEAN_SESSION       (1U<<1)
#define KEEPALIVE           30U         // specified in seconds
#define MESSAGE_ID          1U // not used by QoS 0 - value must be > 0

/* Macros for accessing the MSB and LSB of a uint16_t */
#define MSB(A) ((uint8_t)((A & 0xFF00) >> 8))
#define LSB(A) ((uint8_t)(A & 0x00FF))


#define SET_MESSAGE(M) ((uint8_t)(M << 4))
#define GET_MESSAGE(M) ((uint8_t)(M >> 4))

/**
 * @brief Struct rappresenting the type of message
 *
 * 
 */
typedef enum {
        CONNECT = 1,
        CONNACK,
        PUBLISH,
        PUBACK,
        PUBREC,
        PUBREL,
        PUBCOMP,
        SUBSCRIBE,
        SUBACK,
        UNSUBSCRIBE,
        UNSUBACK,
        PINGREQ,
        PINGRESP,
        DISCONNECT       
} connect_msg_t;

/**
 * @brief Struct for the status of connection
 *
 * 
 */
typedef enum {
        Connection_Accepted,
        Connection_Refused_unacceptable_protocol_version,
        Connection_Refused_identifier_rejected,
        Connection_Refused_server_unavailable,
        Connection_Refused_bad_username_or_password,
        Connection_Refused_not_authorized
} connack_msg_t;


/**
 * @brief Struct for the broker MQTT
 *
 * 
 */
struct mqtt_broker_handle{
        int                 socket;
        struct sockaddr_in  socket_address;
        uint16_t            port;
        char                hostname[16];  // based on xxx.xxx.xxx.xxx format
        char                clientid[30];  // max 50 charaters long with username e password
        bool                connected;
        size_t              topic;
        uint16_t            pubMsgID;
        uint16_t            subMsgID;
};

/**
 * @brief Struct for the header of message MQTT
 *
 * 
 */
typedef struct fixed_header_t{
        uint16_t     retain             : 1;
        uint16_t     Qos                : 2;
        uint16_t     DUP                : 1;
        uint16_t     connect_msg_t      : 4;
        uint16_t     remaining_length   : 8;    
}fixed_header_t;

typedef struct mqtt_broker_handle mqtt_broker_handle_t;

/**
 * @brief Struct for the SenderMqtt
 *
 * 
 */
typedef struct 
{
char *port; /**<The port of the server */
char *ip;  /**< Address IP of the server */
char *client;  /**The client MQTT */
char *topic; /** The topic for MQTT*/
} SenderMqtt;

/**
 * @brief Struct for the message usage for the callback
 *
 * 
 * 
 */
typedef struct {

int auth_sts; /**<Status of message (not used) */
char message[512]; /**<The message received */
} message__state_t;

/**
 * @brief Callback function for message notification.
 * 
 * This callback is used for signaling a new MQTT message received
 *
 * @param message_state	A read-only message__state_t structure
 */

typedef void (*message_state_cb_t)( const message__state_t *message_state);

/**
 * @brief Set the callback for message state updates.
 *
 * @param cb		A funcion pointer or NULL to disable callbacks.
 * @return			Zero on success or NULL on error.
 */
extern int message_register_state_cb( message_state_cb_t cb);

	
	
typedef enum {QoS0, QoS1, QoS2} QoS;

/**
 * @brief Create a sender MQTT
 * 
 * Create a new sender MQTT for send the messages or riceive
 * 
 * @param _ip address ip of the server
 * @param _port port of the server
 * @param _client the name of client MQTT
 * @param _topic the topic used 
 * @return the sender
 */
SenderMqtt newSenderMqtt(char* _ip, char* _port, char* _client, char* _topic);


/**
 * @brief Create the broker MQTT
 * 
 * Create the broker MQTT for subscribe or publisher client
 * 
 * @param server_ip address ip of the server
 * @param port port of the server
 * @param client the name of client MQTT
 * @param userName the name for the authentication
 * @param password the password for the authentication
 * 
 * @return The broker
 */
mqtt_broker_handle_t * mqtt_connect(const char* client, const char * server_ip, uint32_t port, char* userName, char* password);

/**
 * @brief Disconnect the client MQTT
 * 
 * Disconnect the client MQTT
 * 
 * @param broker the broker for the disconnection
 * 
 */
void mqtt_disconnect(mqtt_broker_handle_t *broker);



/**
 * @brief Subscribe a client
 * 
 * Subscribe a client associating a topic with a quality of service. 
 * For the quality of service is preferred use 0
 * 
 * @param broker the broker for receive the message
 * @param topic the topic of the message
 * @param qos the quality of service 
 * 
 * @return Return 1 for publish succed, -1 for error
 */
int mqtt_subscribe(mqtt_broker_handle_t *broker, const char *topic, QoS qos);


/**
 * @brief Display a message
 * 
 * Display a message, used from the subscribe client
 * 
 * @param broker the broker for receive the message
 * @param print Write a character to stdout.
 */
void mqtt_display_message(mqtt_broker_handle_t *broker, int (*print)(int));

/**
 * @brief Publish the message
 * 
 * Publish the message associating a topic with a quality of service. 
 * For the quality of service is preferred use 0
 * 
 * @param broker the broker for send the message
 * @param topic the topic of the message
 * @param msg the message to send
 * @param qos the quality of service
 * 
 * @return Return 1 for message published, -1 for error
 */
int mqtt_publish(mqtt_broker_handle_t *broker, const char *topic, const char *msg, QoS qos);

/**
 * @brief Send the message
 * 
 * Send the message at server. The messagge is sending with MQTT
 * @param sender the sender MQTT
 * @param message the message to send
 * @param userName the name for the authentication
 * @param password the password for the authentication
 * 
 * @return The broker
 */
int sendMessageMqtt(SenderMqtt sender, char *message, char* userName, char* password);

#endif
