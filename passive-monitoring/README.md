Passive Monitoring
==============

This module shows how to implement a passive monitoring
on live streams using MQTT.
Passive monitoring consists in running a validation
process that make some basic checks on data statistics.


Building
--------------
To compile create a lib/ folder with the following jars:

commons-math3-3.3.jar
org.eclipse.paho.client.mqttv3-1.0.0.jar
org-json.jar (the json reference implementation)

Usage
--------------
The main application will connect to an MQTT broker
(no authentication) to listen for an input stream:

Topic: output/<tenant>/<sensor>_<stream>

tenant, sensor and stream can be configured inside
config.properties.

*IMPORTANT:* The stream must have only one component with name "value".

On start the application needs to recover a JSON object with
some parameters for the validation (see: 08ac1af9-98d5-530d-8d67-c65eeb81c919_stream)

To do this, the application will use the configured
sensor and stream id's to build a REST (GET) query using the
base url in the config file.

url=file\:///path/to/running/folder/
will have the effact to load the sample JSON as a local file.
(The path name must be absolute).

url=http\://platform/api/metadata/
will have the effect to query
http://platform/api/metadata/<sensor>_<stream>

Once the application has all info to run the validation it
will listen to the topic and will modify the "validity" field
inside the received observations.

The output is published to the following topic:
output/<tenant>/<sensor>_<stream>/val


