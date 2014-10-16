/**
 * \file wifimanager.h
 * \brief Arduino Library for handling a WiFi connection.
 *
 * \author Marco Boeris Frusca
 *
 */

#ifndef WIFIMANAGER_H_
#define WIFIMANAGER_H_

#include "Arduino.h"
#include <WiFi.h>
#include "networkconfig.h"
/**
 * The WiFiManager class create an interface to handle a WiFi connection. You can set a
 * #NetworkConfig and select WEP, WPA transmission coding or a open wireless network.
 */
class WiFiManager
{
  public:
    //! Connection type
    byte m_connectionType;

    //! ssid
    String m_ssid;

    //! keyword
    String m_key;

    //! key Index number (WEP connection)
    int m_keyIndex;

    //! Pin reset WiFi shield
    int m_pinReset;

  public:

    /**
     * \enum     _wireless_attruibutes
     *
     * WiFi connection internal parameters.
     */
    enum _wireless_attruibutes
    {
      //! Maximum time to wait to connect to the wireless network
      WIFI_CONNECTION_TIMEOUT = 60000,

      //! Number of the analog pin used to reset the WiFi shield (this operation requires an hardware modification).
      RESET_WIFI_PIN = 13,

    };

    /**
     * \enum     _wireless_connection_type
     *
     * Transmission coding of the wireless network.
     */
    enum _wireless_connection_type
    {
      //! Open, no cryptography
      WL_OPEN = 0,

      //! WEP
      WL_WEP = 1,

      //! WPA
      WL_WPA = 2,
    };

    //! Valid firmware version
    static char* FIRMWARE_VERSION;

    /**
     * Default Constructor
     *
     */
    WiFiManager();

    /**
     * Constructor
     *
     * \param[in] type wireless connection type. Valid values are WL_OPEN, WL_WEP or WL_WPA (#_wireless_connection_type)
     * \param[in] ssid wireless SSID
     * \param[in] key wireless key (if connection type is  WL_WEP or WL_WPA). Not used for WL_OPEN
     * \param[in] keyIndex (if connection type is  WL_WEP).
     *
     */
    WiFiManager(unsigned int type, const char* ssid, const char* key = 0,
        int keyIndex = 0);

    /**
     * Copy Constructor
     *
     */
    WiFiManager(const WiFiManager& wl);

    /**
     * Destructor
     */
    virtual ~WiFiManager();

    /**
     * Gets status of WiFi connection. It returns:
     * WL_NO_SHIELD,  WL_IDLE_STATUS,  WL_NO_SSID_AVAIL, WL_SCAN_COMPLETED, WL_CONNECTED, WL_CONNECT_FAILED, WL_CONNECTION_LOST, WL_DISCONNECTED
     *
     * \return status value (defined into the WiFi/utility/wl_definitions.h file)
     */
    byte status()
    {
      return WiFi.status();
    }
    ;

    /**
     * Checks if shield is present.
     *
     * \return true if shield is connected, false otherwise
     */
    byte isShieldConnected()
    {
      return !(WiFi.status() == WL_NO_SHIELD);
    }
    ;

    /**
     * Checks if firmware version is valid (1.1.0).
     *
     * \return true if firmware version is valid, false otherwise
     */
    bool isFirmwareValid();

    /**
     * Sets type of wireless connection
     *
     * \param[in] type type of wireless connection
     */
    void setType(int type)
    {
      m_connectionType = type;
    }
    ;

    /**
     * Gets wireless SSID
     *
     * \return wireless SSID
     */
    int type()
    {
      return m_connectionType;
    }
    ;

    /**
     * Sets wireless SSID
     *
     * \param[in] ssid wireless SSID
     */
    void setSSID(const char* ssid)
    {
      m_ssid = ssid;
    }
    ;

    /**
     * Gets wireless SSID
     *
     * \return wireless SSID
     */
    const char* ssid()
    {
      return m_ssid.c_str();
    }
    ;

    /**
     * Sets wireless key
     *
     * \param[in] key wireless key
     */
    void setKey(const char* key)
    {
      m_key = key;
    }
    ;

    /**
     * Gets wireless key
     *
     * \return wireless key
     */
    const char* key()
    {
      return m_key.c_str();
    }
    ;

    /**
     * Sets the reset pin
     *
     * \param[in] pin number of pin used to reset the shield WiFi
     */
    void setResetPin(int pin)
    {
      m_pinReset = pin;
    }
    ;

    /**
     * Gets the number of the reset pin
     *
     * \return number of the reset pin
     */
    int resetPin()
    {
      return m_pinReset;
    }
    ;

    /**
     * Sets wireless key (for WEp coding)
     *
     * \param[in] keyIndex wireless key
     */
    void setKeyIndex(int keyIndex)
    {
      m_keyIndex = keyIndex;
    }
    ;

    /**
     * Gets wireless keyIndex (for WEp coding)
     *
     * \return wireless keyIndex
     */
    int keyIndex()
    {
      return m_keyIndex;
    }
    ;

    /**
     * Connects to the wireless network
     *
     * \return true if connected, false otherwise
     */
    bool connect();

    /**
     * Gets connection status
     *
     * \return true if connected, false otherwise
     */
    bool isConnect() {return WiFi.status() == WL_CONNECTED;};

    /**
     * Checks if the wireless connection is active. If there is no connection, a new
     * connect operation will be done.
     *
     * \return true if connected, false otherwise
     */
    bool refresh();

    /**
     * Configures the network (IP Address, DNS, network mask, gateway).
     *
     * \param[in] netConfig network configuration
     *
     * \return true if connected, false otherwise
     */
    void config(NetworkConfig &netConfig);

    /**
     * Resets the WiFi shield
     *
     */
    void resetShield();

};

#endif /* WIFIMANAGER_H_ */
