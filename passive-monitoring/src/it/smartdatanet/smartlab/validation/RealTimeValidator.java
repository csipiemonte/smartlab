/**
 * RealTimeValidator.java
 * 
 * This is a Java application whose aim is to demonstrates the
 * real time validation process for numeric streams.
 *
 * The application subscribes to a specific stream queue (provided by
 * a remote MQTT broker). Each time a new observation is received, 
 * it will be validated against per-stream parameters.
 * 
 * The validation tag is then put inside the observation
 * and the result is published to another queue.
 * 
 */
package it.smartdatanet.smartlab.validation;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.URL;
import java.util.Date;
import java.util.Properties;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Real-time stream validation application
 */
public class RealTimeValidator implements MqttCallback {
	
	// Configuration parameters
	private String broker;
	private int qos;
	private String tenant;
	private String sensor;
	private String stream;
	private String sdpStreamURL;
	
	// The validator
	private Validator validator;
	
	// MQTT Client
	private MqttClient client;
	
	
	// Configuration (properties file) name
	public String propName;
	
	/**
	 * Loads the configuration file
	 */
	private void cfgLoad() {
		Properties prop = new Properties();
		InputStream propStream;
		
		log("Loading cofiguration file");

		
		try {
			
			propStream = new FileInputStream(propName);
			prop.load(propStream);

		} catch (IOException e) {
			
		} finally {
			
			broker = prop.getProperty("broker","tcp://127.0.0.1:1883");
			qos = Integer.parseInt(prop.getProperty("qos","0"));
			tenant = prop.getProperty("tenant","smartlab");
			
			// UUID5("DNS/smartdatanet.it/sensor")
			sensor = prop.getProperty("sensor","08ac1af9-98d5-530d-8d67-c65eeb81c919");
			
			/* This is the platform REST end-point that can be used to fetch
			 * the stream metadata (the ones used for validation process)
			 * 
			 * e.g. having uri = "http://smartdatanet.it/api/1.0/sensors/"
			 * will cause a GET to http://smartdatanet.it/api/1.0/sensors/<sensor>_<stream>
			 * that is expected to return a JSON representing a stream (or at least
			 * a subset with the parameters needed for validation)
			 *
			 */
			sdpStreamURL = prop.getProperty("url","");
			stream = prop.getProperty("stream", "stream");
		}
	
	}

	/**
	 * Saves the configuration parameters to the properties file
	 */
	private void cfgSave() {
		Properties prop = new Properties();
		OutputStream propStream;
		
		log("Writing cofiguration file");
		
		try {
			
			propStream = new FileOutputStream(propName);
			prop.setProperty("broker", broker);
			prop.setProperty("qos", Integer.toString(qos));
			prop.setProperty("tenant", tenant);
			prop.setProperty("sensor", sensor);
			prop.setProperty("stream", stream);
			prop.setProperty("url", sdpStreamURL);
			prop.store(propStream, null);
			
		} catch (IOException e) {

		}
		
	}

	/**
	 * From the sensor id, the stream id and the base SDP url
	 * this method will make a GET request to receive the JSON
	 * description of the stream.
	 * Using the validation parameter in it, it will prepare the Validator
	 * object.
	 */
	private void initValidator() {
		try {
			URL streamURL = new URL(sdpStreamURL + sensor + "_" + stream);
			log("Loading stream JSON from: " + streamURL.toString());
			BufferedReader bodyStream = new BufferedReader(new InputStreamReader(streamURL.openStream()));
			
			StringBuilder body = new StringBuilder();
			String line;
			while ((line = bodyStream.readLine()) != null) {
				body.append(line);
			}			
			JSONObject streamObject = new JSONObject(body.toString());
			
			try {
				validator.setMin(streamObject.getDouble("min"));
			} catch (JSONException e) {
			}

			try {
				validator.setMax(streamObject.getDouble("max"));
			} catch (JSONException e) {
			}

			try {
				validator.setFps(streamObject.getDouble("fps"));
			} catch (JSONException e) {
			}
			
			try {
				validator.setN(streamObject.getInt("n"));
			} catch (JSONException e) {
			}

			try {
				validator.setdTStats(streamObject.getDouble("dt_stats"));
			} catch (JSONException e) {
			}

			try {
				validator.setdTSpeed(streamObject.getDouble("dt_speed"));
			} catch (JSONException e) {
			}
			
			try {
				validator.setMaxSpeed(streamObject.getDouble("max_speed"));
			} catch (JSONException e) {
			}

			try {
				validator.setMinSpeed(streamObject.getDouble("min_speed"));
			} catch (JSONException e) {
			}
			
			
			
		} catch (IOException e) {
			log("Error while reading steram URL");

		}
		
		validator.init();
		log("Stream validation parameters: " + validator.toString());
		
	}
	
