GCC = g++
LDFLAGS = -lgmp
CFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o head.o
	$(GCC) $(CFLAGS) -o main main.o head.o $(LDFLAGS)

# The main.o target can be written more simply

main.o: main.cpp head.h
	$(GCC) $(CFLAGS) -c main.cpp

head.o: head.h