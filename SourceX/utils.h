#pragma once

#include <stdio.h>

/**
	Copy path from src to dst. Add last path character to end if not exist.
	@param dst Destination string
	@param size The size of the destination string, in characters
	@param src Source string
	@return Number bytes copied
*/
size_t path_copy(char* dst, size_t size, const char* src);
