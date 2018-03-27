GCC=gcc
LDFLAGS = -lgmp -lcrypto
CFLAGS = -Ofast -Wall
OBJS = rng.o Mersenne_kem.o PQCgenKAT_kem.o 
PQCgenKAT_kem : $(OBJS) 
	$(GCC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
%.o : %.c 
	$(GCC) $(CFLAGS) -o $@ -c $< 
rng.o : rng.h 
api.o : api.h 
PQCgenKAT_kem.o : rng.h api.h 
