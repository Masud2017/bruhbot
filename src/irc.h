#pragma once

/******************************************************/
/* includes */
/******************************************************/
#include "tcp.h"
#include <stdint.h>

/******************************************************/
/* defines */
/******************************************************/
#define IRC_LINE_MAXLEN 512
#define IRC_RXPOLL_INTERVAL_MS 100
#define OAUTH_MAXLEN 128

/******************************************************/
/* functions */
/******************************************************/
int irc_connect(struct tcp_sock *sock, const char *hostname, uint16_t port);
int irc_send(struct tcp_sock *sock, char buffer[IRC_LINE_MAXLEN]);
int read_oauth_file(const char *filename, char oauth[OAUTH_MAXLEN]);
int irc_auth(struct tcp_sock *sock, const char *nick, const char *oauth);
int irc_join(struct tcp_sock *sock, const char *channel);
int irc_readline(struct tcp_sock *sock, char buffer[IRC_LINE_MAXLEN]);

