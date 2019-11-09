#pragma once

#include <CoreFoundation/CoreFoundation.h>

namespace dvl {

template <typename T>
class CoreObject{
protected:
	T _object;
public:
	~CoreObject(){ if (isValid()) CFRelease(_object); }
	T object(){ return _object; }
	bool isValid() { return _object != NULL; }
};

/**
 Object for CFStringRef
 */
class StringObject:public CoreObject<CFStringRef>{
public:
	StringObject(const char* string){
		_object = CFStringCreateWithCString(kCFAllocatorDefault, string, kCFStringEncodingUTF8);
	}
	StringObject(CFPropertyListRef string){
		_object = string != NULL && CFGetTypeID(string) == CFStringGetTypeID() ? (CFStringRef)string : NULL;
	}
};

/**
Object for CFNamberRef
*/
class NumberObject:public CoreObject<CFNumberRef>{
public:
	NumberObject(int value){
		_object = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &value);
	}
	NumberObject(CFPropertyListRef number){
		_object = number != NULL && CFGetTypeID(number) == CFNumberGetTypeID() ? (CFNumberRef)number : NULL;
	}
};

}
