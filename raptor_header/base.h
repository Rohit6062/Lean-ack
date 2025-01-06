#ifndef BASE_H
#define BASE_H
#define MAXLINE 4096
#define LISTENQ 107
#define SERV_PORT 5053
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
    socklen_t send_fd;
    struct sockaddr_in* send_addr;
    socklen_t send_addr_len;
    socklen_t recieve_fd;
}sockinfo;

void err_quit(char*);

void err_quit(char* str){
    perror(str);
    exit(0);
}
bool checksum(byte* data,uint32_t len){
    byte out=0;
    for(uint32_t i=0;i<len;i++) out += __builtin_popcount(data[i]) % 2;   
    return out%2+1;
}
void raptor_print(byte* a,uint32_t n){
    for(uint32_t i=0;i<n;i++)printf("%c",a[i]);
    printf("\n");
}
