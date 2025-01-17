#include "raptor_header.h"
#include <math.h>
#include <stdint.h>
#include<stdbool.h>

void generate_gray_seq(uint32_t *gray_seq) {
  for (uint32_t i = 0; i < 4000; i++)
    gray_seq[i] = i ^ (uint32_t)(floor(i / 2));
}

void xor(byte* result,byte* a,byte* b,uint32_t n){
    for(uint32_t i=0;i<n;i++){
      result[i] = a[i] ^ b[i];
  }
}

uint64_t factorial(uint64_t n) {
  uint64_t result = 1, i;

  for (i = 2; i <= n; i++)
    result *= i;

  return result;
}
int is_prime(uint32_t num) {
    if (num <= 1) {
        return 0; // Numbers less than or equal to 1 are not prime
    }
    if (num == 2) {
        return 1; // 2 is the only even prime number
    }
    if (num % 2 == 0) {
        return 0; // Any other even number is not prime
    }

    // Only check odd numbers from 3 to sqrt(num)
    for (int i = 3; i*i <= num; i += 2) {
        if (num % i == 0) {
            return 0; // If divisible by any number, it's not prime
        }
    }

    return 1; // If no divisors are found, it's prime
}

uint32_t choose(uint32_t i, uint32_t j) {
  // printf("%d %d i,j\n",i,j);
  if(i==j || !j)return factorial(i);
  return (factorial(i) / (factorial(j) * factorial(abs(i - j))));
}

void raptor_Trip(uint32_t K, uint32_t X, uint32_t triple[3], raptor *obj) {
  obj->L = obj->K + obj->S + obj->H;
  uint32_t L_ = obj->L;
  while (!is_prime(L_))
    L_++;

  uint32_t Q = 65521;
  uint32_t A = (53591 + J[K - 4] * 997) % Q;
  uint32_t B = 10267 * (J[K - 4] + 1) % Q;
  uint32_t Y = (B + X * A) % Q;
  // raptor_Rand is passed 2^^20 as required by the RFC5053
  uint32_t v = raptor_Rand(Y, 0, 1<<20);
  uint32_t d = raptor_Deg(v);
  uint32_t a = 1 + raptor_Rand(Y, 1, L_ - 1);
  uint32_t b = raptor_Rand(Y, 2, L_);

  triple[0] = d;
  triple[1] = a;
  triple[2] = b;
}

// raptor* build_raptor(unsigned long file_size){
//     raptor* out = (raptor*) malloc(sizeof(raptor));    
//     bzero(out,sizeof(out));
//     out->K = file_size;
//     out->Kmin = 1024;
//     out->Kmax = 8192;
//     out->Gmax = 10;
//     out->Al = 4;
//     out->P = 65200;
//     return out;
// }
//
//
uint32_t raptor_Rand(uint32_t X, uint32_t i, uint32_t m) {
  return (V0[(X + i) % 256] ^ V1[((uint32_t)floor(X / 256) + i) % 256]) % m;
}

uint32_t raptor_Deg(uint32_t v) {
  if (v < 10241)
    return 1;
  if (v < 491582)
    return 2;
  if (v < 712794)
    return 3;
  if (v < 831695)
    return 4;
  if (v < 948446)
    return 10;
  if (v < 1032189)
    return 11;
  if (v < 1048576)
    return 40;
  return -1;
}

int raptor_build_LDPC_submat(int K, int S, gf2matrix *A) {
  int a = 0, b = 0;
  for (int i = 0; i < K; i++){
    a = 1 + ((int)floor(i / S) % (S - 1));
    b = i % S;
    set_entry(A, b, i, 1);
    b = (b + a) % S;
    set_entry(A, b, i, 1);
    b = (b + a) % S;
    set_entry(A, b, i, 1);
  }
    return 0;
}

