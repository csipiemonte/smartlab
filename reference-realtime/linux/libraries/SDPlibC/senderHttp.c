#include "senderHttp.h"
#include <stdint.h>

SenderHttp newSenderHttp(char* _ip, char* _port, char* _service, char* _key, char* _destination){
        SenderHttp sender;
        sender.ip = _ip;
        sender.port = _port;
        sender.service = _service;
        sender.key = _key;
        sender.destination = _destination;
        return sender;
}

void timeNow(){
        time_t curtime;
        struct tm *timeTF;
        char *buffer;
	buffer=malloc(30*sizeof(char));
        time(&curtime);
        timeTF = localtime( &curtime );
        strftime(buffer,30,"%Y-%m-%dT%XZ",timeTF);
        printf ("\n\n\nTEMPO di invio/ricezioone messaggio:=%s\n\n\n\n",buffer);
}
int test=0;

int sendMessageHttp(SenderHttp sender, char *message){
        int sockfd,confd;
        struct sockaddr_in servaddr;
        char sendline[MAXLINE], recvline[MAXLINE];
        ssize_t n;
        char portS[4];    
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd<0)
            printf("Errore nell'invio del messaggio. Socket non aperto\n");

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;

        servaddr.sin_port = htons(atoi(sender.port));
        inet_pton(AF_INET, sender.ip, &servaddr.sin_addr);
        confd = connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
        if(confd<0)
            printf("Errore nell'invio del messaggio. Connessione non stabilita\n");    
//         test++;
// 
//         char testS[4];
//         sprintf(testS,"%d",test);
        strcpy(sendline,"POST ");
        strcat(sendline,sender.service);
        strcat(sendline, " HTTP/1.1\r\n");
        strcat(sendline,"Host:");
        strcat(sendline,sender.ip);
        strcat(sendline,":");
        strcat(sendline,sender.port);
        strcat(sendline,"\r\n");
        strcat(sendline,"Content-Type: application/x-www-form-urlencoded\r\n");//json/application
        strcat(sendline,"User-Agent: sdp_to_iot\r\n");
        strcat(sendline,"Referer:");
        strcat(sendline,"fromSDPtoIOT");
        strcat(sendline,"\r\n");
        strcat(sendline,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
        strcat(sendline,"Keep-Alive: 115\r\n");
        strcat(sendline,"Content-length: ");

        char length[10];
        sprintf(length,"%d",strlen(message)+strlen(sender.key)+10);
        strcat(sendline,length);
        strcat(sendline,"\r\n\r\n");
//strcat(sendline,"data={\"Name\":\"SDP_T_550e8400-e29b-41d4-a716-446655440000\",\"Model\":\"Arduino\",\"Location\":[\"45.0395710\",\"7.6683410\",\"200\"],\"Description\":\"ArduinoReference\",\"Type\":\"Sensor\",\"ObservedProperty\":\"AirTemperatureA\",\"Unit\":\"C\",\"Allowance\":\"0.1\",\"Frequency\":\"5sec\",\"Author\":\"csp\",\"Refer\":\"SmartDataPlatform\",\"Supply\":\"none\",\"Timestamp\":\"10/14/2014 12:39:25\",\"Value\":\"23.437500\"}&key=494419b8682deef895f06e42cb6ec988");
        if(!strcmp(sender.destination, "iot"))
            strcat(sendline,"data=");
        strcat(sendline,message);
        if(strlen(sender.key)>0){
            strcat(sendline,"&key=");
            strcat(sendline,sender.key); 
        }
//         strcat(sendline,"\r\n");
        timeNow();
        printf("valore finale della post:%s\n",sendline);
//         if(write(sockfd, sendline, strlen(sendline))<0)
        if (send(sockfd, sendline, strlen(sendline), 0) < 0) {
            printf("errore nell'invio del messaggio\n");
        }
        else{
//             shutdown(sockfd, 1);
            while ( ( n = read((sockfd), recvline, MAXLINE)) != 0 ){
                recvline[n]='\0';
                printf("%s",recvline);
                break;
          }   
        }
        timeNow();
        printf("\n\n\n\n\nFINE METODO SENDHTTP\n\n\n\n\n");
        close(sockfd);
}

struct string {
        char *ptr;
        size_t len;
};

void init_string(struct string *s) {
        s->len = 0;
        s->ptr = malloc(s->len+1);
        if (s->ptr == NULL) {
            fprintf(stderr, "malloc() failed\n");
        }
        s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
        size_t new_len = s->len + size*nmemb;
        s->ptr = realloc(s->ptr, new_len+1);
        if (s->ptr == NULL) {
            fprintf(stderr, "realloc() failed\n");
        }
        memcpy(s->ptr+s->len, ptr, size*nmemb);
        s->ptr[new_len] = '\0';
        s->len = new_len;
        return size*nmemb;
}

int sendMessageHttpCurl(SenderHttp sender, char *message){
        CURL *curl;
        CURLcode res;

        char sendline[MAXLINE];
        printf("valore di sender.destination=%s\n",sender.destination);
        if(!strcmp(sender.destination, "iot"))
            strcpy(sendline,"data=");
        strcat(sendline,message);
        if(strlen(sender.key)>0){
            strcat(sendline,"&key=");
            strcat(sendline,sender.key);
        }
        char http_link[MAXLINE];
        strcpy(http_link,"http://");
        strcat(http_link,sender.ip); 
        strcat(http_link,sender.service);
        curl_global_init(CURL_GLOBAL_ALL);
        char response[MAXLINE];
    
        curl = curl_easy_init();
        if (curl)
        {
          struct string s;
          init_string(&s);
          curl_easy_setopt(curl, CURLOPT_URL, http_link);
          curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sendline);

	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
          curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
          curl_easy_setopt(curl, CURLOPT_POST, 1);
          printf("\n\nsono in curl post messaggio=%s\n",sendline);
          
	  res = curl_easy_perform(curl);
          printf("risposta=%s\n", s.ptr);
          curl_easy_cleanup(curl);
      }
      else{
          curl_global_cleanup();
      }
      curl_global_cleanup();

  
  }