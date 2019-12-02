#pragma once

/**
 Copy a path string.
 The return path will be guaranteed to end with a path separator.
 @param dst Location of the destination string buffer
 @param size Size of the destination string buffer in char units
 @param src Null-terminated source string buffer
 @return The number of characters copied
*/
size_t path_copy(char* dst, size_t size, const char* src);

/**
 Creates a copy of a given path with the last path component deleted.
 @param dst Location of the destination string buffer
 @param size Size of the destination string buffer in char units
 @param src Null-terminated source string buffer
 @return The number of characters copied
*/
size_t path_delete_last_component(char* dst, size_t size, const char* src);
