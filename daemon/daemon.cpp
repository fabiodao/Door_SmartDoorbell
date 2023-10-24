
#include "CdSensors.h"
#include <stdio.h>
#include <mqueue.h>   /* mq_* functions */
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    pid_t pid, sid;
    pid_t my_pid;
    
    time_t currtime;

    pid = fork(); // create a new process
    
    if (pid < 0) { // on error exit
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) // parent process (exit)
        exit(EXIT_SUCCESS);
    
    sid = setsid(); // create a new session
    
    if (sid < 0) { // on error exit
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    
    if (chdir("/") < 0) { // on error exit
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    
    umask(0); // make '/' the root directory
    
    close(STDIN_FILENO); // close standard input file descriptor
    close(STDOUT_FILENO); // close standard output file descriptor
    close(STDERR_FILENO); // close standard error file descriptor


	CdSensors sensors;

	sensors.run();

	return 0;

    return 0;
}