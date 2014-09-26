SOURCES=observation.c observation.h senderHttp.c senderHttp.h  mqtt.c mqtt.h cJSON.c cJSON.h gw_firmware.c gw_firmware.c                                                                                                                                        
CC=gcc                                                                                                                                                                                    
                                                                                                                                                                                    
OBJECTS=$(SOURCES:.c=.o)                                                                                                                                                            
                                                                                                                                                                                    
EXECUTABLE=libsdpC.so                                                                                                                                                            
                                                                                                                                                                                    
libsdp.so : $(SOURCES)
	$(CC) -fPIC -c cJSON.c -lm
	$(CC) -fPIC -c observation.c 
	$(CC) -fPIC -c senderHttp.c
	$(CC) -fPIC -c mqtt.c
	$(CC) -fPIC -c gw_firmware.c -lcurl -lcrypto
	$(CC) -shared -o libsdpC.so observation.o senderHttp.o mqtt.o cJSON.o gw_firmware.o -lm -lcurl -lcrypto
                                                                                                                                                                                    
clean:                                                                                                                                                                              
	rm libsdpC.so observation.o senderHttp.o mqtt.o cJSON.o gw_firmware.o
install:
	cp libsdpC.so observation.o senderHttp.o mqtt.o cJSON.o /usr/local/lib
	cp observation.h senderHttp.h mqtt.h cJSON.h gw_firmware.h /usr/local/include

uninstall:
	rm /usr/local/lib/libsdpC.so 
	rm /usr/local/lib/observation.o 
	rm /usr/local/lib/senderHttp.o 
	rm /usr/local/lib/mqtt.o 
	rm /usr/local/lib/cJSON.o
	rm /usr/local/include/observation.h
	rm /usr/local/include/senderHttp.h
	rm /usr/local/include/mqtt.h
	rm /usr/local/include/cJSON.h
	rm /usr/local/include/gw_firmware.h 
