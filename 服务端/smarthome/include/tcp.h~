#ifndef __TCP_H__
#define __TCP_H__

int tcp_server_init(const char *ip, const char *port);
int tcp_server_wait_connect(int listenfd);
ssize_t tcp_server_recv(int connfd, void *buf, size_t count);
ssize_t tcp_server_send(int connfd, const void *buf, size_t count);
ssize_t tcp_server_recv_exact_nbytes(int connfd, void *buf, size_t count);
ssize_t tcp_server_send_exact_nbytes(int connfd, const void *buf, size_t count);
int tcp_server_disconnect(int connfd);
int tcp_server_exit(int listenfd);

int tcp_client_init(void);
int tcp_client_connect(int sockfd, const char *ip, const char *port);
ssize_t tcp_client_recv(int sockfd, void *buf, size_t count);
ssize_t tcp_client_send(int sockfd, const void *buf, size_t count);
ssize_t tcp_client_recv_exact_nbytes(int sockfd, void *buf, size_t count);
ssize_t tcp_client_send_exact_nbytes(int sockfd, const void *buf, size_t count);
int tcp_client_exit(int sockfd);

#endif
