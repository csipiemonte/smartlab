/**
 * @file senderHttp.h
 * @author Leonardo Sileo
 * @date 18 Luglio 2014
 * @brief Allow to send the json
 * 
 */

#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>


#define SA struct sockaddr /**< Struttura per il server */
#define MAXLINE 4096 /**< dimensione massima del messaggio */
#define LISTENQ 1024 /**< Listner per la risposta */

/**
 * @brief Struct for the Sender
 *
 * Struct for sender with parameters of network
 */
typedef struct 
{
char *port; /**<The port of the server */
char *ip;  /**< Address IP of the server */
char *service; /**< Link of the server */
} SenderHttp;


/**
 * @brief Create a sender
 * 
 * Create a new sender for send the message at server
 * 
 * @param _ip address ip of the server
 * @param _port port of the server
 * @param _service link of the server
 * @return the sender
 */
SenderHttp newSenderHttp(char* _ip, char* _port, char* _service);


/**
 * @brief Send the message
 * 
 * Send the message at server. The messagge is sending with POST request
 * 
 * @param sender the object sender 
 * @param message the message to send
 */
int sendMessageHttp(SenderHttp sender, char *message);
