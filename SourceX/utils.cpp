#include "cross_header.h"
#include "utils.h"

size_t path_add_separator(char* dst, size_t size){
	if(dst==NULL || size==0) return 0;
	size_t length = strlen(dst);
	if(length==0) return 0;
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
			dst[length] = 0;
			break;
	}
	return length;
}

size_t path_copy(char* dst, size_t size, const char* src){
	if(dst==NULL || size==0) return 0;
	if(src==NULL) return 0;
	size_t length = strlen(src);
	if(length==0 || length>=size) return 0;
	if(dst!=src) strncpy(dst, src, size);
	return path_add_separator(dst, size);
}

size_t path_delete_last_component(char* dst, size_t size, const char* src){
	if(dst==NULL || size==0) return 0;
	if(src==NULL) return 0;
	for(size_t i = strlen(src); i>0; i-- ){
		switch (src[i-1]) {
			case '\\':
			case '/':
				if(i>=size) return 0;
				if(dst!=src) strncpy(dst, src, i);
				dst[i] = 0;
				return i;
			default:
				break;
		}
	}
	return 0;
}
