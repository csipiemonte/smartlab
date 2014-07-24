/*
 * gpsposition.cpp
 *         Arduino library to model position as GPS location.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */

#include "gpsposition.h"

GPSPosition::GPSPosition()
{
  memset(m_latitude, '\0', COODINATE_LENGTH);
  memset(m_longitude, '\0', COODINATE_LENGTH);
  memset(m_altitude, '\0', ALTITUDE_LENGTH);

}

GPSPosition::GPSPosition(const GPSPosition& p)
{
  memcpy(m_latitude, p.m_latitude, COODINATE_LENGTH);
  memcpy(m_longitude, p.m_longitude, COODINATE_LENGTH);
  memcpy(m_latitude, p.m_latitude, ALTITUDE_LENGTH);
}
GPSPosition::~GPSPosition()
{
}

GPSPosition& GPSPosition::operator=(const GPSPosition& p)
{
  memcpy(m_latitude, p.m_latitude, COODINATE_LENGTH);
  memcpy(m_longitude, p.m_longitude, COODINATE_LENGTH);
  memcpy(m_latitude, p.m_latitude, ALTITUDE_LENGTH);

  return (*this);
}

int GPSPosition::setCoordinate(const char *coord, char *dest)
{
  byte i = 0;
  byte len = strlen(coord);
  byte state = 0; // get sign (minus or plus)

  byte pow = 0;
  byte value = 0;
  bool eos = false;

  if (len >= COODINATE_LENGTH)
  {
    return INVALID_COORD_LEN;
  }

  while( i <= len  && !eos)
  {
    switch(state)
    {
      case 0 : { // look for minus or plus sign
        if (coord[i] == '-' || coord[i] == '+')
        {
          state = 1;
        }
        else
        {
          if (coord[i] >= '0' && coord[i] <= '9')
          {
            value = (coord[i] - '0');
            state = 1;
          }
          else
          {
            return INVALID_COORD_STRING_FORMAT;
          }
        }
      } break;

      case 1 : { // look for integer part
        if (!(coord[i] >= '0' && coord[i] <= '9') && coord[i] != '.')
        {
          return INVALID_COORD_STRING_FORMAT;
        }
        else
        {
          if ( coord[i] >= '0' && coord[i] <= '9' )
          {
            pow++;
            value *= 10;
            value += (coord[i] - '0') ;

            if ( pow == 3 && value > 180 )
            {
              return INVALID_COORD_VALUE;
            }

          }
          else
          {
            if (coord[i] == '.' )
            {
              state = 2;
            }
            else
            {
              return INVALID_COORD_STRING_FORMAT;
            }
          }
        }
      } break;


      case 2 : { // look for decimal part
        if (coord[i] == '\0')
        {
          if (coord[i -1] == '.')
          {
            return INVALID_COORD_STRING_FORMAT;
          }
          eos = true;
        }
        else
        {
          if (coord[i] < '0' && coord[i] > '9')
          {
            return INVALID_COORD_STRING_FORMAT;
          }
          else
          {
            if (coord[i] != '0' && value >= 180)
            {
              return INVALID_COORD_VALUE;
            }
            else
            {

            }
          }
        }
      } break;
    }
    i++;

  }

  memcpy(dest, coord, (i>len)?len:i);
  dest[((len < COODINATE_LENGTH)?len:(COODINATE_LENGTH-1))] = '\0';

  return 1;
}

int GPSPosition::setLatitude(const char *coord)
{
  return setCoordinate(coord, m_latitude);
}

int GPSPosition::setLongitude(const char *coord)
{
  return setCoordinate(coord, m_longitude);
}

int GPSPosition::setAltitude(const char *coord)
{
  byte i = 0;
  byte len = strlen(coord);
  byte state = 0; // get sign (minus or plus)

  if (len > ALTITUDE_LENGTH)
  {
    return INVALID_COORD_LEN;
  }

  // Validate format
  while( i < len )
  {
    switch(state)
    {
      case 0 : { // look for minus or plus sign
        if (coord[i] == '-' || coord[i] == '+')
        {
          state = 1;
        }
        else
        {
          if (coord[i] >= '0' && coord[i] <= '9')
          {
            state = 1;
          }
          else
          {
            return INVALID_COORD_STRING_FORMAT;
          }
        }
      } break;

      case 1 : { // look for integer part
        if (coord[i] < '0' && coord[i] > '9')
        {
          return INVALID_COORD_STRING_FORMAT;
        }
        else
        {
          if ( coord[i] >= '0' && coord[i] <= '9' )
          {

          }
          else
          {
            return INVALID_COORD_STRING_FORMAT;
          }
        }
      } break;
    }
    i++;
  }

  memset(m_altitude,0, ALTITUDE_LENGTH);
  memcpy(m_altitude, coord, len);
  m_altitude[len] = '\0';

  return 1;
}

