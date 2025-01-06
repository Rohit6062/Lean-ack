#include<gf2matrix.h>
#include<raptor10.h>
#include<stack.h>
#include<time.h>
int main() {
  srand(time(NULL));
  printf("Testing gf2matrix\n");
  Raptor10 coder;
  // char* data = "this_is_some_data_to_test";
    char* data = "rohit";
  unsigned int K = strlen(data);
  coder.K = K;
  coder.Kmin = 1024;
  coder.Kmax = 8192;
  coder.Gmax = 10;
  coder.Al = 4;
  coder.N = coder.K;
  coder.T = 1;
  r10_compute_params(&coder);
  uint8_t* src_s = (uint8_t*) calloc(coder.L*coder.T, sizeof(uint8_t));
  uint8_t* enc_s = (uint8_t*) calloc(sizeof(uint8_t),coder.L*coder.T);
  strcpy(src_s,data);
  printf("%s\n",src_s);
  printf("K=%u, S=%u, H=%u, L=%u ,W=%u P=%u", coder.K, coder.S, coder.H, coder.L,coder.W,coder.P);
  my_encode(src_s,enc_s,&coder);
  uint32_t N = coder.K + 3;
  uint8_t* rec_s = (uint8_t*) malloc(sizeof(uint8_t)*N);
  uint32_t* ESIs = malloc(sizeof(uint32_t)*N);
  for(uint32_t i=0;i<N;i++){
    ESIs[i] = rand()%coder.L;
    if(ESIs[i] < coder.K) rec_s[i] = src_s[ESIs[i]];
    else rec_s[i] = enc_s[ESIs[i]-coder.K];
  }
  for(int i=0;i<N;i++)printf("%d ",rec_s[i]);
  printf("\n\n");
  my_decode(rec_s,&coder,ESIs,N);
//   gf2matrix* mat = (gf2matrix*)malloc(sizeof(gf2matrix));
//   allocate_gf2matrix(mat,5,5);
//     char* ESIs_symb = (char*) malloc(sizeof(char)*7);
//     uint8_t ESIs[] = {1,105,116,105,114};
//   int* d = malloc(sizeof(int)*5);
//   for(int i=0;i<5;i++)ESIs_symb[i]=ESIs[i];
//   set_entry(mat,4,0,1);
//   d[4]=1;
//   set_entry(mat,3,3,1);
//   d[3]=1;
//   set_entry(mat,2,4,1);
//   d[2]=1;
//   set_entry(mat,1,0,1);
//   set_entry(mat,1,1,1);
//   set_entry(mat,1,4,1);
//   d[1] = 3;
//   set_entry(mat,0,2,1);
//   set_entry(mat,0,3,1);
//   d[0]=2;
//   print_matrix2(mat,ESIs_symb);
//     
// // int gaussian_elimination(gf2matrix* mat, uint8_t *result, int size,Raptor10* obj){
//   gaussian_elim(mat,ESIs_symb,5,&coder,d);
//   for(int i=0;i<5;i++)printf("%c",ESIs_symb[i]);






  // Now, enc_s should contain the encoded symbols
  // Still, doesn't allow to decide the size of the symbols

  // printf("Constraints matrix:\n");
  // print_matrix(&A);
  //   // decoder 
  //   gf2matrix B;
    // uint32_t ESIs [] = {0,2,3,5,6,7,8};
    // int N = sizeof(ESIs)/sizeof(ESIs[0]);
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
  //   for(int i=0;i<N;i++)for_decoder[i+decoder.S+decoder.H] = ESIs[i]; 
    // uint8_t *dec_s = (uint8_t*) calloc(K, sizeof(uint8_t));
    // uint8_t for_decoder[] = {'r','h','i',27,6,117,1}; 
  //   // void r10_decode(uint8_t *enc_s, uint8_t *dec_s, Raptor10 *obj, gf2matrix *A,uint32_t N_,uint32_t* ESIs) {
    // r10_decode(for_decoder,dec_s,&coder,&A,N,ESIs);
}
