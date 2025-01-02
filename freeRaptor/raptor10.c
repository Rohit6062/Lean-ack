/*
 *  Copyright 2020 Roberto Francescon
 *  Copyright 2022 Dominik Danelski
 *  This file is part of freeRaptor.
 *
 *  freeRaptor is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  freeRaptor is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with freeRaptor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "raptor10.h"
#include <math.h>
#include <stdint.h>
#include<stdbool.h>

void generate_gray_seq(uint32_t *gray_seq) {
  for (uint32_t i = 0; i < 4000; i++)
    gray_seq[i] = i ^ (uint32_t)(floor(i / 2));
}

int factorial(int n) {
  int result = 1, i;

  for (i = 2; i <= n; i++)
    result *= i;

  return result;
}

int is_prime(uint32_t n) {
  if (n <= 1)
    return 0;

  for (uint32_t i = 2; i * i <= n; i++)
    if (!(n % i))
      return 0;

  return 1;
}

int choose(int i, int j) {
  return (factorial(i)) / (factorial(j) * factorial(i - j));
}

void r10_Trip(uint32_t K, uint32_t X, uint32_t triple[3], Raptor10 *obj) {
  uint32_t L = obj->K + obj->S + obj->H;
  uint32_t L_ = obj->L;
  while (!is_prime(L_))
    L_++;

  uint32_t Q = 65521;
  uint32_t A = (53591 + J[K - 4] * 997) % Q;
  uint32_t B = 10267 * (J[K - 4] + 1) % Q;
  uint32_t Y = (B + X * A) % Q;
  // r10_Rand is passed 2^^20 as required by the RFC5053
  uint32_t v = r10_Rand(Y, 0, 1<<20);
  uint32_t d = r10_Deg(v);
  uint32_t a = 1 + r10_Rand(Y, 1, L_ - 1);
  uint32_t b = r10_Rand(Y, 2, L_);

  triple[0] = d;
  triple[1] = a;
  triple[2] = b;
}

uint32_t r10_Rand(uint32_t X, uint32_t i, uint32_t m) {
  return (V0[(X + i) % 256] ^ V1[((uint32_t)floor(X / 256) + i) % 256]) % m;
}

uint32_t r10_Deg(uint32_t v) {
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

int r10_build_LDPC_submat(int K, int S, gf2matrix *A) {
  int a = 0, b = 0;
  for (int i = 0; i < K; i++){
    a = 1 + ((int)floor(i / S) % (S - 1));
    b = i % S;
        printf("%d %d\n",b,i);
    set_entry(A, b, i, 1);
    /* C[K+b] = C[K+b] ^ C[i]; */
    b = (b + a) % S;
        printf("%d %d\n",b,i);
    set_entry(A, b, i, 1);
    /* C[K+b] = C[K+b] ^ C[i]; */
    b = (b + a) % S;
        printf("%d %d\n",b,i);
    set_entry(A, b, i, 1);
    /* C[K+b] = C[K+b] ^ C[i]; */
  }
}

int r10_build_Half_submat(unsigned int K, unsigned int S, unsigned int H,
                          gf2matrix *A) {
  uint32_t g[4000];
  generate_gray_seq(&g[0]);
  uint H_ = ceil((float)H / 2.0);
  size_t n_words = 4000;
  uint32_t m[n_words];

  uint j = 0;
  for (size_t i = 0; i < n_words; i++)
    if (__builtin_popcount(g[i]) == H_) {
      m[j] = g[i];
      j++;
    }
    printf("j = %d\n", j);

  // Build the G_HALF submatrix
  for (uint h = 0; h < H; h++) {
    for (uint j = 0; j < K + S; j++) {
      if (m[j] & (1UL << h)) {
        set_entry(A, h + S, j, 1);
      }
    }
  }
}

int r10_build_LT_submat(uint32_t K, uint32_t S, uint32_t H, Raptor10 *obj,gf2matrix *A) {
  uint32_t L = K + S + H;
  uint32_t L_ = L;
  while (!is_prime(L_))
    L_++;

  for (uint32_t i = 0; i < K; i++) {
    uint32_t triple[3] = {0};
    r10_Trip(K, i, triple, obj);
    uint32_t d = triple[0];
    uint32_t a = triple[1];
    uint32_t b = triple[2];
    uint32_t j_max = fmin((d - 1), (L - 1));

    while (b >= L)
      b = (b + a) % L_;

    set_entry(A, i + S + H, b, 1);

    for (uint j = 1; j <= j_max; j++) {
      b = (b + a) % L_;

      while (b >= L)
        b = (b + a) % L_;

      set_entry(A, i + S + H, b, 1);
    }
  }
}

