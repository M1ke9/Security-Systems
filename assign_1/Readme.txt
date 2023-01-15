

Michail Marketakis 2017030165

gcc (Ubuntu 9.3.0-17ubuntu1~20.04)


-----------------Diffie-Hellman Key Exchange----------------------

--MainDifHellman.c
--DifHellman.c
--DifHellman.h
--Makefile (TARGET1)

Comments:
--to run just type "make" and you will see the 2 executables ( dh_assign_1 , rsa_assign_1)
--the executable for Diffie Hellman is the "dh_assign_1".
--give the right arguments to run in ( ./dh_assign_1 -o output.txt -p 23 -g 7 -a 15 -b 2)



-----------------RSA Algorithm-------------------------------------

--MainRSA.c
--RSA.c
--RSA.h
--plaintext.txt
--Makefile(TARGET2)

Comments:
--to run just type "make" and you will see the 2 executables ( dh_assign_1 , rsa_assign_1)
--the executable for RSA is the "rsa_assign_1".
--First generate the keys!!!
--give the right arguments after generate the keys!!! 

Generate keys : ./rsa_assign_1 -g
Encrypt : ./rsa_assign_1 -i plaintext.txt -o ciphertext.txt -k public.key -e
decrypt:  ./rsa_assign_1 -i ciphertext.txt -o output.txt -k private.key -d

Notes for Decryption : 

I)Use the appopriate key,if the ciphertext is encrypted using the public key,you have to use the private key in order to decrypt data.

II)Don't forget to Use the output of the encryption ( ciphertext.txt) for input file for the decryption!!!




---IMPORTANT NOTE FOR RSA:

 I don't know why,i was looking for it a lot of days but there is a bug with small prime prices for p,q(key generation).

Descryption does not work with small prime numbers( 3,5 | 5,7 | 7,11).

To test the algorith use numbers up to 11 ( 11,13 | 13,19 | 19,23  etc...), it works and it's really impressive!!!












