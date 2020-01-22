#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define NUM_THREAD 100
//1~5 덧셈: thread1이 수행
//6~10 덧셈: thread2가 수행

void* thread_inc(void *arg);
void* thread_des(void *arg);
long long num = 0;

int main(int argc, char* argv[]) {
	pthread_t thread_id[NUM_THREAD];
	int i;

	printf("sizeof long long: %d \n", sizeof(long long));
	for (i = 0; i < NUM_THREAD; i++) {
		if (i % 2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}

	for (i = 0; i < NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);

	printf("result: %d \n", num);
	return 0;
}

void *thread_inc(void* arg) {
	int i;
	for (i = 0; i < 50000000; i++)
		num += 1;

	return NULL;
}
// num이라는 변수가 전역변수이며 공유하는 변수

void *thread_des(void* arg) {
	int i;
	for (i = 0; i < 50000000; i++)
		num += 1;
	return NULL;
}
