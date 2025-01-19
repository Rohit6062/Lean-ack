#include"/home/rohit/Desktop/Network/udp_project/raptor_header/base.h"

//
// void raptor_compute_params(raptor *obj){
//     if (!obj->Al && !obj->K && !obj->Kmax && !obj->Kmin && !obj->Gmax){printf("parameters required\n");exit(0);}
//     uint32_t X = floor(sqrt(2 * obj->K));
//     for (; X * X < 2 * obj->K + X; X++);
//     for (obj->S = ceil(0.01 * obj->K) + X; !is_prime(obj->S); obj->S++);
//     for (obj->H = 1; choose(obj->H, ceil(obj->H / 2)) < obj->K + obj->S; obj->H++);
//     obj->L = obj->K + obj->S + obj->H;
// }
/* {
    FILE* fp;
    uint32_t file_size;
    socklen_t send_fd;
    struct sockaddr_in* send_addr;
    socklen_t send_addr_len;
    socklen_t recieve_fd;
    byte* buffer;
    uint32_t buffer_len;
}sockinfo;
 * */

// void raptor_send_block(raptor* obj,sockinfo* sock,uint16_t block_no){

int main()
{
    sockinfo* tmp = raptor_accept_req();
    raptor* obj = (raptor*) malloc(sizeof(raptor));
    bzero(obj,sizeof(raptor));
    printf("%p\n",tmp);
    obj->K = tmp->file_size;
    obj->Kmin = 1024;
    obj->Kmax = 8192;
    obj->Gmax = 10;
    obj->Al = 4;
    obj->P = 65200;
    obj->N = 1;
    raptor_compute_params(obj);
    raptor_send_block(obj,tmp,0);
    return 0;
}
