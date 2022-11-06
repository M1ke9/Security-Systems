#ifndef _RSA_H
#define _RSA_H

#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gmp.h>





void rsa_keygen(mpz_t p,mpz_t q);



int primenumber(int number);


void rsa_encrypt(char *input_file, char *output_file, char *key_file);


void rsa_decrypt(char *input_file, char *output_file, char *key_file);


unsigned char *read_file(char *path, unsigned long *len);


void PrintHelpMessage();





#endif 