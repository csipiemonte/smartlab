/*
 * Copyright (C) 2014 CSP Innovazione nelle ICT s.c.a r.l. (http://www.csp.it/)
 * All rights reserved.
 *
 * All information contained herein is, and remains the property of
 * CSP Innovazione nelle ICT s.c.a r.l. and its suppliers, if any.
 * The intellectual and technical concepts contained herein are proprietary to
 * CSP Innovazione nelle ICT s.c.a r.l. and its suppliers and may be covered
 * by Italian and Foreign Patents, in process, and are protected by trade secret
 * or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from CSP Innovazione nelle ICT s.c.a r.l.
 *
 * sdpctrlmsg.h
 *         <description>
 *
 * Authors:
 *         Marco Boeris Frusca <marco.boeris@csp.it>
 *
 */

#ifndef SDPCTRLMSG_H_
#define SDPCTRLMSG_H_

#include "Arduino.h"
#include <avr/pgmspace.h>

#include "sdpbasemsg.h"

namespace sdp
{
  namespace message
  {

    class SDPCtrlMsg: public JSON
    {
      public:
        SDPCtrlMsg();
        virtual ~SDPCtrlMsg();

        char* to();

        char* msg();

        char* data();

        aJsonObject* dataObj();

        bool isToAll();

      private:
        aJsonObject* m_to;
        aJsonObject* m_msg;
        aJsonObject* m_data;

    };

    class SDPCtrlMsgHandler: public JSON
    {
      public:
        /**
         * \enum    _sdpctrlmsg_type_error
         *
         * Message type
         */
        enum _sdpsource_error
        {
          //! Unknown commad
          INVALID = 0,

          //! Update firmware
          UPDATE = 1,

          //! Restart smart object
          RESTART = 2,

          //! Enable data send
          ENABLE = 3,

          //! Restore factory configuration
          FACTORY = 4,

          //! Send current status
          STATUS = 5,

          //! Send a measurement to a test input stream
          TEST = 6,

          //! New calibrate coefficients
          CALIBRATE = 7,

          //! Set system time
          SYNC = 8,

          //! Send current configuration
          RCONFIG = 9,

          //! Save configuration on local files
          WCONFIG = 10,

          //! Load configuration from local files
          CONFIG = 11,

          //! Execute a custom command
          CMD = 12,

          //! Send data  in a fixed time range
          HISTORY = 13,
        };

        static const char cmd0[] PROGMEM;
        static const char cmd1[] PROGMEM;
        static const char cmd2[] PROGMEM;
        static const char cmd3[] PROGMEM;
        static const char cmd4[] PROGMEM;
        static const char cmd5[] PROGMEM;
        static const char cmd6[] PROGMEM;
        static const char cmd7[] PROGMEM;
        static const char cmd8[] PROGMEM;
        static const char cmd9[] PROGMEM;
        static const char cmd10[] PROGMEM;
        static const char cmd11[] PROGMEM;
        static const char cmd12[] PROGMEM;
        static const uint8_t N_TYPE = 14;

        static const char* const crtl_msg_type[] PROGMEM;

        SDPCtrlMsgHandler() {};
        virtual ~SDPCtrlMsgHandler() {};

        /**
         * Checks if the control message is broadcasted
         *
         * \param[in] msg control message
         *
         * \return true if the control message is broadcasted
         */
        bool isBroadcast( sdp::message::SDPCtrlMsg &msg )
        {
           return ( strcmp(msg.to(), "any") == 0 );
        }

        /**
         * Checks if the recipient is the smart object
         *
         * \param[in] msg control message
         * \param[in] id recipient identifier
         *
         * \return true if the control message is for the node
         */
        bool isToProcess( sdp::message::SDPCtrlMsg &msg , char* id )
        {
           return ( strcmp(msg.to(), id) == 0 );
        }

        uint8_t getType( sdp::message::SDPCtrlMsg &msg );


    };

  } /* namespace message */
} /* namespace sdp */

#endif /* SDPCTRLMSG_H_ */
