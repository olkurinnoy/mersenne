#include <iostream>
#include <string>
#include "gmpxx.h"
#include "gmp.h"
#include "mers.h"
#include "rng.h"


using namespace std;

int main()
{
	cout << "Waiting ....."<<endl;
	create_req("req.txt", 30, 48);
	create_resp("resp.txt", "req.txt", 30, 48);
	
	cout << "Done!";
	cin.get();
}