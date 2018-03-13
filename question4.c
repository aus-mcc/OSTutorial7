#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <stdlib.h>

#define SHELL "/home/pb12/Desktop/Operating Systems/OSTutorial7/process"


int main(){

    int status;
    pid_t childPid;

    childPid = fork();

    if(childPid < 0){
        perror("Failed to create child process");
        return 1;
    }
    if(childPid == 0){
        printf("You are child process: %d\n", getpid());
        execv(SHELL,(char *) NULL);
        exit(0);
    }
    else{
        printf("You are parent: %d\n", getpid());
        sleep(5);
        kill(childPid, SIGTSTP);
        sleep(10);
        kill(childPid, SIGCONT);
        waitpid(childPid);
        exit(0);
    }

    return 0;
}

//smmpd