/******************************************************/
/* includes */
/******************************************************/
#include "irc.h"
#include "log.h"
#include "on_err.h"
#include "random.h"
#include "sound.h"
#include "tcp.h"
#include <string.h>
#include <stdio.h>

/******************************************************/
/* defines */
/******************************************************/
#define VERSION_MAJOR   0
#define VERSION_MINOR   0
#define VERSION_TYPE 	wip

/******************************************************/
int main(int argc, char **argv)
{
	char errmsg[ERR_BUF_SIZE];
	int err;
	struct tcp_sock sock;
	const char *oauthfile	= ".oauth";
	const char *hostname	= "irc.chat.twitch.tv";
	const char *nick        = "c99bot";
	const char *channel	= "donho";
	const uint16_t port 	= 6667;
	char oauth[OAUTH_MAXLEN];
	al_uint snd_src;
	al_uint snd_buf;

	sound_init();
	rand_seed_init();

	sound_create_source(&snd_src);
	sound_create_buffer(&snd_buf);
	sound_load_wav_file_to_buffer("../res/TRSNDNL003-1274.wav", snd_buf);
	sound_bind_buffer_to_source(snd_buf, snd_src);
	sound_set_pitch(snd_src, 1);
	sound_play(snd_src);

	/* connect to the twitch IRC server */
	err = read_oauth_file(oauthfile, oauth);
	on_err(err, "OATH FAILED HOSS!");
			
	irc_connect(&sock, hostname, port);
	irc_auth(&sock, nick, oauth);
	irc_join(&sock, channel);

	int counter = 0;
	int error;

	for(;;) {
		int size;
		char buffer[IRC_LINE_MAXLEN];

		error = irc_readline(&sock, buffer);

		if (error == ERR_OK) {
			log_print(LDEBUG, "LINE: %s", buffer);
		}
	}

	error = tcp_close_sock(&sock);

	if(error != BRUH_TCP_OK) {
		log_print(LERROR, "tcp_close_sock() failed", error);
		return -1;
	}

	return 0;
}
