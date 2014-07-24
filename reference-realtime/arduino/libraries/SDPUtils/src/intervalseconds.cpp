/*
 * intervalseconds.cpp
 *         Arduino library to handle time interval using second as unit.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */

#include "intervalseconds.h"

IntervalSeconds::IntervalSeconds()
{
  // TODO Auto-generated constructor stub
  m_start = now();
  memset(m_buffer, 0, SIZE_BUFFER);
}

IntervalSeconds::IntervalSeconds(unsigned long start)
{
  m_start = start;
  memset(m_buffer, 0, SIZE_BUFFER);
}

IntervalSeconds::IntervalSeconds(const IntervalSeconds& s)
{
  m_start = s.m_start;
  memcpy(m_buffer, s.m_buffer, SIZE_BUFFER);
}

IntervalSeconds::~IntervalSeconds()
{
  // TODO Auto-generated destructor stub
}

IntervalSeconds& IntervalSeconds::operator=(const IntervalSeconds& s)
{
  m_start = s.m_start;

  return *this;
}

unsigned long IntervalSeconds::time()
{
  unsigned long _now = now();
  return (_now - m_start);
}

void IntervalSeconds::reset(unsigned long start)
{
  if (start != 0)
  {
    m_start = start;
  } else
  {
    m_start = now();
    ;
  }
}

char *IntervalSeconds::getTimeAsString(byte format)
{
  int hh = hour(m_start);
  int mm = minute(m_start);
  int ss = second(m_start);
  int DD = day(m_start);
  int MM = month(m_start);
  int YY = year(m_start);

  char *pYear = 0;
  char *pMonth = 0;
  char *pDay = 0;
  char *pHour = 0;
  char *pminute = 0;
  char *pSecond = 0;

  // Set constant char into string (form used is xx/xx/xxxx xx:xx:xx
  m_buffer[2] = '/';
  m_buffer[5] = '/';
  m_buffer[10] = ' ';
  m_buffer[13] = ':';
  m_buffer[16] = ':';
  m_buffer[19] = '\0';

  switch (format)
  {
    case EN:
    {
      pYear = &(m_buffer[6]);
      pMonth = &(m_buffer[0]);
      pDay = &(m_buffer[3]);
      pHour = &(m_buffer[11]);
      pminute = &(m_buffer[14]);
      pSecond = &(m_buffer[17]);
    }
      break;
    case IOT:
    {
      pYear = &(m_buffer[6]);
      pMonth = &(m_buffer[0]);
      pDay = &(m_buffer[3]);
      pHour = &(m_buffer[11]);
      pminute = &(m_buffer[14]);
      pSecond = 0;
      m_buffer[16] = '\0';
    }
      break;
    default:
    {
      pYear = &(m_buffer[6]);
      pMonth = &(m_buffer[3]);
      pDay = &(m_buffer[0]);
      pHour = &(m_buffer[11]);
      pminute = &(m_buffer[14]);
      pSecond = &(m_buffer[17]);
    }
      break;
  }

  // Month
  if (MM < 10)
  {
    pMonth[0] = '0';
    pMonth[1] = '0' + MM;
  } else
  {
    pMonth[0] = '0' + ((MM / 10) % 10);
    pMonth[1] = '0' + (MM % 10);
  }

  // Day
  if (DD < 10)
  {
    pDay[0] = '0';
    pDay[1] = '0' + DD;
  } else
  {
    pDay[0] = '0' + ((DD / 10) % 10);
    pDay[1] = '0' + (DD % 10);
  }

  //Year
  pYear[0] = '0' + ((YY / 1000) % 10);
  pYear[1] = '0' + ((YY / 100) % 10);
  pYear[2] = '0' + ((YY / 10) % 10);
  pYear[3] = '0' + (YY % 10);

  //Hour
  if (hh >= 24)
  {
    //TODO: testare se questa condizione e' davvero necessaria: ovvero alla mezzanotte restituisce il valore 24?
    pHour[0] = '0';
    pHour[1] = '0';
  } else
  {
    if (hh < 10)
    {
      pHour[0] = '0';
      pHour[1] = '0' + hh;
    } else
    {
      pHour[0] = '0' + ((hh / 10) % 10);
      pHour[1] = '0' + (hh % 10);
    }
  }

  //Minute
  if (mm < 10)
  {
    pminute[0] = '0';
    pminute[1] = '0' + mm;
  } else
  {
    pminute[0] = '0' + ((mm / 10) % 10);
    pminute[1] = '0' + (mm % 10);
  }

  if (pSecond)
  {
    //Seconds
    if (ss < 10)
    {
      pSecond[0] = '0';
      pSecond[1] = '0' + ss;
    } else
    {
      pSecond[0] = '0' + ((ss / 10) % 10);
      pSecond[1] = '0' + (ss % 10);
    }
  }
  // ....
  return m_buffer;
}

