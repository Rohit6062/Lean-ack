#include"../raptor_header/base.h"

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
    obj->F = tmp->file_size;
    obj->Kmin = 1024;
    obj->Kmax = 8192;
    obj->Gmax = 10;
    obj->Al   = 4;
    obj->P    = 65480;
    uint32_t W = 10485760;
    // obj->N    = 1;
    // obj->T    = 8192;
    printf("filesixe %d\n",obj->F);
    obj->G = min(min(ceil( devide( obj->P*obj->Kmin,obj->F)) , ceil(devide(obj->P,obj->Al))), obj->Gmax);
    printf("G => %d\n",obj->G);
    obj->T = floor(devide(obj->P,(obj->Al*obj->G)))*obj->Al;
    printf("T => %d\n",obj->T);
    double kt =(double) ceil(devide(obj->F,obj->T));
    printf("kt=> %f\n",kt);
    obj->Z = ceil( devide(kt,obj->Kmax));
    printf("Z = %d\n",obj->Z);
    obj->N = min(ceil(devide(kt,obj->Z) * devide(obj->T,W)), ceil(devide(obj->T,obj->Al)));
    printf("N %d\n",obj->N);
    obj->K = ceil(devide((uint32_t) kt,obj->Z));
    printf("k %d\n",(uint32_t)obj->K);
    raptor_compute_params(obj);
    printf("k => %d l=> %d s=> %d h=> %d\n",obj->K,obj->L,obj->S,obj->H);
    printf("obj->G %d,obj->p %d , T %d\n",obj->G,obj->P,obj->T);
    gf2matrix k;
    allocate_gf2matrix(&k,obj->L,obj->L);
    printf("\n%p\n",&k);
    for(uint32_t i=0;i<obj->Z;i++){
        raptor_send_block(obj,tmp,i+1);
    }
    return 0;
}
