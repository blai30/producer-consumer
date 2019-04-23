#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define COL_RED     "\x1B[31m"
#define COL_GRN     "\x1B[32m"
#define COL_YEL     "\x1B[33m"
#define COL_BLU     "\x1B[34m"
#define COL_MAG     "\x1B[35m"
#define COL_CYN     "\x1B[36m"
#define COL_WHT     "\x1B[37m"
#define COL_RESET   "\x1B[0m"

pthread_mutex_t lock;
sem_t full;
sem_t empty;

int global_value = 1;

int* buffer;
int in_index;
int out_index;

int* producer_arr;
int* consumer_arr;

int num_buffers;
int num_producers;
int num_consumers;
int items_produced;
int items_consumed;
int over_consume;
int over_consume_amount;
int p_time;
int c_time;

/*
 * Function to remove item.
 * Item removed is returned
 */
int dequeue_item() {
    int item = buffer[out_index];
    out_index = (out_index + 1) % num_buffers;
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
    buffer[in_index] = item;
    in_index = (in_index + 1) % num_buffers;
    return item;
}

/**
 * Produces item in buffer.
 * @param arg
 * @return
 */
void* producer(void* arg) {
    int tid = *((int*) arg);
    int item;

    for (int i = 0; i < items_produced; i++) {
        sleep(p_time);
        item = global_value++;

        sem_wait(&empty);
        pthread_mutex_lock(&lock);

        enqueue_item(item);
        producer_arr[i] = item;
        printf(COL_GRN "%5d was produced by producer->\t%5d\n" COL_RESET, item, tid);

        pthread_mutex_unlock(&lock);
        sem_post(&full);
    }

    pthread_exit(0);
}

/**
 * Consumes item in buffer.
 * @param arg
 * @return
 */
void* consumer(void* arg) {
    int tid = *((int*) arg);
    int item;

    for (int i = 0; i < items_consumed; i++) {
        sleep(c_time);
        sem_wait(&full);
        pthread_mutex_lock(&lock);

        item = dequeue_item();
        consumer_arr[i] = item;
        printf(COL_RED "%5d was consumed by consumer->\t%5d\n" COL_RESET, item, tid);

        pthread_mutex_unlock(&lock);
        sem_post(&empty);
    }

    pthread_exit(0);
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
        printf("Enter 6 arguments: \"$ ./pandc <N> <P> <C> <X> <Ptime> <Ctime>\"\n");
        exit(EXIT_FAILURE);
    }

    // Print current time
    time_t start_time = time(0);
    printf(COL_YEL "Current time: %s\n" COL_RESET, ctime(&start_time));

    // Read command-line args
    num_buffers         = strtol(argv[1], NULL, 10);
    num_producers       = strtol(argv[2], NULL, 10);
    num_consumers       = strtol(argv[3], NULL, 10);
    items_produced      = strtol(argv[4], NULL, 10);
    items_consumed      = (num_producers * items_produced) / num_consumers;
    over_consume        = ((num_producers * items_produced) % num_consumers > 0) ? 1 : 0;
    over_consume_amount = (num_producers * items_produced) - (num_consumers * items_consumed);
    p_time              = strtol(argv[5], NULL, 10);
    c_time              = strtol(argv[6], NULL, 10);

    // Print producer-consumer problem information
    printf("\t                        Number of Buffers : %5d\n", num_buffers);
    printf("\t                      Number of Producers : %5d\n", num_producers);
    printf("\t                      Number of Consumers : %5d\n", num_consumers);
    printf("\tNumber of items Produced by each producer : %5d\n", items_produced);
    printf("\tNumber of items Consumed by each consumer : %5d\n", items_consumed);
    printf("\t                         Over consume on? : %5s\n", (over_consume) ? "yes" : "no");
    printf("\t                      Over consume amount : %5d\n", over_consume_amount);
    printf("\t      Time each Producer Sleeps (seconds) : %5d\n", p_time);
    printf("\t      Time each Consumer Sleeps (seconds) : %5d\n", c_time);
    printf("\n");

    // Initialize mutex, semaphore, buffer, arrays
    pthread_mutex_init(&lock, NULL);    // mutex lock = 1;
    sem_init(&full, 0, 0);              // semaphore full = 0;
    sem_init(&empty, 0, num_buffers);   // semaphore empty = N;
    buffer = malloc(sizeof(int*) * num_buffers);       // buffer[N];
    producer_arr = malloc(sizeof(int*) * num_producers * items_produced);
    consumer_arr = malloc(sizeof(int*) * num_consumers * items_consumed);
    in_index = 0;
    out_index = 0;

    // New threads
    pthread_t producer_ids[num_producers];
    pthread_t consumer_ids[num_consumers];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Create producer and consumer threads
    for (int i = 0; i < num_producers; i++) {
        int id = i + 1;
        pthread_create(&producer_ids[i], &attr, &producer, (void*) &id);
    }
    for (int i = 0; i < num_consumers; i++) {
        int id = i + 1;
        pthread_create(&consumer_ids[i], &attr, &consumer, (void*) &id);
    }

    // Join producer and consumer threads
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producer_ids[i], NULL);
        printf(COL_CYN "Producer thread joined:%5d\n" COL_RESET, i);
    }
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumer_ids[i], NULL);
        printf(COL_MAG "Consumer thread joined:%5d\n" COL_RESET, i);
    }

    time_t end_time = time(0);
    printf(COL_YEL "Current time: %s\n" COL_RESET, ctime(&end_time));

    // Compare and match producer and consumer arrays
    int match = 1;
    printf("Producer Array\t| Consumer Array\n");
    for (int i = 0; i < num_producers * items_produced; i++) {
        printf("%d\t\t\t\t| %d\n", producer_arr[i], consumer_arr[i]);
        if (producer_arr[i] != consumer_arr[i]) {
            match = 0;
        }
    }

    printf("\nConsume and Produce Arrays %s!\n", (match) ? "Match" : "DO NOT Match");
    printf("\nTotal Runtime: %d secs\n", (int) (end_time - start_time));

    // Cleanup
    pthread_mutex_destroy(&lock);
    sem_destroy(&full);
    sem_destroy(&empty);
    free(buffer);

    return 0;
}
