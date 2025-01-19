#ifndef RAPTOR_HEADER
#define RAPTOR10_HEADER

#include <stdint.h>
#include <stdlib.h>
#include<strings.h>
#include "gf2matrix.h"
#include "queue.h"
#include "raptor_consts.h" // J, C1 and C2
typedef unsigned char byte;

/**
 * Struct containing all the fileds needed by the raptor code to operate
 * @struct raptor
 * @brief Structure holding all the fields needed by raptor encoder and
 * decoder
 * @param raptor::F Transfer length of the object, in bytes
 * @param raptor::Al symbol alignment parameter
 * @param raptor::T symbol size, in bytes
 * @param raptor::Z number of source blocks
 * @param raptor::N number of sub-blocks in each source block
 * @param raptor::W a target on the sub-block size
 * @param raptor::P maximum packet payload size (multiple of ~Al~)
 * @param raptor::Kmax maximum number of source symbols per source block
 * @param raptor::Kmin minimum target on the number of symbols per source block
 * @param raptor::Gmax maximum target number of symbols per packet
 * @param raptor::K denotes the number of symbols in a single source block
 * @param raptor::L denotes the number of pre-coding symbols for a single source block
 * @param raptor::S denotes the number of LDPC symbols for a single source block
 * @param raptor::H denotes the number of Half symbols for a single source block
 * @param raptor::G the number of symbols within an encoding symbol group
 */
typedef struct {
  uint64_t F;
  uint32_t W; 
  uint32_t P;
  uint32_t Al;
  uint32_t Kmax;
  uint32_t Kmin;
  uint32_t Gmax;
  uint32_t T;
  uint32_t Z;
  uint32_t N;
  uint32_t K;
  uint32_t L;
  uint32_t S;
  uint32_t H;
  uint32_t G;
} raptor;

// raptor* build_raptor(unsigned long file_size);
uint64_t factorial(uint64_t n);
int is_prime(uint32_t n);
uint32_t choose(uint32_t i, uint32_t j);
uint32_t raptor_Rand(uint32_t X, uint32_t i, uint32_t m);
uint32_t raptor_Deg(uint32_t v);
void raptor_Trip(uint32_t K, uint32_t X, uint32_t triple[3], raptor *obj);
void raptor_LTEnc(uint32_t X, uint32_t K, uint32_t *C, uint32_t triple[3],uint32_t G, raptor obj);
int raptor_build_LDPC_submat(int K, int S, gf2matrix *A);
int raptor_build_Half_submat(unsigned int K, unsigned int S, unsigned int H,gf2matrix *A);
int raptor_build_LT_submat(uint32_t K, uint32_t S, uint32_t H, raptor *obj,gf2matrix *A);
int raptor_build_constraints_mat(raptor *obj, gf2matrix *A);
void raptor_build_LT_mat(uint32_t N_, raptor *obj, gf2matrix *G_LT,uint32_t *ESIs);
void raptor_multiplication(raptor *obj, gf2matrix *A, byte **block,byte** res_block);
void raptor_encode(uint8_t *src_s, uint8_t *enc_s, raptor *obj, gf2matrix *A);
void my_encode(uint8_t *src_s,uint8_t* enc_s, raptor* obj);
void my_decode(uint8_t* enc_s, raptor* obj,uint32_t* ESIs,uint32_t n);
void raptor_decode(uint8_t *enc_s, uint8_t *dec_s, raptor *obj, gf2matrix *A,uint32_t N_, uint32_t *ESIs);
int gaussian_elimination(gf2matrix* mat, uint8_t *result, int size,raptor* obj);
int gaussian_elim(gf2matrix* mat, byte** result,raptor* obj,int*);
byte** rapter_generate_intermediate_symb(raptor* obj,byte** data);
void xor(byte* result,byte* a,byte* b,uint32_t n);
void copy_row(raptor* obj,gf2matrix* mat1,uint32_t mat1_row,gf2matrix* mat2,uint32_t mat2_row);
void LTEncode(raptor* obj,gf2matrix* mat,uint32_t x, uint32_t row_index ,uint32_t L_);
#endif
