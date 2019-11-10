#include "devilution.h"
#include "pref.h"

#ifdef __APPLE__
#include "mac_types.h"
#endif

namespace dvl {

#ifndef __APPLE__
const char* keyname = "devilutionx";
#endif

const char* kPrefSoundVolume = "sound_volume";
const char* kPrefMusicVolume = "music_volume";

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

}
