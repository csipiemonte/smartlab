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
    size_t CSVLine::LINE_SIZE = 128;

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
      //memcpy(m_line, l.m_line, LINE_SIZE + 1);
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
        m_line  = 0;
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
      }

      if (m_fields != 0)
      {
        delete[] m_fields;
      }

      // Create line
      m_line = new char[m_length];
      memset(m_line, 0, m_length);
      memcpy(m_line, line, m_length - 1);

      Serial.print("line: ");
      Serial.println(m_line);
      Serial.println(m_length);

    }
  } /* namespace message */
} /* namespace sdp */
