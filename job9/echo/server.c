#include "std.h"
#include "tcp.h"

void echo_handler(int fd)
{
    char buff[128];
    int count = read(fd, buff, sizeof(buff));
    buff[count] = 0;
    printf("server received: %s\n", buff);

    int i;
    for (i = 0; i < count; i++)
        buff[i] = toupper(buff[i]);
    write(fd, buff, count);
}

void *echo_entry(void *arg)
{
    int fd = (long) arg;
    echo_handler(fd);
    close(fd);
    return NULL;
}

void run_echo_server(char *ip_addr, int port)
{
    int server_fd = tcp_listen(ip_addr, port);

    while (1) {
        int client_fd = tcp_accept(server_fd);
        if (client_fd < 0)
            fatal("accept");
        printf("accept client\n");

#if 0
        pid_t pid = fork();
        if (pid == 0) {
            close(server_fd);
            echo_handler(client_fd);
            close(client_fd);
            exit(0);
        }
        close(client_fd);
#else
        pthread_t echo_thread;
        pthread_create(&echo_thread, NULL, echo_entry, (void *)(long)client_fd);
#endif
    }
    close(server_fd);
}

int main(int argc, char *argv[])
{
    char *ip_addr = "127.0.0.1";
    int port = 1234;
    run_echo_server(ip_addr, port);
    return 0;
}
