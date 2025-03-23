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
 * @param raptor::file_size_bytes Transfer length of the object, in bytes
 * @param raptor::symbol_alignment symbol alignment parameter
 * @param raptor::symbol_size_bytes  symbol size, in bytes
 * @param raptor::num_source_blocks  number of source blocks
 * @param raptor::num_sub_blocks  number of sub-blocks in each source block
 * @param raptor::sub_block_size_target  a target on the sub-block size
 * @param raptor::max_packet_payload_size  maximum packet payload size (multiple of ~symbol_alignment~)
 * @param raptor::max_source_symbols_per_block  maximum number of source symbols per source block
 * @param raptor::min_source_symbols_per_block  minimum target on the number of symbols per source block
 * @param raptor::max_symbols_per_packet  maximum target number of symbols per packet
 * @param raptor::source_symbols_per_block  denotes the number of symbols in a single source block
 * @param raptor::total_precoding_symbols  denotes the number of pre-coding symbols for a single source block
 * @param raptor::ldpc_symbols  denotes the number of total_precoding_symbols DPC symbols for a single source block
 * @param raptor::half_symbols  denotes the number of Half symbols for a single source block
 * @param raptor::symbols_per_group  the number of symbols within an encoding symbol group
 */
typedef struct {
  uint64_t file_size_bytes;
  uint32_t sub_block_size_target ; 
  uint32_t max_packet_payload_size ;
  uint32_t symbol_alignment;
  uint32_t max_source_symbols_per_block ;
  uint32_t min_source_symbols_per_block ;
  uint32_t max_symbols_per_packet ;
  uint32_t symbol_size_bytes ;
  uint32_t num_source_blocks ;
  uint32_t num_sub_blocks ;
  uint32_t source_symbols_per_block ;
  uint32_t total_precoding_symbols ;
  uint32_t ldpc_symbols ;
  uint32_t half_symbols ;
  uint32_t symbols_per_group ;
} raptor;

// raptor* build_raptor(unsigned long file_size);
uint64_t factorial(uint64_t n);
int is_prime(uint32_t n);
uint32_t choose(int32_t i, int32_t j);
uint32_t raptor_Rand(uint32_t X, uint32_t i, uint32_t m);
uint32_t raptor_Deg(uint32_t v);
void raptor_Trip(uint32_t K, uint32_t X, uint32_t triple[3], raptor *obj);
void raptor_LTEnc(uint32_t X, uint32_t K, uint32_t *C, uint32_t triple[3],uint32_t symbols_per_group , raptor obj);
int raptor_build_LDPC_submat(int K, int ldpc_symbols , gf2matrix *A);
int raptor_build_Half_submat(unsigned int K, unsigned int ldpc_symbols , unsigned int half_symbols ,gf2matrix *A);
int raptor_build_LT_submat(uint32_t K, uint32_t ldpc_symbols , uint32_t half_symbols , raptor *obj,gf2matrix *A);
int raptor_build_constraints_mat(raptor *obj, gf2matrix *A);
raptor* raptor_init_obj(uint32_t file_size);
void raptor_build_LT_mat(uint32_t N_, raptor *obj, gf2matrix *G_LT,uint32_t *ESIs);
void raptor_multiplication(raptor *obj, gf2matrix *A, byte **block,byte** res_block);
void raptor_encode(uint8_t *src_s, uint8_t *enc_s, raptor *obj, gf2matrix *A);
void my_encode(uint8_t *src_s,uint8_t* enc_s, raptor* obj);
void my_decode(uint8_t* enc_s, raptor* obj,uint32_t* ESIs,uint32_t n);
void raptor_decode(uint8_t *enc_s, uint8_t *dec_s, raptor *obj, gf2matrix *A,uint32_t N_, uint32_t *ESIs);
int gaussian_elimination(gf2matrix* mat, uint8_t *result, int size,raptor* obj);
int gaussian_elim(gf2matrix* mat, byte** result,raptor* obj,uint32_t*);
byte** rapter_generate_intermediate_symb(raptor* obj,byte** data);
void xor(byte* result,byte* a,byte* b,uint32_t n);
void copy_row(gf2matrix* mat1,uint32_t mat1_row,gf2matrix* mat2,uint32_t mat2_row);
void LTEncode(raptor* obj,gf2matrix* mat,uint32_t x, uint32_t row_index ,uint32_t L_);
void raptor_print_object(raptor* obj);
byte* raptor_serialze(raptor* obj,byte* buffer);
#endif
