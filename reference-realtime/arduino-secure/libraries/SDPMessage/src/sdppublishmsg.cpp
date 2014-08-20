/*
 * sdppublishmsg.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: dtvlab
 */

#include "sdppublishmsg.h"
#include<Time.h>
#include <avr/pgmspace.h>

using namespace sdp::message;

prog_char ValueJSON::LABEL_COMPONENTS[] PROGMEM  = {"components"};

PROGMEM const char* ValueJSON::LABEL[] =       // change "string_table" name to suit
{
    LABEL_COMPONENTS
};


prog_char PublishJSON::LABEL_VALUES[] PROGMEM  = {"values"};
prog_char PublishJSON::LABEL_STREAM[] PROGMEM  = {"stream"};
prog_char PublishJSON::LABEL_SENSOR[] PROGMEM  = {"sensor"};

PROGMEM const char* PublishJSON::LABEL[] =       // change "string_table" name to suit
{
    LABEL_VALUES,
    LABEL_STREAM,
    LABEL_SENSOR
};



prog_char SecureJSON::LABEL_MESSAGE[] PROGMEM  = {"message"};
prog_char SecureJSON::LABEL_DIGEST[] PROGMEM  = {"digest"};

PROGMEM const char* SecureJSON::LABEL[] =       // change "string_table" name to suit
{
    LABEL_MESSAGE,
    LABEL_DIGEST,
};
