#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void exec_app()
{    
    execlp("/bin/cat", "cat", "/etc/passwd", NULL);
}

void read_app(int fd)
{
    while (1) {
        char buff[10000];
        int n;
        n  = read(fd, buff, sizeof(buff));
        if (n == 0)
            break;
        write(1, buff, n);
    }
}

int main()
{
    int fds[2];
    pipe(fds);
    pid_t pid = fork();

    if (pid == 0) {
        dup2(fds[1], 1);
        close(fds[0]);
        close(fds[1]);
        exec_app();
        exit(0);
    }

    close(fds[1]);
    read_app(fds[0]);
    close(fds[0]);
    return 0;
}

