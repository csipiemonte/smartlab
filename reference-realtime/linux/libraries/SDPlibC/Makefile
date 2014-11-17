SOURCES=observation.c observation.h senderHttp.c senderHttp.h  mqtt.c mqtt.h cJSON.c cJSON.h gw_firmware.c gw_firmware.c mqttSecure.c mqttSecure.h status.h status.c
CC=gcc                                                                                                                                                                                    
                                                                                                                                                                                    
OBJECTS=$(SOURCES:.c=.o)                                                                                                                                                            
                                                                                                                                                                                    
EXECUTABLE=libsdpC.so                                                                                                                                                            
                                                                                                                                                                                    
libsdp.so : $(SOURCES)
	$(CC) -fPIC -c cJSON.c -lm
	$(CC) -fPIC -c observation.c 
	$(CC) -fPIC -c senderHttp.c
	$(CC) -fPIC -c mqtt.c
	$(CC) -fPIC -c mqttSecure.c -lmosquitto
	$(CC) -fPIC -c gw_firmware.c -lcurl -lcrypto
	$(CC) -fPIC -c status.c 
	$(CC) -shared -o libsdpC.so observation.o senderHttp.o mqtt.o cJSON.o gw_firmware.o mqttSecure.o status.o -lm -lcurl -lcrypto -lmosquitto
                                                                                                                                                                                    
clean:                                                                                                                                                                              
	rm libsdpC.so observation.o senderHttp.o mqtt.o cJSON.o gw_firmware.o mqttSecure.o status.o
install:
	cp libsdpC.so observation.o senderHttp.o mqtt.o mqttSecure.o cJSON.o status.o /usr/local/lib
	cp observation.h senderHttp.h mqttSecure.h mqtt.h cJSON.h gw_firmware.h status.h /usr/local/include

uninstall:
	rm /usr/local/lib/libsdpC.so 
	rm /usr/local/lib/observation.o 
	rm /usr/local/lib/senderHttp.o 
	rm /usr/local/lib/mqtt.o 
	rm /usr/local/lib/mqttSecure.o
	rm /usr/local/lib/cJSON.o
	rm /usr/local/lib/status.o
	rm /usr/local/include/observation.h
	rm /usr/local/include/senderHttp.h
	rm /usr/local/include/mqtt.h
	rm /usr/local/include/mqttSecure.h
	rm /usr/local/include/cJSON.h
	rm /usr/local/include/gw_firmware.h
	rm /usr/local/include/status.h
