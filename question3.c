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
    printf("1) Process ID for childPid is: %d\n",getpid());
    if(childPid < 0){
        perror("Failed to create child process");
        return 1;
    }
    if(childPid == 0){
        printf("2) You are child process: %d\n", getpid());
        execl(SHELL, SHELL, (char *) NULL);
    }
    else{
        printf("3) You are parent: %d\n", getpid());
        sleep(5);
        kill(childPid, SIGINT);
    }

    return 0;
}
