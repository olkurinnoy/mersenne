GCC = g++
LDFLAGS = -lgmp -lgmpxx -lcrypto
CFLAGS = -Wall -g

main: main.o mers.o rng.o
	$(GCC) $(CFLAGS) -o main main.o mers.o rng.o $(LDFLAGS)

main.o: main.cpp mers.h
	$(GCC) $(CFLAGS) -c main.cpp

head.o: head.cpp mers.h rng.h
	$(GCC) $(CFLAGS) -c mers.cpp

rng.o: rng.c mers.h
	$(GCC) $(CFLAGS) -c rng.c

clean: 
	$(RM) count *.o *~