int raptor_build_Half_submat(unsigned int K, unsigned int S, unsigned int H,gf2matrix *A) {
  uint32_t g[4000];
  generate_gray_seq(&g[0]);
  uint32_t H_ = ceil((float)H / 2.0);
  size_t n_words = 4000;
  uint32_t m[n_words];

  uint32_t j = 0;
  for (size_t i = 0; i < n_words; i++){
    if (__builtin_popcount(g[i]) == H_) {
      m[j] = g[i];
      j++;
    }
    printf("j = %d\n", j);
    }

  // Build the G_HALF submatrix
  for (uint32_t h = 0; h < H; h++) {
    for (uint32_t j = 0; j < K + S; j++) {
      if (m[j] & (1UL << h)) {
        set_entry(A, h + S, j, 1);
      }
    }
  }
    return 0;
}

int raptor_build_LT_submat(uint32_t K, uint32_t S, uint32_t H, raptor *obj,gf2matrix *A) {
  uint32_t L = K + S + H;
  uint32_t L_ = L;
  while (!is_prime(L_))
    L_++;
  for(uint32_t i = 0; i < K; i++)LTEncode(obj,A,i,i+S+H,L_);
    return 0;
}

void LTEncode(raptor* obj,gf2matrix* mat,uint32_t x, uint32_t row_index ,uint32_t L_){
    uint32_t triple[3];
    raptor_Trip(obj->K,x,triple,obj);
    uint32_t j_max = fmin((triple[0]-1),(obj->L-1));
    while(triple[2] >= obj->L) triple[2] = (triple[2] + triple[1]) % L_;
    set_entry(mat,row_index,triple[2],1);
    for(uint32_t j =1;j<=j_max;j++){
        do {
            triple[2] = (triple[2] + triple[1]) % L_;
        }while (triple[2] >= obj->L); 
        set_entry(mat,row_index,triple[2],1);
    }
}
void raptor_build_LT_mat(uint32_t N_, raptor *obj, gf2matrix *G_LT,uint32_t *ESIs) {
    printf("N_ = %d\n", N_);
    obj->L = obj->K + obj->S + obj->H;
    uint32_t L_ = obj->L;
    while (!is_prime(L_))L_++;
    for (uint32_t i = 0; i < N_; i++) {
        LTEncode(obj,G_LT,ESIs[i],i,L_);
    }
}

int gaussian_elim(gf2matrix* mat,byte** result, raptor* obj, int* d){
    char dest;
    int tmp;
    bool is_start = true;
    queue* stk = queue_build();
    uint32_t i = 0;
    uint32_t* counter = (uint32_t*) calloc(sizeof(uint32_t),get_nrows(mat));
    byte* temp_buffer = (byte*)calloc(sizeof(byte),obj->T);
    for(i=0;i<get_ncols(mat);i++){
        if(!get_entry(mat,i,i)){
            is_start = true;
            for(uint32_t j=i+1;j<get_nrows(mat);j++){
                if(get_entry(mat,j,i) && (is_start || d[j] <= d[i])){
                    strncpy(temp_buffer,result[i],obj->T);
                    strncpy(result[j],result[i],obj->T);
                    strncpy(result[i],temp_buffer,obj->T);
                    tmp = d[i];
                    d[i] = d[j];
                    d[j] = tmp;
                    swap_rows(mat,i,j);
                    is_start = false;
                }
            }
        }
        if(!d[i]) return -1;
        if(d[i]>1){queue_push(stk,i);continue;}
        for(uint32_t j=0;j<get_nrows(mat);j++){
            if(j != i && get_entry(mat,j,i)){
                set_entry(mat,j,i,0);
                // result[j]^=result[i];
                xor(result[j],result[j],result[i],obj->T);
                d[j]-=1;
            }
        }
    }
    // print_matrix2(mat,result);
    queue_display(stk);
    while(!queue_isempty(stk)){
        i = queue_pop(stk);
        if(counter[i]>5)continue;
        counter[i]++;
        if(d[i]>1){
            queue_push(stk,i);
            continue;
        }
        for(uint32_t j=0;j<get_nrows(mat);j++){
          if(j != i && get_entry(mat,j,i)){
              set_entry(mat,j,i,0);
              xor(result[j],result[j],result[i],obj->T);
              // result[j]^=result[i];
              d[j]-=1;
          }
      }
      // print_matrix2(mat,result);
  }
  return 0;
} 
int raptor_build_constraints_mat(raptor *obj, gf2matrix *A){
    raptor_build_LDPC_submat(obj->K, obj->S, A);
    raptor_build_Half_submat(obj->K, obj->S, obj->H, A);
    for (uint32_t i = 0; i < obj->S; i++)
    set_entry(A, i, obj->K + i, 1);
    for (uint32_t i = 0; i < obj->H; i++)
    set_entry(A, obj->S + i, obj->K + obj->S + i, 1);
    raptor_build_LT_submat(obj->K, obj->S, obj->H, obj, A);
    gaussjordan_inv(A);
    print_matrix(A);
    return 0;
}

