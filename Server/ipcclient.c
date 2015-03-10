#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char filename[20];
    
    char buffer[256];
    
    portno =12350;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    
    bzero(buffer,256);
    char *fname=argv[1];
    printf("file to be transferred %s\n",fname);
    n = write(sockfd,fname,strlen(fname));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    FILE *fp;
    int recv=0;
    fp=fopen(fname,"a");
    while(1)
    {
        recv=read(sockfd,buffer,256);
        if(recv<=0)
            break;
        fwrite(buffer,1,recv,fp);
    }
    //printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
