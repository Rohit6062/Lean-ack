#include <gf2matrix.h>
#include <raptor10.h>

int main() {
  printf("Testing gf2matrix\n");

  // Create a Raptor10 object and fill it w/ all known needed params
  Raptor10 coder;
  char* data = "rohit";
  unsigned int K = strlen(data);
  coder.K = K;
  coder.Kmin = 1024;
  coder.Kmax = 8192;
  coder.Gmax = 10;
  coder.Al = 4;
  coder.N = 5;
  coder.T = 1;
  r10_compute_params(&coder);
  printf("K=%u, S=%u, H=%u, L=%u\n", coder.K, coder.S, coder.H, coder.L);
  // Allocate and calculate the constraints matrix
  uint8_t* enc_s = (uint8_t*) calloc(sizeof(uint8_t),coder.L*coder.T);
  uint8_t* src_s = (uint8_t*) calloc(coder.K*coder.T, sizeof(uint8_t));
// void my_encode(uint8_t *src_s,uint8_t* enc_s, Raptor10* obj){
  strcpy(src_s,data);
  my_encode(src_s, enc_s, &coder);
    gf2matrix* mat = (gf2matrix*)malloc(sizeof(gf2matrix));
    uint8_t* recived_symb = (uint8_t*) malloc(sizeof(uint8_t)*7);
    recived_symb[0] = 'r';
    set_entry(mat,0,0);
    recived_symb[1] = 'h';
    set_entry(mat,1,2);
    recived_symb[2] = 'i';
    set_entry(mat,2,3);
    recived_symb[3] = 't';
    set_entry(mat,3,4);
    strncpy(recived_symb[4],enc_s[0],1);





  // Now, enc_s should contain the encoded symbols
  // Still, doesn't allow to decide the size of the symbols

  // printf("Constraints matrix:\n");
  // print_matrix(&A);
  //   // decoder 
  //   gf2matrix B;
    // uint32_t recived [] = {0,2,3,5,6,7,8};
    // int N = sizeof(recived)/sizeof(recived[0]);
    // bzero(enc_s+N,coder.L*coder.T-N);
  //   Raptor10 decoder;
  //     decoder.K = K;
  //     decoder.Kmin = 1024;
  //     decoder.Kmax = 8192;
  //     decoder.Gmax = 10;
  //     decoder.Al = 4;
  //     decoder.N = 1;
  //     decoder.T = 1;
  //   r10_compute_params(&decoder);
  // printf("K=%u, S=%u, H=%u, L=%u\n", decoder.K, decoder.S, decoder.H, decoder.L);
  //   int M = decoder.S+decoder.H+N;
  //   allocate_gf2matrix(&B,M,decoder.L);
  //   r10_build_constraints_mat(&decoder,&B);
  //   print_matrix(&B);
  //   uint8_t* for_decoder = (uint8_t*) calloc(sizeof(uint8_t),M);
  //   for(int i=0;i<N;i++)for_decoder[i+decoder.S+decoder.H] = recived[i]; 
    // uint8_t *dec_s = (uint8_t*) calloc(K, sizeof(uint8_t));
    // uint8_t for_decoder[] = {'r','h','i',27,6,117,1}; 
  //   // void r10_decode(uint8_t *enc_s, uint8_t *dec_s, Raptor10 *obj, gf2matrix *A,uint32_t N_,uint32_t* ESIs) {
    // r10_decode(for_decoder,dec_s,&coder,&A,N,recived);

}
