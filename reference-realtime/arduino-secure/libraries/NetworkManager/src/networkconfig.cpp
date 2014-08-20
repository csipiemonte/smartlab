/*
 * networkconfig.cpp
 *         Arduino Library for modeling a network configuration.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */
#include "networkconfig.h"

NetworkConfig::NetworkConfig() :
    m_dhcp(true)
{
  memset(m_ipAddress, 0, 4);
  memset(m_netmask, 0, 4);
  memset(m_gateway, 0, 4);
  memset(m_dns, 0, 4);
}

NetworkConfig::NetworkConfig(const NetworkConfig& nc)
{
  m_dhcp = nc.m_dhcp;
  memcpy(m_ipAddress, nc.m_ipAddress, 4);
  memcpy(m_netmask, nc.m_netmask, 4);
  memcpy(m_gateway, nc.m_gateway, 4);
  memcpy(m_dns, nc.m_dns, 4);
}

NetworkConfig::~NetworkConfig()
{
}

void NetworkConfig::copyIPAddress(byte* dest, byte* src)
{
  memcpy(dest, src, 4);
}

void NetworkConfig::setIPAddress(byte address[])
{
  copyIPAddress(m_ipAddress, address);
}

void NetworkConfig::setNetmask(byte address[])
{
  copyIPAddress(m_netmask, address);
}

void NetworkConfig::setGateway(byte address[])
{
  copyIPAddress(m_gateway, address);
}

void NetworkConfig::setDNS(byte address[])
{
  copyIPAddress(m_dns, address);
}
