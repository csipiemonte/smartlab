SOURCES=observation.c observation.h senderHttp.c senderHttp.h  mqtt.c mqtt.h cJSON.c cJSON.h gw_firmware.c gw_firmware.c mqttSecure.c mqttSecure.h status.h status.c clientMqtt.c clientMqtt.h
CC=gcc                                                                                                                                                                                    
                                                                                                                                                                                    
OBJECTS=$(SOURCES:.c=.o)                                                                                                                                                            
                                                                                                                                                                                    
EXECUTABLE=libsdpC.so                                                                                                                                                            
                                                                                                                                                                                    
libsdp.so : $(SOURCES)
	$(CC) -fPIC -c cJSON.c -lm
	$(CC) -fPIC -c observation.c 
	$(CC) -fPIC -c senderHttp.c
	$(CC) -fPIC -c mqtt.c
	$(CC) -fPIC -c clientMqtt.c -lmosquitto
	$(CC) -fPIC -c mqttSecure.c -lmosquitto
	$(CC) -fPIC -c gw_firmware.c -lcurl -lcrypto
	$(CC) -fPIC -c status.c 
	$(CC) -shared -o libsdpC.so observation.o senderHttp.o mqtt.o cJSON.o gw_firmware.o clientMqtt.o mqttSecure.o status.o -lm -lcurl -lcrypto -lmosquitto
                                                                                                                                                                                    
clean:                                                                                                                                                                              
	rm libsdpC.so observation.o senderHttp.o mqtt.o cJSON.o gw_firmware.o mqttSecure.o status.o clientMqtt.o
install:
	cp libsdpC.so /usr/local/lib
	mkdir -p /usr/local/include/SDPlib	
	cp observation.h senderHttp.h clientMqtt.h mqttSecure.h mqtt.h cJSON.h gw_firmware.h status.h /usr/local/include/SDPlib

uninstall:
	rm /usr/local/include/SDPlib/*.h
	rm /usr/local/lib/libsdpC.so
