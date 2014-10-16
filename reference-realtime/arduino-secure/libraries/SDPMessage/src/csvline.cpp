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
 * cspvine.cpp
 *         <description>
 *
 * Authors:
 *         Marco Boeris Frusca <marco.boeris@csp.it>
 *
 */

#include "Arduino.h"
#include "csvline.h"

namespace sdp
{
  namespace message
  {
    const size_t CSVLine::LINE_SIZE = 128;
    const char CSVLine::FS = ';';

    CSVLine::CSVLine() :
        m_nf(0), m_fields(0), m_isCopy(false), m_line(0), m_length(0)
    {
    }

    CSVLine::CSVLine(const CSVLine& l)
    {
      m_nf = l.m_nf;
      m_length = l.m_length;
      m_line = l.m_line;
      m_fields = l.m_fields;
      m_isCopy = true;
    }

    CSVLine::~CSVLine()
    {
      if (m_fields != 0 && !m_isCopy)
      {
        delete[] m_fields;
        m_fields = 0;
        m_nf = 0;
      }

      if (m_line != 0 && !m_isCopy)
      {
        delete[] m_line;
        m_line = 0;
      }
    }

    void CSVLine::set(const char* line, size_t length)
    {
      size_t m_length = LINE_SIZE + 1;

      // Check line size
      if (length > strlen(line))
      {
        length = strlen(line);
      }

      if (length < m_length)
      {
        m_length = length + 1;
      }

      if (m_line != 0)
      {
        delete[] m_line;
        m_line = 0;
      }

      if (m_fields != 0)
      {
        delete[] m_fields;
        m_fields = 0;
      }

      // Create line
      m_line = new char[m_length];
      memset(m_line, 0, m_length);
      memcpy(m_line, line, m_length - 1);
      length = strlen(m_line);
      if (m_line[length - 1] == FS)
      {
        m_line[length -1] = 0;
      }

      m_nf = 0;

      for (size_t i = 0; i < m_length; i++)
      {
        if (m_line[i] == FS)
        {
          m_nf++;
        }
      }

      if (m_length > 2 && m_line[length - 1] != FS && m_line[length] == 0)
      {
        m_nf++;
      }

      m_fields = new size_t[m_nf];
      memset(m_fields, 0, sizeof(size_t) * m_nf);


      size_t j = 0;
      size_t i = 0;
      for (; i < m_length; i++)
      {
        if (i == 0)
        {
          m_fields[j++] = i;
        }

        if ( i > 0 && j < m_nf && m_line[i -1] == FS)
        {
          m_line[i -1] = 0;
          m_fields[j++] = i;
        }
      }
    }

    bool CSVLine::getItemAsInt(size_t index, int &n)
    {
      char* field = getItem(index);
      return StringParser::toInt(field, n);
    }


    bool CSVLine::getItemAsLong(size_t index, long &n)
    {
      char* field = getItem(index);
      return StringParser::toLong(field, n, strlen(field));
    }

  } /* namespace message */
} /* namespace sdp */