void r10_build_LT_mat(uint32_t N_, Raptor10 *obj, gf2matrix *G_LT,uint32_t *ESIs) {
    printf("N_ = %d\n", N_);
    uint32_t L = obj->K + obj->S + obj->H;
    uint32_t L_ = obj->L;
    while (!is_prime(L_))L_++;
    for (uint32_t i = 0; i < N_; i++) {
        uint32_t triple[3] = {0};
        uint32_t X = ESIs[i];
        r10_Trip(obj->K, X, triple, obj);
        uint32_t d = triple[0];
        uint32_t a = triple[1];
        uint32_t b = triple[2];
        uint32_t j_max = fmin((d - 1), (obj->L - 1));
        while (b >= obj->L)b = (b + a) % L_;
        set_entry(G_LT, i, b, 1);
        for (uint j = 1; j <= j_max; j++) {
            b = (b + a) % L_;
            while (b >= obj->L)b = (b + a) % L_;
            set_entry(G_LT, i, b, 1);
        }
    }
}
int gaussian_elimination(gf2matrix* mat, uint8_t *result, int size,Raptor10* obj){
    char dest;
    for (int i = 0; i < size; i++) {
        if (!get_entry(mat,i,i)) {
            for (int j = i + 1; j < size; j++) {
                if (get_entry(mat,j,i) == 1){
                    dest = result[j];
                    result[j] = result[i];
                    result[i] = dest;
                    swap_rows(mat,i,j);
                    break;
                }
            }
        }
        if (!get_entry(mat,i,i)) return -1;
        for (int j = 0; j < size; j++) {
            if (j != i && get_entry(mat,j,i) == 1) {
                set_entry(mat,j,i,0);
                printf("j-> %d i -> %d \n",result[j],result[i]);
                result[j] ^= result[i];
                printf("%d \n",result[j]);
            }
        }
    }
    return 0; // Success
}
int gaussian_elim(gf2matrix* mat,char* result,int size, Raptor10* obj, int* d){
    char dest;
    int tmp;
    bool is_start = true;
    for(int i=0;i<size;i++){
        is_start = true;
        if(!get_entry(mat,i,i)){
            for(int j=0;j<size;j++){
                if(i!=j && get_entry(mat,j,i) && (is_start || d[j] <= d[i])){
                    dest = result[i];
                    result[i] = result[j];
                    result[j] = dest;
                    tmp = d[i];
                    d[i] = d[j];
                    d[j] = tmp;
                    // printf("swap %d %d\n",i,j);
                    swap_rows(mat,i,j);
                    // print_matrix2(mat,result);
                    is_start = false;
                }
            }
        }
        if(!d[i]) return -1;
        for(int j=0;j<size;j++){
            // printf("j %d i %d d[i] %d \n",j,i,d[i]);
            if(j != i && get_entry(mat,j,i) && d[i] == 1){
                // printf("j %d i %d\n",j,i);
                // printf("xor %d %d\n",i,j);
                set_entry(mat,j,i,0);
                d[j]-=1;
                result[j] ^= result[i];
                // print_matrix2(mat,result);
            }
        }
    }
    return 0;
} 
int r10_build_constraints_mat(Raptor10 *obj, gf2matrix *A) {
    r10_build_LDPC_submat(obj->K, obj->S, A);
    // print_matrix(A);
    r10_build_Half_submat(obj->K, obj->S, obj->H, A);
    // print_matrix(A);
    for (int i = 0; i < obj->S; i++)
    set_entry(A, i, obj->K + i, 1);
    for (int i = 0; i < obj->H; i++)
    set_entry(A, obj->S + i, obj->K + obj->S + i, 1);
    // print_matrix(A);
    r10_build_LT_submat(obj->K, obj->S, obj->H, obj, A);
    // gaussjordan_inv(A);
    // print_matrix(A);
    return 0;
}

void r10_compute_params(Raptor10 *obj){
    if (!obj->Al && !obj->K && !obj->Kmax && !obj->Kmin && !obj->Gmax){printf("parameters required\n");exit(0);}
    uint32_t X = floor(sqrt(2 * obj->K));
    for (; X * X < 2 * obj->K + X; X++);
    for (obj->S = ceil(0.01 * obj->K) + X; !is_prime(obj->S); obj->S++);
    for (obj->H = 1; choose(obj->H, ceil(obj->H / 2)) < obj->K + obj->S; obj->H++);
    obj->L = obj->K + obj->S + obj->H;
}

