CC = gcc
CFLAGS = -g -Wall

all: aesenc.exe symdec.exe symkeygen.exe

symdec.exe: symdec.o aes.o hash.o bcm.o file.o keygen.o
	$(CC) $(CFLAGS) symdec.o aes.o hash.o bcm.o file.o keygen.o -lcrypto -o symdec.exe

symdec.o: decryptor/symdec.c decryptor/symdec.h
	$(CC) $(CFLAGS) decryptor/symdec.c -c

aesenc.exe: aesenc.o bcm.o aes.o hash.o file.o keygen.o
	$(CC) $(CFLAGS) aesenc.o bcm.o aes.o hash.o file.o keygen.o -lcrypto -o aesenc.exe

symkeygen.exe: symkeygen.o hash.o keygen.o
	$(CC) $(CFLAGS) symkeygen.o hash.o keygen.o -lcrypto -o symkeygen.exe

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

testgen:
	head -c 54 /dev/urandom > testinput
	head -c 32 /dev/urandom > testkey

test: aesenc.exe symdec.exe
	./aesenc.exe -i testinput -t 1 -k testkey -b ecb -o testoutput1
	./aesenc.exe -i testinput -t 2 -k testkey -b cbc -o testoutput2
	./aesenc.exe -i testinput -t 3 -k testkey -b ctr -o testoutput3
	./aesenc.exe -i testinput -t 2 -k testkey -b ctr -o testoutput4
	./symdec.exe -i testoutput1 -k testkey
	./symdec.exe -i testoutput2 -k testkey
	./symdec.exe -i testoutput3 -k testkey
	./symdec.exe -i testoutput4 -k testkey

testclean:
	rm test*



clean:
	rm *.exe *.o
