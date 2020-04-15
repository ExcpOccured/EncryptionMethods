# Encryption methods

Contains the basic algorithms encrypt a message in cryptography

## Implemented methods:

### Transopisition cipher:
Method of symmetric encryption in which elements of the source plaintext are swapped. Text elements can be individual characters (the most common case),
pairs of letters, triples of letters, a combination of these cases, and so on.

[Source code](https://github.com/ExcpOccured/EncryptionMethods/blob/master/EncryptionMethods/src/TranspositionCipher.cpp)

### Vigenère cipher:
Vigenère can also be described algebraically. If the letters A–Z are taken to be the numbers 0–25 (A XOR B etc) and addition is performed modulo 26.

[Source code](https://github.com/ExcpOccured/EncryptionMethods/blob/master/EncryptionMethods/src/VigenereCipher.cpp)

### Feistel cipher:

 This cipher is a symmetric structure used in the construction of block ciphers. A large proportion of block ciphers use the scheme, including the Data Encryption Standard (DES). 
 The Feistel structure has the advantage that encryption and decryption operations are very similar, even identical in some cases, requiring only a reversal of the key schedule. Therefore, the size of the code or circuitry 
  required to implement such a cipher is nearly halved.
  
  [Source code](https://github.com/ExcpOccured/EncryptionMethods/blob/master/EncryptionMethods/src/FeistelCipher.cpp)
  
  ## License
  
  This project is licensed under the GPL-3.0 - see the [LICENSE.md](LICENSE.md) file for more details.