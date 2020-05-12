/******************************************************/
/* includes */
/******************************************************/
#include "sound.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/******************************************************/
/* defines */
/******************************************************/
#define PJ_SOUND_OK 0
#define PJ_SOUND_BAD_PARAM 1
#define PJ_SOUND_INIT_ERROR 2 
#define PJ_SOUND_FILE_ERROR 3
#define PJ_SOUND_HEADER_ERROR 4

/******************************************************/
/* structs */
/******************************************************/
struct riff_header {
	uint8_t chunk_id[4];
	uint32_t chunk_size;
	uint8_t format[4];
};

struct wav_format {
	uint8_t sub_chunk_id[4];
	uint32_t sub_chunk_size;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;
};

struct wav_data {
	uint8_t sub_chunk_id[4];
	uint32_t sub_chunk2_size;
};

static ALCdevice *_device = NULL;
static ALCcontext *_context = NULL;

/******************************************************/
/* functions */
/******************************************************/
int sound_init()
{
	ALfloat orientation[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	_device = alcOpenDevice(NULL);

	_context = alcCreateContext(_device, NULL);

	if (!alcMakeContextCurrent(_context)) 
		return PJ_SOUND_INIT_ERROR;

	alListenerfv(AL_ORIENTATION, orientation);

	return PJ_SOUND_OK;
}

/******************************************************/
int sound_create_source(al_uint *source)
{
	if (!source)
		return PJ_SOUND_BAD_PARAM;

	alGenSources(1U, source);
	alSourcei(*source, AL_LOOPING, AL_FALSE);

	return PJ_SOUND_OK;
}

/******************************************************/
int sound_create_buffer(al_uint *buffer)
{

	if (!buffer)
		return PJ_SOUND_BAD_PARAM;

	alGenBuffers(1U, buffer);

	return PJ_SOUND_OK;
}

/******************************************************/
int sound_bind_buffer_to_source(al_uint buffer, al_uint source)
{
	alSourcei(source, AL_BUFFER, buffer);

	return PJ_SOUND_OK;
}

/******************************************************/
int sound_load_wav_file_to_buffer(const char *file, al_uint buffer)
{
	FILE *fh = NULL;
	struct wav_format format;
	struct riff_header header;
	struct wav_data data;
	uint8_t *payload = NULL;
	al_sizei size = 0;
	al_sizei frequency = 0;
	al_enum format_type = AL_INVALID_ENUM;

	// not using file as we need to do some extensive peeking and poking to the file
	//log_print(LERROR, "sound_sample_load(%s)", file);

	fh = fopen(file, "rb");

	if (!fh)
		return PJ_SOUND_FILE_ERROR;

	if (fread(&header, sizeof(header), 1, fh) == 0) {
		fclose(fh);
		return PJ_SOUND_HEADER_ERROR;
	}

	if ((header.chunk_id[0] != 'R' || header.chunk_id[1] != 'I' ||
	     header.chunk_id[2] != 'F' || header.chunk_id[3] != 'F') ||
	    (header.format[0] != 'W' || header.format[1] != 'A' ||
	     header.format[2] != 'V' || header.format[3] != 'E')) {
		fclose(fh);
		return PJ_SOUND_HEADER_ERROR;
	}

	if (fread(&format, sizeof(format), 1, fh) == 0) {
		fclose(fh);
		return PJ_SOUND_HEADER_ERROR;
	}

	if (format.sub_chunk_id[0] != 'f' || format.sub_chunk_id[1] != 'm' ||
	    format.sub_chunk_id[2] != 't' || format.sub_chunk_id[3] != ' ') {
		fclose(fh);
		return PJ_SOUND_HEADER_ERROR;
	}

	if (format.sub_chunk_size > 16) {
		fseek(fh, sizeof(short), SEEK_CUR);
	}

	if (fread(&data, sizeof(data), 1, fh) == 0) {
		fclose(fh);
		return PJ_SOUND_HEADER_ERROR;
	}

	//check for data tag in memory
	if (data.sub_chunk_id[0] != 'd' || data.sub_chunk_id[1] != 'a' ||
	    data.sub_chunk_id[2] != 't' || data.sub_chunk_id[3] != 'a') {
		fclose(fh);
		return PJ_SOUND_HEADER_ERROR;
	}

	payload = (uint8_t *)malloc(sizeof(uint8_t) * data.sub_chunk2_size);
	if (!fread(payload, data.sub_chunk2_size, 1, fh)) {
		fclose(fh);
		return PJ_SOUND_HEADER_ERROR;
		free(payload);
	}

	size = data.sub_chunk2_size;
	frequency = format.sample_rate;

	if (format.num_channels == 1) {
		if (format.bits_per_sample == 8) {
			format_type = AL_FORMAT_MONO8;
		}
		else if (format.bits_per_sample == 16) {
			format_type = AL_FORMAT_MONO16;
		}
	}
	else if (format.num_channels == 2) {
		if (format.bits_per_sample == 8) {
			format_type = AL_FORMAT_STEREO8;
		}
		else if (format.bits_per_sample == 16) {
			format_type = AL_FORMAT_STEREO16;
		}
	}
	

	alBufferData(buffer, format_type, payload, size, frequency);
	fclose(fh);

	free(payload);

	return PJ_SOUND_OK;
}

/******************************************************/
int sound_is_playing(al_uint source)
{
	al_enum state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING) ? 1 : 0;
}

/******************************************************/
int sound_play(al_uint source)
{
	alSourcePlay(source);

	return PJ_SOUND_OK;
}

/******************************************************/
int sound_set_pitch(al_uint source, float pitch)
{
	alSourcef(source, AL_PITCH, pitch);
	return PJ_SOUND_OK;
}

/******************************************************/
int sound_set_gain(al_uint source, float gain)
{
	alSourcef(source, AL_GAIN, gain);
	return PJ_SOUND_OK;
}
