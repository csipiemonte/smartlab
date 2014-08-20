/*
 * ntpclient.cpp
 *         Arduino Library for NTP protocol. This class implements a NTP client
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */
#include "ntpclient.h"

const IPAddress DEFAULT_NTP_SERVER(193, 204, 114, 105);

NTPClient::NTPClient() :
    localPort(LOCAL_PORT), m_udp(NULL)
{
  m_address = DEFAULT_NTP_SERVER;
  memset(m_packetBuffer, 0, NTP_PACKET_SIZE);

}

NTPClient::NTPClient(IPAddress &address) :
    localPort(LOCAL_PORT), m_udp(NULL)
{
  m_address = address;
  memset(m_packetBuffer, 0, NTP_PACKET_SIZE);

}

NTPClient::NTPClient(const NTPClient &n)
{
  m_address = n.m_address;
  memcpy(m_packetBuffer, n.m_packetBuffer, NTP_PACKET_SIZE);
  localPort = n.localPort;
  m_udp = n.m_udp;
}

NTPClient::~NTPClient()
{
  // TODO Auto-generated destructor stub
}

void NTPClient::createPacket()
{
  // set all bytes in the buffer to 0
  memset(m_packetBuffer, 0, NTP_PACKET_SIZE);

  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  //Serial.println("2");
  m_packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  m_packetBuffer[1] = 0;     // Stratum, or type of clock
  m_packetBuffer[2] = 6;     // Polling Interval
  m_packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  m_packetBuffer[12] = 49;
  m_packetBuffer[13] = 0x4E;
  m_packetBuffer[14] = 49;
  m_packetBuffer[15] = 52;

}

int NTPClient::setSystemTime()
{
  //the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:
  unsigned long highWord = word(m_packetBuffer[40], m_packetBuffer[41]);
  unsigned long lowWord = word(m_packetBuffer[42], m_packetBuffer[43]);

  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  time_t ntptime = highWord << 16 | lowWord;

  // now convert NTP time into everyday time:
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;

  // subtract seventy years:
  ntptime -= seventyYears;

  setTime(ntptime);

  return 1;
}

int NTPClient::send()
{
  if (m_udp == NULL)
    return 0;

  createPacket();
  IPAddress address(m_address);

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  m_udp->beginPacket(address, 123); //NTP requests are to port 123
  //Serial.println("4");
  m_udp->write(m_packetBuffer, NTP_PACKET_SIZE);
  //Serial.println("5");
  return m_udp->endPacket();
}

int NTPClient::receive()
{
  int nbyte = m_udp->parsePacket();
  if (nbyte > 0)
  {
    // We've received a packet, read the data from it
    m_udp->read(m_packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long highWord = word(m_packetBuffer[40], m_packetBuffer[41]);
    unsigned long lowWord = word(m_packetBuffer[42], m_packetBuffer[43]);

    /* DEBUG */
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;

    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    time_t ntptime = highWord << 16 | lowWord;

    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
//	    const unsigned long seventyYears = 2208988800UL;

    // subtract seventy years:
    ntptime -= seventyYears;

    setTime(ntptime);
  } else
  {
    return 0;
  }
  return 1;

}
