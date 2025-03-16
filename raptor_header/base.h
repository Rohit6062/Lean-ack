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
#include"math.h"
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
#include"raptor_header.h"
#include"pthread.h"

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

// #ifdef HAVE_PTHREAD_H
// #include<pthread.h>
// #endif 
#endif 

/*
 * file Pointer
 * send_fd;
 * listen_fd;
 * send_addr;
 */
typedef struct{
    FILE* fp;
    int32_t recieve_fd;
    int32_t send_fd;
    uint64_t file_size;
    struct sockaddr_in* send_addr;
    socklen_t send_addr_len;
    byte* buffer;
    uint32_t buffer_len;
}sockinfo;

long double devide(uint32_t a, uint32_t b) {
    if (b == 0) {
        printf("Error: Division by zero is not allowed.\n");
        return -1.0; // Return a special value to indicate an error
    }
    long double out = (long double)a / (long double)b;
    // printf("Inputs: a = %u, b = %u, Result: %Lf\n", a, b, out); // Debug print
    // printf("ceil(out)=> %f\n",ceil(out));
    return out;
}
// double devide(uint32_t a , uint32_t b){
//     printf("in devide a %u b %u \n",a,b);
//     double out =  ((double) a / (double) b);
//     printf("in devide %u\n",out);
//     return out;
// }

void err_quit(char*);
bool checksum(byte* data,uint32_t len);
void raptor_print(byte* a,uint32_t n);
int32_t setup_recieve_socket(uint16_t serv_port);
sockinfo* raptor_build_sockinfo();
sockinfo* raptor_accept_req();
int pthread_tryjoin_np(pthread_t thread, void **retval);
void setup_send_socket(char* serv_addr_str,uint16_t serv_port,struct sockaddr_in* serv_addr,int32_t* sock_fd);

// print error and exit the process
void err_quit(char* str){
    perror(str);
    exit(0);
}


