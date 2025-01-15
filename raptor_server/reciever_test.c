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



int main()
{
    sockinfo* tmp = raptor_accept_req();      
    raptor* obj = (raptor*) malloc(sizeof(raptor)); 
    bzero(obj,sizeof(raptor));
    printf("%p\n",tmp);
    obj->K = tmp->;
    obj->Kmin = 1024;
    obj->Kmax = 8192;
    obj->Gmax = 10;
    obj->Al = 4;
    obj->P = 65200;
    return 0;
}
