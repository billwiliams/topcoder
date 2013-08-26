#include<stdio.h>
#include<math.h>
int fac(int x);
float e(int count);
int main()
{ 
        int count;
        float e;


        for (count=1;count<10;count++)
        {

        e+=(1.0/fac(count));
        }




        printf("%f",e+1);
        return 0;
}
int fac(int x)
{
if(x==0)
return 1;
else
return x*fac(x-1);
}

        





