/*
 * Copyright (C) 2014 CSP Innovazione nelle ICT s.c.a r.l. (http://www.csp.it/)
 * All rights reserved.
 *
 * All information contained herein is, and remains the property of
 * CSP Innovazione nelle ICT s.c.a r.l. and its suppliers, if any.
 * The intellectual and technical concepts contained herein are proprietary to
 * CSP Innovazione nelle ICT s.c.a r.l. and its suppliers and may be covered
 * by Italian and Foreign Patents, in process, and are protected by trade secret
 * or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from CSP Innovazione nelle ICT s.c.a r.l.
 *
 * stringparser.h
 *         <short description>
 *
 * Authors:
 *         Marco Boeris Frusca <marco.boeris@csp.it>
 *         boeris
 *
 */

#ifndef STRINGPARSER_H_
#define STRINGPARSER_H_

#include "Arduino.h"
#include <avr/pgmspace.h>

/**
 * The class StringParser gives the functions to manage an array of characters.
 *
 */
class StringParser
{
  public:
    /**
     * Default Constructor
     *
     */
    StringParser();

    /**
     * Destructor
     *
     */
    virtual ~StringParser();

    /**
     * Converts an array of characters in a byte variable
     *
     * \param[in] str array of characters (it represents a integer number).
     * \param[out] n variable where the number will be saved.
     * \param[in] size max size of the array of characters.
     *
     * \return true if no error, false otherwise
     */
    static bool toInt(char* str, byte &n, byte size = 255);

    /**
     * Converts an array of characters in a integer variable
     *
     * \param[in] str array of characters (it represents a integer number).
     * \param[out] n variable where the number will be saved.
     * \param[in] size max size of the array of characters.
     *
     * \return true if no error, false otherwise
     */
    static bool toInt(char* str, int &n, byte size = 255);

    /**
     * Converts an array of characters in a unsigned integer variable
     *
     * \param[in] str array of characters (it represents a integer number).
     * \param[out] n variable where the number will be saved.
     * \param[in] size max size of the array of characters.
     *
     * \return true if no error, false otherwise
     */
    static bool toInt(char* str, unsigned int &n, byte size);

    /**
     * Converts an array of characters in a long variable
     *
     * \param[in] str array of characters (it represents a integer number).
     * \param[out] n variable where the number will be saved.
     * \param[in] size max size of the array of characters.
     *
     * \return true if no error, false otherwise
     */
    static bool toLong(char* str, long &n, byte size);

    /**
     * Converts an array of characters in a unsigned long variable
     *
     * \param[in] str array of characters (it represents a integer number).
     * \param[out] n variable where the number will be saved.
     * \param[in] size max size of the array of characters.
     *
     * \return true if no error, false otherwise
     */
    static bool toLong(char* str, unsigned long &n, byte size);

    /**
     * Converts an array of characters in a float variable
     *
     * \param[in] str array of characters (it represents a float number).
     * \param[out] n variable where the number will be saved.
     * \param[in] size max size of the array of characters.
     *
     * \return true if no error, false otherwise
     */
    static bool toFloat(char* str, float &n, byte size);

    /**
     * Converts an array of characters in IP address as a byte array of 4 elements
     *
     * \param[in] str array of characters.
     * \param[out] ip array where will be saved the IP address octets
     * \param[in] size max size of the array of characters.
     *
     * \return true if no error, false otherwise
     */
    static bool toIPAddress(char* str, byte*ip, byte size = 255);

    /**
     * Checks if the array has only decimal digit characters.
     *
     * \param[in] str array of characters.
     * \param[in] size max size of the array of characters.
     *
     * \return true if array has only decimal digit characters, false otherwise.
     */
    static bool isDigitStr(char* str, byte size = 255);

    /**
     * Checks if the array contains an IP address.
     *
     * \param[in] str array of characters.
     * \param[in] size max size of the array of characters.
     *
     * \return true if array contains an IP address, false otherwise.
     */
    static bool isIPAddress(char* str, byte size = 255);

