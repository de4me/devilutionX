#pragma once

#include <SDL.h>

#include "diablo.h"
#include "utils.h"
#include "pref.h"

using namespace dvl;

namespace inj {

extern const char* data_path;

/**
 Get dungeon_type for level
 @param level Game level
 @return DTYPE_NONE on failure
 */
dungeon_type dungeon_type_from_level(int level);

/**
 Get dungeon_type for dungeon guest level
 @param level Dungeon level
 @return DTYPE_NONE on failure
*/
dungeon_type dungeon_type_from_quest_level(int level);

/**
 Validate window frame
 @param frame Frame
 @return True if frame inside monitor bounds
 */
bool CheckWindowFrame(const SDL_Rect* frame);

/**
 Save window frame
 @param window Window
*/
void SaveWindowFrame(SDL_Window* window);

/**
 Get saved window frame
*/
SDL_Rect LoadWindowFrame(const SDL_Rect& defaultFrame);

/**
 Save global values: gamma_correction and color_cycling_enabled
*/
void SaveGamma();

/**
 Restore global values: gamma_correction and color_cycling_enabled
*/
void LoadGamma();

/**
 Parse arguments passed on launch
 @param argc Non-negative value representing the number of arguments passed to the program from the environment in which the program is run
 @param argv Pointer to the first element of an array of argc + 1 pointers, of which the last one is null and the previous ones, if any, point to null-terminated multibyte strings that represent the arguments passed to the program from the execution environment. If argv[0] is not a null pointer (or, equivalently, if argc > 0), it points to a string that represents the name used to invoke the program, or to an empty string.
 @return True if program can launch
*/
bool parse_flags(int argc, char * const *argv);

}


