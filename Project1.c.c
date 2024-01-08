/* 


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

/* Defining required values */

#define pi M_PI
#define E M_E
#define window 5
#define th 0.8
#define Amp_Max 1
#define eps_a_ratio 1/60
#define eps_T_ratio 1/60
#define n_ratio 1/30
#define smooth 0.2

/* Defining structure for different peaks of noisy data */

struct peak{
        double loc;
	double index;
        double w;
};

void srand(unsigned seed);
double nrand(int i);
double lorentz(double t,double a);
double gaussian(double t,double a);

int main(int argc, char** argv){
	FILE*ptr=fopen("graph.txt","w");
	int v=0,pos=0,check=0;
	int M=atoi(argv[1]);
	int T=atoi(argv[2]);
	double a=atof(argv[3]);


	
	/* Checking if Gaussian or Lorentzian */

	if (argc>4 && strcmp(argv[4],"Gaussian")==0){
		check=1;
	}
	int p;
	if (T%2==0){
		p=T+1;
	}
	else{
		p=T;
	}
	
	/* Defining arrays to hold data */

	double arr[p*(M+1)-1][2],arr2[p*(M+1)-1][2],s,r_width;
	struct peak list[M];

	/* Adding first few points of noisy data */

	for (int i=0;i<(p-1)/2;i++){
		double n=((Amp_Max)*nrand(v))*n_ratio;
		v+=1;
		fprintf(ptr,"%lf %lf\n",(double)i,n);
	}
	
	/* Creating time series of Gaussian or lorentzian */

	for (int m=1;m<M+1;m++){

		/* Adding width noise */

		double eps_a=(a*nrand(v))*eps_a_ratio;
		v+=1;
		double an=a+eps_a;

		/* Adding position of peak noise */

		double eps_T=(T*nrand(v))*eps_T_ratio;
		v+=1;
		double Tn=m*T+eps_T;
		double t=-(p-1)/2;
		for (int j=0;j<p;j++){
			if (check==1){
				r_width = 2*sqrt(2*log(2))*a;
				s=gaussian(t,an);
			}
			else{
				r_width=a*2;
				s=lorentz(t,an);
			}

			/* Adding amplitude noise */

			double n=((Amp_Max)*nrand(v))*n_ratio;
			v+=1;
			double sn=s+n;
			fprintf(ptr,"%lf %lf\n",Tn+t,sn);
			t+=1;
		}
	}

	int counter=0;

	/* Adding ending few points of noise */

	for (int i=(M-1)*p+p+(p-1)/2; i<p*(M+1)-1;i++){
		double n=((Amp_Max)*nrand(v))*n_ratio;
		v+=1;
		fprintf(ptr,"%lf %lf\n",(double)(M*T+(int)(T/2)+counter+1),n);
		counter+=1;
	}

	fclose(ptr);
	
	/* Reading  from file point by point like  a real time simulation */
	
	FILE*ptr2=fopen("graph.txt","r");
	double tt,b;
	fscanf(ptr2,"%lf %lf",&tt,&b);
	arr[0][0]=tt;
	arr2[0][0]=tt;
	arr[0][1]=b;
	arr2[0][1]=b;
	int index=1;
	int peakpos=0;
	int dummy=0;
	int chk=0;
	while (1){
		double max;
		int start,end;
		if (fscanf(ptr2,"%lf %lf",&tt,&b)!=-1){
			arr[index][0]=tt;
			arr[index][1]=b;
			arr2[index][0]=tt;

			/* Applying exponential filter to arr2 */

			arr2[index][1]=arr2[index-1][1] + (1-smooth)*(arr[index][1]-arr2[index-1][1]);
			if (arr2[index][1] < arr2[index-1][1]){

				/* Finding peaks */

				if (dummy==0 && arr2[index-1][1]>th){
					max=arr2[index-1][1];
					struct peak new;
					list[peakpos]=new;
					list[peakpos].loc=arr2[index-1][0];
					list[peakpos].index=index-1;
					for (int i=index-1;i>-1;i--){
						if (arr[i][1]<max/2){
							if ((max/2-arr[i][1])<(arr[i+1][1]-max/2)){
								start=i;
							}
							else{
								start=i+1;
							}
							break;
						}
					}
					chk=1;
					dummy=1;
				}

				/*  Using FWHM to find  width */

				else  if(chk==1 && arr2[index][1]<max/2){
					if ((max/2-arr[index][1])<(arr[index-1][1]-max/2)){
						end=index;
                                        }
                                        else{
						end=index-1;
					}
					chk=0;
					list[peakpos].w=arr2[end][0]-arr2[start][0];
					peakpos+=1;
				}            			
			}
			if (arr2[index][1] > arr2[index-1][1] && dummy==1 && arr2[index][1]>th){
				dummy=0;
			}
			index+=1;
		}
		else{
			break;
		}
	}

	/* Fidning avg and standard deviations */

	double Tarr[M-1];
        for (int i=0;i<M-1;i++){
                Tarr[i]=(list[i+1].loc) - (list[i].loc);
        }
        double avg_T=0,avg_a=0,std_T,std_a,sqsum_T=0,sqsum_a=0,a_val;
        for (int i=0;i<M-1;i++){
                avg_T=((i)*avg_T + Tarr[i])/(i+1);
                sqsum_T+=Tarr[i]*Tarr[i];
        }
        if (check==1){
                for (int i=0;i<M;i++){
                        a_val=(list[i].w)/(2*sqrt(2*log(2)));
                        avg_a=((i)*avg_a + a_val)/(i+1);
                        sqsum_a+=a_val*a_val;
                }
        }
        else{
                for (int i=0;i<M;i++){
                        a_val=(list[i].w)/2;
                        avg_a=((i)*avg_a + a_val)/(i+1);
                        sqsum_a+=a_val*a_val;
                }
        }
        std_T=sqrt((sqsum_T/(M-1)) - (avg_T)*(avg_T));
        std_a=sqrt((sqsum_a/M) - (avg_a)*(avg_a));
        printf("%lf,%lf,%lf,%lf\n",avg_T,avg_a,std_T,std_a);
	          
}

/* Function to geerate random standard normal distribution value */

double nrand(int i){
        double z,u1,u2;
        time_t t;
        srand((unsigned) i+time(&t));
        u1=(double)rand()/RAND_MAX;
        srand((unsigned) i+1+time(&t));
        u2=(double)rand()/RAND_MAX;
        z=sqrt(-2*log(u1))*sin(2*pi*u2);
        return z;
}

/* Function to find lorentz value */

double lorentz(double t,double a){
        double val;
        val=(a*a)/(t*t + a*a);
        return val;
}

/* Function to find gaussian value */

double gaussian(double t,double a){
        double val;
        val=pow(E,(-(t*t)/(2*a*a)));
        return val;
}