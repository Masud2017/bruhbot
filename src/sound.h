#pragma once
/******************************************************/
/* includes */
/******************************************************/
#include "AL/al.h"
#include "AL/alc.h"

/******************************************************/
/* defines */
/******************************************************/
#define al_int ALint
#define al_uint ALuint
#define al_sizei ALsizei
#define al_enum ALenum

/******************************************************/
/* functions */
/******************************************************/
int sound_init();
int sound_create_source(al_uint *source);
int sound_create_buffer(al_uint *buffer);
int sound_bind_buffer_to_source(al_uint buffer, al_uint source);
int sound_load_wav_file_to_buffer(const char *file, al_uint buffer);
int sound_is_playing(al_uint source);
int sound_play(al_uint source);
int sound_set_pitch(al_uint source, float pitch);
int sound_set_gain(al_uint source, float gain);
