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
proc curProc;

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

proc *pop(){
    
    que *next_que = NULL; // initalize and empty position for the queue

    if(queue->next == NULL){ // if the head is NULL (aka list is empty)
        printf("Unable to pop, queue is empty"); 
        return NULL; // return a NULL queue
    }

    next_que = queue->next->next; // the next node
    que *curr = queue->next; // the current node
    curProc = curr->process; // get the process from the node

    free(curr); // free the memory
    queue->next = next_que; // set the head to the next node
    return &curProc; // return the process
}

proc *delete_name(char *name){
    
    que *nextNode = queue->next; // get the second node in the list
    que *thisNode = queue; // first node in the list as a place holder
    while(nextNode != NULL){ // while there are still elements in the queue
        curProc = nextNode->process; // the current process is the process being examined
        if(strcmp(curProc.name,name)==0){ // if the process.name is the same as argument name
            que *delNode = nextNode; // that node is now the del node
            thisNode->next = nextNode->next; // make the node point to the node after
            free(delNode); // free memory of deleted node
            return &curProc; // return the node
        }
        thisNode = nextNode; // make current node the next node
        nextNode = nextNode->next; // make next node the node after
    }
    printf("Specified name could not be found"); // if not found return null
    return NULL;
}

proc *delete_pid(int pid){

    que *nextNode = queue->next; // get second node in list
    que *thisNode = queue; // get first node in list
    while(nextNode != NULL){ // while the next node is existant
        curProc = nextNode->process; // the examined process is the next node's process
        if(curProc.pid == pid){ // if the examined process is the argument PID
            que *delNode = nextNode; // the next node will be deleted
            thisNode->next = nextNode->next; // make this node point to the node after the deleted node
            free(delNode); // free memory
            return &curProc; // return process
        }
        thisNode = nextNode; // this node is now the next node
        nextNode = nextNode->next; // next node is now the node after
    }
    printf("Specified pid could not be found"); // if it not found 
    return NULL; // return null
}

// Function to print out the queue
void print(){ 
    que *current = queue;
    current = current->next;
    while(current!=NULL){
        proc p = current->process;
        printf("Name: %s Priority: %d Pid: %d RunTime: %d\n",
        p.name,p.priority,p.pid,p.runtime);
        current =current->next;
    }
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
    
    // Print the queue before the required actions
    printf("Queue before actions\n");
    print();

   // Actions
    delete_name("emacs");
    delete_pid(12235);

    while(queue->next != NULL){
        pop();
    }

    // Print out the queue after the actions
    printf("Queue after actions.\n");
    print();
    free(line); // free memory
    return 0;
}