#ifndef BASE_H
#define BASE_H
#define MAXLINE 4096
#define LISTENQ 107
#define SERV_PORT_RECEIVE 5053
#define SERV_PORT_SEND 5054
#define Bufflen (1<<16)-1
typedef struct sockaddr SA;
typedef unsigned char byte;
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#include<netdb.h>
#include<signal.h>
#include<stdio.h>
#include"stdbool.h"
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/uio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/un.h>
#include<stdint.h>
#include"gf2matrix.h"
#include "raptor_header.h"


#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#ifdef HAVE_POLL_H 
#include <poll.h>
#endif 

#ifdef HAVE_SRING_H
#include <string.h>
#endif 


/* three headers are normally needed for socket/file ioctl's:
 * <sys/ioctl.h> 
 * <sys.filio.h>
 * <sys/sockio.h>
 * */

#ifdef HAVE_SYS_IOCTL_H
#include<sys/ioctl.h>
#endif

#ifdef HAVE_SYS_FILIO_H
#include<sys/filio.h>
#endif

#ifdef HAVE_SYS_SOCKIO_H
#include<sys/sockio.h>
#endif

#ifdef HAVE_PTHREAD_H
#include<pthread.h>
#endif 
#endif 

/*
 * file Pointer
 * send_fd;
 * listen_fd;
 * send_addr;
 */
typedef struct{
    FILE* fp;
    uint32_t file_size;
    socklen_t send_fd;
    struct sockaddr_in* send_addr;
    socklen_t send_addr_len;
    socklen_t recieve_fd;
}sockinfo;

void err_quit(char*);
bool checksum(byte* data,uint32_t len);
void raptor_print(byte* a,uint32_t n);
socklen_t setup_recieve_socket(uint16_t serv_port);
sockinfo* raptor_build_sockinfo();
sockinfo* raptor_accept_req();
void setup_send_socket(char* serv_addr_str,uint16_t serv_port,struct sockaddr_in* serv_addr,socklen_t* sock_fd);

// print error and exit the process
void err_quit(char* str){
    perror(str);
    exit(0);
}

// calculate and return checksum
bool checksum(byte* data,uint32_t len){
    byte out=0;
    for(uint32_t i=0;i<len;i++) out += __builtin_popcount(data[i]) % 2;   
    return out%2+1;
}

// printf buffer irrispective of 0
void raptor_print(byte* a,uint32_t n){
    printf("raptor print:\n");
    for(uint32_t i=0;i<n;i++)printf("%c",a[i]);
    printf("\n");
}

// returns sock_fd for reciving on given port
socklen_t setup_recieve_socket(uint16_t serv_port){
    socklen_t output;
    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
    serv_addr.sin_port = htons(serv_port);
    if((output = socket(AF_INET,SOCK_DGRAM,0))==-1)err_quit("socket");
    if(bind(output,(SA*)&serv_addr,sizeof(serv_addr))==-1)err_quit("bind");
    return output;
}

// initialize (malloc) sockinfo 
sockinfo* raptor_build_sockinfo(){
    sockinfo* output = (sockinfo*) malloc(sizeof(sockinfo));
    output->send_addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    bzero(output->send_addr,sizeof(output->send_addr));
    return output;
}
// typedef struct{
//     FILE* fp;
//     socklen_t send_fd;
//     struct sockaddr_in* send_addr;
//     socklen_t send_addr_len;
//     socklen_t recieve_fd;
// }sockinfo;

sockinfo* raptor_accept_req(){
    sockinfo* output = raptor_build_sockinfo();
    output->recieve_fd = setup_recieve_socket(SERV_PORT_RECEIVE);
    byte* buffer = (byte*) malloc(sizeof(byte)*1025);
    byte* data = (byte*) calloc(sizeof(byte),1025);
    uint32_t bufflen;
    output->send_addr_len = sizeof(output->send_addr);
    printf("setup Done\n");
    if((bufflen = recvfrom(
        output->recieve_fd,
        buffer,
        Bufflen,
        0,
        (SA*)output->send_addr,
        &output->send_addr_len))==-1)err_quit("raptor_accept_req recvfrom");
    buffer[bufflen] = 0;
    if((output->send_fd = socket(AF_INET,SOCK_DGRAM,0))==-1)err_quit("socket raptor_accept_req");
    output->send_addr->sin_port = htons(SERV_PORT_SEND);
    raptor_print(buffer,bufflen);
    if(checksum(buffer+1,bufflen-1) != buffer[0]){
        printf("currepted Data\n");
        close(output->recieve_fd);
        free(output->send_addr);
        free(output);
        return raptor_accept_req();
    }
    if((output->fp = fopen(buffer+1,"r"))==NULL){
        printf("file not Found \n");
        strcpy(data+1,"File Not Found!\n");
        data[0] = checksum(data+1,strlen(data+1));
        uint32_t tmp;
        if((tmp=sendto(output->send_fd,data,strlen(data+1)+1,0,(SA*)output->send_addr,output->send_addr_len))==-1)err_quit("sendto raptor_accept_req");
        printf("send => %d\n",tmp);
        close(output->recieve_fd);
        free(output->send_addr);
        close(output->send_fd);
        free(output);
        free(data);
        free(buffer);
        return raptor_accept_req();
    }
    return output;
}



void setup_send_socket(char* serv_addr_str,uint16_t serv_port,struct sockaddr_in* serv_addr,socklen_t* sock_fd){
    if(!serv_addr){printf("serv_addr ia nill\n");return;}
    bzero(serv_addr,sizeof(serv_addr));
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(serv_port);
    if(inet_pton(AF_INET,serv_addr_str,(SA*)&serv_addr->sin_addr)==-1)err_quit("inet setup_send_socket");
    if((*sock_fd = socket(AF_INET,SOCK_DGRAM,0))==-1)err_quit("socket setup_send_socket");
    return;
}
