#pragma once

namespace dvl {

extern const char* kPrefWindowFrame;
extern const char* kPrefWindowFrame;
extern const char* kPrefUpscale;
extern const char* kPrefFullscreen;
extern const char* kPrefGrabInput;

/**
Sets bool for a given value name.
@param valuename Value name
@param newValue New value
@return False on failure
*/
bool PrefSetBool(const char* valuename, bool newValue);

/**
Gets bool for a given value name, or the default specified if the key does not exist.
@param valuename Value name
@param defaultValue Default
@return If value not exist or invalid then return defaultValue
 */
bool PrefGetBool(const char* valuename, bool defaultValue);

/**
Sets string for a given value name.
@param valuename Value name
@param newValue New value
@return False on failure
*/
bool PrefSetString(const char* valuename, const char* newValue);

/**
Gets string for a given value name, or the default specified if the key does not exist.
@param valuename Value name to get
@param buffer Buffer for string
@param bufferLength Buffer length in chars
@param defaultValue Default
@return If value not exist or invalid then return defaultValue
*/
const char* PrefGetString(const char* valuename, char* buffer, unsigned int bufferLength, const char* defaultValue);

/**
Sets Int for a given value name.
@param valuename Value name
@param newValue New value
@return False on failure
*/
bool PrefSetInt(const char* valuename, int newValue);

/**
Gets int for a given value name, or the default specified if the key does not exist.
@param valuename Value name
@param defaultValue Default
@return If value not exist or invalid then return defaultValue
 */
int PrefGetInt(const char* valuename, int defaultValue);

/**
Sets SDL_Rect for a given value name.
@param valuename Value name
@param newValue New value
@return False on failure
*/
bool PrefSetRect(const char* valuename, const SDL_Rect newValue);

/**
Gets SDL_Rect for a given value name, or the default specified if the key does not exist.
@param valuename Value name
@param defaultValue Default
@return If value not exist or invalid then return defaultValue
 */
SDL_Rect PrefGetRect(const char* valuename, const SDL_Rect defaultValue);

}
