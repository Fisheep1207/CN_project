int server_fd = socket(domain, type, protocol);
int bind(int socket, const struct sockaddr *address, socklen_t address_len);
int listen(int socket, int backlog);
int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);