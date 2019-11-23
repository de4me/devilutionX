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

}


