#pragma once

#include "diablo.h"
#include "utils.h"

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

}


