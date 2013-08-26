/******************************************************************************
* pset1.c
*by  wilson ndirangu ndirangu alias craigvanwilson on edx
*a program to display mario block in terms of ####
*the program takes the height of the of the pyramid and 
******************************************************************************/

#include<cs50.h>
#include<stdio.h>
int main()
  { 
     int sp,r,c;//where sp is space ,r and c are for the loops
     printf("enter height of the pyramid\n");
     int h=GetInt();//where h is the height of the pyramid
     if(h>=0 && (h<=23))
       {

          for(r=1;h>=r;r++)
             {
                  for(sp=h-r;sp>=1;sp--)//for loop to implement the spaces at every height
                  printf(" ");
                  for(c=1;c<=r+1;c++)//for loop to reduce the width of the block by 1 #
                  printf("#");
                  printf("\n");
              }
         }
       else
            {
            printf("Height:%d\n",h);
            printf("RETRY:");
            int h=GetInt();
            while(h<0 ||(h>23))
                {
                   printf("Height:%d\n",h);
                   printf("RETRY:");
                   h=GetInt();
                }
              for(r=1;h>=r;r++)
             {
                  for(sp=h-r;sp>=1;sp--)//for loop to implement the spaces at every height
                  printf(" ");
                  for(c=1;c<=r+1;c++)//for loop to ensure the top bloock has two ##
                  printf("#");
                  printf("\n");
              }
              }
                
           
 
 
  }  
