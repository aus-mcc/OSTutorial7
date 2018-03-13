#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>

#define SHELL "/home/pb12/Desktop/Operating Systems/OSTutorial7/process"


int main(int argc, char *argv){

    int status;
    pid_t childPid;

    childPid = fork();

    if(childPid < 0){
        perror("Failed to create child process");
        return 1;
    }
    if(childPid == 0){
        printf("You are child process: %d\n", getpid());
        execl(SHELL, SHELL, (char *) NULL);
        kill(childPid, SIGKILL);
        sleep(5);
        kill(childPid, SIGINT);
        kill(childPid);
    }
    else{
        printf("You are parent: %d\n", getpid());
    }

    return 0;
}
