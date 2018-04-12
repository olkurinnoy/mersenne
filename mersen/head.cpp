#include <iostream>
#include <gmpxx.h>
#include <gmp.h>
#include "head.h"
#include "rng.h"


typedef AES_XOF_struct ExpandableState;

void InitExpandableState(unsigned char *input,ExpandableState *State)
{
  unsigned char diversifier[8]={0,0,0,0,0,0,0,0};
  seedexpander_init(State, input, diversifier, 1UL<<30);
}

void GetExpandableOutput(unsigned char *output, unsigned int outputByteLen, ExpandableState *State)
{
  seedexpander(State, output, outputByteLen);
}

void get_P(mpz_t P)
{
	mpz_ui_pow_ui(P,2,n);
	mpz_sub_ui(P,P,1);
}

void print_byte_arr(unsigned char* arr, const char* desc, int len)
{
	std::cout << desc;
	for (int i=0; i<len; i++)
	{
		printf("%02X", arr[i]);
	}
	printf("\n");
}

int random_mod(int mod, ExpandableState *state)
{
  	unsigned char random_bytes[3];
	int v;
	do
	{
		v = 0;
		GetExpandableOutput(random_bytes, 3, state);
		for (int i=0; i<3; i++)
		{
			v = v + int(random_bytes[i])*(1<<(8*i));	
		}
		v = v % n0;
	} while (v > mod);
	return v;
}

void gen_sparse_byte_arr(unsigned char* B, ExpandableState *state)
{
	unsigned char swap;
	int bit_size = K*8;
	unsigned char B_bit[bit_size];
	for (int i=0; i<h; i++)
	{
		B_bit[i] = '1';
	}
	for (int i=h; i<bit_size; i++)
	{
		B_bit[i] = '0';
	}

	int i = h - 1, j;
	while (i >= 0)
	{
		j = random_mod(n - i, state);
		swap = B_bit[i];
		B_bit[i] = B_bit[i + j];
		B_bit[i + j] = swap;
		i--;
	}
	int size = 0, temp = 0;
	for (i=0; i<bit_size; i=i+8)
	{	
		temp = 0;
		for (int j=0; j<8; j++)
		{
			temp = temp + (B_bit[i+j]-'0')*(1<<j);
		}
		B[size] = temp;
		size++;
	}	
}

void det_key_pair(unsigned char* pk, unsigned char* longsk, unsigned char* seed)
{
	ExpandableState state;
	unsigned char *Af, *Ag, *AR;
	Af = new unsigned char[K];
	Ag = new unsigned char[K];
	AR = new unsigned char[K];
	InitExpandableState(seed, &state);
	gen_sparse_byte_arr(Af, &state);
	gen_sparse_byte_arr(Ag, &state);
	GetExpandableOutput(AR, K, &state);
	mpz_t f, g, R, P, T;
	mpz_init(f);
	mpz_init(g);
	mpz_init(R);
	mpz_init(P);
	mpz_init(T);
	get_P(P);
	mpz_import(f, K, -1, 1, 0, 0, Af);
	mpz_import(g, K, -1, 1, 0, 0, Ag);
	mpz_import(R, K, -1, 1, 0, 0, AR);
	mpz_mod(R,R,P);
	mpz_mul(T,f,R);
	mpz_add(T,T,g);
	mpz_mod(T,T,P);
	unsigned char temp_R[K], temp_T[K];
	size_t countp;
	mpz_export(temp_R, &countp, -1, 1, 0, 0, R);
	mpz_export(temp_T, &countp, -1, 1, 0, 0, T);
	std::copy(temp_R, temp_R + K, pk);
	std::copy(temp_T, temp_T + K, pk + K);
	for (int i=0; i<K; i++)
	{
		longsk[i] = Af[i];
	}
	mpz_clear(f);
	mpz_clear(g);
	mpz_clear(R);
	mpz_clear(P);
	mpz_clear(T);
}

void key_pair(unsigned char* pk, unsigned char* sk)
{
	unsigned char seed[32];
	unsigned char longsk[K];

	randombytes(sk,h/8);
	for (int i=0; i<h/8; i++)
	{
		seed[i] = sk[i];
	}
	det_key_pair(pk, longsk, seed);

}