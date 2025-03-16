#include"../raptor_header/base.h"
int main(){
    sockinfo* sock = (sockinfo*) raptor_build_sockinfo(); 
    char filename[] ="test";// (byte*) malloc(sizeof(byte)*50);
    byte* buffer = (byte*) malloc(sizeof(byte)*1025);
    raptor* obj = (raptor*) malloc(sizeof(raptor));
    int32_t send_count;
    setup_send_socket("127.0.0.1",SERV_PORT_RECEIVE,sock->send_addr,&sock->send_fd);
    printf("%p %d\n",sock->send_addr,sock->send_fd);
    sock->send_addr_len = sizeof(*sock->send_addr);
    sock->recieve_fd = setup_recieve_socket(SERV_PORT_SEND);
    sock->buffer[0]=0;
    raptor_print_socket(sock);
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
    raptor_print_object(obj);
    for(uint16_t i=0;i<obj->K;i++){
        raptor_recieve_block(obj,sock,i,obj->K+5);
    }
    return 0; 
}
