AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = aesenc symkeygen symdec
aesenc_SOURCES = src/symmetric/aes/aesenc.c \
		 src/symmetric/aes/aesenc.h \
		 src/symmetric/aes/aes.c \
		 src/symmetric/aes/aes.h \
		 src/symmetric/bcm/bcm.c \
		 src/symmetric/bcm/bcm.h \
		 src/misc/hash.c \
		 src/misc/hash.h \
		 src/misc/file.c \
		 src/misc/file.h \
		 src/symmetric/keygen/keygen.c \
		 src/symmetric/keygen/keygen.h
aesenc_LDADD = -lcrypto

symkeygen_SOURCES = src/symmetric/keygen/symkeygen.c \
		    src/symmetric/keygen/keygen.c \
		    src/misc/hash.c
symkeygen_LDADD= -lcrypto

symdec_SOURCES = src/decryptor/symdec.c \
		 src/symmetric/aes/aes.c \
		 src/misc/hash.c \
		 src/misc/file.c \
		 src/symmetric/bcm/bcm.c \
		 src/symmetric/keygen/keygen.c
symdec_LDADD = -lcrypto
