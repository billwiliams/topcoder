/*******************************************************************************************
*ceasar cypher
*pset2 
*A program to encrypt a message using ceasar cypher method
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

if (argc < 2) //condition for no arguements provided
   {
          printf("!!!!!!!!!!error  no arguemeants provided!!!!!!\n");
          return 1;
   }




   
   int k=atoi(argv[1]);
   string s=GetString();

for (int i = 0, n= strlen(s); i < n; i++)//for loop to iterate within the characters of the string provided
     {
  
  
         if (islower(s[i])) //condition for lower case letters 
           {
            
            s[i] = (s[i]-'a' + k) % 26+'a';//caesar cypher the letter to the key provided
            s[i]+= (s[i] < 'a') ? 26 : 0;
            }
         else if (isupper(s[i]))//condition for upper case letters 
           {
             s[i]= (s[i]-'A' + k) % 26 + 'A';
             s[i]+= (s[i] < 'A') ? 26 : 0;
           }

    
          printf("%c",s[i]);//printing the ciphered letters
   
 
   
          }



 printf("\n");

return 0;


}
