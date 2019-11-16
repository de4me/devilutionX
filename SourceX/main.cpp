#include "cross_header.h"
#include <SDL.h>
#include "devilution.h"
#include "injection.h"
#include "config.h"

#if !defined(__APPLE__)
extern "C" const char *__asan_default_options()
{
	return "halt_on_error=0";
}
#endif

using namespace dvl;

enum OPT_ID: int {
	opt_unknown,
	opt_key_inverted_v,
	opt_key_b,
	opt_key_d,
	opt_fps,
	opt_disable_network_timeout,
	opt_key_J_trigger,
	opt_level,
	opt_monst,
	opt_intro,
	opt_quest,
	opt_map,
	opt_key_s,
	opt_quest_level,
	opt_vision,
	opt_key_w,
	opt_fullscreen,
	opt_god_mode,
	opt_path,
	opt_help
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

void quit(){
	init_cleanup();
	SaveGamma();
	SDL_Quit();
}

int main(int argc, char **argv){
	atexit(quit);
	if(!parse_flags(argc, argv)) return 0;
	return dvl::WinMain(NULL, NULL, "", 0);
}
