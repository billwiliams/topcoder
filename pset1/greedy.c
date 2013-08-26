/*************************************************************************
#greedy.c
A program to compute the least amount of coins to give out
# written by wilson ndirangu ndirangu 
#the program takes in the amount owed and calculates the least number of coins to give out
*************************************************************************/
#include<cs50.h>
#include<math.h>

#include<stdio.h>
int main()
  {
     printf("oh Hai,How much change is owed?\n");
     float ch=GetFloat();
     while(ch<0)
           { 
             printf("RETRY:");
             ch=GetFloat();
           
           } 
      int q,n,d,p;//where q is quarters ,n is nickels,d is dimes and p is penny
      float A,B,C;//A is number of coins after subtracting max number of quaters
      q=ch/0.25;
      A=ch-(q*0.25);
      d=A/0.10;
      B=A-(d*0.10);//Bis number of coins after subtracting max number of quaters and dimes
      n=B/0.05;
      C=((B-(n*0.05))/0.01);//Cis number of dimes after subtracting max number of quaters and dimes and nickels in flaoting point
      p=round(C);// rounding of the number of dimes to the nearest whole number
      

     int s=q+d+n+p;//s is the sum of the number of dimes ,nickels,pennys and quarters
     printf("%d",s);
     printf("\n");
  }
