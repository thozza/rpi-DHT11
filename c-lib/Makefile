CC=gcc
CFLAGS=-std=c99 -lbcm2835 -I.
OUTPUT=run

all: dht11.c example.c
	$(CC) -o $(OUTPUT) dht11.c example.c $(CFLAGS)
