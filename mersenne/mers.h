#ifndef MERS_H
#define MERS_H
#include "rng.h"

const int n = 756839, h = 256, ro = 2048, K = 94624, n0 = 1048576;

typedef AES_XOF_struct xof_state;

void init_xof_state(unsigned char *input, xof_state *state);

void get_xof_state(unsigned char *output, unsigned int outputByteLen, xof_state *state);

void get_mers_numb(mpz_t P);

void print_byte_arr(unsigned char* arr, const char* desc, int len);

void fprint_byte_arr(FILE *fp, unsigned char *arr, const char* desc, int len);

void init_arr(unsigned char *arr, int len);

int random_mod(int mod, xof_state *state);

void gen_sparse_byte_arr(unsigned char* B, xof_state *state);

void det_key_pair(unsigned char* pk, unsigned char* longsk, unsigned char* seed);

void key_pair(unsigned char* pk, unsigned char* sk);

void det_kem_encap(unsigned char* ct, unsigned char* ss, unsigned char* pk, unsigned char* seed);

void kem_encap(unsigned char* ct, unsigned char* ss, unsigned char* pk);

int kem_decap(unsigned char* ss, unsigned char* ct, unsigned char* sk);

#endif