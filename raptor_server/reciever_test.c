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
    printf("%p\n",sock);
    raptor* obj =  raptor_init_obj(sock->file_size);
    raptor_print_socket(sock);
    raptor_send_object(obj,sock);
    gf2matrix k;
    allocate_gf2matrix(&k,obj->L,obj->L);
    sleep(5);
    for(uint32_t i=0;i<obj->Z;i++){
        raptor_send_block(obj,sock,i+1);
        sleep(1);
    }
    return 0;
}
