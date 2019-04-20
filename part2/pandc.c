#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>


/* 
 * Function to remove item.
 * Item removed is returned
 */
int dequeue_item() {
    
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

}
