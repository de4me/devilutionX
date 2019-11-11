#include "devilution.h"
#include "pref.h"

#ifdef __APPLE__
#include "mac_types.h"
#endif

namespace dvl {

#ifndef __APPLE__
const char* keyname = "devilutionx";
#endif

const char* kPrefWindowFrame = "window_frame";
const char* kPrefUpscale = "upscale";
const char* kPrefFullscreen = "fullscreen";
const char* kPrefGrabInput = "grab_input";
const char* kPrefSoundVolume = "sound_volume";
const char* kPrefMusicVolume = "music_volume";
const char* kPrefGammaCorrection = "gamma_correction";
const char* kPrefColorCycling = "color_cycling";


bool PrefSetBool(const char* valuename, bool newValue){
#ifdef __APPLE__
	StringObject cf_key(valuename);
	if (!cf_key.isValid()) return false;
	CFPreferencesSetAppValue(cf_key.object(), newValue ? kCFBooleanTrue : kCFBooleanFalse, kCFPreferencesCurrentApplication);
	return true;
#else
	return PrefSetInt(valuename, newValue);
#endif
}

bool PrefGetBool(const char* valuename, bool defaultValue){
#ifdef __APPLE__
	StringObject cf_key(valuename);
	if (!cf_key.isValid()) return defaultValue;
	Boolean is_valid;
	Boolean result = CFPreferencesGetAppBooleanValue(cf_key.object(), kCFPreferencesCurrentApplication, &is_valid);
	return is_valid ? result : defaultValue;
#else
	return PrefGetInt(valuename, defaultValue) != 0;
#endif
}

const char* PrefGetString(const char* valuename, char* buffer, unsigned int bufferLength, const char* defaultValue){
#ifdef __APPLE__
	StringObject cf_key(valuename);
	if (!cf_key.isValid()) return defaultValue;
	StringObject cf_value(CFPreferencesCopyAppValue(cf_key.object(), kCFPreferencesCurrentApplication));
	if (!cf_value.isValid()) return defaultValue;
	return CFStringGetCString(cf_value.object(), buffer, bufferLength, kCFStringEncodingUTF8) ? (const char*)buffer : defaultValue;
#else
	return SRegLoadString(keyname, valuename, 0, (char*)buffer, bufferLength) ? (const char*)buffer : defaultValue;
#endif
}

bool PrefSetString(const char* valuename, const char* newValue){
#ifdef __APPLE__
	StringObject cf_key(valuename);
	if (!cf_key.isValid()) return false;
	StringObject cf_string(newValue);
	if (!cf_string.isValid()) return false;
	CFPreferencesSetAppValue(cf_key.object(), cf_string.object(), kCFPreferencesCurrentApplication);
	return true;
#else
	return SRegSaveString(keyname, valuename, 0, (char*)newValue);
#endif
}

bool PrefSetInt(const char* valuename, int newValue){
#ifdef __APPLE__
	StringObject cf_key(valuename);
	if (!cf_key.isValid()) return false;
	NumberObject cf_number(newValue);
	if (!cf_number.isValid()) return false;
	CFPreferencesSetAppValue(cf_key.object(), cf_number.object(), kCFPreferencesCurrentApplication);
	return true;
#else
	return SRegSaveValue(keyname, valuename, 0, newValue);
#endif
}

int PrefGetInt(const char* valuename, int defaultValue){
#ifdef __APPLE__
	StringObject cf_key(valuename);
	if (!cf_key.isValid()) return false;
	Boolean is_valid;
	int result = (int)CFPreferencesGetAppIntegerValue(cf_key.object(), kCFPreferencesCurrentApplication, &is_valid);
	return is_valid ? result : defaultValue;
#else
	int value;
	return SRegLoadValue(keyname, valuename, 0, &value) ? value : defaultValue;
#endif
}

bool PrefSetRect(const char* valuename, const SDL_Rect newValue){
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%i,%i,%i,%i", newValue.x, newValue.y, newValue.w, newValue.h);
	return PrefSetString(valuename, buffer);
}

SDL_Rect PrefGetRect(const char* valuename, const SDL_Rect defaultValue){
	char buffer[64];
	const char* buffer_ptr = PrefGetString(valuename, buffer, sizeof(buffer), NULL);
	if(buffer_ptr == NULL) return defaultValue;
	SDL_Rect rect;
	return sscanf(buffer_ptr, "%i,%i,%i,%i", &rect.x, &rect.y, &rect.w, &rect.h) == 4 ? rect : defaultValue;
}

}
