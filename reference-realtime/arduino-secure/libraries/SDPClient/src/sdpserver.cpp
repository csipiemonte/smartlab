/*
 * sdpserver.cpp
 *         Arduino Library for describes a Smart Data Server.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */
#include "sdpserver.h"
#include "stringparser.h"

using namespace sdp::client;

const char* SDPServer::DEFAULT_DOMAIN = "smartdatanet.it";

SDPServer::SDPServer() :
    m_port(DEFAULT_PORT), m_defined_by_IP(false), m_isCopy(false), m_domain(0), m_service(0)
{
  // Set IP address to 0
  for (int i = 0; i < 4; i++)
  {
    m_ipAddress[i] = 0;
  }

  // Set default domain
  setDomain(DEFAULT_DOMAIN);
}

SDPServer::SDPServer(char* domain, uint16_t port) :
    m_port(port), m_defined_by_IP(false), m_isCopy(false), m_domain(0), m_service(0)
{
  // Set IP address to 0
  for (int i = 0; i < 4; i++)
  {
    m_ipAddress[i] = 0;
  }

  setDomain(domain);
}

SDPServer::SDPServer(uint8_t address[], uint16_t port) :
    m_port(port), m_defined_by_IP(true), m_isCopy(false), m_domain(0), m_service(0)
{
  // Set IP address to 0
  for (int i = 0; i < 4; i++)
  {
    m_ipAddress[i] = address[i];
  }
}

SDPServer::SDPServer(const SDPServer& s)
{
  m_isCopy = true;
  m_port = s.m_port;
  m_defined_by_IP = s.m_defined_by_IP;
  m_domain = s.m_domain;
  m_service = s.m_service;
}

SDPServer::~SDPServer()
{
  if (!m_isCopy)
  {
    StringParser::delBuffer(m_domain);
    StringParser::delBuffer(m_service);
  }
}

bool SDPServer::setIPAddress(uint8_t address[])
{
  for (int i = 0; i < 4; i++)
  {
    if (address[i] > 255)
    {
      return false;
    }
    m_ipAddress[i] = address[i];
  }

  m_defined_by_IP = true;

  return true;
}

bool SDPServer::setDomain(const char* domain)
{
  m_defined_by_IP = false;

  return StringParser::initBuffer(m_domain, (char*) domain,  DOMAIN_SIZE);
}

bool SDPServer::setService(const char* service)
{
  return StringParser::initBuffer(m_service, (char*) service,  SERVICE_SIZE);
}
