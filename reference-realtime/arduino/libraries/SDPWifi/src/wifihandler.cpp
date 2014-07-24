/*
 * wifihandler.cpp
 *         Arduino Library for handling a WiFi connection.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */

#include "wifihandler.h"
#include <interval.h>
#include <avr/wdt.h>

char* WiFiHandler::FIRMWARE_VERSION = "1.1.0";

WiFiHandler::WiFiHandler() :
    m_connectionType(WL_OPEN), m_keyIndex(0), m_pinReset(RESET_WIFI_PIN)
{
  // TODO Auto-generated constructor stub
  pinMode(m_pinReset, OUTPUT);
  digitalWrite(m_pinReset, LOW);
}

WiFiHandler::WiFiHandler(unsigned int type, const char* ssid, const char* key,
    int keyIndex) :
    m_connectionType(WL_OPEN), m_keyIndex(keyIndex), m_pinReset(RESET_WIFI_PIN)
{
  m_ssid = ssid;
  m_key = key;
  pinMode(m_pinReset, OUTPUT);
  digitalWrite(m_pinReset, LOW);
}

WiFiHandler::WiFiHandler(const WiFiHandler& wl)
{
  m_connectionType = wl.m_connectionType;
  m_ssid = wl.m_ssid;
  m_key = wl.m_key;
  m_keyIndex = wl.m_keyIndex;
  m_pinReset = wl.m_pinReset;
}

WiFiHandler::~WiFiHandler()
{
  // TODO Auto-generated destructor stub

}

bool WiFiHandler::isFirmwareValid()
{
  String fv = WiFi.firmwareVersion();
  return (fv == FIRMWARE_VERSION);
}

bool WiFiHandler::connect()
{
  Interval timeout;

  //! Wifi radio's status
  int status = WL_IDLE_STATUS;

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED
      && timeout.time() <= WIFI_CONNECTION_TIMEOUT)
  {
    /*
     Serial.print("Attempting to connect to SSID: ");
     Serial.println( (char* )m_ssid.c_str());

     // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
     debug.println("WiFi.status(): %d - timeout %ul", WiFi.status(), timeout.time());
     */

    switch (m_connectionType)
    {
      case WL_WEP:
      {
        status = WiFi.begin((char*) m_ssid.c_str(), m_keyIndex,
            (char*) m_key.c_str());
      }
        break;

      case WL_WPA:
      {
        status = WiFi.begin((char*) m_ssid.c_str(), (char*) m_key.c_str());
      }
        break;

      case WL_OPEN:

      default:
      {
        status = WiFi.begin((char*) m_ssid.c_str());
      }
        break;
    }

    // Wait 10 seconds
    for (byte i = 0; i < 10; i++)
    {
      wdt_reset();
      delay(1000);
    }

  }

  if (status != WL_CONNECTED)
  {
    return false;
  }

  return true;
}

bool WiFiHandler::refresh()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.disconnect();
    // Wait 10 seconds
    for (byte i = 0; i < 5; i++)
    {
      wdt_reset();
      delay(1000);
    }

    while (connect())
    {
      wdt_reset();
      delay(1000);
    }
  }
  return (WiFi.status() == WL_CONNECTED);
}

void WiFiHandler::config(NetworkConfig &netConfig)
{
  WiFi.config(IPAddress(netConfig.ip()), IPAddress(netConfig.dns()),
      IPAddress(netConfig.gateway()), IPAddress(netConfig.netmask()));
}

void WiFiHandler::resetShield()
{
  wdt_reset();

  pinMode(m_pinReset, OUTPUT);
  digitalWrite(m_pinReset, HIGH);
  delay(500);
  digitalWrite(m_pinReset, LOW);
  delay(1000);

  wdt_reset();
}
