#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // create a new process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // child process
        printf("Hello from the child process\n");
        exit(0);
    } else {
        // parent process
        int status;
        waitpid(pid, &status, 0);
        printf("Child process %d exited with status %d\n", pid, WEXITSTATUS(status));
    }

    return 0;
}