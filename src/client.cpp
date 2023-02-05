#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>


void *addrr(struct sockaddr *sa){
   return &((struct sockaddr_in*)sa)->sin_addr;
}

int main(int arg,char **argv){
   if(arg<4){
    printf("Please provide args as: [IP] [PORT] [MESSAGE_TO_ECHO]\n");
    exit(1);
  }
  std::string initial_message = argv[3];
 

  addrinfo hint,*res,*p;
  memset(&hint,0,sizeof(hint));   
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;
  int rv = getaddrinfo(argv[1],argv[2],&hint,&res);
    if( rv == -1){
    fprintf(stderr,"1.%s \n",gai_strerror(rv));
  }

  int sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(sockfd ==-1){
    fprintf(stderr,"2.%s \n",gai_strerror(sockfd));
  }

  int connectfd = connect(sockfd,res->ai_addr,res->ai_addrlen);
  if( connectfd == -1){
    fprintf(stderr,"3.%s \n",gai_strerror(connectfd));
  }
 char s[100];
 inet_ntop(res->ai_family,addrr((struct sockaddr*)res->ai_addr),s,sizeof s);
 printf("Connected: %s\n",s);
 int numbytes;
 int sendd;

  if((sendd = send(sockfd,argv[3],100,0)) == -1){
    perror("sending buffer");
    exit(1);
 }
char buff_recv[10];
 if((numbytes = recv(sockfd,buff_recv,initial_message.length(),0)) == -1){
     perror("error in recv");
     exit(1);
 }  

 buff_recv[numbytes] = '\0';
 printf("%s \n",buff_recv);
 close(sockfd);
 return 0;

}