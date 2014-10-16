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
 * sdpctrlmsg.cpp
 *         <description>
 *
 * Authors:
 *         Marco Boeris Frusca <marco.boeris@csp.it>
 *
 */

#include "sdpctrlmsg.h"

namespace sdp
{
  namespace message
  {
    const char SDPCtrlMsgHandler::cmd0[] PROGMEM = "update";
    const char SDPCtrlMsgHandler::cmd1[] PROGMEM = "restart";
    const char SDPCtrlMsgHandler::cmd2[] PROGMEM = "enable";
    const char SDPCtrlMsgHandler::cmd3[] PROGMEM = "factory";
    const char SDPCtrlMsgHandler::cmd4[] PROGMEM = "status";
    const char SDPCtrlMsgHandler::cmd5[] PROGMEM = "test";
    const char SDPCtrlMsgHandler::cmd6[] PROGMEM = "calibrate";
    const char SDPCtrlMsgHandler::cmd7[] PROGMEM = "sync";
    const char SDPCtrlMsgHandler::cmd8[] PROGMEM = "rconfig";
    const char SDPCtrlMsgHandler::cmd9[] PROGMEM = "wconfig";
    const char SDPCtrlMsgHandler::cmd10[] PROGMEM = "config";
    const char SDPCtrlMsgHandler::cmd11[] PROGMEM = "cmd";
    const char SDPCtrlMsgHandler::cmd12[] PROGMEM = "history";
    const char* SDPCtrlMsgHandler::BROADCAST = "any";

    const char* const SDPCtrlMsgHandler::crtl_msg_type[] PROGMEM =
    {
        cmd0,
        cmd1,
        cmd2,
        cmd3,
        cmd4,
        cmd5,
        cmd6,
        cmd7,
        cmd8,
        cmd9,
        cmd10,
        cmd11,
        cmd12
    };

    uint8_t SDPCtrlMsgHandler::getType( sdp::message::SDPCtrlMsg &msg )
    {
      char buffer[12];
      uint8_t i = 0;
      for ( i = 0; i < N_TYPE - 1; i++ )
      {
        memset(buffer, 0, 12);
        strcpy_P(buffer, (char*) pgm_read_word(&(crtl_msg_type[i])));

        if ( strcmp(buffer, msg.msg()) == 0)
        {
          break;
        }
      }
      switch (i)
      {
        case 0 : { return UPDATE; }; break;
        case 1 : { return RESTART; }; break;
        case 2 : { return ENABLE; }; break;
        case 3 : { return FACTORY; }; break;
        case 4 : { return STATUS; }; break;
        case 5 : { return TEST; }; break;
        case 6 : { return CALIBRATE; }; break;
        case 7 : { return SYNC; }; break;
        case 8 : { return RCONFIG; }; break;
        case 9 : { return WCONFIG; }; break;
        case 10 : { return CONFIG; }; break;
        case 11 : { return CMD; }; break;
        case 12 : { return HISTORY; }; break;
        default : { }; break;
      }
      return INVALID;
    }


    SDPCtrlMsg::SDPCtrlMsg() :
        m_to(0), m_msg(0), m_data(0)
    {
      // TODO Auto-generated constructor stub

    }

    SDPCtrlMsg::~SDPCtrlMsg()
    {
      // TODO Auto-generated destructor stub
    }


    char* SDPCtrlMsg::to()
    {
      if (m_to == 0)
      {
        m_to = aJson.getObjectItem(m_json, "to");
      }

      return (m_to != 0) ? m_to->valuestring : 0;
    }
    char* SDPCtrlMsg::msg()
    {
      if (m_msg == 0)
      {
        m_msg = aJson.getObjectItem(m_json, "msg");
      }

      return (m_msg != 0) ? m_msg->valuestring : 0;
    }
    char* SDPCtrlMsg::data()
    {
      if (m_data == 0)
      {
        m_data = aJson.getObjectItem(m_json, "data");
      }

      return (m_data != 0) ? m_data->valuestring : 0;
    }
    aJsonObject* SDPCtrlMsg::dataObj()
    {
      if (m_data == 0)
      {
        m_data = aJson.getObjectItem(m_json, "data");
      }

      return m_data;
    }

    bool SDPCtrlMsg::isToAll()
    {
      if (m_to == 0)
      {
        return false;
      }
      return (m_to->valuestring == "any") ? true : false;
    }

  } /* namespace message */
} /* namespace sdp */
