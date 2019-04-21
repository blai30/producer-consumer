#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

pthread_mutex_t lock;
sem_t full;
sem_t empty;

int* buffer;
int buffer_index;

/* 
 * Function to remove item.
 * Item removed is returned
 */
int dequeue_item() {
    int item = buffer[buffer_index];
    buffer[buffer_index] = 0;
    return item;
}

/* 
 * Function to add item.
 * Item added is returned.
 * It is up to you to determine
 * how to use the ruturn value.
 * If you decide to not use it, then ignore
 * the return value, do not change the
 * return type to void. 
 */
int enqueue_item(int item) {
    buffer[buffer_index] = item;
    return item;
}

void* producer(void* arg) {
    int tid = *((int*) arg);
}

void* consumer(void* arg) {
    int tid = *((int*) arg);
}

/*
 * $ ./pandc <N> <P> <C> <X> <Ptime> <Ctime>
 *      N = number of buffers to maintain
 *      P = number of producer threads
 *      C = number of consumer threads
 *      X = number of items each producer thread will produce
 *  Ptime = how long each producer thread will sleep after producing an item in seconds
 *  Ctime = how long each consumer thread will sleep after consuming an item in seconds
 */
int main(int argc, char** argv) {
    // argv[0] is the program itself ("./pandc")
    if (argc != 7) {
        perror("Enter 6 arguments: \"$ ./pandc <N> <P> <C> <X> <Ptime> <Ctime>\"\n");
        return 0;
    }

    // Print current time
    time_t cur_time = time(0);
    printf("Current time: %s\n", ctime(&cur_time));

    // Read command-line args
    int num_buffers         = strtol(argv[1], NULL, 10);
    int num_producers       = strtol(argv[2], NULL, 10);
    int num_consumers       = strtol(argv[3], NULL, 10);
    int items_produced      = strtol(argv[4], NULL, 10);
    int items_consumed      = (num_producers * items_produced) / num_consumers;
    int over_consume        = ((num_producers * items_produced) % num_consumers > 0) ? 1 : 0;
    int over_consume_amount = (num_producers * items_produced) - (num_consumers * items_consumed);
    int p_time              = strtol(argv[5], NULL, 10);
    int c_time              = strtol(argv[6], NULL, 10);

    struct timespec Ptime = {p_time};
    struct timespec Ctime = {c_time};

    // Print producer-consumer problem information
    printf("\t                        Number of Buffers : %6d\n", num_buffers);
    printf("\t                      Number of Producers : %6d\n", num_producers);
    printf("\t                      Number of Consumers : %6d\n", num_consumers);
    printf("\tNumber of items Produced by each producer : %6d\n", items_produced);
    printf("\tNumber of items Consumed by each consumer : %6d\n", items_consumed);
    printf("\t                         Over consume on? : %6s\n", (over_consume) ? "yes" : "no");
    printf("\t                      Over consume amount : %6d\n", over_consume_amount);
    printf("\t      Time each Producer Sleeps (seconds) : %6d\n", p_time);
    printf("\t      Time each Consumer Sleeps (seconds) : %6d\n", c_time);

    // Initialize mutex, semaphore, buffer
    pthread_mutex_init(&lock, NULL);    // mutex lock = 1;
    sem_init(&full, 0, 0);              // semaphore full = 0;
    sem_init(&empty, 0, num_buffers);   // semaphore empty = N;
    buffer = malloc(num_buffers);       // buffer[N];

    return 0;
}