	/**
	 * Constructor (called only inside the main())
	 */
	private RealTimeValidator() {
		propName = "config.properties";
		validator = new Validator();
	}

	/**
	 * Closes an existing connection to the broker
	 */
	private void mqttDisconnect() {
		try {
			log("Disconnecting");
			client.close();
		} catch (MqttException e) {
		}		
	}

	/**
	 * Setup MQTT client and connect to the broker.
	 * Also it will subscribe the client to the input topic.
	 */
	private boolean mqttConnect() {
		MqttConnectOptions 	conOpt;
		try {
			conOpt = new MqttConnectOptions();
			conOpt.setCleanSession(true);
			log("Broker: " + broker);
			log("Client: " + "RealTimeValidator");
			client = new MqttClient(broker, "RealTimeValidator", null);
			client.setCallback(this);
			log("Connecting");
			client.connect(conOpt);
			client.setTimeToWait(1); // publish will not block for broker ack
			String topic = "output/" + tenant + "/" + sensor + "_" + stream;
			log("Subscribing topic: " + topic);
			log("QOS: " + Integer.toString(qos));
			client.subscribe(topic, qos);
		} catch (MqttException e) {
			log("Unable to connect to the MQTT broker");
			return false;
		}
		
		return true;
	}

	/**
	 * Called when the connection to the broker is lost.
	 */
	@Override
	public void connectionLost(Throwable trowable) {
		
	}

	/**
	 * Called when the observation has been sent in output.
	 */
	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {

	}

	/**
	 * Called when a new observation arrives from the input queue.
	 * The method will parse the resulting JSON object to find and 
	 * tag the values with a validation flag.
	 * At the end the new tagged observation will be published on the output
	 * queue.
	 */
	@Override
	public void messageArrived(String topic, MqttMessage message) throws Exception {

		try {
			String json = new String (message.getPayload());
			JSONObject observation = new JSONObject(json);
			JSONArray values = observation.getJSONArray("values");
			for (int i=0; i< values.length(); i++) {
				JSONObject value = values.getJSONObject(i);
				JSONObject components = value.getJSONObject("components");

				//TODO: Here we are supporting measures with only one component named "value"
				double val = components.getDouble("value");

				String validity = validator.getValidity(val);
				log(validity + " : " + Double.toString(val));
				value.put("validity", validity);
			}
			String taggedJson = observation.toString();
			try {
				client.publish(topic + "_v", taggedJson.getBytes(), qos, false);
			} catch (MqttException e) {
			}
		} catch (JSONException e) {
			log("Message is not a well formatted JSON observation");
		}
		
	}

	/**
	 * A very simple log function.
	 */
	private static void log(String string) {
		Date now = new Date();
		System.out.println("[" + now.toString() + "] " + string);		
	}	
	
	/**
	 * Application entry point
	 */
	public static void main(String[] args) {

		RealTimeValidator app = new RealTimeValidator();
		
		// Configuration file may be the first parameter on the command line
		if (args.length == 1) {
			app.propName = args[0];
		}
		
		app.cfgLoad();
		app.cfgSave(); // This will create a default config
		
		app.initValidator();
		
		if (app.mqttConnect()) {
			
			log("Press any key to stop.");
			try {
				System.in.read();
			} catch (IOException e) {
				
			}
			app.mqttDisconnect();			
		}
		
		log("Done.");
		System.exit(0);

	}

}
