#include"./../raptor_header/base.h"
int main()
{
  uint32_t n;
    byte* buffer =(char*) malloc(sizeof(char)*1025);
    byte* buffer2 =(char*) malloc(sizeof(char)*1025);
    struct sockaddr_in serv_addr,temp;
    socklen_t serv_addr_len,temp_len;
    uint32_t sock_fd;
    uint32_t send_count;
    setup_send_socket("127.0.0.1",SERV_PORT_RECEIVE,&serv_addr,&sock_fd);
    uint32_t recvive_fd = setup_recieve_socket(SERV_PORT_SEND);
    strcpy(buffer+1,"tet");
        buffer[0] = checksum(buffer,strlen(buffer+1)+1);
    for(int i=0;i<1;i++){
        if((send_count=sendto(sock_fd,buffer,strlen(buffer+1)+1,0,(SA*)&serv_addr,sizeof(serv_addr)))==-1)err_quit("send");
        printf("send_count = %d\n", send_count);
        if((n=recvfrom(recvive_fd,buffer2,1025,0,NULL,NULL))==-1)err_quit("recvfrom");
        buffer2[n]=0;
        printf("recieved %d\n",n);
        raptor_print(buffer2,n);
    }
    
    raptor_recieve_block(obj,sockinfo* sock,uint16_t block_no,uint32_t symbols_count);
}