void raptor_compute_params(raptor *obj){
    if (!obj->Al && !obj->K && !obj->Kmax && !obj->Kmin && !obj->Gmax){printf("parameters required\n");exit(0);}
    uint32_t X = floor(sqrt(2 * obj->K));
    for (; X * X < 2 * obj->K + X; X++);
    for (obj->S = ceil(0.01 * obj->K) + X; !is_prime(obj->S); obj->S++);
    obj->H=1;
    while(true){
        if(choose(obj->H,ceil( (double)obj->H/(double)2 )) > obj->K+obj->S )break;
        obj->H = obj->H + 1;
    }
    obj->L = obj->K + obj->S + obj->H;
}

void raptor_multiplication(raptor *obj, gf2matrix *A, byte **block,byte** res_block) {
    for (uint32_t j = 0; j < get_ncols(A); j++){
        for (uint32_t i = 0; i < get_nrows(A); i++){
            if(get_entry(A, i,j))xor(res_block[i],res_block[i],block[j],obj->T);
        }
    }
}
void copy_row(raptor* obj,gf2matrix* mat1,uint32_t mat1_row,gf2matrix* mat2,uint32_t mat2_row){
  for(uint32_t i=0;i<mat1->n_words;i++){
    mat1->rows[mat1_row][i]^=mat2->rows[mat2_row][i];
  }
}
/*
 * Assumptions:
 * src_s data is padded wiht 0 so that len(src_s) == obj->L*obj->T;
 * all parameters of obj (raptor) is properly set
    */
