#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>

#define SHELL "/home/pb12/Desktop/Operating Systems/OSTutorial7/process"


int main(){

    int status;
    pid_t childPid;

    //Create child process
    childPid = fork();
    printf("1) Process ID for childPid is: %d\n",getpid());
    
    //Creating child process failed
    if(childPid < 0){
        perror("Failed to create child process");
        return 1;
    }
    //Child process created successfully 
    if(childPid == 0){
        printf("2) You are child process: %d\n", getpid());
        execl(SHELL, SHELL, (char *) NULL);
    }
    //Parent process where it sleeps for 5 and then sends interrupt signal
    else{
        printf("3) You are parent: %d\n", getpid());
        //Sleep for 5 seconds
        sleep(5);
        //Sends interrupt signal
        kill(childPid, SIGINT);
    }

    return 0;
}
