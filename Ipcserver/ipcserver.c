#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{

    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if(argc < 2) 
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)

        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi("12238");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
    printf("here1\n");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
     if (newsockfd < 0) 
         error("ERROR on accept");
     bzero(buffer,256);
    printf("here2\n" );
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("File to be transferred: %s\n",buffer);
    
     FILE *fp;
     
     fp=fopen(buffer,"r");
     int sz=fread(buffer,1,256,fp);
     
        while(sz>0)
        {
            write(newsockfd,buffer,sz);
            sz=fread(buffer,1,256,fp);  

        }
    
     
     close(newsockfd);
     close(sockfd);
     return 0; 
}