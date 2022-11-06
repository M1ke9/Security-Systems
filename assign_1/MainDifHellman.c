#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

#include "DifHellman.h"




void main(int argc,char** argv) 
{

int option;
char *path;
int semiPrimeNumber;
int PrimeNumber=-1;
int PnRoot;
int PkeyA;
int PkeyB;
int check;

char *PrePnRoot;
char *PreSemiPrimeNumber;
char *PrePkeyA;
char *PrePkeyB;
	





while ((option = getopt(argc, argv, "o:p:g:a:b:h")) != -1) {
		switch (option) {
		case 'o':
			path= strdup(optarg);
			break;
		case 'p':
			PreSemiPrimeNumber=strdup(optarg);
			semiPrimeNumber=atoi(PreSemiPrimeNumber);
			break;
		case 'g':
			PrePnRoot= strdup(optarg);
			PnRoot=atoi(PrePnRoot);
			break;
		case 'a':

		PrePkeyA=strdup(optarg);
		PkeyA=atoi(PrePkeyA);
        
			break;

		case 'b':
		  PrePkeyB=strdup(optarg);
		  PkeyB=atoi(PrePkeyB);
			break;
		
		case 'h':
		default:
		PrintHelpMessage();
		exit(EXIT_FAILURE);
		break;
		
		}
	}

if( primenumber(semiPrimeNumber)==0)
PrimeNumber=semiPrimeNumber;


	 if( primenumber(semiPrimeNumber)==1)
	 {
	printf("\nThe number: %d is not a prime number\n",semiPrimeNumber);
	exit(0);
	 }


	 if(isPrimitive(PrimeNumber,PnRoot)==false)
	{
		printf("\nThe number %d  is not primitive root of prime number %d\n",PnRoot,PrimeNumber);
		exit(0);
	}





 check=CalculateSecret(PnRoot,PkeyA, PkeyB, PrimeNumber);

if(check!=-1)
//printf("\n\nThe secret is: %d " , check);

if(check==-1)
{
printf("\n The secret keys are different , Error!!! \n ");
exit(0);

}


int PublicKeyA= ReturnPublicKey(PnRoot,PkeyA,PrimeNumber);
int PublicKeyB=ReturnPublicKey(PnRoot,PkeyB,PrimeNumber);

//printf("KLEIDIA: %d", PublicKeyA);


//convert integers to strings

char textA[20];
char textB[20];
char secretKey[20];


sprintf(textA , "%d", PublicKeyA);
sprintf(textB , "%d" , PublicKeyB);
sprintf(secretKey,"%d",check);





WriteToFile(path,textA,textB,secretKey);

PrintInCommand(textA,textB,secretKey);



}
