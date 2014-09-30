/**
 * @file gw_firmware.h
 * @author Leonardo Sileo
 * @date 15 Settembre 2014
 * @brief Allows you to locate and downlad a new firmware for Arduino
 * 
 */

#include <stdint.h>
#include "mqtt.h"
//lib curl
#include <curl/curl.h>
#include <curl/easy.h>
//lib hash
#include <openssl/md5.h>
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
#include "cJSON.h"

typedef struct 
{
char *url; /**<The url of the firmware */
char *hash;  /**< The hash for check the downlad */
} FirmwareComponent;
/**
 * @brief Create a Json for download the firmware
 * 
 * Used a cJSON class for create a json and define the value used for to download the firmware.
 *
 * @return return the json create
 */
cJSON *firmwareJson();

/**
 * @brief Add a value to the json
 * 
 * Add particular value into the cJSON
 * 
 * @param root the cJSON
 * @param field the name of field
 * @param value the value of field
 */
void addValue(cJSON *root, char *field, char *value);

/**
 * @brief Return the json
 * 
 * Return a array representing the content of the json
 * @param root the json
 * 
 * @return the json
 */
char* getJson(cJSON *root);

/**
 * @brief Parser for the JSON received
 * 
 * Parser the json for search URL and HASH value
 * @param buffer a array representing the content of the json
 * 
 * @return Return 1 for message published, -1 for error
 */
FirmwareComponent getFirmwareComponent(char *buffer);

/**
 * @brief Write the data into the file
 * 
 * Write the data into the file
 * @param ptr the data
 * @param size the dimension of data
 * @param nmemb the dimension of write into the file
 * @param stream the file
 * 
 * @return Return 1 for message published, -1 for error
 */
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

/**
 * @brief Download a file
 * 
 * This function does the download from the network following a GET request
 * @param url the link of file
 * @param pathFile the path where to save the file 
 * 
 * @return Return 1 if dowload is correct, -1 for error
 */
int getFile(char *url,char *pathFile);

/**
 * @brief Checksum of the file
 * 
 * Checksum of the file download from the network following a GET with a webservice
 * @param filename the name of file
 * @param hash the value of hash
 * 
 * @return Return 1 if checksum is correct, -1 for error
 */
int checkHash(char* filename, char* hash);

/**
 * @brief Load the firmware for Arduino
 * 
 * Load the firmware for Arduino. At moment this function works for Arduino MEGA 2560
 * @param filename the name of file
 * @param device the serial port where is attached the Arduino
 * 
 * 
 */
void loadFirmware(char *filename, char *device);
