#ifndef SLIGHT_OBJECT_HEADER
#define SLIGHT_OBJECT_HEADER

#include <windows.h>

#ifdef _WIN32
	#define SLIGHT_WINDOWS
	#pragma message("[Slight] Target platform : Windows. | include windows.h")
	#include <windows.h>
	
	#ifdef _DEBUG
		#define SLIGHT_DEBUG
	#endif
	
    #ifdef _UNICODE
        #define lsprintf swprintf
        #define tout    wcout
        #define tstring wstring
        #define tmemmove wmemmove
        #define lstrchr wcschr
        #define lstrrchr wcsrchr
        #define lstrstr wcsstr
        #define lsnprintf _snwprintf
    #else
        #define lsprintf sprintf
        #define tout cout
        #define tstring string
        #define tmemmove memmove
        #define lstrchr strchr
        #define lstrrchr strrchr
        #define lstrstr strstr
        #define lsnprintf _snprintf
    #endif
    
#endif

#ifdef __GNUC__
	#define SLIGHT_GNU
#endif

#ifndef __cplusplus
	#error [Slight] Slight only can be compiled by c++ compiler.
#endif


//assertion
#define DUMMY
#define CONDITION_RETURN(x ,Value) if(x) return Value

#ifndef SLIGHT_DISABLE_INTERNAL_ASSERT
	#ifdef SLIGHT_DEBUG
		#ifdef SLIGHT_WINDOWS
			#include <crtdbg.h>
			#define ASSERT(x) \
				while(!(x))\
				{\
					if( 1 != ::_CrtDbgReport( _CRT_ASSERT ,__FILE__ ,__LINE__ ,NULL ,NULL) )\
						::DebugBreak();\
				}
		#else
			#pragma message("Warnning : Unknown environment for assertion.Use assert() from <cassert>.")
			
			#include <cassert>
			
			#define ASSERT(x) ::assert(x) 
		#endif
	#else
		#define ASSERT(x) (x)
	#endif
#endif

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char byte;
typedef unsigned short ushort;

typedef unsigned long dword;
typedef unsigned short word;


namespace Slight {

class CEvent;
class CThreadData;

class CObject
{
public:
	typedef uint Status;
	
	Status ErrorCode;
	
public:
	inline CObject()
	:ErrorCode(0)
	{}
	
	inline Status getError() const {return ErrorCode;}
	inline Status setError(Status Code) {return ErrorCode = Code;}
	inline Status inheritError(const CObject &Object) 
	{
		ErrorCode = Object.ErrorCode;
		return ErrorCode;
	}
	
public:
	virtual ~CObject() = 0;
	
	virtual int event(CEvent& Event);

private:
	CThreadData *thread_data;
};

static const CObject::Status ERR_OK = 0;
static const CObject::Status ERR_ACCESS_DENIED = 1;
static const CObject::Status ERR_INVAILD_PARAMETER = 2;
static const CObject::Status ERR_NOT_IMPLEMENTED = 3;
static const CObject::Status ERR_OS_ERROR = 4;
static const CObject::Status ERR_NOT_SUPPORTED = 5;
static const CObject::Status ERR_INVAILD_OBJECT = 6;
static const CObject::Status ERR_VAILD_OBJECT = 7;

static const CObject::Status ERR_UNKNOWN = 4999;
static const CObject::Status ERR_USER = 5000;

}

#endif