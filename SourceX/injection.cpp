#include "cross_header.h"
#include "injection.h"

using namespace dvl;

namespace inj {

const char* data_path = NULL;
bool is_gamma_valid = false;

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

bool CheckWindowFrame(const SDL_Rect* frame){
	if(frame==NULL) return false;
	if(SDL_RectEmpty(frame)) return false;
	for(int i = 0; i<SDL_GetNumVideoDisplays(); i++){
		SDL_Rect bounds;
		if(SDL_GetDisplayBounds(i, &bounds)!=0) continue;
		SDL_Rect intersects;
		if(!SDL_IntersectRect(frame, &bounds, &intersects)) continue;
		if(SDL_RectEquals(&intersects, frame))
			return true;
	}
	return false;
}

void SaveWindowFrame(SDL_Window* window){
	if(window == NULL) return;
	Uint32 flags = SDL_GetWindowFlags(window);
	if(flags & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_MAXIMIZED))
		return;
	SDL_Rect frame;
	SDL_GetWindowSize(window, &frame.w, &frame.h);
	SDL_GetWindowPosition(window, &frame.x, &frame.y);
	if(!CheckWindowFrame(&frame)) return;
	PrefSetRect(kPrefWindowFrame, frame);
}

SDL_Rect LoadWindowFrame(const SDL_Rect& defaultFrame){
	SDL_Rect current = PrefGetRect(kPrefWindowFrame, defaultFrame);
	if(SDL_RectEquals(&current, &defaultFrame)) return current;
	return CheckWindowFrame(&current) ? current : defaultFrame;
}

void SaveGamma(){
	if(!is_gamma_valid) return;
	PrefSetInt(kPrefGammaCorrection, gamma_correction);
	PrefSetBool(kPrefColorCycling, color_cycling_enabled);
}

int NormalizeGamma(int gamma){
	if(gamma>100) return GAMMA_NORMAL;
	if(gamma<30) return 30;
	return gamma;
}

void LoadGamma(){
	int gamma = NormalizeGamma(PrefGetInt(kPrefGammaCorrection, GAMMA_NORMAL));
	gamma_correction = gamma - gamma % 5;
	color_cycling_enabled = PrefGetBool(kPrefColorCycling, true);
	is_gamma_valid = true;
}

}

