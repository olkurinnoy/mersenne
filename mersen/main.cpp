#include <iostream>
#include "gmpxx.h"
#include "gmp.h"
#include "head.h"
#include "rng.h"

using namespace std;

int main()
{
	ExpandableState state;
	unsigned char *a, *B;
	a = new unsigned char[10];
	a[0] = a[3] = 1;
	InitExpandableState(a,&state);
	//cout<<random_mod(1000000, &state);
	B = new unsigned char[n];
	//gen_sparse_byte_arr(B, &state);
	
	cin.get();
}