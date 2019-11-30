#include "cross_header.h"
#include <SDL.h>
#ifdef __SWITCH__
#include "platform/switch/network.h"
#endif

#include "devilution.h"
#include "injection.h"
#include "config.h"

#if !defined(__APPLE__)
extern "C" const char *__asan_default_options()
{
	return "halt_on_error=0";
}
#endif

void quit(){
	init_cleanup();
	SDL_Quit();
}

int main(int argc, char **argv){
	atexit(quit);
	#ifdef __SWITCH__
		switch_enable_network();
	#endif
	if(!inj::parse_flags(argc, argv)) return 0;
	return dvl::WinMain(NULL, NULL, "", 0);
}
