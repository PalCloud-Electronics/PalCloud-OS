#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
    // install the package
    if (system("apt-get install package-name") != 0) {
        perror("apt-get install");
        exit(1);
    }

    // configure the package
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "echo 'option1 = value1\noption2 = value2' > /etc/package-name.conf");
    if (system(buffer) != 0) {
        perror("echo");
        exit(1);
    }

    // restart the service
    if (system("systemctl restart package-name.service") != 0) {
        perror("systemctl restart");
        exit(1);
    }

    return 0;
}