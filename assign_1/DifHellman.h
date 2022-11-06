#ifndef _DifHellman_H
#define _DifHellman_H

#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>




// function which checks if a number is prime or not.
// returns 0 if it's prime ---else 1.

int primenumber(int number);

//just a simple function to print for check.

void Print( char *PathName,int PrNumber,int Root,int PkeyA,int PkeyB);

//Function which calculates the Secret Key.
//Arguments:Prime number Primitive Root, Private Key A , Private Key B , Prime Number.
//Returns: -1 in case the keys are diffrent, else returns the shared Secret Key.

int CalculateSecret(int PnRoot,int PkeyA,int PkeyB,int PrimeNumber);


//Returns the public Key
//Arguments: Prime number Primitive Root, Private Key , Primenumber

int ReturnPublicKey(int PnRoot,int Pkey, int PrimeNumber);

//function to store in text file.

void adx_store_data(const char *filepath,const char *data);

//function whick checks if integer " a " is primitive root of prime integer q.
//returns "true" in case it's a primitive root -- else false.

bool isPrimitive (int q, int a);

//fuction to write 4 strings into a text file.

int WriteToFile(const char *filepath, const char *first,const char *second ,const char *third);

//print the output in the command line.
void PrintInCommand(char *PublicKeyA,char *PublicKeyB,char *secretKey);

//Help message
void PrintHelpMessage();

#endif 