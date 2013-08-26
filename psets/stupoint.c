#include<stdio.h>
#include<cs50.h>

struct students
{ 
  char *name;
  char *course;
  int id;
};

int main()
{
  int s=0;
  int i;
  char c;
  
  struct students stu[50];
  printf("do you want to add a student y or n\n");
  scanf("%c",&c);
  if(c=='y'||c=='Y')
  {
      printf("input the  students name\n");
      scanf("%s",&stu[s].name);
      printf("input the  students course\n");
      scanf("%s",&stu[s].course);
      printf("input the  students id\n");
      scanf("%d",&stu[s].id);
    
          for (i=1;i<3;i++)
             {
               char h,z;
             system("read -n 1 -s -p*pressenterkeytocontinue...*  \n");
              printf("\n");
             
             printf("do you want to add another student y or n\n");
             
             scanf("%c",&z);
             
             scanf("%c",&h);
             
               if(h=='y'||h=='Y')
                 {
                  printf("input the  students name\n");
                  scanf("%s",&stu[i].name);
                  printf("input the  students course\n");
                  scanf("%s",&stu[i].course);
                  printf("input the  students id\n");
                  scanf("%d",&stu[i].id);
               
                 }
           
              else 
                {
                 printf("students name :\t students course :\t students id :\n%s\t\t\t  %s\t\t\t  %d\n",stu[s].name,stu[s].course,stu[s].id);
                 
                 return 1;
                 
                 }
               
             }
              
                     {
                      printf("u entered\n");
                      printf("students name :\t\t  students course :\t\tstudents id :\n%s\t\t\t  %s\t\t\t  %d\n",stu[s].name,stu[s].course,stu[s].id);
                      for(i=1;i<3;i++)
                      printf("%s\t\t\t  %s\t\t\t  %d\n",stu[i].name,stu[i].course,stu[i].id);
                      
                      
                      }
             
             
             
             
         }
              
             
      else 
             {
              return 1;
             }
      
      
             
              return 0;
}
