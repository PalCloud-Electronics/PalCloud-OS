#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
    // create a pipe
    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }

    // fork a child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // child process

        // close the read end of the pipe
        close(fd[0]);

        // write to the pipe
        char buffer[BUFFER_SIZE];
        sprintf(buffer, "Hello from the child process\n");
        if (write(fd[1], buffer, strlen(buffer)) < 0) {
            perror("write");
            exit(1);
        }

        // close the write end of the pipe
        close(fd[1]);

    } else {
        // parent process

        // close the write end of the pipe
        close(fd[1]);

        // read from the pipe
        char buffer[BUFFER_SIZE];
        if (read(fd[0], buffer, BUFFER_SIZE) < 0) {
            perror("read");
            exit(1);
        }

        // print the message from the child process
        printf("%s", buffer);

        // close the read end of the pipe
        close(fd[0]);

    }

    return 0;
}