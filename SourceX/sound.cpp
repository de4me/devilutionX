#include "devilution.h"
#include "miniwin/dsound.h"
#include "stubs.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include "pref.h"

namespace dvl {

BOOLEAN gbSndInited;
int sglMusicVolume;
int sglSoundVolume;
HANDLE sgpMusicTrack;

Mix_Music *music;
SDL_RWops *musicRw;
char *musicBuffer;

/* data */

BOOLEAN gbMusicOn = true;
BOOLEAN gbSoundOn = true;
int sgnMusicTrack = NUM_MUSIC;

char *sgszMusicTracks[NUM_MUSIC] = {
#ifdef SPAWN
	"Music\\sTowne.wav",
	"Music\\sLvlA.wav",
	"Music\\sintro.wav"
#else
	"Music\\DTowne.wav",
	"Music\\DLvlA.wav",
	"Music\\DLvlB.wav",
	"Music\\DLvlC.wav",
	"Music\\DLvlD.wav",
	"Music\\Dintro.wav"
#endif
};

BOOL snd_playing(TSnd *pSnd)
{
	if (pSnd == NULL || pSnd->DSB == NULL)
		return false;

	return pSnd->DSB->IsPlaying();
}

void snd_play_snd(TSnd *pSnd, int lVolume, int lPan)
{
	LPDIRECTSOUNDBUFFER DSB;
	DWORD tc;

	if (!pSnd || !gbSoundOn) {
		return;
	}

	DSB = pSnd->DSB;
	if (!DSB) {
		return;
	}

	tc = GetTickCount();
	if (tc - pSnd->start_tc < 80) {
		return;
	}

	lVolume += sglSoundVolume;
	if (lVolume < VOLUME_MIN) {
		lVolume = VOLUME_MIN;
	} else if (lVolume > VOLUME_MAX) {
		lVolume = VOLUME_MAX;
	}
	DSB->Play(lVolume, lPan);
	pSnd->start_tc = tc;
}

TSnd *sound_file_load(char *path)
{
	HANDLE file;
	BYTE *wave_file;
	TSnd *pSnd;
	DWORD dwBytes;
	int error;

	WOpenFile(path, &file, false);
	pSnd = (TSnd *)DiabloAllocPtr(sizeof(TSnd));
	memset(pSnd, 0, sizeof(TSnd));
	pSnd->sound_path = path;
	pSnd->start_tc = GetTickCount() - 81;

	dwBytes = SFileGetFileSize(file, NULL);
	wave_file = DiabloAllocPtr(dwBytes);
	SFileReadFile(file, wave_file, dwBytes, NULL, NULL);

	pSnd->DSB = new DirectSoundBuffer();
	error = pSnd->DSB->SetChunk(wave_file, dwBytes);
	WCloseFile(file);
	mem_free_dbg(wave_file);
	if (error != 0) {
		ErrSdl();
	}

	return pSnd;
}

void sound_file_cleanup(TSnd *sound_file)
{
	if (sound_file) {
		if (sound_file->DSB) {
			sound_file->DSB->Stop();
			sound_file->DSB->Release();
			delete static_cast<DirectSoundBuffer *>(sound_file->DSB);
			sound_file->DSB = NULL;
		}

		mem_free_dbg(sound_file);
	}
}

int NormilizeVolume(int volume){
	if(volume>0) return VOLUME_NORMAL;
	if(volume<VOLUME_MIN) return VOLUME_MIN;
	return volume;
}

int SoundVolume(const char *value_name){
	int result = NormilizeVolume(PrefGetInt(value_name, VOLUME_NORMAL));
	return result - result % 100;
}

void snd_init(HWND hWnd)
{
	sglSoundVolume = SoundVolume(kPrefSoundVolume);
	gbSoundOn = sglSoundVolume > VOLUME_MIN;

	sglMusicVolume = SoundVolume(kPrefMusicVolume);
	gbMusicOn = sglMusicVolume > VOLUME_MIN;

	int result = Mix_OpenAudio(22050, AUDIO_S16LSB, 2, 1024);
	if (result < 0) {
		SDL_Log(Mix_GetError());
	}
	Mix_AllocateChannels(25);
	Mix_ReserveChannels(1); // reserve one channel for naration (SFileDda*)

	gbSndInited = true;
}

void sound_load_volume(char *value_name, int *value)
{
	if(value==NULL) return;
	*value = SoundVolume(value_name);
}

void sound_cleanup()
{
	SFileDdaDestroy();

	if (gbSndInited) {
		gbSndInited = false;
		PrefSetInt(kPrefSoundVolume, sglSoundVolume);
		PrefSetInt(kPrefMusicVolume, sglMusicVolume);
	}
}

void music_stop()
{
	if (sgpMusicTrack) {
		Mix_HaltMusic();
		SFileCloseFile(sgpMusicTrack);
		sgpMusicTrack = NULL;
		Mix_FreeMusic(music);
		music = NULL;
		musicRw = NULL;
		mem_free_dbg(musicBuffer);
		sgnMusicTrack = NUM_MUSIC;
	}
}

void music_start(int nTrack)
{
	BOOL success;

	assert((DWORD) nTrack < NUM_MUSIC);
	music_stop();
	if (gbMusicOn) {
		success = SFileOpenFile(sgszMusicTracks[nTrack], &sgpMusicTrack);
		if (!success) {
			sgpMusicTrack = NULL;
		} else {
			int bytestoread = SFileGetFileSize(sgpMusicTrack, 0);
			musicBuffer = (char *)DiabloAllocPtr(bytestoread);
			SFileReadFile(sgpMusicTrack, musicBuffer, bytestoread, NULL, 0);

			musicRw = SDL_RWFromConstMem(musicBuffer, bytestoread);
			if (musicRw == NULL) {
				ErrSdl();
			}
			music = Mix_LoadMUSType_RW(musicRw, MUS_NONE, 1);
			Mix_VolumeMusic(MIX_MAX_VOLUME - MIX_MAX_VOLUME * sglMusicVolume / VOLUME_MIN);
			Mix_PlayMusic(music, -1);

			sgnMusicTrack = nTrack;
		}
	}
}

void sound_disable_music(BOOL disable)
{
	if (disable) {
		music_stop();
	} else if (sgnMusicTrack != NUM_MUSIC) {
		music_start(sgnMusicTrack);
	}
}

int sound_get_or_set_music_volume(int volume)
{
	if (volume == 1)
		return sglMusicVolume;

	sglMusicVolume = volume;

	if (sgpMusicTrack)
		SFileDdaSetVolume(sgpMusicTrack, volume, 0);

	return sglMusicVolume;
}

int sound_get_or_set_sound_volume(int volume)
{
	if (volume == 1)
		return sglSoundVolume;

	sglSoundVolume = volume;

	return sglSoundVolume;
}

} // namespace dvl
