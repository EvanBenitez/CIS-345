#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

int initial; //input value to calculate the sum of squares
double total = 0; //total of sum of squares

void *square_root(void *t){
	long  index = (long) t;
	double sub_total = 0.0;
	for(int i = (index*initial)+1; i <= (index+1)*initial;i++){
		sub_total += sqrt(i);
	}

	printf("thread %ld: %lf\n", index, sub_total);

	pthread_mutex_lock(&lock);
	total += sub_total;
	pthread_mutex_unlock(&lock);

	pthread_barrier_wait(&barrier);
}

int main(int argc,char *argv[]){
	if(argc == 3){
		initial = atoi(argv[2])/ atoi(argv[1]);

		pthread_t roots[atoi(argv[1])];
		//roots = malloc(sizeof(pthread_t) * atoi(argv[1]));

		pthread_barrier_init(&barrier,NULL,atoi(argv[1])+1);

		for(long i=0;i<=atoi(argv[1])-1;i++){
			if(pthread_create(&roots[i],NULL,square_root,(void *) i) != 0){
				printf("Did not create thread\n");
			}
		}
		pthread_barrier_wait(&barrier);
		printf("sum of square roots: %f\n",total);
	}
	else{
		printf("Must enter number of threads and number to total");
	}
}
