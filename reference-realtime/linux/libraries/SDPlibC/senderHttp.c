#include "senderHttp.h"

SenderHttp newSenderHttp(char* _ip, char* _port, char* _service){
        SenderHttp sender;
        sender.ip = _ip;
        sender.port = _port;
        sender.service = _service;
        return sender;
}

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

        strcpy(sendline,"POST ");
        strcat(sendline,sender.service);
        strcat(sendline, " HTTP/1.0\r\n");
        strcat(sendline,"Host:");
        strcat(sendline,sender.ip);
        strcat(sendline,":");
        strcat(sendline,sender.port);
        strcat(sendline,"\r\n");
        strcat(sendline,"Content-type: application/json\r\n");
        strcat(sendline,"Content-length: ");
        char length[10];
        sprintf(length,"%d",strlen(message));
        strcat(sendline,length);
        strcat(sendline,"\r\n\r\n");
        strcat(sendline,message);
        printf("valore finale della post:%s\n",sendline);
        write(sockfd, sendline, strlen(sendline));
        while ( ( n = read(sockfd, recvline, MAXLINE)) != 0 ){
            recvline[n]='\0';
            fprintf(stderr,"%s",recvline);
        }    
}

