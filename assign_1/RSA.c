
#include "RSA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <gmp.h>
#include <assert.h>




int primenumber(int number)
{
    int i;
    

    for (i = 2; i <= number / 2; i++) {
        if (number % i != 0)
            continue;
        else
            return 1;
    }
    return 0;
}



	void rsa_keygen(mpz_t p,mpz_t q)
{
	
	
	mpz_t n;
    mpz_t e;
	mpz_t d;
	mpz_t e1;

	long GCD;
	long MOD;

//initialize	
mpz_inits(n,d,NULL);
mpz_inits(e,e1,NULL);

	mpz_t p_1, q_1, lambda, gcd, mul, mod;
    mpz_inits(p_1, q_1, lambda, gcd, mul, mod,NULL);

//n=p*q
	mpz_mul(n, p, q);
//p_1=p-1
    mpz_sub_ui(p_1, p, 1);
//q_1=q-1
    mpz_sub_ui(q_1, q, 1);
//lamda=p_1 * q_1
    mpz_mul(lambda, p_1, q_1);

    printf("lambda = %s\n", mpz_get_str(NULL, 0, lambda));

 //calculating e...  
do{

	mpz_nextprime(e,e1);
	mpz_set(e1,e);

	mpz_gcd(gcd,e,lambda);
	mpz_mod(mod,e,lambda);
	
	GCD=mpz_get_si(gcd);
	MOD=mpz_get_si(mod);

}
while(MOD==0 || GCD!=1);

//checks for e..
 assert(mpz_cmp_ui(gcd, 1) == 0);
 assert(mpz_cmp_ui(mod,0)==1);

//d=modular inverse of(e,lambda);
    mpz_invert(d, e, lambda);

   

   

FILE* public_key;
FILE* private_key;


	public_key = fopen("public.key", "wb");
	private_key = fopen("private.key", "wb");

//convert from mpz_t to size_t to write into files.
	size_t N=(size_t)mpz_get_si(n);
	size_t D=(size_t)mpz_get_si(d);
	size_t E=(size_t)mpz_get_si(e);

	fwrite(&N, sizeof(size_t), 1, public_key);
	fwrite(&D, sizeof(size_t), 1, public_key);

	fwrite(&N, sizeof(size_t), 1, private_key);
	fwrite(&E, sizeof(size_t), 1, private_key);



   fclose(public_key);
	fclose(private_key);

}



unsigned char *read_file(char *path, unsigned long *len) {

  FILE *f;
  char *data;

  f = fopen (path, "r");

  fseek (f, 0, SEEK_END);
  *len = ftell(f);

  fseek (f, 0, SEEK_SET);
  data = malloc(*len);

  if (data) fread(data, 1, *len, f);
  fclose(f);


  return data;
}


void rsa_encrypt(char *input_file, char *output_file, char *key_file)
{
	FILE *output_data;
	unsigned char *in_data, *key_data;
	size_t n, d_or_e, *out_data;

	unsigned long input_len ;
	unsigned long out_len ;
	unsigned long key_len ;



	in_data 	= read_file(input_file, &input_len);
	if (in_data == 0){
		printf("Could not read file.\n");
		exit(0);
	}


	key_data	= read_file(key_file,&key_len);
	if (key_data == 0){
		printf("Could not read key file.\n");
		exit(0);
	
	}
	memcpy(&n, key_data, sizeof(size_t));
	memcpy(&d_or_e, key_data+sizeof(size_t), sizeof(size_t));
	

	output_data = fopen(output_file,"w");
	out_len  = input_len*sizeof(size_t);
	out_data = (size_t*)malloc(out_len);

//gmp library..




mpz_t OUT_DATA;

mpz_t IN_DATA;
mpz_t D_OR_E;
mpz_t N;

//initialize 

	mpz_init(OUT_DATA);
	mpz_init(IN_DATA);

mpz_init(D_OR_E);
mpz_init(N);
mpz_init(OUT_DATA);
mpz_init(IN_DATA);

//convert from size_t to mpz_t
mpz_set_si(D_OR_E,d_or_e);
mpz_set_si(N,n);

size_t Outt;


	for (int i=0; i<input_len; i++){

		mpz_set_si(IN_DATA,in_data[i]);

		//modular_pow!!!
		mpz_powm(OUT_DATA,IN_DATA,D_OR_E,N);
		Outt=mpz_get_si(OUT_DATA);
		out_data[i]=(size_t)Outt;

	}
	

		fwrite(out_data, sizeof(size_t),input_len, (FILE *)output_data);

	

	fclose(output_data);

}






void rsa_decrypt(char *input_file, char *output_file, char *key_file)
{

		FILE *output_data;

		unsigned char *in_data, *key_data, *plaintext;
		size_t n, d_or_e, *out_data;

		unsigned long input_len ;
		unsigned long out_len ;
		unsigned long key_len ;


		in_data 	= read_file(input_file, &input_len);
		if (in_data == 0){
			printf("Could not read file.\n");
			exit(0);
		}

		key_data	= read_file(key_file,&key_len);
		if (key_data == 0){
			printf("Could not read key file.\n");
			exit(0);
		}

		output_data = fopen(output_file,"wb");
		out_data = (size_t*)malloc(input_len);


		memcpy(out_data, in_data, input_len/sizeof(size_t));

		memcpy(&n, key_data, sizeof(size_t));
		memcpy(&d_or_e, key_data+sizeof(size_t), sizeof(size_t));


		plaintext = (unsigned char*)malloc((input_len/sizeof(size_t))*sizeof(unsigned char));
		size_t tmp=1;


mpz_t D_OR_E;
mpz_t N;
mpz_t BASE;
mpz_t PLAINTEXT;
size_t OUT_DATA;




mpz_init(PLAINTEXT);
mpz_init(BASE);
//mpz_inits(D_OR_E,N,NULL);
mpz_init(D_OR_E);
mpz_init(N);

//convert from size_t to mpz_t
mpz_set_si(D_OR_E,d_or_e);
mpz_set_si(N,n);


		for (int i=0; i<input_len/sizeof(size_t); i++){

			mpz_set_si(BASE,out_data[i]);
           mpz_powm(PLAINTEXT,BASE,D_OR_E,N);

		   OUT_DATA=mpz_get_si(PLAINTEXT);
		   plaintext[i]=(unsigned char)(OUT_DATA);


		}
	
		fwrite(plaintext, sizeof(char),input_len/sizeof(size_t), (FILE *)output_data);

	fclose(output_data);

}


void PrintHelpMessage()
{
printf("\n Options: \n");
printf("-i path---->Path to the input file\n");
printf("-o path---->Path to the output file\n");
printf("-k path---->Path to the key file\n");
printf("-g  ---->Perform RSA key-pair generation\n");
printf("-d  ---->Descrypt input and store results to output\n");
printf("-e  ---->Encrypt input and store results to output\n");

}













