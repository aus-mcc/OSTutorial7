#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct{
    char name[256];
    int priority;
    int pid;
    int runtime;
}proc;

typedef struct queue{
    proc process;
    struct queue *next;
}que;

que *queue;

void push(proc process){
    que *n = queue;
    while(n->next!=NULL){
        n=n->next;
    }

    n->next = (que*)malloc(sizeof(que));
    n->next->process = process;
    n->next->next=NULL;

    /*
    n->process.name[256] = process.name[256];
    n->process.priority = process.priority;
    n->process.pid = process.pid;
    n->process.runtime = process.runtime;
    */
}    

int main(void){
    FILE *fp;
    fp = fopen("processes.txt","r");
    queue = NULL;
    queue = malloc(sizeof(que));
    queue->next = NULL;

    size_t len = 0;
    ssize_t read;

    char *line = NULL;

    while((read = getline(&line, &len, fp)) != -1){
        proc p;
        char * tok = NULL;
        tok =  strtok(line,",");
        strcpy(p.name, tok);
        tok = strtok(NULL,",");
        p.priority = atoi(tok);
        tok = strtok(NULL,",");
        p.pid = atoi(tok);
        tok =strtok(NULL,",");
        p.runtime = atoi(tok);
        push(p);
    }
    fclose(fp);
    
    que *current = queue;
    current = current->next;
    while(current!=NULL){
        proc p = current->process;
        printf("Name: %s Priority: %d Pid: %d RunTime: %d\n",
        p.name,p.priority,p.pid,p.runtime);
        current =current->next;
    }
    free(line);
    return 0;
}