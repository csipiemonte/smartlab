Smart Data Platform (SDP)
==============

Server reference implementation
--------------
The reference solution uses a server environment that allow to emulate some of the interfaces that the SDP will provide. In particular, the components that must be installed on the server allow you to emulate the functions of acquisition and MQTT stream to the browser via WebSockets.

Installation
-----------

 *  Installation of ActiveMQ
 
 Downland the ActiveMq version 5.9.0 to link:
 http://archive.apache.org/dist/activemq/apache-activemq/5.9.0/apache-activemq-5.9.0-bin.tar.gz 
 
 Unpack the archive in the preferred path (in this case used as an example is "/home/<user>/reference/" as the user is used csp)
```
cd /home/csp/reference/
tar xvf apache-activemq-5.9.0-bin.tar.gz 
```

 *  Configuration of ActiveMQ
 
 Copy the files for configure ActiveMQ

```
cp activemq.xml credentials.properties /home/csp/reference/apache-activemq-5.9.0/conf 
```
 
 *  Installation of the web page
 
 The final part is to insert the page html demo and the necessary dependencies JavaScript within a web server. For example, if you installed Apache from the official repositories of Ubuntu, just copy the necessary files into a folder under "/var/www" or "/var/www/html".
```
sudo apt-get install apache2
sudo cp -r sdp-ref/* /var/www/sdp-ref/
```
Usage
-----   
    
 *  At this point it is necessary to launch ActiveMQ. Is possible start the program in debug mode or background mode

```
cd /home/csp/reference/apache-activemq-5.9.0/bin 
./activemq console (for debug mode, Ctrl+c for stopping the service)
./activemq start (for background mode, ./activemq stop for stopping the service)
```
  * For testing the reference is possible send the acquisition usage mosquitto
```
mosquitto_pub -h 127.0.0.1 -t input/sandbox -m '{"stream": "thermo_a","sensor": "550e8400-e29b-41d4-a716-446655440000", "values": [{"time": "2014-05-13T17:08:58Z","components": {"c0":"40.00"}}]}' -u sandbox -P sandbox\$1
```
    
 *  Using your browser you can then connect to the server prepared to see the demo page.
``` 
http://<indirizzo-server>/sdp-ref/
```