#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "RSA.h"

#include <gmp.h>


void main(int argc,char ** argv)
{


    int option;			
	int choise;		
	char *input_file=NULL;		
	char *output_file=NULL;		
	char *key_file=NULL;	
	int P;
	int Q;
	int check=0;

	 mpz_t p;
	 mpz_t q;

	mpz_inits(p,q,NULL);

while ((option = getopt(argc, argv, "i:o:k:degh:")) != -1) {
		switch (option) {
		case 'i':
			input_file = strdup(optarg);
			break;
		case 'o':
			output_file = strdup(optarg);
			break;
		case 'k':
			key_file = strdup(optarg);
			break;
		case 'd':
          //Decrypt
        if(input_file!=NULL && output_file!=NULL && key_file!=NULL)
		     choise = 0;

        else
        {
            printf("You have to enter Input,Output and Key files!!!");
            exit(0);
        }
			break;

		case 'e':
		  //Encrypt
            if(input_file!=NULL && output_file!=NULL && key_file!=NULL)
			choise = 1;

            else
        {
            printf("You have to enter Input,Output and Key files!!!");
            exit(0);
        }

			break;
		case 'g':
			/* if op_mode == 2 the tool performs keygen */
			choise = 2;
			break;
		case 'h':
		default:
		PrintHelpMessage();
		exit(EXIT_FAILURE);
		break;
		
		}
	}



    switch (choise) {
	case 0:
		rsa_decrypt(input_file, output_file, key_file);
		break;
	case 1:
		rsa_encrypt(input_file, output_file, key_file);
		break;
	case 2:
	

	do{
	
	printf("\nEnter the first prime number: ");
	scanf("%d",&P);

	printf("\nEnter the second prime number: ");
	scanf("%d",&Q);

if(primenumber(P)==0 && primenumber(Q)==0)
check=1;

else
printf("One of the numbers you entered is not prime,try again !!");
	
	}
	while(check==0);

	mpz_set_ui(q,(int)Q);
	mpz_set_ui(p,(int)P);


		rsa_keygen(p,q);

		break;
	default:
		break;
	}


}