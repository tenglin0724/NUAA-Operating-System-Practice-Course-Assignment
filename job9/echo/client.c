#include "std.h"
#include "tcp.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        puts("Usage: client msg\n");
        return 0;
    }
    int client_fd = tcp_connect("127.0.0.1", 1234);

    char *msg = argv[1];
    write(client_fd, msg, strlen(msg));
    printf("client send: %s\n", msg);

    char buff[128];
    int count = read(client_fd, buff, sizeof(buff));
    buff[count] = 0;
    printf("client receive: %s\n", buff);
    return 0;
}
