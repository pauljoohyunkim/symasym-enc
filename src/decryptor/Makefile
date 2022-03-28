CC = gcc
CFLAGS = -g




symdec.bin: symdec.o hash.o aes.o aesbcm.o
	$(CC) $(CFLAGS) symdec.o hash.o aes.o aesbcm.o -lcrypto -o symdec.bin

symdec.o: symdec.c symdec.h
	$(CC) $(CFLAGS) symdec.c -c

hash.o: ../misc/hash.c ../misc/hash.o
	$(CC) $(CFLAGS) ../misc/hash.c -c

aes.o: ../symmetric/aes/aes.c ../symmetric/aes/aes.h
	$(CC) $(CFLAGS) ../symmetric/aes/aes.c -c

aesbcm.o: ../symmetric/aes/aesbcm.c ../symmetric/aes/aesbcm.h
	$(CC) $(CFLAGS) ../symmetric/aes/aesbcm.c -c

clean:
	rm *.o *.bin