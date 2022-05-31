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
		    src/symmetric/keygen/symkeygen.h \
		    src/symmetric/keygen/keygen.c \
		    src/symmetric/keygen/keygen.h \
		    src/misc/hash.c \
		    src/misc/hash.h 
symkeygen_LDADD= -lcrypto

symdec_SOURCES = src/decryptor/symdec.c \
		 src/decryptor/symdec.h \
		 src/symmetric/aes/aes.c \
		 src/symmetric/aes/aes.h \
		 src/misc/hash.c \
		 src/misc/hash.h \
		 src/misc/file.c \
		 src/misc/file.h \
		 src/symmetric/bcm/bcm.c \
		 src/symmetric/bcm/bcm.h \
		 src/symmetric/keygen/keygen.c \
		 src/symmetric/keygen/keygen.h 

symdec_LDADD = -lcrypto
