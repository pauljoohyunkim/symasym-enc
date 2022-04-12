# symasym-enc
This project is an extension of aes project, to accomodate other cryptographic algorithms quickly for encrypting files.

Note that these are created for proof of concept purposes, and you are welcome to study the code.
It may not be the best to directly implement them. (It might be more efficient to use a professional library, such as openssl.)

## Executables
### aesenc (src/symmetric/aes/aesenc)
usage: aesenc [options]

Mandatory options:

	-t <1,2,3> : AES type (1: AES-128, 2: AES-192, 3: AES-256)
	-i <file>  : file to encrypt
	-k <file>  : key file. Depending on the type, only the first few bytes will be used.
	-p         : use integrated keygen instead of a key file. (Mutually exclusive with -k option)
	-b <mode>  : block cipher mode of operation; (ecb, cbc, ctr)

Common options:

	-o <file>  : specify output file. (default: <input file>.aes)
	-s         : skip password check during decryption.
	-f         : do not include file integrity hash.
	-h         : show help.

### symkeygen (src/symmetric/keygen/symkeygen)
usage: symkeygen <key file>

Common options:

	-l <length>: specify the number of bytes. (default: 32)
	-h         : show help.

 If <key file> argument is missing, it will be set to "keyfile" by default.

### symdec (src/decryptor/symdec)
 usage: symdec -i <input file> -k <key file> [options]

Mandatory options:

	-i <file>  : file to decrypt
	-k <file>  : key file. Depending on the type, only the first few bytes will be used.

Common options:

	-o <file>  : specify output file. (default: <input file>.decrypted)
	-s         : skip password check.
	-f         : skip file integrity check at the end.
	-h         : show help.

## Symmetric Encryption File Formats

Encrypted files produced by the symmetric encryption programs will generally be in the following format. (Parts in round brackets may be missing based on the configuration.)

0x00 A B C [.] (*) (^) [%] [Cipher text]

* A
    * 0x00: AES
    * 0x01: Blowfish
	* 0x02: Hill

* B: types of A (eg. AES-128)

* C: configuration
    * Bits: c0, c1, c2, c3, c4, c5, c6, c7
    * c6: 1 for skipping password check
    * c7: 1 for skipping file integrity check

* [.]: IV (initialization vector)
    * This will always be added regardless of it being used or not. The length depends on the encryption algorithm.
* (*): SHA256 hash of the password (with IV as prepended as salt) (Missing if c6==1)
* (^): SHA256 hash of the original file (with key as prepended as salt) (Missing if c7==1)
* [%]: Extra information used for block cipher mode of operation. The length depends on the encryption algorithm and the block cipher mode of operation.

### AES (src/symmetric/aes)
0x00 0x00 B C [.] (*) (^) [%] [Cipher text]

* B: 0x00 for AES-128, 0x01 for AES-192, 0x02 for AES-256
* C:
    * (c3, c4, c5) = (0, 0, 0) ECB
    * (c3, c4, c5) = (0, 0, 1) CBC
    * (c3, c4, c5) = (0, 1, 0) CTR
    * c6: 1 for skipping password check
    * c7: 1 for skipping file integrity check
    * Rest are set to zero.
* [.]: 16 byte IV
* [%]: One byte reserved for signalling how many bytes from the end to get rid of after decryption. (Missing if CTR mode)

### Blowfish
(Currently under construction)

### Hill
(Currently under construction)
NOTE: NEITHER PRACTICAL NOR RECOMMENDED

This is an implementation of hill cipher in mod 256, instead of traditionally used mod 26.

0x00 0x02 0x00 C [.] (*) (^) [%] [Cipher text]
* B is set to zero for 4-byte header format.


