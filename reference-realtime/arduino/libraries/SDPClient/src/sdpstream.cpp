/*
 * sdpstream.cpp
 *         Arduino library for modeling a SDP data stream.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */
#include "sdpstream.h"

SDPStream::SDPStream() :
    m_sensor(NULL), m_mobile(false)
{
  memset(m_identifier, 0, ID_SIZE);
}

SDPStream::SDPStream(const SDPStream &s)
{
  m_sensor = s.m_sensor;
  m_mobile = s.m_mobile;
  setID((const char*) s.m_identifier);
}

SDPStream::~SDPStream()
{
}

void SDPStream::setID(const char* id)
{
  uint8_t len = strlen(id);
  memset(m_identifier, 0, ID_SIZE);
  memcpy(m_identifier, id, (len > ID_SIZE) ? ID_SIZE : len);
}

