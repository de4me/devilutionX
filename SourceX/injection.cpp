#include "cross_header.h"
#include "injection.h"
#include "config.h"

#ifdef __APPLE__
#include "mac_types.h"
#include <dlfcn.h>
#endif

using namespace dvl;

namespace inj {

const char* base_path = NULL;
const char* data_path = NULL;
bool is_gamma_valid = false;

enum OPT_ID: int {
	opt_unknown,
	opt_key_inverted_v,				//Enable god mode and debug tools
	opt_key_b,						//Enable item drop log
	opt_key_d,						//Increaased item drops
	opt_fps,						//Display frames per second
	opt_disable_network_timeout,	//Disable network timeout
	opt_key_J_trigger,				//Init trigger at level
	opt_level,						//Start in level
	opt_monst,						//Add debug monster, up to 10 allowed
	opt_intro,						//Show startup videos
	opt_quest,						//Force a certain quest
	opt_map,						//Set map seed
	opt_key_s,
	opt_quest_level,				//Set current quest level
	opt_vision,						//Highlight visibility
	opt_key_w,						//Enable cheats
	opt_fullscreen,					//Run in fullscreen mode
	opt_god_mode,					//Enable god mode
	opt_path,						//Set search folder for diabdat.mpq file
	opt_help						//Print help and exit
};

struct OPT_INFO{
	const char* cmd;
	const OPT_ID id;
};

const OPT_INFO options_array[] = {
		{"--help", opt_help},
		{"-h", opt_help},
		{"-?", opt_help},
		{"-fps", opt_fps},
		{"-f", opt_fps},
		{"-fullscreen", opt_fullscreen},
		{"-path", opt_path},
		{"-folder", opt_path},
		{"--data-dir", opt_path},
#if _DEBUG
		{"-vision", opt_vision},
		{"-v", opt_vision},
		{"-intro", opt_intro},
		{"-gm", opt_god_mode},
		{"-$", opt_god_mode},
		{"-^", opt_key_inverted_v},
		{"-d", opt_key_d},
		{"-m", opt_monst},
		{"-w", opt_key_w},
		{"-notimeout", opt_disable_network_timeout},
		{"-i", opt_disable_network_timeout},
		{"-map", opt_map},
		{"-r", opt_map},
		{"-quest", opt_quest},
		{"-q", opt_quest},
		{"-level", opt_level},
		{"-l", opt_level},
		{"-ql", opt_quest_level},
		{"-t", opt_quest_level},
		//??
		{"-b", opt_key_b},
		{"-j", opt_key_J_trigger},
		{"-s", opt_key_s},
#endif
	};

OPT_ID get_option(const char* str){
	for(size_t i=0; i<sizeof(options_array)/sizeof(options_array[0]); i++){
		if(strcasecmp(options_array[i].cmd, str) == 0)
			return options_array[i].id;
	}
	return opt_unknown;
}

void print_option(OPT_ID id, const char* params, const char* description){
	char buffer[128];
	size_t index = 0;
	for(size_t i=0; i<sizeof(options_array)/sizeof(options_array[0]); i++){
		if(options_array[i].id!=id) continue;
		const char* cmd = options_array[i].cmd;
		if(cmd==NULL) continue;
		size_t length = strlen(cmd);
		if(length==0) continue;
		if(index+length>=sizeof(buffer)) break;
		if(index!=0){
			if(index+2>=sizeof(buffer)) break;
			strncpy(&buffer[index], ", ", 2);
			index+=2;
		}
		strncpy(&buffer[index], cmd, sizeof(buffer)-index);
		index+=length;
		if(params!=0){
			size_t length = strlen(params);
			if(length!=0 && index+length+1<sizeof(buffer)){
				buffer[index++]=' ';
				strncpy(&buffer[index], params, sizeof(buffer)-index);
				index+=length;
			}
		}
		buffer[index]=0;
	}
	if(index==0) return;
	printf("%-30s: %s\n", buffer, description);
}

void print_help(){
	printf("%s %s\n\n", PROJECT_NAME, PROJECT_VERSION);
	printf("%s\n", "Supported options:");
	print_option(opt_help, NULL, "This help;");
	print_option(opt_fps, NULL , "Show fps;");
	print_option(opt_fullscreen, NULL, "Run in fullscreen mode;");
	print_option(opt_path, "{path}", "Set search folder for diabdat.mpq file;");
#ifdef _WIN32
	printf("\n%s\n", "demo: DevilutionX.exe -fps -fullscreen -folder c:\\Users\\Me\\DevilutionX");
#else
	printf("\n%s\n", "demo: DevilutionX -fps -fullscreen -path /Users/Me/DevilutionX");
#endif
}

bool parse_flags(int argc, char * const *argv){
	if(argc == 0) return true;
	base_path=argv[0];
	for(int i = 1; i<argc; i++){
		int next = i + 1;
		switch (get_option(argv[i])) {
			case opt_fps:
				EnableFrameCount();
				continue;
			case opt_fullscreen:
				fullscreen = true;
				continue;
			case opt_path:
				if(next>=argc) return true;
				inj::data_path = argv[next];
				break;
			case opt_help:
				print_help();
				return false;
#if _DEBUG
			case opt_vision:
				visiondebug = true;
				continue;
			case opt_key_w:
				debug_mode_key_w = true;
				continue;
			case opt_intro:
				showintrodebug = true;
				continue;
			case opt_god_mode:
				debug_mode_dollar_sign = true;
				continue;
			case opt_key_inverted_v:
				debug_mode_key_inverted_v = true;
				continue;
			case opt_key_d:
				showintrodebug = false;
				debug_mode_key_d = true;
				continue;
			case opt_disable_network_timeout:
				debug_mode_key_i = true;
				continue;
			case opt_map:
				if(next>=argc) return true;
				if(!isdigit(*argv[next])) continue;
				setseed = atoi(argv[next]);
				break;
			case opt_monst:
				for(; next<argc && isdigit(*argv[next]); next++){
					if(debugmonsttypes<int(sizeof(DebugMonsters)/sizeof(DebugMonsters[0])))
						DebugMonsters[debugmonsttypes++] = atoi(argv[next]);
					i = next;
				}
				monstdebug = debugmonsttypes != 0;
				continue;
			case opt_level:
			{
				if(next>=argc) return true;
				if(!isdigit(*argv[next])) continue;
				int level = atoi(argv[next]);
				dungeon_type type = inj::dungeon_type_from_level(level);
				if(type==DTYPE_NONE) break;
				leveltype = type;
				currlevel = level;
//				plr[0].plrlevel = current;
				setlevel = false;
				leveldebug = true;
				break;
			}
			case opt_quest_level:
			{
				if(next>=argc) return true;
				if(!isdigit(*argv[next])) continue;
				int level = atoi(argv[next]);
				dungeon_type type = inj::dungeon_type_from_quest_level(level);
				if(type==DTYPE_NONE) break;
				setlvlnum = level;
				leveltype = type;
				currlevel = 1;
				leveldebug = true;
				setlevel = true;
				break;
			}
			case opt_quest:
			{
				if(next>=argc) return true;
				if(!isdigit(*argv[next])) continue;
				int current = atoi(argv[next]);
				for(size_t i=0;i<sizeof(questlist)/sizeof(questlist[0]); i++){
					if(questlist[i]._qdtype==current){
						currlevel = questlist[i]._qdlvl;
						leveltype = questlist[i]._qlvlt;
						if(leveltype==DTYPE_NONE)
							leveltype = inj::dungeon_type_from_level(currlevel);
						if(leveltype==DTYPE_NONE) break;
						printf("<<< active quest: %s >>>\n", questlist[i]._qlstr);
						questdebug = current;
						leveldebug = true;
						setlevel = false;
						break;
					}
				}
				break;
			}
			//??
			case opt_key_b:
//				debug_mode_key_b = true;
				continue;
			case opt_key_J_trigger:
				if(next>=argc) return true;
				if(!isdigit(*argv[next])) continue;
//				debug_mode_key_J_trigger = atoi(argv[i]);
				break;
			case opt_key_s:
				debug_mode_key_s = true;
				continue;
#endif
			default:
				continue;
		}
		i = next;
	}
	return true;
}

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

#ifdef __APPLE__

URLObject OriginalPathForTranslocation(URLObject& url){
	typedef Boolean (*SecIsTranslocatedURL)(CFURLRef, bool *, CFErrorRef *);
	typedef CFURLRef __nullable (*SecCreateOriginalPathForURL)(CFURLRef, CFErrorRef *);
	CFURLRef result = NULL;
	void *handle = dlopen("/System/Library/Frameworks/Security.framework/Security", RTLD_LAZY);
	if(!handle) { return url; }
	SecIsTranslocatedURL IsTranslocatedURL = (SecIsTranslocatedURL) dlsym(handle, "SecTranslocateIsTranslocatedURL");
	bool translocated;
	if( IsTranslocatedURL!=NULL && IsTranslocatedURL(url.object(), &translocated, NULL ) && translocated ){
		SecCreateOriginalPathForURL CreateOriginalPathForURL = (SecCreateOriginalPathForURL) dlsym(handle, "SecTranslocateCreateOriginalPathForURL");
		if( CreateOriginalPathForURL!=NULL )
			result = CreateOriginalPathForURL(url.object(), NULL);
	}
	dlclose(handle);
	return result != NULL ? result : url;
}

#endif

size_t app_path(char* dst, size_t size){
#ifdef __APPLE__
	BundleObject bundle = BundleObject::main();
	if(!bundle.isValid()) return 0;
	URLObject bundle_url = bundle.url();
	if(!bundle_url.isValid()) return 0;
	URLObject original_url = OriginalPathForTranslocation(bundle_url);
	URLObject url = original_url.deletingLastPathComponent();
	if(!url.isValid()) return 0;
	StringObject string = url.path();
	char buffer[MAX_PATH];
	if(!string.getCString(buffer, sizeof(buffer))) return 0;
	return path_copy(dst, size, buffer);
#else
	return path_delete_last_component(dst, size, inj::base_path);
#endif
}

}

