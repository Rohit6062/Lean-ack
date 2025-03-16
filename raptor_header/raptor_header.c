#include "raptor_header.h"
#include <math.h>
#include <stdint.h>
#include<stdbool.h>



void generate_gray_seq(uint32_t *gray_seq) {
  for (uint32_t i = 0; i < 4000; i++)
    gray_seq[i] = i ^ (uint32_t)(floor(i / 2));
}

void xor(byte* result,byte* a,byte* b,uint32_t n){
    // printf("\t%p %p\n",a,b);
    for(uint32_t i=0;i<n;i++){
        // printf("\t %d\t%d\n",n,i);
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

uint32_t choose(int32_t i, int32_t j) {
  // printf("%d %d i,j\n",i,j);
  if(i==j || !j)return factorial(i);
  return (factorial(i) / (factorial(j) * factorial(abs(i - j))));
}

void raptor_Trip(uint32_t K, uint32_t X, uint32_t triple[3], raptor *obj) {
    obj->L = obj->K + obj->S + obj->H;
    uint32_t L_ = obj->L;
    while (!is_prime(L_))
    L_++;
    uint32_t A;
    uint32_t B;

    uint32_t Q = 65521;
    if(K>=4){
        A = (53591 + J[K - 4] * 997) % Q;
        B = 10267 * (J[K - 4] + 1) % Q;
    }
    else{
        A =(53591 + J[0]*997)%Q;
        B = 10267 + (J[0]+1)%Q;
    }
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
  for(int i = 0; i < K; i++){
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
    // printf("j = %d\n", j);
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
  for(uint32_t i = 0; i < K; i++)printf("%d\n",i),LTEncode(obj,A,i,i+S+H,L_);
    return 0;
}

void LTEncode(raptor* obj,gf2matrix* mat,uint32_t x, uint32_t row_index ,uint32_t L_){
    printf("done\n");
    uint32_t* triple = (uint32_t*)malloc(sizeof(uint32_t)*3);
    raptor_Trip(obj->K,x,triple,obj);
    printf("triple\n");
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

int gaussian_elim(gf2matrix* mat,byte** result, raptor* obj, uint32_t* d){
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
                    memcpy(temp_buffer,result[i],obj->T);
                    memcpy(result[j],result[i],obj->T);
                    memcpy(result[i],temp_buffer,obj->T);
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
    printf("done raptor_build_constraints_mat\n");
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

void raptor_multiplication(raptor *obj, gf2matrix *A, byte **block,byte** res_block){
    for (uint32_t j = 0; j < get_ncols(A); j++){
        for (uint32_t i = 0; i < get_nrows(A); i++){
            if(get_entry(A, i,j))xor(res_block[i],res_block[i],block[j],obj->T);
        }
    }
    printf("Done raptor_multiplication\n");
}
void copy_row(gf2matrix* mat1,uint32_t mat1_row,gf2matrix* mat2,uint32_t mat2_row){
  for(uint32_t i=0;i<mat1->n_words;i++){
    mat1->rows[mat1_row][i]^=mat2->rows[mat2_row][i];
  }
}

byte** rapter_generate_intermediate_symb(raptor* obj,byte** data){
    gf2matrix* A = malloc(sizeof(gf2matrix));
    byte** output = (byte**) calloc(sizeof(byte*),obj->L);
    for(uint32_t i=0;i<obj->L;i++)output[i] = (byte*)calloc(sizeof(byte),obj->T);
    allocate_gf2matrix(A,obj->L,obj->L);
    raptor_build_constraints_mat(obj,A);
    raptor_multiplication(obj,A,data,output);
    return output;
}

void raptor_print_object(raptor* obj){
    printf("<- Raptor Object ->\n");
    printf("\tF %ld\n",obj->F);
    printf("\tKmin %d\n",obj->Kmin);
    printf("\tKmax %d\n",obj->Kmax);
    printf("\tGmax %d\n",obj->Gmax);
    printf("\tAl %d\n",obj->Al);
    printf("\tP %d\n",obj->P);
    printf("\tN %d\n",obj->N);
    printf("\tT %d\n",obj->T);
    printf("\tG %d\n",obj->G);
    printf("\tZ %d\n",obj->Z);
    printf("\tK %d\n",obj->K);
    printf("\tL %d\n",obj->L);
    printf("\tS %d\n",obj->S);
    printf("\tH %d\n",obj->H);
    printf("------------\n");
}

byte* serialize_int(byte*buffer, int value)
{
  /* Write big-endian int value into buffer; assumes 32-bit int and 8-bit char. */
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;
  return buffer + 4;
}
byte* serialize_char(byte*buffer, char value)
{
  buffer[0] = value;
  return buffer + 1;
} 
byte* raptor_serialze(raptor* obj,byte* buffer){
    printf("obj -> %p and buffer %p \n",obj,buffer); 
    buffer = serialize_int(buffer,obj->F);
    printf("done f\n");
    buffer = serialize_int(buffer,obj->W);
    printf("done W\n");
    buffer = serialize_int(buffer,obj->P);
    buffer = serialize_int(buffer,obj->Al);
    buffer = serialize_int(buffer,obj->Kmax);
    buffer = serialize_int(buffer,obj->Kmin);
    buffer = serialize_int(buffer,obj->Gmax);
    buffer = serialize_int(buffer,obj->T);
    buffer = serialize_int(buffer,obj->Z);
    buffer = serialize_int(buffer,obj->N);
    buffer = serialize_int(buffer,obj->K);
    buffer = serialize_int(buffer,obj->L);
    buffer = serialize_int(buffer,obj->S);
    buffer = serialize_int(buffer,obj->H);
    buffer = serialize_int(buffer,obj->G);
    return buffer;
} 