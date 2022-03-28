CC = gcc
CFLAGS = -g




symdec.bin: symdec.o hash.o
	$(CC) $(CFLAGS) symdec.o hash.o -o symdec.bin

symdec.o: symdec.c symdec.h
	$(CC) $(CFLAGS) symdec.c -c

hash.o: ../misc/hash.c ../misc/hash.o
	$(CC) $(CFLAGS) ../misc/hash.c -c


clean:
	rm *.o *.bin