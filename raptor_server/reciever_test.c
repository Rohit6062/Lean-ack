#include"../raptor_header/base.h"
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
int main()
{
    sockinfo* sock = raptor_accept_req();
    raptor* obj = (raptor*) malloc(sizeof(raptor));
    bzero(obj,sizeof(raptor));
    printf("%p\n",sock);
    obj->F = sock->file_size;
    obj->Kmin = 1024;
    obj->Kmax = 8192;
    obj->Gmax = 10;
    obj->Al   = 4;
    obj->P    = 65480;
    uint32_t W = 10485760;
    // obj->N    = 1;
    // obj->T    = 8192;
    printf("filesize %ld\n",obj->F);
    obj->G = min(min(ceil( devide( obj->P*obj->Kmin,obj->F)) , ceil(devide(obj->P,obj->Al))), obj->Gmax);
    obj->T = floor(devide(obj->P,(obj->Al*obj->G)))*obj->Al;
    double kt =(double) ceil(devide(obj->F,obj->T));
    obj->Z = ceil(devide(kt,obj->Kmax));
    obj->N = min(ceil(devide(kt,obj->Z) * devide(obj->T,W)), ceil(devide(obj->T,obj->Al)));
    obj->K = ceil(devide((uint32_t) kt,obj->Z));
    raptor_compute_params(obj);
    raptor_print_socket(sock);
    raptor_print_object(obj);
    raptor_send_object(obj,sock);
    gf2matrix k;
    allocate_gf2matrix(&k,obj->L,obj->L);
    for(uint32_t i=0;i<obj->Z;i++){
        raptor_send_block(obj,sock,i+1);
        sleep(1);
    }
    return 0;
}