// void my_encode(uint8_t *src_s,uint8_t* enc_s, raptor* obj){
//     gf2matrix* A = malloc(sizeof(gf2matrix));
//     gf2matrix* G_LT = malloc(sizeof(gf2matrix));
//     uint8_t* int_symbols = calloc(sizeof(uint8_t),obj->L*obj->T);
//     allocate_gf2matrix(A,obj->L,obj->L);
//     raptor_build_constraints_mat(obj,A);
//     print_matrix(A);
//     bzero(src_s+obj->K,obj->L-obj->K);
//     raptor_multiplication(obj,A,src_s,int_symbols);
//     for(int i=0;i<obj->L;i++)printf("%d ",int_symbols[i]);
//     printf("intermediate symbols encode\n");
//     allocate_gf2matrix(G_LT,obj->L,obj->L);
//     uint32_t* esi = (uint32_t*) malloc(sizeof(uint32_t)*obj->L);
//     for(int i=0;i<obj->L;i++)esi[i] = i+obj->K;
//     raptor_build_LT_mat(obj->L,obj,G_LT,esi);
//     print_matrix(G_LT);
//     printf("GLT\n");
//     raptor_multiplication(obj,G_LT,int_symbols,enc_s);
//     for(int i=0;i<obj->L;i++)printf("%d ",enc_s[i]);
//     printf("encoded symbols encode\n");
// }
// void my_decode(uint8_t* enc_s, raptor* obj,uint32_t* ESIs,uint32_t n){
//     gf2matrix* A =malloc(sizeof(gf2matrix));
//     gf2matrix* gauss_mat = malloc(sizeof(gf2matrix));
//     gf2matrix* dummy_row = malloc(sizeof(gf2matrix));
//     allocate_gf2matrix(dummy_row,1,obj->L);
//     allocate_gf2matrix(gauss_mat,n,obj->K);
//     allocate_gf2matrix(A,obj->L,obj->L);
//     raptor_build_constraints_mat(obj,A);  
//     uint32_t L_ = obj->L;
//     while(!is_prime(L_))L_++;
//     uint32_t* d = calloc(sizeof(uint32_t),n);
//     for(int i=0;i<n;i++){
//         printf("i %d data %d\n",i,enc_s[i]);
//         if( ESIs[i] < obj->K ){
//             d[i] = 1;
//             set_entry(gauss_mat,i,ESIs[i],1);
//         }
//         else{
//             LTEncode(obj,dummy_row,ESIs[i],0,L_);
//             print_matrix2(dummy_row,enc_s);
//             printf("step 1\n");
//             for(int j=0;j<obj->L;j++){
//                 if(get_entry(dummy_row,0,j)){
//                     printf("copy %d %d \n",i,j);
//                     copy_row(obj,gauss_mat,i,A,j);
//                     print_matrix2(gauss_mat,enc_s);
//                 }
//             }
//             print_matrix2(gauss_mat,enc_s);
//             printf("step 2\n");
//             for(uint32_t j=0;j<gauss_mat->n_words;j++){
//                 d[i]+= __builtin_popcount(gauss_mat->rows[i][j]);
//               printf("gauss_mat->rows[0][j] = %d\n", gauss_mat->rows[i][j]);
//             }
//             for(uint32_t p=0;p<dummy_row->n_words;p++)dummy_row->rows[0][p]=0;
//             print_matrix2(dummy_row,enc_s);
//             printf("Done d %d\n",d[i]);
//         }
//       print_matrix2(gauss_mat,enc_s);
//     }
//     printf("gaussian_elimination\n");
//     printf("elim = > %d\n",gaussian_elim(gauss_mat,enc_s,obj,d));
//     for(int i=0;i<obj->K;i++)printf("%c ",enc_s[i]);
// }

// void raptor_decode(uint8_t *enc_s, uint8_t *dec_s, raptor *obj, gf2matrix *A,uint32_t N_,uint32_t* ESIs) {
//     gaussjordan_inv(A);
//     uint8_t* int_symbols = (uint8_t*) calloc(obj->L, sizeof(uint8_t));
//     raptor_multiplication(obj, A, enc_s, int_symbols);
//     for(int i=0;i<obj->L;i++)printf("%d ",int_symbols[i]);
//     printf("<-- decode intermediate \n");
//     // gf2matrix G_LT;
//     // allocate_gf2matrix(&G_LT, obj->L, obj->K);
//     // Build the LT matrix and decode
//     // raptor_build_LT_mat(N_, obj, &G_LT, ESIs);
//     // gaussjordan_inv(&G_LT);
//     // print_matrix(&G_LT);
//     raptor_multiplication(obj, A, int_symbols, dec_s);
//     for(int i=0;i<obj->K;i++)printf("%c ",dec_s[i]);
// }

byte** rapter_generate_intermediate_symb(raptor* obj,byte** data){
    gf2matrix* A = malloc(sizeof(gf2matrix));
    byte** output = (byte**) malloc(sizeof(byte*)*obj->L);
    for(uint32_t i=0;i<obj->L;i++)output[i] = (byte*)malloc(sizeof(byte)*obj->T);
    allocate_gf2matrix(A,obj->L,obj->L);
    raptor_build_constraints_mat(obj,A);
    raptor_multiplication(obj,A,data,output);
    return output;
}
