/*******************************************************************************************
*vigenere cypher
*pset2 
*A program to encrypt a message using vegenere cypher method
*written by Ndirangu Ndirangu Wilson from edx as Craigvanwilson
*
*
*
******************************************************************************************/
#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
if (argc !=2  ) 
   {
    printf("error\n");
    return 1;
    }
   
    

char *k=argv[1];

int w=strlen(k);



  for (int i = 0; i < w; i++)//for loop to iterate within the characters of the key provided
     {
     
      
  
         if (islower(k[i])) //condition for lower case letters 
           {
           
            k[i]=k[i] -'a';
            ;//vigenere cypher the letter to the key provided
            
            }
         else if (isupper(k[i]))//condition for upper case letters 
           {
             k[i]=k[i] -'A';
             
           }
           else
           {
            printf("error\n");
            return 1;
            }
         }
         
         
          int p=0;//position of the cursor
          char *s=GetString();
           int n=strlen(s);
          
            for (int i = 0; i < n; i++)
           {
        char a;
        if (isupper(s[i]))
            a = 'A';
        else if (islower(s[i]))
            a = 'a';
        else
            continue; // not alphanumeric.
        s[i]=(s[i]-a + k[p]) % 26+ a;
        p++;
        p %= w;
        }

    
printf("%s",s);//printing the ciphered letters
 printf("\n");


return 0;

}


