#include"./../raptor_header/base.h"

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
    output->send_addr->sin_port = SERV_PORT_SEND;
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
// void setup_send_socket(char* serv_addr_str,uint16_t serv_port,struct sockaddr_in* serv_addr,socklen_t* sock_fd){
        uint32_t send_fd;
        struct sockaddr_in serv_addr;
        setup_send_socket("127.0.0.1",SERV_PORT_SEND,&serv_addr,&send_fd); 
        
        if((tmp=sendto(output->send_fd,data,strlen(data+1)+1,0,(SA*)output->send_addr,output->send_addr_len))==-1)err_quit("sendto raptor_accept_req");
        if((tmp=sendto(send_fd,data,strlen(data+1)+1,0,(SA*)&serv_addr,output->send_addr_len))==-1)err_quit("sendto raptor_accept_req");
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

int main()
{
    sockinfo* tmp = raptor_accept_req();
    printf("%p %p\n",tmp,tmp->fp);
}
