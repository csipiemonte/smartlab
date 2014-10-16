/**
 * \file sdpsource.h
 * \brief Arduino library for modeling a SDP data source.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef SDPSOURCE_H_
#define SDPSOURCE_H_

#include <Arduino.h>
#include <avr/pgmspace.h>

#include <SPI.h>
#include <SD.h>

#include "Client.h"
#include "Stream.h"
#include "sdpserver.h"
#include "sdpstream.h"
#include "measure.h"
#include "smartobjstatus.h"
#include <aJSON.h>

#include "sdpctrlmsg.h"
#include "csvline.h"

// External include
#include "PubSubClient.h"

using namespace sdp::sensor;

namespace sdp
{
  namespace client
  {

    /**
     * The SDPSource class describes the Smart Data Source. It has got all functions to create messages
     * and to communicate with the data server. This class implements the data publication using MQTT
     * protocol. The server is the MQTT broker
     *
     * \note At the moment it is possible send only one measurement with one component.
     *
     */
    class SDPSource
    {
      private:

        //! Control message (received from server)
        static char* CONTROL_MSG;

      public:
        /**
         * \enum    _sdpsource_error
         *
         * Error codes for SDP data source
         */
        enum _sdpsource_error
        {
          //! No errors
          NO_ERROR = 0,
          //! Undefined server
          UNDEFINED_SERVER = 1,
          //! Undefined client
          UNDEFINED_CLIENT = 2,
        };

        /**
         * \enum   _internal_parameter
         *
         * Internal parameter (buffer sizes) for SDP data source
         */
        enum _internal_parameter
        {
          //! Max size for client identifier
          ID_SIZE = 38,

          //! Max size for username
          USERNAME_SIZE = 15,

          //! Max size for password
          PASSWORD_SIZE = 15,

          //! HMAC keyword max size
          HMAC_KEY_SIZE = 64,

        };

        //! Default identifier value
        static const char* DEFAULT_ID;

        /**
         * Default Constructor.
         *
         */
        SDPSource();

        /**
         * Constructor.
         *
         * \param[in] server data server
         * \param[in] client connection client (it can be WiFIClient or EthernetClient).
         * \param[in] id source identifier
         */
        SDPSource(SDPServer& server, Client& client, const char* id =
            SDPSource::DEFAULT_ID);

        /**
         * Copy-Constructor
         *
         */
        SDPSource(const SDPSource& s);

        /**
         * Destructor
         *
         */
        virtual ~SDPSource();

        /**
         * Sets the HMAC keyword.
         *
         * \param[in] key HMAC keyword.
         * \param[in] length length of HMAC keyword.
         *
         */
        void setHMACKey(uint8_t * key, size_t length);

        /**
         * Sets the MQTT username.
         *
         * \param[in] user MQTT username.
         *
         * \return false if there is an error while allocating memory, true otherwise
         */
        bool setUsername(char * user);

        /**
         * Sets the MQTT password.
         *
         * \param[in] pass MQTT password.
         *
         * \return false if there is an error while allocating memory, true otherwise
         */
        bool setPassword(char * pass);

        /**
         * Sets the client identifier.
         *
         * \param[in] id client identifier.
         *
         * \return false if there is an error while allocating memory, true otherwise
         */
        bool setId(char * id);

        /**
         * Gets the client identifier.
         *
         * \return client identifier
         */
        const char* id() const
        {
          return m_id;
        }

        /**
         * Sets the Smart Data server.
         *
         * \param[in] server Smart Data server
         *
         */
        void setServer(SDPServer& server)
        {
          this->m_server = &server;
        }
        ;

        /**
         * Gets the Smart Data server.
         *
         * \return Smart Data server reference
         */
        const SDPServer* server() const
        {
          return m_server;
        }

        /**
         * Creates a MQTT connection with the Smart Data server. If field username is set, the node authenticates
         * itself on the MQTT broker.
         *
         * \return NO_ERRORS or #_sdpsource_error code error otherwise
         */
        int8_t connect();

        /**
         * Close connection to the Smart Data server.
         *
         * \return NO_ERRORS or #_sdpsource_error code error otherwise
         */
        int8_t disconnect();

        /**
         * Gets the connection state (true if client is connected to the server)
         *
         * \return connection state. true if client is connected to server, false otherwise.
         */
        int8_t isConnected()
        {
          if (m_subclient)
            return m_subclient->connected();
          else
            return false;
        }
        ;

        /**
         * This function subscribes client to a specific topic.
         *
         * \param[in] tenant first component of the topic (tenant)
         * \param[in] stream output stream
         * \param[in] sensor sensor
         *
         * \return 1 if no error, 0 oterwise
         */
        uint8_t subscribe(const char* tenant, SDPStream& stream, GenericSensor& sensor);

        /**
         * This function published an information of a specific topic.
         *
         * \param[in] topic output topic
         * \param[in] msg message to publish
         *
         * \return
         */
        uint8_t publish(char* topic, char* msg);

        /**
         * This function published an information of a specific topic.
         *
         * \param[in] topic output topic
         * \param[in] msg message to publish
         *
         * \return
         */
        uint8_t publish(SmartObjStatus & status, const char* tenant/* = DEFAULT_TENANT*/);

        /**
         * This function published a measurement.
         *
         * \param[in] stream Data stream associated to sensor
         * \param[in] measure Measure measurement to send
         * \param[in] tenant publishing tenant(or domain)
         *
         * \return
         */
        uint8_t publish(SDPStream& stream, Measure& measure,
            const char* tenant/* = DEFAULT_TENANT*/);

        /**
         * This function keep connection alive and receive message from MQTT broker.
         *
         * \return true if no errors, false otherwise
         */
        bool loop()
        {
          if (m_subclient)
            return m_subclient->loop();
          else
            return false;
        }
        ;

      protected:
        /// MQTT connection type (send to broker)
        static const char CONN_DIRECTION_I[] PROGMEM;

        /// MQTT connection type (receive from broker)
        static const char CONN_DIRECTION_O[] PROGMEM;

        /// MQTT control queue
        static const char ATTR_CONTROL[] PROGMEM;

        /// Table where store source attributes (in flash memory)
        static PGM_P const SOURCE_TABLE_P[] PROGMEM;

        /// Character used to concatenate Sensor ID and stream ID
        static const char CONC_CHAR;

        /// Default component label
        static const char* DEFAULT_COMPONENT_LABEL;

      public:
        /**
         * Gets control message (JSON object).
         *
         * \return message buffer
         */
        static char* getControlMsg()
        {
          return CONTROL_MSG;
        }

        /**
         * Deletes control message (JSON object).
         *
         */
        static void deleteControlMsg()
        {
          if (CONTROL_MSG != 0)
          {
            delete[] CONTROL_MSG;
            CONTROL_MSG = 0;
          }
        }

        /**
         * Save configuration (CVS) in a file.
         *
         * \param[in] filename name of the configuration file
         * \param[in] conf configuration as a line CSV
         *
         * \return true if no error, false otherwise
         */
        static bool saveConfiguration(char* filename, sdp::message::CSVLine &conf);

        //! Size of an internal input buffer
        static const uint16_t RBUF_SIZE;

        /**
         * Loads configuration from the configuration file (it reads only the first line).
         *
         * \param[in] filename name of the configuration file
         * \param[out] conf node configuration
         *
         * \return true if no error, false otherwise
         */
        static bool loadConfiguration(char* filename, sdp::message::CSVLine &conf);

      protected:
        /**
         * This routine is called automatically when something comes in from the broker.
         * Also use this to prepare a response.
         *
         * \param[in] topic incoming topic
         * \param[in] payload received payload
         * \param[in] length payload length
         *
         */
        static void callback(char* topic, byte* payload, unsigned int length);

      private:
        /**
         * Creates topic from three information: direction, tenant and attribute. the last parameter
         * is optional.
         *
         * \param[in] direction first element of the topic
         * \param[in] tenant second element of the topic
         * \param[in] attribute third element of the topic. It is optional, if you don't use it use NULL (or 0) value.
         * \param[out] buffer buffer where topic will be saved
         * \param[in] bSize buffer size
         *
         * \return false if buffer size is not enough big, true otherwise
         */
        bool createTopic(char* direction, char* tenant, char* attribute, char* buffer, unsigned int bSize);
      private:
        //! Smart Data server
        SDPServer* m_server;

        //! Network client (e.g. EthernetClient)
        Client* m_netclient;

        //! Client identifier
        char *m_id;
        //char m_id[ID_SIZE];

        //! Message to send to the server
        char *m_message;

        //! MQTT Username
        char *m_username;
        //char m_username[USERNAME_SIZE];

        //! MQTT Password
        char *m_password;
        //char m_password[PASSWORD_SIZE];

        //! HMAC keyword
        uint8_t m_hmacKey[HMAC_KEY_SIZE];

        //! HMAC keyword length
        size_t m_hmacKeylength;

        //! Publish subscriber client
        PubSubClient *m_subclient;

        //! Internal flag. TRUE is object is a copy
        bool m_isCopy;

    };

  }
}
#endif /* SDPSOURCE_H_ */
