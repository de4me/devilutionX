#include "cross_header.h"
#include "utils.h"


size_t path_copy(char* dst, size_t size, const char* src){
	if(dst==NULL || size==0) return 0;
	if(src==NULL) return 0;
	size_t length = strlen(src);
	if(length==0 || length>=size) return 0;
	strncpy(dst, src, size);
	switch (dst[length-1]) {
		case '\\':
		case '/':
			break;
		default:
			if(length+2>size) return 0;
#ifdef _WIN32
			dst[length++] = '\\';
#else
			dst[length++] = '/';
#endif
	}
	return length;
}