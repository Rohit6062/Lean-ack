#include"./../raptor_header/base.h"
// #include<unistd.h>
int main()
{
    byte* buffer =(char*) malloc(sizeof(char)*1025);
    strcpy(buffer+1,"tes");
    buffer[0] = checksum(buffer,20);
    struct sockaddr_in serv_addr;
    uint32_t sock_fd;
    uint32_t send_count;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET,"127.0.0.1",(SA*)&serv_addr.sin_addr)<0)err_quit("inet_pton");
    if((sock_fd = socket(AF_INET,SOCK_DGRAM,0))==-1)err_quit("socket");  
    for(int i=0;i<100;i++){
    if(send_count=sendto(sock_fd,buffer,strlen(buffer),0,(SA*)&serv_addr,sizeof(serv_addr))<0)err_quit("send");
    sleep(1);

    }
}
