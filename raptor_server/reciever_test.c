#include"./../raptor_header/base.h"
void raptor_send_block(raptor* obj,sockinfo* sock,uint16_t block_no)
{
  byte** data = (byte**) malloc(sizeof(byte*)*obj->K);
  uint32_t n;
  uint8_t symb_id=0;
  uint8_t header_size = 3;
  for(int i=0;i<obj->K;i++)
  {
    data[i] = (byte*)calloc(sizeof(byte),obj->T+header_size);
    fread(data[i]+3,1,obj->T,sock->fp);
  }
  byte** enc_data;// = raptor_encode();
  for(uint32_t i=0;i<obj->K;i++){
      data[i][0] = block_no >> 8;
      data[i][1] = block_no & 255;
      data[i][2] = symb_id++;
      if((n=sendto(sock->send_fd,data[i],obj->T+header_size,0,(SA*)sock->send_addr,sock->send_addr_len))==-1)perror("sendto");
  }
        // if((tmp=sendto(sock->send_fd,data,strlen(data+1)+1,0,(SA*)sock->send_addr,sock->send_addr_len))==-1)err_quit("sendto raptor_accept_req");

}
int main()
{
    sockinfo* tmp = raptor_accept_req();      
    raptor Obj; 

}
