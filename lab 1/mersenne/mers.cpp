#include <iostream>
#include <gmpxx.h>
#include <gmp.h>
#include "mers.h"
#include "rng.h"


typedef AES_XOF_struct xof_state;

void init_xof_state(unsigned char *input, xof_state *state)
{
  unsigned char diversifier[8]={0,0,0,0,0,0,0,0};
  seedexpander_init(state, input, diversifier, 1UL<<30);
}

void get_xof_state(unsigned char *output, unsigned int outputByteLen, xof_state *state)
{
  seedexpander(state, output, outputByteLen);
}

void get_mers_numb(mpz_t P)
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

void fprint_byte_arr(FILE *fp, unsigned char *arr, const char* desc, int len)
{
	fprintf(fp, "%s", desc);

	for (int i=0; i<len; i++ )
		fprintf(fp, "%02X", arr[i]);

	fprintf(fp, "\n");
}

void init_arr(unsigned char *arr, int len)
{
	for (int i=0; i<len; i++)
	{
		arr[i] = 0;
	}
}

int random_mod(int mod, xof_state *state)
{
  	unsigned char random_bytes[3];
	int v;
	do
	{
		v = 0;
		get_xof_state(random_bytes, 3, state);
		for (int i=0; i<3; i++)
		{
			v = v + int(random_bytes[i])*(1<<(8*i));	
		}
		v = v % n0;
	} while (v > mod);
	return v;
}

void gen_sparse_byte_arr(unsigned char* B, xof_state *state)
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
	xof_state state;
	unsigned char *Af, *Ag, *AR;
	Af = new unsigned char[K];
	Ag = new unsigned char[K];
	AR = new unsigned char[K];
	init_xof_state(seed, &state);
	gen_sparse_byte_arr(Af, &state);
	gen_sparse_byte_arr(Ag, &state);
	get_xof_state(AR, K, &state);
	mpz_t f, g, R, P, T;
	mpz_init(f);
	mpz_init(g);
	mpz_init(R);
	mpz_init(P);
	mpz_init(T);
	get_mers_numb(P);
	mpz_import(f, K, -1, 1, 0, 0, Af);
	mpz_import(g, K, -1, 1, 0, 0, Ag);
	mpz_import(R, K, -1, 1, 0, 0, AR);
	mpz_mod(R,R,P);
	mpz_mul(T,f,R);
	mpz_add(T,T,g);
	mpz_mod(T,T,P);

	unsigned char temp_R[K], temp_T[K];
	size_t countp;
	init_arr(pk, 2*K);
	init_arr(temp_R, K);
	init_arr(temp_T, K);
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

void det_kem_encap(unsigned char* ct, unsigned char* ss, unsigned char* pk, unsigned char* seed)
{
	xof_state state;
	unsigned char *Aa, *Ab1, *Ab2;
	init_xof_state(seed, &state);
	get_xof_state(ss, h/8, &state);
	Aa = new unsigned char[K];
	Ab1 = new unsigned char[K];
	Ab2 = new unsigned char[K];
	gen_sparse_byte_arr(Aa, &state);
	gen_sparse_byte_arr(Ab1, &state);
	gen_sparse_byte_arr(Ab2, &state);
	mpz_t P, a, b1, b2, R, T, C1, C2;
	mpz_init(P);
	get_mers_numb(P);
	mpz_init(a);
	mpz_init(b1);
	mpz_init(b2);
	mpz_init(R);
	mpz_init(T);
	mpz_init(C1);
	mpz_init(C2);
	mpz_import(a, K, -1, 1, 0, 0, Aa);
	mpz_import(b1, K, -1, 1, 0, 0, Ab1);
	mpz_import(b2, K, -1, 1, 0, 0, Ab2);
	mpz_import(R, K, -1, 1, 0, 0, pk);
	mpz_import(T, K, -1, 1, 0, 0, pk + K);
	mpz_mul(C1,a,R);
	mpz_add(C1,C1,b1);
	mpz_mod(C1,C1,P);
	mpz_mul(C2,a,T);
	mpz_add(C2,C2,b2);
	mpz_mod(C2,C2,P);

	size_t countp;
	unsigned char temp_C1[K], temp_C2[K];
	init_arr(temp_C1, K);
	init_arr(temp_C2, (h/8)*ro);
	mpz_export(temp_C1, &countp, -1, 1, 0, 0, C1);
	mpz_export(temp_C2, &countp, -1, 1, 0, 0, C2);

	unsigned char M[(h/8)*ro];
	for (int i=0; i<h; i++)
	{
		if ((seed[i/8]>>(i%8))&1)
		{
			for (int j=(i*ro)/8;j<((i+1)*ro)/8;j++)
			{
				M[j] = 255;
			}
		}
		else
		{
			for (int j=(i*ro)/8;j<((i+1)*ro)/8;j++)
			{
				M[j] = 0;
			}
		}
	}

	for (int i=0;i<(h/8)*ro;i++)
	{
		M[i] = M[i]^temp_C2[i];
	}
	init_arr(ct, K + (h/8)*ro);
	std::copy(temp_C1, temp_C1 + K, ct);
	std::copy(M, M + (h/8)*ro, ct + K);

	mpz_clear(P);
	mpz_clear(a);
	mpz_clear(b1);
	mpz_clear(b2);
	mpz_clear(R);
	mpz_clear(T);
	mpz_clear(C1);
	mpz_clear(C2);
}

