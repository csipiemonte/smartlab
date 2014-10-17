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
 * stringparser.cpp
 *         <short description>
 *
 * Authors:
 *         Marco Boeris Frusca <marco.boeris@csp.it>
 *         boeris
 *
 */

#include "stringparser.h"
#include <Time.h>

StringParser::StringParser()
{
  // TODO Auto-generated constructor stub

}

StringParser::~StringParser()
{
  // TODO Auto-generated destructor stub
}

bool StringParser::isDigitStr(char* str, byte size)
{
  // Check if is a nmber
  byte i = 0;
  for (; i < strlen(str) && (i < size); i++)
  {
    if (!isDigit(str[i]) /*str[i] < '0' || str[i] > '9'*/)
    {
      return false;
    }
  }

  if (i >= size)
  {
    return false;
  }

  return true;
}

bool StringParser::isIPAddress(char* str, byte size)
{
  char n[4] = {0};
  byte iStr = 0;
  char* p = &str[iStr];

  Serial.print("isIPAddress");
  Serial.println(str);

  for (byte j = 0; j < 4; j++)
  {

    byte i = 0;
    for (i = 0; i < 4 && p[iStr] != '.' && p[iStr] != '\0'; i++)
    {
      n[i] = p[iStr++];

      if (n[i] < '0' || n[i] > '9')
      {
        break;
      }
    }
    if ( p[iStr] == '.' || p[iStr] == '\0')
    {
      n[i] = '\0';
      memset(n, 0, 4);
      iStr++;
    }
    else
    {
      return false;
    }

  }
  return true;
}


bool StringParser::toIPAddress(char* str, byte*ip, byte size)
{
  char *field;
  char *p = str;
  byte nWord = 0;

  while ((field = strtok_r(p, ".", &p)) != NULL)  // Don't use \n here it fails
  {
    if (nWord < 4)
    {
      byte n = 0;
      if (!StringParser::toInt(field, n, strlen(field ) + 1))
      {
        return false;
      }
      else
      {
        if (n > 255)
        {
          return false;
        }
        ip[nWord++] = n;
      }
    }
    else
    {
      return false;
    }
  }

  return true;
}



bool StringParser::isFloatStr(char* str, byte size)
{
  // Check if is a nmber
  byte i = 0;
  byte state = 0; // decimal part
  bool d = false;
  if (str[0] == '+' || str[0] == '-')
  {
    i++;
  }

  for (; i < strlen(str) && (i < size); i++)
  {
    if (str[i] == '.' && d)
    {
      state = 1;
    }
    else
    {
      if (!isDigit(str[i]) && state == 0)
      {
        return false;
      }
      else
      {
        d = true;
      }
      if (!isDigit(str[i]) && state == 1)
      {
        return false;
      }
    }
  }

  if (i >= size)
  {
    return false;
  }

  return true;
}


bool StringParser::toInt(char* str, byte &n, byte size)
{
  bool ret = true;

  if ( !isDigitStr(str, size) )
  {
    ret = false;
  }
  else
  {
    n = atoi(str);
  }

  return ret;
}

bool StringParser::toInt(char* str, int &n, byte size)
{
  bool ret = true;

  if ( !isDigitStr(str, size) )
  {
    ret = false;
  }
  else
  {
    n = atoi(str);
  }

  return ret;
}


bool StringParser::toInt(char* str, unsigned int &n, byte size)
{
  bool ret = true;

  if ( !isDigitStr(str, size) )
  {
    ret = false;
  }
  else
  {
    n = (unsigned int) atol(str);
  }

  return ret;
}

bool StringParser::toLong(char* str, unsigned long &n, byte size)
{
  bool ret = true;

  if ( !isDigitStr(str, size) )
  {
    ret = false;
  }
  else
  {
    n = atol(str);
  }

  return ret;
}

bool StringParser::toLong(char* str, long &n, byte size)
{
  bool ret = true;

  if ( !isDigitStr(str, size) )
  {
    ret = false;
  }
  else
  {
    n = atol(str);
  }

  return ret;
}

bool StringParser::toFloat(char* str, float &n, byte size)
{
  bool ret = true;

  if ( !isFloatStr(str, size) )
  {
    ret = false;
  }
  else
  {
    n = atof(str);
  }

  return ret;
}


