# symasym-enc
This project is an extension of aes project, to accomodate other cryptographic algorithms quickly for encrypting files.

Note that these are created for proof of concept purposes, and you are welcome to study the code.
It may not be the best to directly implement them. (It might be more efficient to use a professional library, such as openssl.)
## Symmetric Encryption File Formats

Encrypted files produced by the symmetric encryption programs will generally be in the following format. (Parts in paranthesis may be missing based on the configuration.)
> 0x00 A B C (.) (*) (^) [Cipher text]

* A
    * 0x00: AES

* B: types of A (eg. AES-128)

* C: configuration
    * Bits: c0, c1, c2, c3, c4, c5, c6, c7
    * c6: 1 for skipping password check
    * c7: 1 for skipping file integrity check

* (.): 16 byte IV (initialization vector) (Missing if c6==1 && c7==1)
* (*): SHA256 hash of the password (with IV as prepended as salt) (Missing if c6==1)
* (^): SHA256 hash of the original file (with IV as prepended as salt) (Missing if c7==1)

### AES (src/symmetric/aes)
0x00 0x00 B C (.) (*) (^) [Cipher text]
* B: 0x00 for AES-128, 0x01 for AES-192, 0x02 for AES-256
* C:
    * (c0, c1, c2) = (0, 0, 0) ECB
    * (c0, c1, c2) = (0, 0, 1) CBC
    * (c0, c1, c2) = (0, 1, 0) CTR
    * c6: 1 for skipping password check
    * c7: 1 for skipping file integrity check

