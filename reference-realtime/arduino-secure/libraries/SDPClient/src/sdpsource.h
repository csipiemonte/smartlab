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
#include <SPI.h>
#include <SD.h>

#include "Client.h"
#include "Stream.h"
#include "sdpserver.h"
#include "sdpstream.h"
#include "measure.h"

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
        //! Configuration buffer (received from server)
        static sdp::message::CSVLine *configuration;

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
          ID_SIZE = 24,

/*
          //! Max size for message (or JSON) to send to the server
          MSG_SIZE = 1024,
*/
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
         */
        void setUsername(char * user)
        {
          memcpy(m_username, user, USERNAME_SIZE);
        }
        ;

        /**
         * Sets the MQTT password.
         *
         * \param[in] pass MQTT password.
         *
         */
        void setPassword(char * pass)
        {
          memcpy(m_password, pass, PASSWORD_SIZE);
        }
        ;

        /**
         * Sets the client identifier.
         *
         * \param[in] id client identifier.
         *
         */
        void setId(char * id);

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
         * This function published a measurement.
         *
         * \param[in] stream Data stream associated to sensor
         * \param[in] measure Measure measurement to send
         * \param[in] tenant publishing tenant(or domain)
         *
         * \return
         */
        uint8_t publish(SDPStream& stream, Measure& measure,
            const char* tenant = DEFAULT_TENANT);

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
      protected:
        /// Separator field (constant string)
        static const char *DEFAULT_TENANT;

        /// MQTT connection type (send)
        static const char *CONNECTION_TYPE;

        /// MQTT connection type (receive)
        static const char *CONNECTION_TYPE_1;

        /// Character used to concatenate Sensor ID and stream ID
        static const char CONC_CHAR;

        /// Default component label
        static const char* DEFAULT_COMPONENT_LABEL;

      public:
        /**
         * Gets configuration buffer.
         *
         * \return configuration buffer, 0 otherwise
         */
        static sdp::message::CSVLine *getConfiguration()
        {
          return configuration;
        }

        /**
         * Deletes configuration buffer.
         *
         */
        static void deleteConfiguration()
        {
          if (configuration != 0)
          {
            delete configuration;
            configuration = 0;
          }
        }

        /**
         * Gets configuration buffer.
         *
         * \param[in] filename name of the configuration file
         *
         * \return true if no error, false otherwise
         */
        static bool saveConfiguration(char* filename)
        {
          // Save new configuration
          //    Serial.println( F("Save on SD") );
          if (SD.exists(filename))
          {
            // Delete old configuration
            SD.remove(filename);
          }

          File myFile = SD.open(filename, FILE_WRITE);
          // if the file opened okay, write to it:
          if (myFile)
          {
            for (size_t i = 0;
                i < sdp::client::SDPSource::getConfiguration()->NF(); i++)
            {
              myFile.print(
                  sdp::client::SDPSource::getConfiguration()->getItem(i));
              myFile.print(sdp::message::CSVLine::FS);
            }
            // close the file:
            myFile.close();
            //      Serial.println();
          } else
          {
            // if the file didn't open, print an error:
            return false;
          }
          return true;

        }


        //! Size of an internal input buffer
        static  uint16_t RBUF_SIZE = 128


        /**
         * Loads configuration from the configuration file (it reads only the first line).
         *
         * \param[in] filename name of the configuration file
         * \param[out] conf node configuration
         *
         * \return true if no error, false otherwise
         */
        static bool loadConfiguration(char* filename, sdp::message::CSVLine &conf)
        {
          char rBuffer[RBUF_SIZE] = {0};
          File myFile = SD.open(filename, FILE_READ);
          // if the file opened okay, read it and get only the first line
          if (myFile)
          {
            size_t i = 0;
            bool endline = false;
            while (myFile.available() && !endline)
            {
              rBuffer[i] = myFile.read();
              if (rBuffer[i] == '\n' || i == RBUF_SIZE)
              {
                endline = true;
              }
              i++;
            }
            myFile.close();
          }
          else
          {
            return false;
          }

          conf.set(rBuffer, RBUF_SIZE);

          return true;
        }

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
        //! Smart Data server
        SDPServer* m_server;

        //! Network client (e.g. EthernetClient)
        Client* m_netclient;

        //! Client identifier
        char m_id[ID_SIZE];

        //! Message to send to the server
        char *m_message;

        //! MQTT Username
        char m_username[USERNAME_SIZE];

        //! MQTT Password
        char m_password[PASSWORD_SIZE];

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
