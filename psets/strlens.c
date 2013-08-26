#include<stdio.h>


int len(char *str);
int main()
{
char s[100000];
printf ("enter a string:");
scanf("%s",&s);
printf (" string length is %d:",len(s));
return 0;
}
int len(char *str)
{
int c=0;
while(str[c]!='\0')
c++;
return c;

}
