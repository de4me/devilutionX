#include <stdio.h>
#include <string>
#include "injection.h"

using namespace dvl;

namespace inj {

const char* data_path = NULL;

dungeon_type dungeon_type_from_level(int level){
	if(level>=1 && level<5)
		return DTYPE_CATHEDRAL;
	else if(level>=5 && level<9)
		return DTYPE_CATACOMBS;
	else if(level>=9 && level<13)
		return DTYPE_CAVES;
	else if(level>=13 && level<17)
		return DTYPE_HELL;
	return DTYPE_NONE;
}

dungeon_type dungeon_type_from_quest_level(int level){
	switch (level) {
		case SL_SKELKING:
			return DTYPE_CATHEDRAL;
		case SL_BONECHAMB:
			return DTYPE_CATACOMBS;
		case SL_MAZE:
			//??
			return DTYPE_NONE;
		case SL_POISONWATER:
			return DTYPE_CAVES;
		case SL_VILEBETRAYER:
			return DTYPE_CATHEDRAL;
		default:
			return DTYPE_NONE;
	}
}

}

