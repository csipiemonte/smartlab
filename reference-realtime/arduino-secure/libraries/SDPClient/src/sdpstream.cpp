/*
 * sdpstream.cpp
 *         Arduino library for modeling a SDP data stream.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */
#include "sdpstream.h"
#include <stringparser.h>

using namespace sdp::client;

SDPStream::SDPStream() :
    m_sensor(NULL), m_mobile(false), m_isCopy(false), m_identifier(0)
{
}

SDPStream::SDPStream(const SDPStream &s)
{
  m_sensor = s.m_sensor;
  m_mobile = s.m_mobile;
  m_isCopy = true;
  m_identifier = s.m_identifier;
}

SDPStream::~SDPStream()
{
  if (!m_isCopy)
  {
    StringParser::delBuffer(m_identifier);
  }
}

bool SDPStream::setID(const char* id)
{
  return StringParser::initBuffer(m_identifier, (char*) id, ID_SIZE);
}

