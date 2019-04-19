#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
/**
 * THESE DEFINE VALUES CANNOT BE CHANGED.
 * DOING SO WILL CAUSE POINTS TO BE DEDUCTED
 * FROM YOUR GRADE
 */
/** BEGIN VALUES THAT CANNOT BE CHANGED */
#define MAX_THREADS 36
#define MAX_ITERATIONS 22
/** END VALUES THAT CANNOT BE CHANGED */


/**
 * use this struct as a parameter for the
 * nanosleep function.
 * For exmaple : nanosleep(&ts, NULL);
 */
struct timespec ts = {0, 3000000};   // default: {0, 150000}

int global_value = 0;

void* add_thread_func(void* arg) {
    int tid = *((int*) arg);
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        int temp = global_value;
        temp += 10;
        global_value = temp;

        printf("Current Value written to Global Variables by ADDER thread id: %d is %d\n", tid, temp);
        nanosleep(&ts, NULL);
    }
}

void* sub_thread_func(void* arg) {
    int tid = *((int *) arg);
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        int temp = global_value;
        nanosleep(&ts, NULL);
        temp -= 10;
        global_value = temp;

        printf("Current Value written to Global Variables by SUBTRACTOR thread id: %d is %d\n", tid, temp);
        nanosleep(&ts, NULL);
    }
}

int main(int argc, char** argv) {
    pthread_t ids[MAX_THREADS];

    int i;
    for (i = 0; i < MAX_THREADS; i += 2) {
        int idx = i;
        pthread_create(&ids[i], NULL, add_thread_func, (void*) &idx);
    }

    for (i = 1; i < MAX_THREADS; i += 2) {
        int idx = i;
        pthread_create(&ids[i], NULL, sub_thread_func, (void*) &idx);
    }

    for (i = 0; i < MAX_THREADS; i++) {
        pthread_join(ids[i], NULL);
    }

    fprintf(stderr, "Final Value of Shared Variable : %4d\n", global_value);

    return 0;
}
