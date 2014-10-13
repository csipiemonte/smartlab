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

const char ValueJSON::LABEL_COMPONENTS[] PROGMEM = "components";

//! Label for a valid measurement
const char ValueJSON::LABEL_VALID[] PROGMEM = "valid";

//! Label for a erroneous measurement
const char ValueJSON::LABEL_ERRONEOUS[] PROGMEM = "erroneous";

//! Label for a doubtful measurement
const char ValueJSON::LABEL_DOUBTFUL[] PROGMEM = "doubtful";

//! Label for a unknown measurement
const char ValueJSON::LABEL_UNKNOWN[] PROGMEM = "unknown";

//! Flash table for wireless configuration
const char* const ValueJSON::VALUE_TABLE[] PROGMEM =
{
    LABEL_VALID,
    LABEL_ERRONEOUS,
    LABEL_DOUBTFUL,
    LABEL_UNKNOWN,
    LABEL_COMPONENTS,
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
