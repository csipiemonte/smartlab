/**
 * \file ntpclient.h
 * \brief Arduino Library for NTP protocol. This class implements a NTP client
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef NTPCLIENT_H_
#define NTPCLIENT_H_

#include "Arduino.h"
#include <Udp.h>
#include <Time.h>
#include <IPAddress.h>

/**
 * The NTPClient class implements the NTP protocol to get the current time in UTC format and the functions
 * to update the local time using the received time. This class can use either WiFiUDP or EthernetUDP.
 *
 */
class NTPClient
{
  public:
    /**
     * \enum    _ntp_attributes
     *
     * NTP client paramater.
     */
    enum _ntp_attributes
    {

      //! UDP receive port
      LOCAL_PORT = 2390,

      //! NTP time stamp is in the first 48 bytes of the message
      NTP_PACKET_SIZE = 48,
    };

    //! Default NTP server address
    static const IPAddress DEFAULT_NTP_SERVER;

    /**
     * Default Constructor
     *
     */
    NTPClient();

    /**
     * Constructor
     *
     * \param[in] address IP address of the NTP server
     *
     */
    NTPClient(IPAddress &address);

    /**
     * Copies Constructor
     */
    NTPClient(const NTPClient &n);

    /**
     * Destructor
     */
    virtual ~NTPClient();

    /**
     * Sets UDP connection. It can be EthernetUDP or WiFiUDP.
     *
     * \param udp UDP connection
     *
     * \return
     *
     */
    void setUDP(UDP *udp)
    {
      m_udp = udp;
    }
    ;

    /**
     * Processes NTP response to get current time in UTC format and updates the system time.
     *
     * \return
     *
     */
    int setSystemTime();

    /**
     * Sends an NTP request to the server.
     *
     * \return number of byte sent.
     *
     */
    virtual int send(); // = 0;

    /**
     * Receivers a packet from NTP server and save it into internal buffer.
     *
     * \return number of byte sent.
     *
     */
    virtual int receive(); // = 0;

    /**
     * Opens UDP connection
     *
     * \return  1: if successful, 0: if there are no sockets available to use
     */
    virtual int on()
    {
      return 1;
    }
    ; // = 0;

    /**
     * Close UDP connection
     *
     * \return  1: if successful, 0: otherwise
     */
    virtual int off()
    {
      return 1;
    }
    ; // = 0;

  protected:
    /**
     * Creates a NTP request.
     *
     */
    void createPacket();

    //! IP address of the NTP server
    IPAddress m_address;

    //! Local port to listen for UDP packets
    unsigned int localPort;

    //! Buffer to hold incoming and outgoing packets
    byte m_packetBuffer[NTP_PACKET_SIZE];

  private:
    //! UDP connection
    UDP* m_udp;

};

#endif /* NTPCLIENT_H_ */