    /**
     * Checks if the array contains a float number.
     *
     * \param[in] str array of characters.
     * \param[in] size max size of the array of characters.
     *
     * \return true if array contains a float number, false otherwise.
     */
    static bool isFloatStr(char* str, byte size = 255);

    /**
     * Checks if the array ends with a slash character.
     *
     * \param[in] buffer array of characters.
     * \param[in] len size of the array of characters.
     *
     * \return true if the last character is a '/', false otherwise.
     */
    static bool checkSlash(char* buffer, unsigned int len);

    /**
     * Creates an absolute path, appending a filename (or directory) to a generic path.
     *
     * \param[in] buffer array of characters where the absolute path will be saved.
     * \param[in] bufferSize size of the array of characters.
     * \param[in] folder generic path or folder.
     * \param[in] filename filename or directory to append
     *
     * \return true if no error, false otherwise.
     */
    static bool createPath(char* buffer, unsigned int bufferSize, char* folder,
        char* filename);

    /**
     * Creates an absolute path, appending a directory (component) and a file name to a generic path. For example:
     * createComposedPath(buffer, 128, "/tmp/", "myproject", "test.dat");
     * will create the following path: "/tmp/myproject/test.dat".
     *
     * \param[in] buffer array of characters where the absolute path will be saved.
     * \param[in] bufferSize size of the array of characters.
     * \param[in] folder generic path or folder.
     * \param[in] component directory to append
     * \param[in] filename filename or directory to append
     *
     * \return true if no error, false otherwise.
     */
    static bool createComposedPath(char* buffer, unsigned int bufferSize, char* folder, char* component, char* filename);


    /**
     * Removes tab, space characters from the end or the beginning of the string and gets the pointer to the first characters that is not a '\\t' and ' '.
     *
     * \param[in] str array of characters.
     * \param[in] size max size of the array of characters.
     * \param[in] end true remove characters from the end, false from the beginning.
     *
     * \return the pointer to the first character different from '\\t' and ' '.
     */
    static char* removeTabs(char* str, byte size = 255, bool end = false);

    /**
     * Allocates a new array of chars.
     *
     * \param[out] dest pointer to the array to allocate
     * \param[in] maxSize array size
     *
     * \return false if the array is just allocated or if there is a memory error, true otherwise.
     */
    static bool newBuffer(char* &dest, size_t maxSize);

    /**
     * Allocates a char array and copies in it a string.
     *
     * \param[out] dest pointer to the array to allocate
     * \param[out] src string to copy
     * \param[in] maxSize destination array size
     *
     * \return false if the array is just allocated or if there is a memory error, true otherwise.
     */
    static bool initBuffer(char* &dest, const char* src, size_t maxSize);

    /**
     * Copies a string into the array of chars.
     *
     * \param[out] dest pointer to the array to allocate
     * \param[out] src string to copy
     * \param[in] maxSize destination array size
     *
     * \return false if there is a memory error, true otherwise.
     */
    static bool setBuffer(char* &dest, const char* src, size_t maxSize);

    /**
     * Deletes the array of chars an puts the pointer to NULL (0 value).
     *
     * \param[out] buf pointer to the array to delete
     *
     * \return false if the array has been just deleted before, true otherwise.
     */
    static bool delBuffer(char* &buf);

    /**
     * Reads a word from flash and returns it as a string, using a global buffer ( flashBuffer ).
     *
     * \param[in] table string table stored in the flash memory
     * \param[in] index position of the string to get in the table
     * \param[out] flashBuffer array of char where save the string
     * \param[in] bSize size of flashBuffer array
     *
     * \return pointer to the string (flashBuffer)
     */
    static char* getFlashString(PROGMEM const char *table[], byte index, char* flashBuffer, size_t bSize);

    static bool convertTimeISO8601(unsigned long time, char * buffer, size_t bSize = 21);

};

#endif /* STRINGPARSER_H_ */
