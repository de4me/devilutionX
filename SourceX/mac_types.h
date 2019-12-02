#pragma once

#include <CoreFoundation/CoreFoundation.h>

template <typename T>
class CoreObject{
protected:
	T _object;
	bool _strong;
	void retain() { if(isStrong()) CFRetain(_object); }
	void release() { if(isStrong()) CFRelease(_object); }
public:
	CoreObject(bool strong = true)
	:_strong(strong){
	}
	CoreObject(T object, bool strong = true)
	:_object(object), _strong(strong){
	}
	CoreObject(const CoreObject& copy){
		_strong = copy._strong;
		_object = copy._object;
		retain();
	}
	~CoreObject(){ release(); }
	T object(){ return _object; }
	bool isValid() { return _object != NULL; }
	bool isStrong() { return _strong && _object != NULL; }
};

/**
 Object for CFStringRef
 */
class StringObject:public CoreObject<CFStringRef>{
public:
	StringObject(const char* string){
		_object = CFStringCreateWithCString(kCFAllocatorDefault, string, kCFStringEncodingUTF8);
	}
	StringObject(CFPropertyListRef string, bool strong = true)
	:CoreObject(strong){
		_object = string != NULL && CFGetTypeID(string) == CFStringGetTypeID() ? (CFStringRef)string : NULL;
	}
	
/**
 Copies the character contents of a string to a local C string buffer after converting the characters to a given encoding.
 @param dst The C string buffer into which to copy the string
 @param size The length of buffer in bytes
 @param coding The string encoding to which the character contents of theString should be converted
 @return True upon success or False if the conversion fails or the provided buffer is too small.
 */
	bool getCString(char* dst, size_t size, CFStringEncoding coding = kCFStringEncodingUTF8) { return CFStringGetCString(object(), dst, size, coding); }
};

/**
Object for CFNumberRef
*/
class NumberObject:public CoreObject<CFNumberRef>{
public:
	NumberObject(int value){
		_object = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &value);
	}
	NumberObject(CFPropertyListRef number, bool strong = true)
	:CoreObject(strong){
		_object = number != NULL && CFGetTypeID(number) == CFNumberGetTypeID() ? (CFNumberRef)number : NULL;
	}
};

/**
Object for CFBundleRef
*/
class URLObject:public CoreObject<CFURLRef>{
public:
	URLObject(CFURLRef object, bool strong = true)
	:CoreObject(object, strong){
	}
	
/**
 Creates a copy of a given URL with the last path component deleted.
 @return A copy of url with the last path component deleted.
 */
	URLObject deletingLastPathComponent() { return CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, object()); }
	
/**
 Returns the path portion of a given URL.
 @return The URL's path
 */
	StringObject path(){ return CFURLCopyFileSystemPath(object(), kCFURLPOSIXPathStyle); }
};

/**
Object for CFBundleRef
*/
class BundleObject:public CoreObject<CFBundleRef>{
public:
	BundleObject(CFBundleRef bundle, bool strong = true)
	:CoreObject(bundle, strong){
	}
	
/**
 Returns an application’s main bundle.
 @return A BundleObject object representing the application’s main bundle
 */
	static BundleObject main(){ return BundleObject(CFBundleGetMainBundle(), false); }
	
/**
 Returns the location of a bundle.
 @return A URLObject object describing the location of bundle
 */
	URLObject url(){ return CFBundleCopyBundleURL(object()); }
};

/**
Object for CFErrorRef
*/
class ErrorObject:public CoreObject<CFErrorRef>{
	ErrorObject(CFErrorRef error, bool strong = true)
	:CoreObject(error, strong){
	}
/**
 Returns a human-presentable description for a given error.
 @return A localized, human-presentable description of err.
 */
	StringObject description() { return CFErrorCopyDescription(object()); }
};

