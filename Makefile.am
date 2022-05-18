AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = aesenc symkeygen symdec
aesenc_SOURCES = src/symmetric/aes/aesenc.c \
		 src/symmetric/aes/aes.c \
		 src/symmetric/bcm/bcm.c \
		 src/misc/hash.c \
		 src/misc/file.c \
		 src/symmetric/keygen/keygen.c
aesenc_CFLAGS = -crypto

symkeygen_SOURCES = src/symmetric/keygen/symkeygen.c \
		    src/symmetric/keygen/keygen.c \
		    src/misc/hash.c
symkeygen_CFLAGS= -crypto

symdec_SOURCES = src/decryptor/symdec.c \
		 src/symmetric/aes/aes.c \
		 src/misc/hash.c \
		 src/misc/file.c \
		 src/symmetric/bcm/bcm.c \
		 src/symmetric/keygen/keygen.c
symdec_CFLAGS= -crypto
