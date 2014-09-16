/*
 * sdpserver.cpp
 *         Arduino Library for describes a Smart Data Server.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */
#include "sdpserver.h"

using namespace sdp::client;

const char* SDPServer::DEFAULT_DOMAIN = "smartdatanet.it";

SDPServer::SDPServer() :
    m_port(DEFAULT_PORT), m_defined_by_IP(false)
{
  // Set IP address to 0
  for (int i = 0; i < 4; i++)
  {
    m_ipAddress[i] = 0;
  }

  // set service page to NULL
  memset(m_service, 0, SERVICE_SIZE);
  memset(m_domain, 0, DOMAIN_SIZE);
  memcpy(m_domain, DEFAULT_DOMAIN, strlen(DEFAULT_DOMAIN));
}

SDPServer::SDPServer(char* domain, uint16_t port) :
    m_port(port), m_defined_by_IP(false)
{
  // Set IP address to 0
  for (int i = 0; i < 4; i++)
  {
    m_ipAddress[i] = 0;
  }

  // set service page to NULL
  memset(m_service, 0, SERVICE_SIZE);
  memset(m_domain, 0, DOMAIN_SIZE);
  memcpy(m_domain, domain, strlen(domain));
}

SDPServer::SDPServer(uint8_t address[], uint16_t port) :
    m_port(port), m_defined_by_IP(true)
{
  // Set IP address to 0
  for (int i = 0; i < 4; i++)
  {
    m_ipAddress[i] = address[i];
  }

  // set service page to NULL
  memset(m_service, 0, SERVICE_SIZE);
  memset(m_domain, 0, DOMAIN_SIZE);
}

SDPServer::~SDPServer()
{
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

void SDPServer::setDomain(const char* domain)
{
  int len = strlen(domain);
  memset(m_domain, 0, DOMAIN_SIZE);
  memcpy(m_domain, domain, (len > DOMAIN_SIZE) ? (DOMAIN_SIZE - 1) : len);

  m_defined_by_IP = false;
}

void SDPServer::setService(const char* service)
{
  int len = strlen(service);
  memset(m_service, 0, SERVICE_SIZE);
  memcpy(m_service, service, (len > SERVICE_SIZE) ? (SERVICE_SIZE - 1) : len);
}
