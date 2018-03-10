#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct{
    char name[256];
    int priority;
    int pid;
    int runtime;
}proc;

typedef struct{
    proc process;
    struct queue *next;
}queue;

void push(proc process){
    
}

void iterate(queue q){

}
int main(void){

}