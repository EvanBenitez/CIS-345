#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;
pthread_t *roots;

int k;
int initial; //input value to calculate the sum of squares
double *total;; //total of sum of squares

void *square_root(void *t){
	long  index = (long) t;
	double sub_total = 0.0;
	for(int i = (index*initial)+1; i <= (index+1)*initial;i++){
		sub_total += sqrt(i);
	}


	total[index] = sub_total;
	int i = 1;
	while(index < k/(int)pow(2,i)){
		int partner = ((int)(pow(2.0,log(k)/log(2)-i))) ^ (int)index;
		pthread_join(roots[partner],NULL);
		total[index] +=  total[partner];
		i++;
	}
	if(index == 0){
		printf("sum of squares: %lf\n",total[index]);
	}
}

int main(int argc,char *argv[]){
	if(argc == 3){
		initial = atoi(argv[2])/ atoi(argv[1]);
		k = atoi(argv[1]);
		total = malloc(sizeof(double)*atoi(argv[1]));

		roots = malloc(sizeof(pthread_t)*atoi(argv[1]));


		for(long i=k-1;i>=0;i--){
			if(pthread_create(&roots[i],NULL,square_root,(void *) i) != 0){
				printf("Did not create thread\n");
			}
		}
	pthread_exit(NULL);
	}
	else{
		printf("Must enter number of threads and number to total");
	}
}
