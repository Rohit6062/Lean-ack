#include"../raptor_header/base.h"
/*
int main1()
{
  uint32_t n;
    byte* buffer =(byte*) malloc(sizeof(char)*1025);
    byte* buffer2 =(byte*) malloc(sizeof(char)*1025);
    struct sockaddr_in serv_addr,temp;
    socklen_t serv_addr_len,temp_len;
    uint32_t sock_fd;
    uint32_t send_count;
    sockinfo* sock ; 
    setup_send_socket("127.0.0.1",SERV_PORT_RECEIVE,&serv_addr,&sock_fd);
    // printf("%p %d\n",serv_addr,sock_fd);
    uint32_t recvive_fd = setup_recieve_socket(SERV_PORT_SEND);
    memcpy(buffer+1,"test",strlen("test"));
    // buffer[0] = checksum(buffer,strlen(buffer+1)+1);
    while(true){
        if((send_count=sendto(sock_fd,buffer,strlen(buffer+1)+1,0,(SA*)&serv_addr,sizeof(serv_addr)))==-1)err_quit(" main send");
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
*/
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
    char filename[] ="test";// (byte*) malloc(sizeof(byte)*50);
    byte* buffer = (byte*) malloc(sizeof(byte)*1025);
    raptor* obj = (raptor*) malloc(sizeof(raptor));
    int32_t send_count;
    setup_send_socket("127.0.0.1",SERV_PORT_RECEIVE,sock->send_addr,&sock->send_fd);
    printf("%p %d\n",sock->send_addr,sock->send_fd);
    sock->recieve_fd = setup_recieve_socket(SERV_PORT_SEND);
    sock->buffer[0]=0;
    memcpy(filename,"test",strlen("test"));
    while(true){
        memcpy(sock->buffer+1,filename,4);
        sock->buffer[0]=0;
        printf("%p\n",sock->buffer);
        if((send_count=sendto(sock->send_fd,sock->buffer,strlen(filename)+1,0,(SA*)sock->send_addr,sock->send_addr_len))==-1)printf("send_count %d\n",send_count),err_quit("main send");
        printf("send_count = %d\n", send_count);
        if((send_count=recvfrom(sock->recieve_fd,buffer,1025,0,NULL,NULL))==-1)err_quit("recvfrom");
        if(buffer[0]==0)break;
    }
    memcpy(obj,sock->buffer+1,sizeof(raptor));
    for(uint16_t i=0;i<obj->K;i++){
        raptor_recieve_block(obj,sock,i,obj->K+5);
    }
    return 0; 
}