void raptor_print_socket(sockinfo* sock){
    printf("\t<- sockinfo -> \n");
    printf("\trecieve_fd = %d\n",sock->recieve_fd);
    printf("\tsend_fd = %d\n",sock->send_fd);
    printf("\tfile_size = %ld\n",sock->file_size);
    printf("\tsend_addr = %p\n",sock->send_addr);
    printf("\tsend_addr_len = %d\n",sock->send_addr_len);
    printf("\tbuffer_len = %d\n",sock->buffer_len);
    printf("\tbuffer = %p : %s\n",sock->buffer,sock->buffer);
    printf("\t -------------- \n");
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
int32_t setup_recieve_socket(uint16_t serv_port){
    int32_t output;
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
    output->buffer = (byte*) calloc(sizeof(byte*),Bufflen);
    bzero(output->send_addr,sizeof(*output->send_addr));
    return output;
}

sockinfo* raptor_accept_req(){
    uint32_t minus_one = 4294967295;
    sockinfo* output = raptor_build_sockinfo();
    output->recieve_fd = setup_recieve_socket(SERV_PORT_RECEIVE);
    byte* data = (byte*) calloc(sizeof(byte),1025);
    output->send_addr_len = sizeof(output->send_addr);
    printf("setup Done\n");
    /*
    if((output->buffer_len = recvfrom(
        output->recieve_fd,
        output->buffer,
        Bufflen,
        0,
        (SA*)output->send_addr,
        &output->send_addr_len))==minus_one)err_quit("raptor_accept_req recvfrom"); 
    */
    strcpy(output->buffer+1,"test");
    output->buffer_len = 5;
    output->buffer[output->buffer_len] = 0;
    printf("bufferlen %d\n",output->buffer_len);
    if((output->send_fd = socket(AF_INET,SOCK_DGRAM,0))==-1)err_quit("socket raptor_accept_req");
    output->send_addr->sin_port = htons(SERV_PORT_SEND);
    raptor_print(output->buffer,output->buffer_len);
    if((output->fp = fopen( (char*) output->buffer+1,"r"))==NULL){
        char ack[] = "file Not Found!\n";
        memcpy(data+1,ack,strlen(ack));
        data[0] = 0;//checksum(data+1,strlen(data+1));
        uint32_t tmp;
        if((tmp=sendto(output->send_fd,data,strlen(ack)+1,0,(SA*)output->send_addr,output->send_addr_len))==minus_one)err_quit("sendto raptor_accept_req");
        printf("send=>%d\n",tmp);
        close(output->recieve_fd);
        free(output->send_addr);
        close(output->send_fd);
        free(output->buffer);
        free(output);
        free(data);
        return raptor_accept_req();
    }
    fseek(output->fp,0,SEEK_END);
    output->file_size = ftell(output->fp);
    fseek(output->fp,0,SEEK_SET);
    return output;
}

void setup_send_socket(char* serv_addr_str,uint16_t serv_port,struct sockaddr_in* serv_addr,int32_t* sock_fd){
    if(!serv_addr){printf("serv_addr ia nill\n");return;}
    bzero(serv_addr,sizeof(*serv_addr));
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(serv_port);
    if(inet_pton(AF_INET,serv_addr_str,(SA*)&serv_addr->sin_addr)==-1)err_quit("inet setup_send_socket");
    if((*sock_fd = socket(AF_INET,SOCK_DGRAM,0))==-1)err_quit("socket setup_send_socket");
    return;
}

void* raptor_listen(void* x){
    int32_t n;
    sockinfo* sock = (sockinfo*)x;
    if((n=recvfrom(sock->recieve_fd,sock->buffer,sock->buffer_len,0,(SA*)sock->send_addr,&sock->send_addr_len))==-1)err_quit("raptor_listen recvfrom"); 
    return NULL;
}

void raptor_send_object(raptor* obj,sockinfo* sock){
    // send object as metadata
    sock->buffer[0]=0;
    memcpy(sock->buffer+1,obj,sizeof(*obj));
    byte temp = sock->buffer+1;
    temp = raptor_serialze(obj,temp);
    raptor* obj2 = (raptor*) malloc(sizeof(raptor));
    memcpy(obj2,obj,sizeof(*obj2));
    raptor_print_object((raptor*)sock->buffer+1);
    raptor_print_object(obj2); 
    if((sendto(sock->send_fd,sock->buffer,sizeof(obj)+1,0,(SA*)sock->send_addr,sock->send_addr_len))==-1)perror("sendto src");
}

void raptor_send_block(raptor* obj,sockinfo* sock,uint16_t block_no){
    byte** data = (byte**) calloc(sizeof(byte*),obj->K);
    byte** int_symb;
    int32_t n;
    uint16_t symb_id=0;
    uint8_t header_size = 4;
    // uint8_t res;
    uint32_t i;
    pthread_t thread;
    uint16_t recieved_block;
    byte* buffer = (byte*) calloc(sizeof(byte),obj->T+header_size);

    for(uint32_t i=0;i<obj->K;i++){
        data[i] = (byte*)calloc(sizeof(byte),obj->T+header_size+1);
        fread(data[i]+header_size,1,obj->T,sock->fp);
        raptor_print(data[i],obj->T+header_size);
    }
    for(uint32_t i=obj->K;i<obj->L;i++)data[i] = (byte*) calloc(sizeof(byte),obj->T+header_size+1);

    // byte** enc_data;// = raptor_encode();
    for(i=0;i<obj->K;i++){
        data[i][0] = block_no >> 8;
        data[i][1] = block_no & 255;
        data[i][2] = symb_id >> 8;
        data[i][3] = symb_id++;
        if((n=sendto(sock->send_fd,data[i],obj->T+header_size,0,(SA*)sock->send_addr,sock->send_addr_len))==-1)perror("sendto src");
    }
    printf("original Data send -------->\n");
    
    pthread_create(&thread,NULL,raptor_listen,sock);
    int_symb = rapter_generate_intermediate_symb(obj,data);
    printf("intermediate DOne++++++++>\n");
    gf2matrix* G_LT = (gf2matrix*)malloc(sizeof(gf2matrix));
    printf("Don2 till this\n");
    allocate_gf2matrix(G_LT,obj->L,obj->L);
    uint32_t* esi = (uint32_t*) malloc(sizeof(uint32_t)*obj->L);
    for(uint32_t i=0;i<obj->L;i++)esi[i] = i+obj->K;
    raptor_build_LT_mat(obj->L,obj,G_LT,esi);
    printf("Donw till this\n");;
    for(uint32_t i=0;i<obj->L;i++){
        if(pthread_tryjoin_np(thread,NULL)==0){
            if((sock->buffer[0] & 128)==0)raptor_send_object(obj,sock);
            else{
                sock->buffer[0] = sock->buffer[0] & 127;
                memcpy(&recieved_block,sock->buffer,sizeof(recieved_block));                
                recieved_block = ntohs(recieved_block);
                if(recieved_block==block_no)break; 
            }
        }
        for(uint32_t j=0;j<obj->L;j++){
            if(get_entry(G_LT,i,j))xor(buffer+header_size,buffer+header_size,int_symb[j],obj->T);
        }
        buffer[0] = (block_no >> 8) | 128;
        buffer[1] = block_no & 255;
        buffer[2] = symb_id >> 8;
        buffer[3] = symb_id++;
        if((n=sendto(sock->send_fd,buffer,obj->T+header_size,0,(SA*)sock->send_addr,sock->send_addr_len))==-1)err_quit("sendto enc");
    }
}

void stostr(byte* output,uint16_t number){
    int bound = 10000;
    uint32_t i=0;
    while(bound){
        output[i++] = number / bound + '0';
        number%=bound;
        bound/=10;
    }
}

void recvfrom_with_timeout(sockinfo* sock ,int timeout_secs){
    fd_set read_fds;
    struct timeval timeout;
    timeout.tv_sec = timeout_secs;
    timeout.tv_usec = 0;
    FD_ZERO(&read_fds);
    FD_SET(sock->recieve_fd, &read_fds);
    int retval = select(sock->recieve_fd + 1, &read_fds, NULL, NULL, &timeout);
    if (retval == -1)err_quit("recv_with_timeout select") ;
    else if (retval == 0)err_quit("Timeout ! No data recieved !"); 
    ssize_t bytes_received ;
    if((bytes_received=recvfrom(sock->recieve_fd,sock->buffer,sock->buffer_len,0,(SA*)sock->send_addr,&sock->send_addr_len))==-1)err_quit("raptor_listen recvfrom"); 
    sock->buffer[bytes_received]=0;
}

void raptor_recieve_block(raptor* obj,sockinfo* sock,uint16_t block_no,uint32_t symbols_count){
    uint32_t received_symbols=0;
    uint16_t recieved_sid;
    uint16_t recieved_block;
    byte header_size = 4;
    uint32_t timeout = 10;
    int32_t previous_sid = -1;
    byte** recieved_data = (byte**) malloc(sizeof(byte*)*symbols_count);
    uint32_t* ESI = (uint32_t*) malloc(sizeof(uint32_t)*symbols_count);
    uint32_t* D = (uint32_t*) calloc(sizeof(uint32_t),symbols_count);
    for(uint32_t i=0;i<symbols_count;i++)recieved_data[i] = (byte*) calloc(sizeof(byte),obj->T);
    while(received_symbols < symbols_count){
        recvfrom_with_timeout(sock,timeout);
        if(sock->buffer[0]==0)continue;
        else sock->buffer[0] = sock->buffer[0] & 127;
        memcpy(&recieved_block,sock->buffer,sizeof(recieved_block));
        recieved_block = ntohs(recieved_block);
        memcpy(&recieved_sid,sock->buffer+2,sizeof(recieved_sid));
        recieved_sid = ntohs(recieved_sid);
        if(received_symbols != block_no && recieved_sid <= previous_sid)continue;
        ESI[received_symbols] = recieved_sid;
        previous_sid = recieved_sid;
        memcpy(recieved_data[received_symbols],sock->buffer+header_size,obj->T);
        received_symbols++;
    }
    gf2matrix* A = (gf2matrix*) malloc(sizeof(gf2matrix));
    allocate_gf2matrix(A,obj->L,obj->L);
    raptor_build_constraints_mat(obj,A);
    gf2matrix* gauss_mat = (gf2matrix*) malloc(sizeof(gf2matrix));
    allocate_gf2matrix(gauss_mat,symbols_count,obj->K);
    gf2matrix* dummy_row = (gf2matrix*) malloc(sizeof(gf2matrix));
    allocate_gf2matrix(dummy_row,1,obj->L);
    uint32_t L_ = obj->L;
    while(!is_prime(L_))L_++;
    for(uint32_t i=0;i<symbols_count;i++){
        if(ESI[i]<obj->K){
            D[i] = 1;
            set_entry(gauss_mat,i,ESI[i],1);
        }
        else{
            LTEncode(obj,dummy_row,ESI[i],0,L_);
            for(uint32_t j=0;j<obj->L;j++){
                if(get_entry(dummy_row,0,j)){
                    copy_row(gauss_mat,i,A,j);
                }
            }
            for(uint32_t j=0;j<gauss_mat->n_words;j++){
                D[i] += __builtin_popcount(gauss_mat->rows[i][j]);
            } 
            for(uint32_t p=0;p<dummy_row->n_words;p++)dummy_row->rows[0][p]=0;
        }  
    }
    printf("gaussian elimination => %d \n",gaussian_elim(gauss_mat,recieved_data,obj,D));
    for(uint32_t i=0;i<obj->K;i++)printf("%hhn ",recieved_data[i]);
}
