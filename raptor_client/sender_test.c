#include"../raptor_header/base.h"
int main1()
{
  uint32_t n;
    byte* buffer =(char*) malloc(sizeof(char)*1025);
    byte* buffer2 =(char*) malloc(sizeof(char)*1025);
    struct sockaddr_in serv_addr,temp;
    socklen_t serv_addr_len,temp_len;
    uint32_t sock_fd;
    uint32_t send_count;
    sockinfo* sock ; 
    setup_send_socket("127.0.0.1",SERV_PORT_RECEIVE,&serv_addr,&sock_fd);
    uint32_t recvive_fd = setup_recieve_socket(SERV_PORT_SEND);
    strcpy(buffer+1,"test");
        buffer[0] = checksum(buffer,strlen(buffer+1)+1);
    while(true){
        if((send_count=sendto(sock_fd,buffer,strlen(buffer+1)+1,0,(SA*)&serv_addr,sizeof(serv_addr)))==-1)err_quit("send");
        printf("send_count = %d\n", send_count);
        if((n=recvfrom(recvive_fd,buffer2,1025,0,NULL,NULL))==-1)err_quit("recvfrom");
        buffer2[n]=0;
        printf("recieved %d\n",n);
        raptor_print(buffer2,n);
    }
    // raptor_compute_params(obj); 
    // printf("%d %d %d %d\n",obj->K,obj->L,obj->S,obj->H);
    // raptor_recieve_block(obj,sockinfo* sock,uint16_t block_no,uint32_t symbols_count);
}
/*
typedef struct{
    FILE* fp;
    uint64_t file_size;
    socklen_t send_fd;
    struct sockaddr_in* send_addr;
    socklen_t send_addr_len;
    socklen_t recieve_fd;
    byte* buffer;
    uint32_t buffer_len;
}sockinfo;
*/

int main(){
    sockinfo* sock = (sockinfo*) raptor_build_sockinfo(); 
    byte* filename = (byte*) malloc(sizeof(byte)*50);
    byte* buffer = (byte*) malloc(sizeof(byte)*1025);
    raptor* obj = (raptor*) malloc(sizeof(raptor));
    uint32_t send_count;
    setup_send_socket("127.0.0.1",SERV_PORT_RECEIVE,sock->send_addr,&sock->send_fd);
    sock->recieve_fd = setup_recieve_socket(SERV_PORT_SEND);
    sock->buffer[0]=0;
    strcpy(filename,"test");
    while(true){
        strcpy(sock->buffer+1,filename);
        sock->buffer[0]=0;
        if((send_count=sendto(sock->send_fd,sock->buffer,strlen(sock->buffer+1)+1,0,(SA*)sock->send_addr,sock->send_addr_len))==-1)printf("send_count %d\n",send_count),err_quit("send");
        printf("send_count = %d\n", send_count);
        if(send_count=recvfrom(sock->recieve_fd,buffer,1025,0,NULL,NULL)==-1)err_quit("recvfrom");
        if(buffer[0]==0)break;
    }
    memcpy(obj,sock->buffer+1,sizeof(raptor));
    for(uint16_t i=0;i<obj->K;i++){
        raptor_recieve_block(obj,sock,i,obj->K+5);
    }
    return 0; 
}
