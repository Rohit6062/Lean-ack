#include"./../raptor_header/base.h"
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
/*
 *typedef struct{
    FILE* fp;
    socklen_t send_fd;
    struct sockaddr_in* send_addr;
    socklen_t recieve_fd;
}sockinfo;
 * */
sockinfo* raptor_build_sockinfo(){
    sockinfo* output = (sockinfo*) malloc(sizeof(sockinfo));
    output->send_addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    bzero(output->send_addr,sizeof(output->send_addr));
    return output;
}

int main2()
{
    byte* buffer =(byte*) malloc(sizeof(byte)*1025);
    strcpy(buffer,"hello world!\n");
    struct sockaddr_in serv_addr;
    uint32_t sock_fd;
    uint32_t send_count;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET,"127.0.0.1",(SA*)&serv_addr.sin_addr)<0)err_quit("inet_pton");
    if((sock_fd = socket(AF_INET,SOCK_DGRAM,0))==-1)err_quit("socket");  
    if(send_count=sendto(sock_fd,buffer,strlen(buffer),0,(SA*)&serv_addr,sizeof(serv_addr))<0)err_quit("send");
}
sockinfo* raptor_accept_req(){
    sockinfo* output = raptor_build_sockinfo();
    output->recieve_fd = setup_recieve_socket(5053);
    byte* buffer = (byte*) malloc(sizeof(byte)*1025);
    uint32_t bufflen;
    if((bufflen = recvfrom(
        output->recieve_fd,
        buffer,
        Bufflen,
        0,
        (SA*)output->send_addr,
        &output->send_addr_len))<=0)err_quit("raptor_accept_req recvfrom");
    buffer[bufflen] = 0;
    raptor_print(buffer,bufflen);
    if(checksum(buffer+1,bufflen-1) != buffer[0]){
        printf("currepted Data\n");
        free(output->send_addr);
        free(output);
        return raptor_accept_req();
    }
    if((output->fp = fopen(buffer+1,"r"))==NULL){
        printf("file not Found\n");
        close(output->recieve_fd);
        free(output->send_addr);
        free(output);
        return raptor_accept_req();
    }
    return output;
}
int main()
{
    sockinfo* tmp = raptor_accept_req();
    printf("%p %p\n",tmp,tmp->fp);
}



