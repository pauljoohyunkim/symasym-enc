CC = gcc
CFLAGS = -g -Wall

all: aesenc.bin symdec.bin symkeygen.bin

symdec.bin: symdec.o aes.o hash.o bcm.o file.o keygen.o
	$(CC) $(CFLAGS) symdec.o aes.o hash.o bcm.o file.o keygen.o -lcrypto -o symdec.bin

symdec.o: decryptor/symdec.c decryptor/symdec.h
	$(CC) $(CFLAGS) decryptor/symdec.c -c

aesenc.bin: aesenc.o bcm.o aes.o hash.o file.o keygen.o
	$(CC) $(CFLAGS) aesenc.o bcm.o aes.o hash.o file.o keygen.o -lcrypto -o aesenc.bin

symkeygen.bin: symkeygen.o hash.o keygen.o
	$(CC) $(CFLAGS) symkeygen.o hash.o keygen.o -lcrypto -o symkeygen.bin

symkeygen.o: keygen/symkeygen.c keygen/symkeygen.h
	$(CC) $(CFLAGS) keygen/symkeygen.c -c

keygen.o: keygen/keygen.c keygen/keygen.h
	$(CC) $(CFLAGS) keygen/keygen.c -c

aesenc.o: aes/aesenc.c aes/aesenc.h
	$(CC) $(CFLAGS) aes/aesenc.c -c

bcm.o: bcm/bcm.c bcm/bcm.h
	$(CC) $(CFLAGS) bcm/bcm.c -c

aes.o: aes/aes.c aes/aes.h
	$(CC) $(CFLAGS) aes/aes.c -c

hash.o: misc/hash.c misc/hash.h
	$(CC) $(CFLAGS) misc/hash.c -c

file.o: misc/file.c misc/file.h
	$(CC) $(CFLAGS) misc/file.c -c


test: aesenc.bin symdec.bin
	./aesenc.bin -i testinput -t 1 -k testkey -b ecb -o testoutput1
	./aesenc.bin -i testinput -t 2 -k testkey -b cbc -o testoutput2
	./aesenc.bin -i testinput -t 3 -k testkey -b ctr -o testoutput3
	./aesenc.bin -i testinput -t 2 -k testkey -b ctr -o testoutput4
	./symdec.bin -i testoutput1 -k testkey
	./symdec.bin -i testoutput2 -k testkey
	./symdec.bin -i testoutput3 -k testkey
	./symdec.bin -i testoutput4 -k testkey

testclean:
	rm *.aes*


clean:
	rm *.bin *.o