#include <fcntl.h>
#include <alloca.h>
#include <alsa/asoundlib.h>

#define	CARD		"default"
#define	MIX_INDEX	0
#define	MIX_NAME	"Master"

typedef struct {
	short int status;
	unsigned short int muted;
	double value;
} retData;

retData get() {
	retData			ret;
	snd_mixer_t*		handle;
	snd_mixer_elem_t*	elem;
	snd_mixer_selem_id_t*	sid;
	long			cur, min, max;
	int			enabled;

	ret.status = -255;

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, MIX_INDEX);
	snd_mixer_selem_id_set_name(sid, MIX_NAME);

	if ((snd_mixer_open(&handle, 0)) < 0) {
		ret.status = -1;
		return ret;
	}

	if ((snd_mixer_attach(handle, CARD)) < 0) {
		snd_mixer_close(handle);
		ret.status = -2;
		return ret;
	}

	if ((snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
		snd_mixer_close(handle);
		ret.status = -3;
		return ret;
	}

	if (snd_mixer_load(handle) < 0) {
		snd_mixer_close(handle);
		ret.status = -4;
		return ret;
	}

	elem = snd_mixer_find_selem(handle, sid);
	if (!elem) {
		snd_mixer_close(handle);
		ret.status = -5;
		return ret;
	}

	if (snd_mixer_selem_get_playback_volume_range(elem, &min, &max) < 0) {
		ret.status = -6;
		return ret;
	}

	if (snd_mixer_selem_get_playback_volume(elem, 0, &cur) < 0) {
		snd_mixer_close(handle);
		ret.status = -7;
		return ret;
	}

	if (snd_mixer_selem_get_playback_switch(elem, 0, &enabled) < 0) {
		snd_mixer_close(handle);
		ret.status = -8;
		return ret;
	}

	if (cur < min) {
		ret.status = -9;
		return ret;
	}
	if (cur > max) {
		ret.status = -10;
		return ret;
	}

	ret.status = 0;
	if (enabled == 0) {
		ret.muted = 1;
	} else {
		ret.muted = 0;
		ret.value = 100 * (double) cur / (double) max;
	}

	snd_mixer_close(handle);

	return ret;
}

int main(void) {
	retData d;

	d = get();
	if (d.status < 0) {
		printf("Error: %d\n", d.status);
		return 1;
	}

	if (d.muted == 1) {
		printf("MUTE\n");
	} else {
		printf("%.1f%%\n", d.value);
	}

	return 0;
}
