/* a program to input students mark and determine the grade
* grade.c
**********************************************************/
#include<stdio.h>
float input(float mark1,float mark2,float mark3);
char Getgrade(float grade);

int main()
{
   float mark1,mark2,mark3;
   float c;
  
  
   printf("input the mark of the student\n");
   scanf("%f%f%f",&mark1,&mark2,&mark3);
   c=input(mark1,mark2,mark3);
   if(mark1>100||mark1<0)
     {
     printf("invalid marks for mark1\n");
     return 1;
      
    }
   else if(mark2>100||mark2<0)
   {
      printf("invalid marks for mark2\n");
     return 1;
      
    }
   else if(mark3>100||mark3<0)
   {
     printf("invalid marks  for mark3\n");
     return 1;
    }
   
       printf("the students average mark is %0.2f\n",input(mark1,mark2,mark3));
       
       printf("the students grade is %c\n",Getgrade(c));
   
}
float input(float mark1,float mark2,float mark3)
{
  float sum;
  sum=((mark1+mark2+mark3)/3);
  return sum;
}
char Getgrade(float grade)
{  
  
  if(grade>74 && grade<100)
  return 'A';
  else if(grade>59&&grade<75)
   return 'B';
  else if(grade>49&&grade<60)
   return 'C';
  else if(grade>40&&grade<50)
  return 'D';
   else if(grade<40 &&grade>=0)
  return 'E';
  else
  {
     printf("*****invalid grades*****\n");
     return 1;
  }
  
}
