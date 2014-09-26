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
        //char *url,*hash;
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
   /*     int okCheck=0;
        int cont=0;
        while(okCheck!=1){
            if(cont<5){
                okCheck=getFile(url, hash);
                cont++;
            }else{
                okCheck=1;
                printf("impossibile scaricare un file non corrotto\n");
                break;
            }

            if(okCheck==1){
                printf("il file scaricato è corretto ed è stato caricato\n");
            }else{
                printf("il file scaricato è scorretto riscarico\n");
            }
        } */
	  
	return firmwareGet;
//         free(firmwareGet.url);
//         free(firmwareGet.hash);
}

int getFile(char *url,char *pathFile){
        printf("sono in getFile con out=%s e hash=%s\n", url, pathFile);
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
        printf("sono in getFile con out=%s e hash=%s\n", url, pathFile);
        return 1;
       /* if(checkHash(outfilename,hashTemp)==1){
            loadFirmware();
            return 1;
        }else{
            printf("checksum errato riscarico il file\n"); 
            return -1;
        }*/
       
}

int checkHash(char* filename, char* hash)
{
        printf("sono in checkhash con hash=%s\n",hash);
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

            //printf("[%d] %c %c\n", i, md5string[i], hash[i]);
            if(md5string[i]!=hashTemp[i]){
                printf("\ntest checksum fallito\n");
                fclose (inFile);
                return -1;
            }
        }
        printf (" checksum riuscito\n");	
        fclose (inFile);
        return 1;
}

void loadFirmware(char *filename){
        char command[500];
        memset(command,0,500);
        printf("carico il firmware segnalato dal messaggio\n");
        snprintf(command, 500,"avrdude -p m2560 -P /dev/ttyACM0 -c stk500v2 -b 115200 -U flash:w:%s &",filename);
        printf("valore del comando:\n%s \n",command);
        memset(command,0,500);  
}