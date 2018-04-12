#include <iostream>
#include <gmpxx.h>
#include <gmp.h>
#include "head.h"
#include "rng.h"


typedef AES_XOF_struct ExpandableState;

void InitExpandableState(unsigned char *input,
			ExpandableState *State)
{
  unsigned char diversifier[8]={0,0,0,0,0,0,0,0};
  seedexpander_init(State, input, diversifier, 1UL<<30);
}

void GetExpandableOutput(unsigned char *output,
			 unsigned int outputByteLen,
			 ExpandableState *State)
{
  seedexpander(State, output, outputByteLen);
}

void get_P(mpz_t P)
{
	mpz_init(P);
	mpz_ui_pow_ui(P,2,n);
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
			v = (v + int(random_bytes[i])*(1<<(8*i))) % n0;	
		}
	} while (v > mod);
	return v;
}

void gen_sparse_byte_arr(unsigned char* B, ExpandableState *state)
{
	unsigned char swap;
	for (int i=0; i<h; i++)
	{
		B[i] = '1';
	}
	for (int i=h; i<n; i++)
	{
		B[i] = '0';
	}
	int i = h - 1, j;
	while (i >= 0)
	{
		j = random_mod(n - i, state);
		swap = B[i];
		B[i] = B[i + j];
		B[i + j] = swap;
		i--;
	}	
}

