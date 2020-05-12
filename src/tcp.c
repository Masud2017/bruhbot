/******************************************************/
/* includes */
/******************************************************/
#include "tcp.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

/******************************************************/
/* functions */
/******************************************************/

/******************************************************/
int tcp_create_sock(struct tcp_sock *sock)
{
	sock->fd = socket(AF_INET, SOCK_STREAM, 0);

	if (sock->fd == -1)
		return BRUH_TCP_SOCKET_ERROR;

	return BRUH_TCP_OK;
}

/******************************************************/
int tcp_close_sock(struct tcp_sock *sock)
{
	if (close(sock->fd) == -1)
		return BRUH_TCP_SOCKET_ERROR;

	return BRUH_TCP_OK;
}

/******************************************************/
int tcp_connect(struct tcp_sock *sock, const char *hostname, uint16_t port)
{
	struct sockaddr_in addr;
	struct hostent *host;

	if (!sock)
		return BRUH_TCP_BAD_PARAM;
	if (!hostname)
		return BRUH_TCP_BAD_PARAM;
	if (hostname[0] == '\0')
		return BRUH_TCP_BAD_PARAM;
	if (sock->fd < 0)
		return BRUH_TCP_SOCKET_ERROR;

	host = gethostbyname(hostname);
	
	addr.sin_addr.s_addr = *(unsigned long *)host->h_addr_list[0];
	addr.sin_family = AF_INET;
	addr.sin_port = htons((unsigned short)port);

	if (connect(sock->fd, (struct sockaddr *)&addr, sizeof(addr)))
		return BRUH_TCP_CONNECT_ERROR;

	return BRUH_TCP_OK;
}

/******************************************************/
int tcp_send(struct tcp_sock *sock, char *buffer, int size)
{
	if (!sock)
		return BRUH_TCP_BAD_PARAM;

	if (sock->fd == -1)
		return BRUH_TCP_SOCKET_ERROR;

	if (write(sock->fd, buffer, size) < 0)
		return BRUH_TCP_SEND_ERROR;

	return BRUH_TCP_OK;
}

/******************************************************/
int tcp_recv(struct tcp_sock *sock, char *buffer, int *size, 
		int buffer_capacity)
{
	ssize_t rxsize;
	int errcode;

	if(!size)
		return BRUH_TCP_BAD_PARAM;

	if (!sock)
		return BRUH_TCP_BAD_PARAM;

	if (sock->fd == -1)
		return BRUH_TCP_SOCKET_ERROR;

	if (!buffer)
		return BRUH_TCP_BAD_PARAM;

	if (buffer_capacity == 0)  
		return BRUH_TCP_BAD_PARAM;

	rxsize = read(sock->fd, buffer, buffer_capacity);
	errcode = errno;

	if (rxsize < 0 && errcode == EAGAIN)
		return BRUH_TCP_RECV_NONE;

	if (rxsize < 0 && errcode == EWOULDBLOCK)
		return BRUH_TCP_RECV_NONE;

	if(rxsize < 0)
		return BRUH_TCP_RECV_ERROR;

	*size = (int)rxsize;

	return BRUH_TCP_OK;
}

/******************************************************/
int tcp_recv_wait(struct tcp_sock *sock, char *buffer, int *size, 
		int buffer_capacity, unsigned int timeout_ms)
{
	fd_set rset;
	struct timeval timeout;
	int ready;

	if (!sock)
		return BRUH_TCP_BAD_PARAM;

	if (sock->fd == -1)
		return BRUH_TCP_SOCKET_ERROR;

	if (!buffer)
		return BRUH_TCP_BAD_PARAM;

	if (buffer_capacity == 0)  
		return BRUH_TCP_BAD_PARAM;

	FD_ZERO(&rset);

	FD_SET(sock->fd, &rset);
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000 * timeout_ms;

	ready = select(FD_SETSIZE, &rset, NULL, NULL, &timeout);

	if (ready == 0) 
		return BRUH_TCP_RECV_NONE;

	return tcp_recv(sock, buffer, size, buffer_capacity);
}
