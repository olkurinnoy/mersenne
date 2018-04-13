#include <iostream>
#include "gmpxx.h"
#include "gmp.h"
#include "head.h"
#include "rng.h"

using namespace std;

int main()
{
	unsigned char pk[2*K], sk[h/8], ct[K + (h/8)*ro], ss[h/8], ss1[h/8];
	unsigned char seed[48];
	unsigned char entropy_input[48];
	for (int i=0; i<48; i++)
        entropy_input[i] = i;
    randombytes_init(entropy_input, NULL, 256);
	randombytes(seed, 48);

	randombytes_init(seed, NULL, 256);
	key_pair(pk, sk);
	//print_byte_arr(seed,"seed = ",48);
	//print_byte_arr(pk,"pk = ",2*K);
	//print_byte_arr(sk,"sk = ",h/8);

	//det_kem_encap(ct, ss, pk, seed);

	kem_encap(ct, ss, pk);
	//print_byte_arr(ct,"ct = ",K + (h/8)*ro);
	//print_byte_arr(ss,"ss = ", h/8);
	kem_decap(ss1, ct, sk);

	cin.get();
}