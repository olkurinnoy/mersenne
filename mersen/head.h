#ifndef HEAD_H
#define HEAD_H
#include "rng.h"

const int n = 756839, h = 256, ro = 248, K = 94624, n0 = 1048576;

typedef AES_XOF_struct ExpandableState;

void InitExpandableState(unsigned char *input, ExpandableState *State);

void GetExpandableOutput(unsigned char *output, unsigned int outputByteLen, ExpandableState *State);

void get_P(mpz_t P);

void print_byte_arr(unsigned char* arr, const char* desc, int len);



int random_mod(int mod, ExpandableState *state);

void gen_sparse_byte_arr(unsigned char* B, ExpandableState *state);

void det_key_pair(unsigned char* pk, unsigned char* longsk, unsigned char* seed);

void key_pair(unsigned char* pk, unsigned char* sk);

#endif