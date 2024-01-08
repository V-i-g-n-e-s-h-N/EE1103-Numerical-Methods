/*Name: Vignesh.N
 * Roll: EE23B087
 * Date: 17/08/2023
 * Code to create a function called mysin which calculates the sine of an argument based on 
 Taylor expansion and then the error from the expected value
 */
 
 #include <stdio.h>;
 #include <math.h>;
 #include <stdlib.h>;
 
 int factorial{int}
 
 double mysin{int, double}
 
 int main(int argc, char* argv){
     int index1=atoi(argv[1]);
     double argsin=atof(argv[2]);
     double mysin1=mysin(index1, argsin);
     printf("The value of sine of the given argument calculated with expansion = %lf\n", mysin1);
     double err1= abs(mysin1-sin(argsin));
     printf("The absoulute error in the sine value obtained with expansion = %lf\n", err1);
     return 0
 }
 
 
 int factorial(int x){
     int factorial=1;
     for (i=1, i<=x, i++)
         factorial*=i;
     return factorial;
 }


double mysin(int x, double y){
   double mysin=0;
   for (i=1, i<=x,i++)
       mysin+= ((y**(2i-1))*((-1)**(n+1)))/factorial(2i-1);
    return mysin;
}
     
 
 
