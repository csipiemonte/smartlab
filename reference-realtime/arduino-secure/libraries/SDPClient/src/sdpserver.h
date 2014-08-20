/**
 * \file sdpserver.h
 * \brief Arduino Library for describes a Smart Data Server.
 *
 * \author Marco Boeris Frusca
 *
 */
#ifndef SDPSERVER_H_
#define SDPSERVER_H_

#include <Arduino.h>

namespace sdp
{
  namespace client
  {

    /**
     * The SDPServer class describes a Smart Data Server as a IP address, a port number and a path
     * (or service page).
     *
     */
    class SDPServer
    {
      public:

        /**
         * \enum    _server_internal_attributes
         *
         * Internal values and buffer sizes
         */
        enum _server_internal_attributes
        {
          //! Maximum length of the description field
          SERVICE_SIZE = 64,
          //! Maximum length of the domain field
          DOMAIN_SIZE = 64,
          //! Default server port number
          DEFAULT_PORT = 1883,
        };

        //! Default server common name (or domain)
        static const char* DEFAULT_DOMAIN;

        /**
         * Default Constructor.
         *
         */
        SDPServer();

        /**
         * Constructor.
         * It initializes IP address and port of the server to a given value.
         *
         * \param address IP address as array of byte
         * \param port port number
         */
        SDPServer(uint8_t address[], uint16_t port);

        /**
         * Constructor.
         * It initializes server and port of the server to a given value.
         *
         * \param domain server domain as a string
         * \param port port number
         */
        SDPServer(char* domain, uint16_t port);

        /**
         * Copy-Constructor
         *
         */
        SDPServer(const SDPServer& s);

        /**
         * Destructor
         *
         */
        virtual ~SDPServer();

        /**
         * Sets the IP address of the server.
         *
         * \param[in] address IP Address
         *
         * \return false if address is not valid, true otherwise
         */
        bool setIPAddress(uint8_t address[]);

        /**
         * Gets the IP address of the server.
         *
         * \return IP address as constant pointer to an array of 4 byte (octects)
         */
        const uint8_t* ip() const
        {
          return (const uint8_t*) &m_ipAddress;
        }
        ;

        /**
         * Sets the port of the server.
         *
         * \param[in] port port number
         *
         */
        void setPort(uint16_t port)
        {
          m_port = port;
        }
        ;

        /**
         * Gets the port number of the server.
         *
         * \return port number
         */
        uint16_t port() const
        {
          return m_port;
        }
        ;

        /**
         * Sets the domain of the server.
         *
         * \param[in] domain common name or domain of the server
         *
         */
        void setDomain(const char* domain);

        /**
         * Gets the domain of the server.
         *
         * \return server domain
         */
        const char* domain() const
        {
          return m_domain;
        }
        ;

        /**
         * Sets the service of the server.
         *
         * \param[in] service service page (or path)
         *
         */
        void setService(const char* service);

        /**
         * Gets the service of the server.
         *
         * \return service page (or path)
         */
        const char* service() const
        {
          return m_service;
        }
        ;

        /**
         * This function returns true if server is defined by IP address, false otherwise.
         *
         * \return true if server is defined by IP address, false otherwise.
         */
        bool isDefinedByIP() const
        {
          return m_defined_by_IP;
        }
        ;

      private:
        //! IP address of the server
        uint8_t m_ipAddress[4];

        //! Port number
        uint16_t m_port;

        //! Service page (To delete if POST generated from file!)
        char m_domain[SERVICE_SIZE];

        //! Service page (not used!)
        char m_service[DOMAIN_SIZE];

        //! Flag true if the server is defined by IP, false otherwise
        bool m_defined_by_IP;
    };
  }
}
#endif /* SDPSERVER_H_ */
