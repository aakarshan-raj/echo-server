#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
using namespace std;


#define BACKLOG 10


void *addrr(struct sockaddr *sa){
   return &((struct sockaddr_in*)sa)->sin_addr;
}

int main(int argc,char **argv){
   if(argc<2){
      printf("Specify the [PORT]");
      exit(1);
   }
struct addrinfo hint,*res,*p;
struct sockaddr_storage their_addr;
socklen_t sin_size;
int sockfd,new_fd,yes,rv;
yes = 1;
char buf[INET6_ADDRSTRLEN];
memset(&hint,0,sizeof hint);
hint.ai_addr = AF_UNSPEC;
hint.ai_socktype = SOCK_STREAM;
hint.ai_flags = AI_PASSIVE;
if(rv == getaddrinfo(NULL,argv[1],&hint,&res)){
    fprintf(stderr,"1.%s\n",gai_strerror(rv));
    exit(1);
}
p=res;
if( (sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1){
    fprintf(stderr,"2.%s\n",gai_strerror(rv));
    exit(1);
}
if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1){
   perror("Error has occured");
   exit(1);
}
if(bind(sockfd,p->ai_addr,p->ai_addrlen) == -1){
   perror("error binding");
   exit(1);
}

if(listen(sockfd,BACKLOG) == -1){
  perror("3.error"); 
}

int checkk;
char recvv[100];
while(true){

sin_size = sizeof their_addr;
new_fd = accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);
if(new_fd == -1){
    perror("4.error");
    continue;
}
 inet_ntop(their_addr.ss_family,addrr((struct sockaddr*)&their_addr),buf,sizeof buf);
 printf("Connected to:%s\n",buf);
   if(!fork()){
      close(sockfd);

      if((checkk = recv(new_fd,recvv,sizeof recvv,0)) == -1){
     perror("error in recv");
     exit(1);
 } 
      if(send(new_fd,recvv,sizeof recvv,0)==-1){
         perror("error sending");
      }
      close(new_fd);
      exit(0);
   }
   close(new_fd);

  

}





}