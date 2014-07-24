/**
 * \file networkconfig.h
 * \brief Arduino Library for modeling a network configuration.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef NETWORKCONFIG_H_
#define NETWORKCONFIG_H_

#include "Arduino.h"

/**
 * The NetworkConfig class models the network configuration. It is possible set a dynamic configuration
 * (DHCP) or a static one. In the second case, it is required to set the IP address, network mask,
 * default gateway.
 *
 */
class NetworkConfig
{
    //! Type (static or dynamic configuration)
    bool m_dhcp;

    //! IP address
    byte m_ipAddress[4];

    //! Network mask
    byte m_netmask[4];

    //! Default gateway
    byte m_gateway[4];

    //! DNS
    byte m_dns[4];

    /**
     * Copies an IP address from a source array to a destination one
     *
     * \param[out] dest destination array
     * \param[in] src source array
     *
     */
    void copyIPAddress(byte* dest, byte* src);

  public:
    /**
     * Default Constructor
     *
     */
    NetworkConfig();

    /**
     * Copy Constructor
     *
     */
    NetworkConfig(const NetworkConfig& nc);

    /**
     * Destructor
     */
    virtual ~NetworkConfig();

    /**
     * Gets the IP address.
     *
     * \return IP address as constant pointer to an array of 4 byte (octects)
     */
    const byte* ip() const
    {
      return (const byte*) &m_ipAddress;
    }
    ;

    /**
     * Sets the IP address.
     *
     * \param[in] address IP Address
     *
     */
    void setIPAddress(byte address[]);

    /**
     * Gets the network mask.
     *
     * \return network mask as constant pointer to an array of 4 byte (octects)
     */
    const byte* netmask() const
    {
      return (const byte*) &m_netmask;
    }
    ;

    /**
     * Sets the network mask.
     *
     * \param[in] address mask
     *
     */
    void setNetmask(byte address[]);

    /**
     * Gets the IP address of the default gateway.
     *
     * \return IP address of the default gateway as constant pointer to an array of 4 byte (octects)
     */
    const byte* gateway() const
    {
      return (const byte*) &m_gateway;
    }
    ;

    /**
     * Sets the IP address of the default gateway.
     *
     * \param[in] address IP address of the default gateway
     *
     */
    void setGateway(byte address[]);

    /**
     * Gets the IP address of the DNS.
     *
     * \return IP address of the DNS as constant pointer to an array of 4 byte (octects)
     */
    const byte* dns() const
    {
      return (const byte*) &m_dns;
    }
    ;

    /**
     * Sets the DNS IP address.
     *
     * \param[in] address IP address of the DNS
     *
     */
    void setDNS(byte address[]);

    /**
     * Gets DHCP state (true if it is enabled, false otherwise)
     *
     * \return DHCP state
     */
    const bool dhcp() const
    {
      return m_dhcp;
    }
    ;

    /**
     * Enables/disables dhcp.
     *
     * \param[in] dhcp dhcp state (true if enable, false otherwise)
     *
     */
    void setDHCP(bool dhcp)
    {
      m_dhcp = dhcp;
    }
    ;

};

#endif /* NETWORKCONFIG_H_ */
