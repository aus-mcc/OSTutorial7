#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct proc{
    char name[256];
    int priority;
    int pid;
    int runtime;
}proc;

typedef struct queue{
    proc process;
    struct queue *next;
}queue;

void push(proc process){
    queue *n = malloc(sizeof(queue));
    while(n->next!=NULL){
        n=n->next;
    }

    n->next = malloc(sizeof(queue));
    n->next->process = process;
    n->next->next=NULL;

    /*
    n->process.name[256] = process.name[256];
    n->process.priority = process.priority;
    n->process.pid = process.pid;
    n->process.runtime = process.runtime;
    */
    
}

void iterate(queue *q){
    queue * current = q;
    while(current!=NULL){
        printf("%d/n", current->process);
    }
}
int main(void){
    FILE *fp;
    fp = fopen("processes.txt","r");
    queue *queue = malloc(sizeof(queue));
    while(!feof(fp)){
        struct proc *p = (struct proc*)malloc(sizeof(struct proc));
        fscanf(fp, "%s%*c %d%*c %d%*c %d%*c",&p->name, &p->priority,&p->pid,&p->runtime);
        push(*p);
    }
    iterate(queue);
}