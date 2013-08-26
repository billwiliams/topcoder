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

int main(int argc, string argv[])
{


string s=GetString ();
int k=atoi(argv[1]);
if (argc < 2 ||argc==0 ) 
   {
    printf("error\n");
    return 1;
    }
    else
    {

for (int i = 0, n= strlen(s); i < n; i++)
{
  
  
 
s[i] = (s[i]-'a' + k) % 26+'a';
s[i] += (s[i] == ' ') ? 26 : 0;

           

    
   printf("%c",s[i]);
 
   
}

}
  printf("\n");

return 0;


}
