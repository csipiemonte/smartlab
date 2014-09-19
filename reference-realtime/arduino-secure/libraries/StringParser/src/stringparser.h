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

class StringParser
{
  public:
    StringParser();
    virtual ~StringParser();

    static bool toInt(char* str, byte &n, byte size = 255);

    static bool toInt(char* str, int &n, byte size = 255);

    static bool toInt(char* str, unsigned int &n, byte size);

    static bool toLong(char* str, long &n, byte size);

    static bool toLong(char* str, unsigned long &n, byte size);

    static bool toFloat(char* str, float &n, byte size);

    static bool toIPAddress(char* str, byte*ip, byte size = 255);

    static bool isDigitStr(char* str, byte size = 255);

    static bool isIPAddress(char* str, byte size = 255);

    static bool isFloatStr(char* str, byte size = 255);

    static bool checkSlash(char* buffer, unsigned int len);

    static bool createPath(char* buffer, unsigned int bufferSize, char* folder, char* filename);

    static bool createComposedPath(char* buffer, unsigned int bufferSize, char* folder, char* component, char* filename);

    static char* removeTabs(char* str, byte size = 255, bool end = false);
};

#endif /* STRINGPARSER_H_ */
