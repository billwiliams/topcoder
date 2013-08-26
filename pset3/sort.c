#include <stdio.h>

#define SIZE 8

void sort(int array[], int size)
{
    int i,d,swap;
    for(i=0;i<(size-1);i++)
    {
      for(d=0;d<(size-i-1);d++)
      {
       if(array[d]>array[d+1])
         {
           swap=array[d];
           array[d]=array[d+1];
            array[d+1]=swap;
          }
       }
   }
}

int main(void)
{
    int numbers[SIZE] = { 4, 15, 16, 50, 8, 23, 42, 108 };
    for (int i = 0; i < SIZE; i++)
        printf("%d ", numbers[i]);
    printf("\n");
    sort(numbers, SIZE);
    for (int i = 0; i < SIZE; i++)
        printf("%d ", numbers[i]);
    printf("\n");
    return 0;
}
