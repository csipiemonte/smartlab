#include "gw_firmware.h"

cJSON *firmwareJson(){
        cJSON *root;
        root=cJSON_CreateObject();
        return root;
}

void addValue(cJSON *root, char *field, char *value){
        cJSON_AddItemToObject(root, field, cJSON_CreateString(value));
}

char* getJson(cJSON *root){
   return cJSON_Print(root);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        size_t written = fwrite(ptr, size, nmemb, stream);
        return written;
}

FirmwareComponent getFirmwareComponent(char *bufferJson){
	FirmwareComponent firmwareGet;
        cJSON *json;
        cJSON *jsonObj,*jsonObjCheck;

        json=cJSON_Parse(bufferJson);
        if (!json){
            printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        }
        else{
            jsonObj= cJSON_GetObjectItem( json, "url" );
            firmwareGet.url=cJSON_Print(jsonObj);
            jsonObjCheck= cJSON_GetObjectItem( json, "hash" );
            firmwareGet.hash=cJSON_Print(jsonObjCheck);	    
            if(firmwareGet.url){
	    }
            else{
                return;
            }
            if(firmwareGet.hash){
	    }
            else{
                return;
            }	    
        }
	  
	return firmwareGet;
}

int getFile(char *url,char *pathFile){
        CURL *curl;
        FILE *fp;
        CURLcode res;
        char *urlD;
        urlD=malloc(strlen(url)-2);
        memset(urlD,0,strlen(url)-2);
        strncpy(urlD,url+1,strlen(url)-2);

        
        char *outfilename = pathFile;//"data.hex";
        curl = curl_easy_init();
        if (curl) {
            fp = fopen(outfilename,"wb");
            curl_easy_setopt(curl, CURLOPT_URL, urlD);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
        }
        return 1;      
}

int checkHash(char* filename, char* hash)
{
        FILE *inFile = fopen (filename, "rb");	
        unsigned char c[MD5_DIGEST_LENGTH];
        int i=0;
	char *hashTemp;
        hashTemp=malloc(strlen(hash)-2);
        memset(hashTemp,0,strlen(hash)-2);
        strncpy(hashTemp,hash+1,strlen(hash)-2);
        MD5_CTX mdContext;
        int bytes;
        unsigned char data[1024];
        char temp[32];

        if (inFile == NULL) {
            printf ("%s can't be opened the file.\n",filename);
            return -1;
        }

        MD5_Init (&mdContext);
        while ((bytes = fread (data, 1, 1024, inFile)) != 0)
            MD5_Update (&mdContext, data, bytes);
        MD5_Final (c,&mdContext);

        char md5string[33];
        for(i = 0; i < 16; ++i)
            sprintf(&md5string[i*2], "%02x", (unsigned int)c[i]);	      

        for(i = 0; i < 32; i++){ 
            if(md5string[i]!=hashTemp[i]){
                printf("\ntest checksum failed\n");
                fclose (inFile);
                return -1;
            }
        }
        printf ("test checksum ok\n");	
        fclose (inFile);
        return 1;
}

void loadFirmware(char *filename, char *device){
        char command[500];
        memset(command,0,500);
        printf("load the firmware\n");
        snprintf(command, 500,"avrdude -p m2560 -P %s -c stk500v2 -b 115200 -U flash:w:%s &", device, filename);
	//system(command);	
        memset(command,0,500);  

}