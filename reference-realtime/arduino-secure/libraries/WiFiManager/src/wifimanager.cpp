/*
 * wifimanager.cpp
 *         Arduino Library for handling a WiFi connection.
 *
 * Authors:
 *         Marco Boeris Frusca
 *
 */

#include "wifimanager.h"
#include <interval.h>
#include <avr/wdt.h>

char* WiFiManager::FIRMWARE_VERSION = "1.1.0";

WiFiManager::WiFiManager() :
    m_connectionType(WL_OPEN), m_keyIndex(0), m_pinReset(RESET_WIFI_PIN)
{
  // TODO Auto-generated constructor stub
  pinMode(m_pinReset, OUTPUT);
  digitalWrite(m_pinReset, LOW);
}

WiFiManager::WiFiManager(unsigned int type, const char* ssid, const char* key,
    int keyIndex) :
    m_connectionType(WL_OPEN), m_keyIndex(keyIndex), m_pinReset(RESET_WIFI_PIN)
{
  m_ssid = ssid;
  m_key = key;
  pinMode(m_pinReset, OUTPUT);
  digitalWrite(m_pinReset, LOW);
}

WiFiManager::WiFiManager(const WiFiManager& wl)
{
  m_connectionType = wl.m_connectionType;
  m_ssid = wl.m_ssid;
  m_key = wl.m_key;
  m_keyIndex = wl.m_keyIndex;
  m_pinReset = wl.m_pinReset;
}

WiFiManager::~WiFiManager()
{
  // TODO Auto-generated destructor stub

}

bool WiFiManager::isFirmwareValid()
{
  String fv = WiFi.firmwareVersion();
  return (fv == FIRMWARE_VERSION);
}

bool WiFiManager::connect()
{
  Interval timeout;

  //! Wifi radio's status
  int status = WL_IDLE_STATUS;

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED
      && timeout.time() <= WIFI_CONNECTION_TIMEOUT)
  {
    /*
#ifdef DEBUG
     Serial.print(F("Attempting to connect to SSID: "));
     Serial.println( (char* )m_ssid.c_str());
#endif
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

bool WiFiManager::refresh()
{
  if (WiFi.status() != WL_CONNECTED)
  {
#ifdef DEBUG
    Serial.println( F("Reconnecting wifi...") );
#endif
    WiFi.disconnect();
    // Wait 10 seconds
    for (byte i = 0; i < 5; i++)
    {
      wdt_reset();
      delay(1000);
    }

    uint8_t n = 0;

    while (!connect() && n++ < 1)
    {
      wdt_reset();
      delay(1000);
    }
  }
  return (WiFi.status() == WL_CONNECTED);
}

void WiFiManager::config(NetworkConfig &netConfig)
{
  WiFi.config(IPAddress(netConfig.ip()), IPAddress(netConfig.dns()),
      IPAddress(netConfig.gateway()), IPAddress(netConfig.netmask()));
}

void WiFiManager::resetShield()
{
  wdt_reset();

  pinMode(m_pinReset, OUTPUT);
  digitalWrite(m_pinReset, HIGH);
  delay(500);
  digitalWrite(m_pinReset, LOW);
  delay(1000);

  wdt_reset();
}
