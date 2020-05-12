/******************************************************/
/* includes */
/******************************************************/
#include "irc.h"
#include "on_err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************/
/* defines */
/******************************************************/
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/******************************************************/
/* functions */
/******************************************************/

/******************************************************/
int irc_connect(struct tcp_sock *sock, const char *hostname, uint16_t port) 
{
	/* connect to the IRC server */
	int error = tcp_create_sock(sock);
	on_err(error, "tcp_create_sock() failed");

	error = tcp_connect(sock, hostname, port);
	on_err(error, "tcp_create_sock() failed");

	return error;
}

/******************************************************/
int irc_send(struct tcp_sock *sock, char buffer[IRC_LINE_MAXLEN])
{
	int err;
	size_t len;

	// just to be safe
	buffer[IRC_LINE_MAXLEN - 3] = '\r';
	buffer[IRC_LINE_MAXLEN - 2] = '\n';
	buffer[IRC_LINE_MAXLEN - 1] = '\0';

	len = strnlen((const char*)buffer, (size_t)(IRC_LINE_MAXLEN - 1));
	
	err = tcp_send(sock, buffer, strlen(buffer));
	on_err(err, "tcp_send() failed");
}

/******************************************************/
int read_oauth_file(const char *filename, char oauth[OAUTH_MAXLEN])
{
	FILE *fp;
	int filesize;
	size_t result;
	char *line;

	/* open the file */
	fp = fopen(filename, "r");

	if (!fp) {
		oauth[0] = '\0';
		return ERR_FILE_NOT_FOUND;
	}

	/* find the filesize */
	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp);

	if (filesize >= OAUTH_MAXLEN) {
		oauth[0] = '\0';
		fclose(fp);
		return ERR_INVALID_OAUTH_FILE;
	}

	/* lazy assumption- entire oauth string can be read in one call */
	fseek(fp, 0L, SEEK_SET);
	line = fgets(oauth, filesize, fp);
	
	/* force a null terminator */
	oauth[filesize] = '\0';

	fclose(fp);

	return ERR_OK;
}

/******************************************************/
int irc_auth(struct tcp_sock *sock, const char *nick, const char *oauth)
{
	char buffer[IRC_LINE_MAXLEN];

	/* oauth authentication */
	snprintf(buffer, IRC_LINE_MAXLEN, "PASS %s\r\n", oauth);
	irc_send(sock, buffer);

	/* nick */
	snprintf(buffer, IRC_LINE_MAXLEN, "NICK %s\r\n", nick);
	irc_send(sock, buffer);
}

/******************************************************/
int irc_join(struct tcp_sock *sock, const char *channel)
{
	char buffer[IRC_LINE_MAXLEN];

	snprintf(buffer, IRC_LINE_MAXLEN, "JOIN #%s\r\n", channel);
	irc_send(sock, buffer);
}

/******************************************************/
int irc_readline(struct tcp_sock *sock, char buffer[IRC_LINE_MAXLEN])
{
	int err;
	int size;
	int strsize;
	err = tcp_recv_wait(sock, buffer, &size, IRC_LINE_MAXLEN, 
			IRC_RXPOLL_INTERVAL_MS);

	if (err == BRUH_TCP_RECV_NONE)
		return ERR_TIMEOUT;

	if (err != BRUH_TCP_OK)
		return ERR_CONNECTION;

	/* strip off /r/n */
	strsize = MIN(size, IRC_LINE_MAXLEN - 1);	
	buffer[strsize] = '\0';

	return ERR_OK;
}

