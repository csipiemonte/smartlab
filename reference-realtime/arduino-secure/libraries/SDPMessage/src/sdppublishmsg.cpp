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

const char PROGMEM ValueJSON::LABEL_COMPONENTS[] = "components";
//prog_char ValueJSON::LABEL_COMPONENTS[] PROGMEM  = {"components"};

//const char* PROGMEM ValueJSON::LABEL[] =       // change "string_table" name to suit
const char* const ValueJSON::LABEL[] PROGMEM = 
{
    LABEL_COMPONENTS
};

const char PROGMEM PublishJSON::LABEL_VALUES[] = "values";
const char PROGMEM PublishJSON::LABEL_STREAM[] = "stream";
const char PROGMEM PublishJSON::LABEL_SENSOR[] = "sensor";
//prog_char PublishJSON::LABEL_VALUES[] PROGMEM  = {"values"};
//prog_char PublishJSON::LABEL_STREAM[] PROGMEM  = {"stream"};
//prog_char PublishJSON::LABEL_SENSOR[] PROGMEM  = {"sensor"};

//PROGMEM const char* PublishJSON::LABEL[] =       // change "string_table" name to suit
const char* const PublishJSON::LABEL[] PROGMEM = 
{
    LABEL_VALUES,
    LABEL_STREAM,
    LABEL_SENSOR
};



const char PROGMEM SecureJSON::LABEL_MESSAGE[] = "object";
const char PROGMEM SecureJSON::LABEL_DIGEST[] = "signature";
//prog_char SecureJSON::LABEL_MESSAGE[] PROGMEM  = {"object"};
//prog_char SecureJSON::LABEL_DIGEST[] PROGMEM  = {"signature"};

//PROGMEM const char* SecureJSON::LABEL[] =       // change "string_table" name to suit
const char* const SecureJSON::LABEL[] PROGMEM = 
{
    LABEL_MESSAGE,
    LABEL_DIGEST,
};
