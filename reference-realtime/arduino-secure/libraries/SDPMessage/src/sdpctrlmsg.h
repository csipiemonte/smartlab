/**
 * \file sdpctrlmsg.h
 * \brief Arduino Library for describes a SDP control message.
 *
 * \author Marco Boeris Frusca
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

    /**
     * Class SDPCtrlMsg model a proposal SDP control message
     *
     */
    class SDPCtrlMsg: public JSON
    {
      public:
        /**
         * Default Constructor.
         *
         */
        SDPCtrlMsg();

        /**
         * Destructor
         *
         */
        virtual ~SDPCtrlMsg();

        /**
         * Get field "to" from the control message as a char array
         *
         * \return field "to" or 0 if error
         */
        char* to();

        /**
         * Get field "msg" from the control message as a char array
         *
         * \return field "msg" or 0 if error
         */
        char* msg();

        /**
         * Get field "data" from the control message as a char array
         *
         * \return field "data" or 0 if error
         */
        char* data();

        /**
         * Get field "data" from the control message ad object JSON
         *
         * \return field "data" or 0 if error
         */
        aJsonObject* dataObj();

        /**
         * Check if the control message is broadcast (for all node) or not
         *
         * \return true if the message is broadcastm false otherwise
         */
        bool isToAll();

      private:
        //! field "to"
        aJsonObject* m_to;

        //! field "msg"
        aJsonObject* m_msg;

        //! field "data"
        aJsonObject* m_data;

    };

    class SDPCtrlMsgHandler: public JSON
    {
      public:
        /**
         * \enum    _sdpctrlmsg_type
         *
         * Message type
         */
        enum _sdpctrlmsg_type
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

        //! Control message 0
        static const char cmd0[] PROGMEM;

        //! Control message 1
        static const char cmd1[] PROGMEM;

        //! Control message 2
        static const char cmd2[] PROGMEM;

        //! Control message 3
        static const char cmd3[] PROGMEM;

        //! Control message 4
        static const char cmd4[] PROGMEM;

        //! Control message 5
        static const char cmd5[] PROGMEM;

        //! Control message 6
        static const char cmd6[] PROGMEM;

        //! Control message 7
        static const char cmd7[] PROGMEM;

        //! Control message 8
        static const char cmd8[] PROGMEM;

        //! Control message 9
        static const char cmd9[] PROGMEM;

        //! Control message 10
        static const char cmd10[] PROGMEM;

        //! Control message 11
        static const char cmd11[] PROGMEM;

        //! Control message 12
        static const char cmd12[] PROGMEM;

        //! Number of the control message supported (13 + invalid)
        static const uint8_t N_TYPE = 14;

        static const char* const crtl_msg_type[] PROGMEM;

        /**
         * Default Constructor.
         *
         */
        SDPCtrlMsgHandler() {};

        /**
         * Destructor
         *
         */
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

        /**
         * Gets the type of the control message from the msg field following #_sdpctrlmsg_type format
         *
         * \param[in] msg control message
         *
         * \return type of the control message following #_sdpctrlmsg_type format
         */
        uint8_t getType( sdp::message::SDPCtrlMsg &msg );


    };

  } /* namespace message */
} /* namespace sdp */

#endif /* SDPCTRLMSG_H_ */
