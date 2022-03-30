CC = gcc
CFLAGS = -g -Wall

all: aesenc.bin symdec.bin symkeygen.bin

symdec.bin: symdec.o aes.o hash.o aesbcm.o file.o keygen.o
	$(CC) $(CFLAGS) symdec.o aes.o hash.o aesbcm.o file.o keygen.o -lcrypto -o symdec.bin

symdec.o: decryptor/symdec.c decryptor/symdec.h
	$(CC) $(CFLAGS) decryptor/symdec.c -c

aesenc.bin: aesenc.o aesbcm.o aes.o hash.o file.o keygen.o
	$(CC) $(CFLAGS) aesenc.o aesbcm.o aes.o hash.o file.o keygen.o -lcrypto -o aesenc.bin

symkeygen.bin: symkeygen.o hash.o keygen.o
	$(CC) $(CFLAGS) symkeygen.o hash.o keygen.o -lcrypto -o symkeygen.bin

symkeygen.o: keygen/symkeygen.c keygen/symkeygen.h
	$(CC) $(CFLAGS) keygen/symkeygen.c -c

keygen.o: keygen/keygen.c keygen/keygen.h
	$(CC) $(CFLAGS) keygen/keygen.c -c

aesenc.o: aes/aesenc.c aes/aesenc.h
	$(CC) $(CFLAGS) aes/aesenc.c -c

aesbcm.o: aes/aesbcm.c aes/aesbcm.h
	$(CC) $(CFLAGS) aes/aesbcm.c -c

aes.o: aes/aes.c aes/aes.h
	$(CC) $(CFLAGS) aes/aes.c -c

hash.o: misc/hash.c misc/hash.h
	$(CC) $(CFLAGS) misc/hash.c -c

file.o: misc/file.c misc/file.h
	$(CC) $(CFLAGS) misc/file.c -c


test: aesenc.bin symdec.bin
	./aesenc.bin -i testinput1 -t 3 -k testkey -b ctr
	./aesenc.bin -i testinput2 -t 3 -k testkey -b ctr
	./aesenc.bin -i testinput3 -t 2 -k testkey -b ctr
	./aesenc.bin -i testinput4 -t 2 -k testkey -b ctr
	./symdec.bin -i testinput1.aes -k testkey
	./symdec.bin -i testinput2.aes -k testkey
	./symdec.bin -i testinput3.aes -k testkey
	./symdec.bin -i testinput4.aes -k testkey

testclean:
	rm *.aes*


clean:
	rm *.bin *.o