bool StringParser::checkSlash(char* buffer, unsigned int len)
{
  unsigned int length = strlen(buffer);

  if ((len == 0) || (length > (len - 2))) // (len - 2) because one char is eos, and one is '/' to add
  {
    return false;
  }

  if (buffer[length - 1] != '/')
  {
    buffer[length++] = '/';
    buffer[length] = '\0';
  }

  return true;
}

bool StringParser::createPath(char* buffer, unsigned int bufferSize, char* folder, char* filename)
{
  unsigned int len1 = strlen(folder);
  unsigned int len2 = strlen(filename);

  byte nSlash = ((folder[len1] == '/') ? 0 : 1);
  char *p = NULL;

  // Check sizes
  if ((bufferSize - 1) < (len1 + len2 + nSlash))
  {
    return false;
  }

  memset(buffer, 0, bufferSize);

  // Copy main folder
  memcpy(buffer, folder, len1);

  // Add if necessary '/' char to the string
  if (!StringParser::checkSlash(buffer, (unsigned int) bufferSize))
  {
    return false;
  }

  // Update string length and go to last char of that one
  len1 = strlen(buffer);
  p = &buffer[len1];

  // Add filename
  memcpy(p, filename, len1);

  return true;
}

bool StringParser::createComposedPath(char* buffer, unsigned int bufferSize, char* folder, char* component, char* filename)
{
  unsigned int len1 = strlen(folder);
  unsigned int len2 = strlen(component);
  unsigned int len3 = strlen(filename);

  byte nSlash = ((folder) ? 0 : 1)
      + ((component[len2] == '/') ? 0 : 1);
  char *p = NULL;

  // Check sizes
  if ((bufferSize - 1) < (len1 + len2 + len3 + nSlash))
  {
    return false;
  }

  memset(buffer, 0, bufferSize);

  // Copy main folder
  memcpy(buffer, folder, len1);

  // Add if necessary '/' char to the string
  if (!checkSlash(buffer, (unsigned int) bufferSize))
  {
    return false;
  }

  // Update string length and go to last char of that one
  len1 = strlen(buffer);
  p = &buffer[len1];

  // Add component folder
  memcpy(p, component, len2);

  // Add if necessary '/' char to the string
  if (!checkSlash(buffer, (unsigned int) bufferSize))
  {
    return false;
  }

  // Update string length and go to last char of that one
  len1 = strlen(buffer);
  p = &buffer[len1];

  // Add filename
  memcpy(p, filename, len3);

  return true;
}

char* StringParser::removeTabs(char* str, byte size, bool end)
{
  char* p = str;
  if (!end)
  {
    for (byte i = 0; (i < size && p[i] == ' ' && p[i] == '\t'); i++)
    {
      p++;
    }
  }
  else
  {
    int len = strlen(str);

    len = (len > size)?size:len;

    if (str[len] != '\0')
    {
      return NULL;
    }

    for (byte i = len - 1; i >= 0 && (str[i] == ' ' || str[i] == '\t'); i--)
    {
      str[i] = '\0';
    }

  }
  return p;
}

bool StringParser::newBuffer(char* &dest, size_t maxSize)
{
  if (dest != 0)
  {
    return false;
  }

  dest = new char[maxSize];
  if (dest == 0)
  {
    return false;
  }

  memset(dest, 0, maxSize);

  return true;
}

bool StringParser::initBuffer(char* &dest, const char* src, size_t maxSize)
{
  // Delete old string

  if (delBuffer(dest))
  {
  }

  return setBuffer(dest, src, maxSize);
}

bool StringParser::setBuffer(char* &dest, const char* src, size_t maxSize)
{
  // Get length source string
  size_t length = strlen(src) + 1;
  length = (maxSize > length)?length:maxSize;


  if (!newBuffer(dest, length))
  {
    return false;
  }

  memcpy(dest, src, length - 1);

  return true;

}
bool StringParser::delBuffer(char* &buf)
{
  if (buf != 0)
  {
    delete[] buf;
    buf = 0;
    return true;
  }
  return false;
}

char* StringParser::getFlashString(PROGMEM const char *table[], byte index, char* flashBuffer, size_t bSize)
{
  memset(flashBuffer, 0, bSize);
  strcpy_P(flashBuffer, (char*) pgm_read_word(&(table[index])));
  return flashBuffer;
}



bool StringParser::convertTimeISO8601(unsigned long time, char * buffer, size_t bSize)
{
  snprintf(buffer, bSize, "%04d-%02d-%02dT%02d:%02d:%02dZ",
            year(time), month(time), day(time),
            hour(time), minute(time), second(time));
}
