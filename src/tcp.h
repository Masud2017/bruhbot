#pragma once

/******************************************************/
/* includes */
/******************************************************/
#include <stdint.h>

/******************************************************/
/* structs */
/******************************************************/
struct tcp_sock {
	int fd;
};

/******************************************************/
/* defines */
/******************************************************/
#define BRUH_TCP_OK 0
#define BRUH_TCP_BAD_PARAM 1
#define BRUH_TCP_SOCKET_ERROR 2
#define BRUH_TCP_CONNECT_ERROR 3

#define BRUH_TCP_SEND_ERROR 4
#define BRUH_TCP_RECV_NONE 5
#define BRUH_TCP_RECV_ERROR 6

/******************************************************/
/* functions */
/******************************************************/
int tcp_create_sock(struct tcp_sock *sock);

int tcp_close_sock(struct tcp_sock *sock);

int tcp_connect(struct tcp_sock *sock, const char *hostname, uint16_t port);

int tcp_send(struct tcp_sock *sock, char *buffer, int size);

int tcp_recv(struct tcp_sock *sock, char *buffer, int *size, 
		int buffer_capacity);

int tcp_recv_wait(struct tcp_sock *sock, char *buffer, int *size, 
		int buffer_capacity, unsigned int timeout_ms);
