CC = gcc
CFLAGS = -g -Wall

all: aesenc.exe symdec.exe symkeygen.exe

symdec.exe: symdec.o aes.o hash.o bcm.o file.o keygen.o
	$(CC) $(CFLAGS) symdec.o aes.o hash.o bcm.o file.o keygen.o -lcrypto -o symdec.exe

symdec.o: ../src/decryptor/symdec.c ../src/decryptor/symdec.h
	$(CC) $(CFLAGS) ../src/decryptor/symdec.c -c

aesenc.exe: aesenc.o bcm.o aes.o hash.o file.o keygen.o
	$(CC) $(CFLAGS) aesenc.o bcm.o aes.o hash.o file.o keygen.o -lcrypto -o aesenc.exe

symkeygen.exe: symkeygen.o hash.o keygen.o
	$(CC) $(CFLAGS) symkeygen.o hash.o keygen.o -lcrypto -o symkeygen.exe

symkeygen.o: ../src/symmetric/keygen/symkeygen.c ../src/symmetric/keygen/symkeygen.h
	$(CC) $(CFLAGS) ../src/symmetric/keygen/symkeygen.c -c

keygen.o: ../src/symmetric/keygen/keygen.c ../src/symmetric/keygen/keygen.h
	$(CC) $(CFLAGS) ../src/symmetric/keygen/keygen.c -c

aesenc.o: ../src/symmetric/aes/aesenc.c ../src/symmetric/aes/aesenc.h
	$(CC) $(CFLAGS) ../src/symmetric/aes/aesenc.c -c

bcm.o: ../src/symmetric/bcm/bcm.c ../src/symmetric/bcm/bcm.h
	$(CC) $(CFLAGS) ../src/symmetric/bcm/bcm.c -c

aes.o: ../src/symmetric/aes/aes.c ../src/symmetric/aes/aes.h
	$(CC) $(CFLAGS) ../src/symmetric/aes/aes.c -c

hash.o: ../src/misc/hash.c ../src/misc/hash.h
	$(CC) $(CFLAGS) ../src/misc/hash.c -c

file.o: ../src/misc/file.c ../src/misc/file.h
	$(CC) $(CFLAGS) ../src/misc/file.c -c

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
