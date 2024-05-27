# 1. 在后台启动 server
$ ./server &

# 2. 运行 client
$ ./client hello

# 3. 把后台程序置换到前台
$ fg

# 4. 把前台程序置换到后台
ctrl-Z

int socket(int domain, int type, int protocol);
int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);

int connect(int sockfd, struct sockaddr *addr, socklen_t addrlen);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int send(int sockfd, void *buf, size_t len, int flags);
int write(int fd, void *buf, size_t len);

int recv(int sockfd, void *buf, size_t len, int flags);
int read(int fd, void *buf, size_t len);

int close(int fd);