void r10_multiplication(Raptor10 *obj, gf2matrix *A, uint8_t *block,uint8_t *res_block) {
    for (uint j = 0; j < get_ncols(A); j++)
        for (uint i = 0; i < get_nrows(A); i++)
            if (get_entry(A, i, j)) {
                for (uint t = 0; t < obj->T; t++)
                    res_block[i + t] = res_block[i + t] ^ block[j + t];
            }
}

void r10_encode(uint8_t *src_s, uint8_t *enc_s, Raptor10 *obj, gf2matrix *A) {
    // Multiply constraints matrix with input block to obtain intermediate
    // symbols
    uint8_t* int_symbols = calloc(sizeof(uint8_t),obj->L);
    // bzero(int_symbols,obj->L);
    // Calculate the LT matrix and encoded symbols
    gf2matrix G_LT;
    allocate_gf2matrix(&G_LT, obj->L, obj->N);
    // Create vector of ESIs
    uint32_t *ESIs =(uint32_t*) malloc(sizeof(uint32_t)*obj->N);
    for (uint32_t i = 0; i < obj->N; i++)ESIs[i] = i;
    // Build the LT matrix and encode
    r10_build_LT_mat(obj->N, obj, &G_LT, ESIs);
    print_matrix(&G_LT);
    r10_multiplication(obj, &G_LT, src_s, int_symbols);
    for(int i=0;i<obj->L;i++)printf("%d ",int_symbols[i]);
    printf("<-- intermediate done\n");
    // printf("GLT \n");
    // print_matrix(&G_LT);
    r10_multiplication(obj, A, int_symbols, enc_s);
    for(int i=0;i<obj->L;i++)printf("%d ",enc_s[i]);
    printf("<-- encrypted done\n");
}
/*
 * Assumptions:
 * src_s data is padded wiht 0 so that len(src_s) == obj->L*obj->T;
 * all parameters of obj (Raptor10) is properly set
 *
    */

void my_encode(uint8_t *src_s,uint8_t* enc_s, Raptor10* obj){
    gf2matrix* A = malloc(sizeof(gf2matrix));
    gf2matrix* G_LT = malloc(sizeof(gf2matrix));
    uint8_t* int_symbols = calloc(sizeof(uint8_t),obj->L*obj->T);
    allocate_gf2matrix(A,obj->L,obj->L);
    r10_build_constraints_mat(obj,A);
    gaussjordan_inv(A);
    print_matrix(A);
    r10_multiplication(obj,A,src_s,int_symbols);
    for(int i=0;i<obj->L;i++)printf("%d ",int_symbols[i]);
    printf("intermediate symbols encode\n");
    allocate_gf2matrix(G_LT,obj->L,obj->L);
    uint32_t* esi = (uint32_t*) malloc(sizeof(uint32_t)*obj->L);
    for(int i=0;i<obj->L;i++)esi[i] = i+obj->K;
    r10_build_LT_mat(obj->L,obj,G_LT,esi);
    print_matrix(G_LT);
    r10_multiplication(obj,G_LT,int_symbols,enc_s);
    for(int i=0;i<obj->L;i++)printf("%d ",enc_s[i]);
    printf("encoded symbols encode\n");
}


void r10_decode(uint8_t *enc_s, uint8_t *dec_s, Raptor10 *obj, gf2matrix *A,uint32_t N_,uint32_t* ESIs) {
    // Calculate intermediate symbols
    // Build constraint matrix
    // To check if r10_build_constraints_mat relies on a already defined N !!!!
    gaussjordan_inv(A);
    uint8_t* int_symbols = (uint8_t*) calloc(obj->L, sizeof(uint8_t));
    r10_multiplication(obj, A, enc_s, int_symbols);
    for(int i=0;i<obj->L;i++)printf("%d ",int_symbols[i]);
    printf("<-- decode intermediate \n");
    // Calculate the LT matrix and encoded symbols
    // gf2matrix G_LT;
    // allocate_gf2matrix(&G_LT, obj->L, obj->K);
    // Build the LT matrix and decode
    // r10_build_LT_mat(N_, obj, &G_LT, ESIs);
    // gaussjordan_inv(&G_LT);
    // print_matrix(&G_LT);
    r10_multiplication(obj, A, int_symbols, dec_s);
    for(int i=0;i<obj->K;i++)printf("%c ",dec_s[i]);
}
