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
    fp = fopen("processes_q5.txt","r");
    queue = NULL;
    queue = malloc(sizeof(que));
    queue->next = NULL;

    size_t len = 0;
    ssize_t read;

    char *line = NULL;

    // Read lines from the file, but i commented out hte pid read portion
    while((read = getline(&line, &len, fp)) != -1){
        proc p;
        char * tok = NULL;
        tok =  strtok(line,",");
        strcpy(p.name, tok);
        tok = strtok(NULL,",");
        p.priority = atoi(tok);
        /*
        tok = strtok(NULL,",");
        p.pid = atoi(tok);
        */
        p.pid = 0;
        tok =strtok(NULL,",");
        p.runtime = atoi(tok);
        push(p);
    }

    // new instance of current for this queue
    que *current = queue;
    current = current->next; // set current to point past sentinal
    while(current!=NULL){ // while there are still nodes
        proc p = current->process; // p is current node
        if(p.priority==0){ // if p.priority is 0
            const char *com = p.name; // char to store name
            int status; // status for sigint
            pid_t pid; // set the pid
            pid = fork(); // fork process
            if(pid<0){ // if fork failed
                printf("Fork Failed");
                status = -1;
            }
            else if(pid == 0){ // if fork was successful
                exec(com); // execute the function name
                exit(0); // exit and return to parent
            }
            else{ // if parent
                sleep(p.runtime); // sleep for p's runtime
                kill(pid, SIGINT); // kill the process
                waitpid(pid, &status,0); // wait for the child to terminate
                if(status==0){ // if terminated normally
                    p.pid = pid; // set pid to the pid created
                    // print process information
                     printf("Name: %s Priority: %d Pid: %d RunTime: %d\n",
                            p.name,p.priority,p.pid,p.runtime);
                    current=current->next; // point to next node
                }
            }
        }

        // Method here is same as above but added pop statement at bottom
        // to remove processes from the queue
        else{
            const char *com = p.name;
            int status;
            pid_t pid;
            pid = fork();
            if(pid<0){
                printf("Fork Failed");
                status = -1;
            }
            else if(pid == 0){
                exec(com);
                delete_name(p.name);
                exit(0);
            }
            else{
                sleep(p.runtime);
                kill(pid, SIGINT);
                waitpid(pid, &status,0);
                if(status==0){
                    p.pid = pid;
                     printf("Name: %s Priority: %d Pid: %d RunTime: %d\n",
                            p.name,p.priority,p.pid,p.runtime);
                    current=current->next;
                    pop();
                }
            }

        }




    }
    fclose(fp); // close fp
    free(line); // free memory
    return 0;
}
    