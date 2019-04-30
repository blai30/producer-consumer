# CSC 415 - Project 5 - Producer Consumer - Part 2

## Student Name : Brian Lai

## Student ID : 916818167

## Build Instructions
### Linux
```
make
```

## Run Instructions
### Linux
```
./pandc <N> <P> <C> <X> <Ptime> <Ctime>
```
* N = number of buffers to maintain
* P = number of producer threads
* C = number of consumer threads
* X = number of items each producer thread will produce
* Ptime = how long each producer thread will sleep after producing an item in seconds
* Ctime = how long each consumer thread will sleep after consuming an item in seconds

Example:
```
./pandc 7 7 9 15 1 1
```
