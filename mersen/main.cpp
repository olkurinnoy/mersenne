#include <iostream>
#include "gmpxx.h"
#include "gmp.h"
#include "head.h"
#include "rng.h"

#include <string>


using namespace std;

int main()
{
	FILE *fp;
	fp = fopen("ex.txt", "w");

	unsigned char pk[2*K], sk[h/8], ct[K + (h/8)*ro], ss[h/8], ss1[h/8];
	unsigned char seed[48];
	unsigned char entropy_input[48];
	for (int i=0; i<48; i++)
        entropy_input[i] = i;
    randombytes_init(entropy_input, NULL, 256);
	randombytes(seed, 48);

	randombytes_init(seed, NULL, 256);
	fprint_byte_arr(fp, seed,"seed = ",48);
	key_pair(pk, sk);
	fprint_byte_arr(fp, pk,"pk = ",2*K);
	fprint_byte_arr(fp, sk,"sk = ",h/8);
	kem_encap(ct, ss, pk);
	fprint_byte_arr(fp, ct, "ct = ",K + (h/8)*ro);
	fprint_byte_arr(fp, ss, "ss  = ", h/8);
	kem_decap(ss1, ct, sk);
	fprint_byte_arr(fp, ss1, "ss1 = ", h/8);
	
	fclose(fp);
	cin.get();
}