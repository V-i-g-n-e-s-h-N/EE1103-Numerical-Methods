/* Vignesh.N, code to understnad the usage of Scanf function*/
#include <stdio.h>
int main()
{
    int i;
    int x; int y; int z;
    x=60;
    y=40;
    z=30;
    
    printf("Enter a value:");
    scanf("%d", &i);
    printf("The value you entered is= %d", i);
    return 0;
}