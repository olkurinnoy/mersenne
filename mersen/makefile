GCC = g++
LDFLAGS = -lgmp -lgmpxx -lcrypto
CFLAGS = -Wall -g

main: main.o head.o rng.o
	$(GCC) $(CFLAGS) -o main main.o head.o rng.o $(LDFLAGS)

main.o: main.cpp head.h
	$(GCC) $(CFLAGS) -c main.cpp

head.o: head.cpp head.h rng.h
	$(GCC) $(CFLAGS) -c head.cpp

rng.o: rng.c head.h
	$(GCC) $(CFLAGS) -c rng.c

clean: 
	$(RM) count *.o *~