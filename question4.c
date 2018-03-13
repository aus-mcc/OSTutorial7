#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <stdlib.h>

//Path for process
#define SHELL "/home/pb12/Desktop/Operating Systems/OSTutorial7/process"


int main(){

    int status;
    pid_t childPid;

    //Creates child process
    childPid = fork();

    //Failed to fork
    if(childPid < 0){
        perror("Failed to create child process");
        return 1;
    }
    //Child process created and execute process using path above
    if(childPid == 0){
        printf("You are child process: %d\n", getpid());
        //Executes the process file
        execv(SHELL,(char *) NULL);
        exit(0);
    }
    else{
        printf("You are parent: %d\n", getpid());
        //Sleeps for 5 seconds
        sleep(5);
        //Sends the stop signal
        kill(childPid, SIGTSTP);
        //Sleep for 10 seconds
        sleep(10);
        //Sends the continuation signal
        kill(childPid, SIGCONT);
        //Waits for child process to terminate
        waitpid(childPid);
        exit(0);
    }

    return 0;
}