void kem_encap(unsigned char* ct, unsigned char* ss, unsigned char* pk)
{
	unsigned char seed[32];
	randombytes(seed, h/8);
	det_kem_encap(ct, ss, pk, seed);
}

int kem_decap(unsigned char* ss, unsigned char* ct, unsigned char* sk)
{
	
	unsigned char seed[h/8], pk[2*K], longsk[K];
	for (int i=0; i<h/8; i++)
	{
		seed[i] = sk[i];
	}
	init_arr(pk, 2*K);
	init_arr(longsk, K);
	det_key_pair(pk, longsk, seed);
	mpz_t P, f, C1, C2_;
	mpz_init(P);
	get_mers_numb(P);
	mpz_init(f);
	mpz_init(C1);
	mpz_init(C2_);
	mpz_import(f, K, -1, 1, 0, 0, longsk);
	mpz_import(C1, K, -1, 1, 0, 0, ct);
	mpz_mul(C2_, f, C1);
	mpz_mod(C2_, C2_, P);
	
	unsigned char M[(h/8)*ro], temp_C2_[K + (h/8)*ro];
	init_arr(temp_C2_, K + (h/8)*ro);
	size_t countp;
	mpz_export(temp_C2_, &countp, -1, 1, 0, 0, C2_);

	for (int i=0; i<(h/8)*ro; i++)
	{
		M[i] = temp_C2_[i]^ct[i + K];
	}

	mpz_clear(P);
	mpz_clear(f);
	mpz_clear(C1);
	mpz_clear(C2_);
	
	unsigned char S_[32];
	for (int i=0; i<32; i++)
	{
		S_[i] = 0;
	}
	
	int ham_w;
	
	for (int i=0; i<h; i++)
	{
		ham_w = 0;
		for (int j=(i*ro)/8; j<((i+1)*ro)/8; j++)
		{
			ham_w = ham_w + __builtin_popcount(M[j]);
		}
		if (ham_w>ro/2)
		{
			S_[i/8] = S_[i/8]^(1 << (i%8));
		}
	}
	
	unsigned char ct_new[K + (h/8)*ro];
	init_arr(ct_new, K + (h/8)*ro);


	det_kem_encap(ct_new, ss, pk, S_);
	int check = 1;
	for (int i=0; i<K + (h/8)*ro; i++)
	{
		if (ct[i]!=ct_new[i])
		{
			check = -1;
		}
	}
	if (check==-1)
	{
		for (int i=0; i<h/8; i++)
		{
			ss[i] = 0;
		}
	}
	return check;
}

void results(FILE *fp, unsigned char* seed, int seed_size)
{
	unsigned char pk[2*K], sk[h/8], ct[K + (h/8)*ro], ss[h/8], ss1[h/8];
	int status;
	randombytes_init(seed, NULL, 256);
	fprint_byte_arr(fp, seed,"seed = ",seed_size);
	key_pair(pk, sk);
	fprint_byte_arr(fp, pk,"pk = ",2*K);
	fprint_byte_arr(fp, sk,"sk = ",h/8);
	kem_encap(ct, ss, pk);
	fprint_byte_arr(fp, ct, "ct = ",K + (h/8)*ro);
	fprint_byte_arr(fp, ss, "ss  = ", h/8);
	status = kem_decap(ss1, ct, sk);
	//fprintf(fp, "decap status = %d\n", status);
	//fprint_byte_arr(fp, ss1, "ss1 = ", h/8);
}

void create_req(const char* name, int num, int seed_size)
{
	FILE *fp;
	fp = fopen(name, "w");

	
	unsigned char seed[seed_size];
	unsigned char entropy_input[seed_size];
	for (int i=0; i<seed_size; i++)
        entropy_input[i] = i;
    randombytes_init(entropy_input, NULL, 256);
	for (int i=0; i<num; i++)
	{
		randombytes(seed, seed_size);
		fprintf(fp, "No = %d\n", i);	
		fprint_byte_arr(fp, seed, "seed = ", seed_size);
	}
	fclose(fp);
}
	
void get_byte_arr_from_hex(unsigned char *seed, char *hex, int seed_size)
{	
	for (int i = 0; i < seed_size; i++) {
        sscanf(hex + 2*i + 7, "%02x", &seed[i]);
    }
}

void create_resp(const char* resp, const char* req, int num, int seed_size)
{
	FILE *fresp, *freq;
	freq = fopen(req, "r");
	fresp = fopen(resp, "w");
	char buffer1[10], buffer2[seed_size*2 + 10];
	unsigned char seed[seed_size];
	for (int i=0; i<num; i++)
	{
		fgets(buffer1, 10, freq);
		fgets(buffer2, seed_size*2 + 10, freq);
		get_byte_arr_from_hex(seed, buffer2, seed_size);
		fprintf(fresp, "%s", buffer1);
		results(fresp, seed, seed_size);
	}
	fclose(freq);
	fclose(fresp);
}