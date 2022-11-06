#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "DifHellman.h"



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


int CalculateSecret(int PnRoot,int PkeyA,int PkeyB,int PrimeNumber)
{
	int x=(long long int)pow(PnRoot,PkeyA) % (PrimeNumber);
    int y=(long long int) pow(PnRoot,PkeyB) % (PrimeNumber);


// exchange numbers

int k1=(long long int)(pow(y,PkeyA))% (PrimeNumber);

int k2=(long long int)(pow(x,PkeyB)) % (PrimeNumber);


if(k1==k2)
return k1;

return -1;

}




void Print( char *PathName,int PrNumber,int Root,int PkeyA,int PkeyB)
{
printf("\nFile name: %s \n",PathName);
printf("PrimeNumber: %d \n",PrNumber);
printf("PnRoot: %d \n",Root);
printf("PkeyA: %d \n",PkeyA);
printf("PkeyB: %d\n ",PkeyB);

}



int ReturnPublicKey(int PnRoot,int Pkey, int PrimeNumber)
{
	int z=(long long int)pow(PnRoot,Pkey) % (PrimeNumber);

	return z;
}



bool isPrimitive (int q, int a) {
int i,z;
double k,s;

s=1;
i=0;

  double *factors = (double*)(malloc(sizeof(double)*(q-2)));

while (s>0 && i<q-2) {
    k=pow(a,i);
    s=k-(q*floor(k/q));
    for (z=0;z<(sizeof(*factors)/sizeof(factors[0]));z++) {
        if (factors[z]==s) {
            return false;
        }
    }
    factors[i]=s;
    i++;
}
return true;
}



int WriteToFile(const char *filepath, const char *first,const char *second ,const char *third)
{
    FILE *fp;
	
    fp = fopen(filepath, "w");
 
    if(fp == NULL) {
        printf("file can't be opened\n");
        exit(1);
    }
 

        fprintf(fp, "<: %s > \t < %s > \t < %s > \n", first,second, third);
 
        printf("\n Details successfully written to the file\n\n");
    
 
    fclose(fp);
 
    return 0;

}



void PrintInCommand(char *PublicKeyA,char *PublicKeyB,char *secretKey)
{
    printf("\n < %s > < %s> < %s > \n",PublicKeyA,PublicKeyB,secretKey);
}


void PrintHelpMessage()
{
    printf("Options: \n");
    printf("-o  path----->path to output file\n");
    printf("-p  number--->Prime number\n");
    printf("-g  number--->Primitive Root for the previous prime number\n");
    printf("-a  number--->Private key A\n");
    printf("-b  number--->Private key B\n");

}