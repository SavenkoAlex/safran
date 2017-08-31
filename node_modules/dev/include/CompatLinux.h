// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __COMPAT_LINUX_H
#define __COMPAT_LINUX_H 

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <unistd.h>
#include <assert.h>

#undef __declspec
#define __declspec(a) 

static inline int strncpy_s ( char *strDest, size_t numberOfElements, const char *strSource, size_t count )
{
	assert(numberOfElements > count);
	strDest[count] = 0;
	strncpy ( strDest, strSource, count );
	return 0;
}

#define strncat_s(A,B,C,D){strncat(A,C,D);}
#define strcat_s(Dst,NElt,Src){strncat(Dst,Src,NElt-1);}
#define strcpy_s(Dst,NElt,Src)	{	char* tmp;					\
									tmp = Dst+NElt-1;			\
									*tmp = 0;					\
									strncpy(Dst,Src,NElt-1);	\
								}


#ifndef DISABLE_WINTYPES

typedef void* HMODULE;
typedef void* HANDLE;

typedef unsigned char BYTE;
typedef unsigned short WORD;
/* PortageLP64 : This is the modification to ensure the portability of code on x64 Linux platforms */
#if defined( __LP64__ ) || defined( _LP64 )
    typedef unsigned int	DWORD;	/* DWORD forced to 32bits on 64bits platform */
	typedef int				LONG;	/* LONG forced to 32bits on 64bits platform */
#else
	typedef unsigned long DWORD;
	typedef long LONG;
#endif
typedef unsigned int UINT;
typedef void* LPVOID;

typedef unsigned char* PCHAR;
typedef const char* LPCTSTR;
typedef const char* LPCSTR;

typedef	char* LPSTR;
typedef DWORD ULONG;
typedef ULONG* PULONG;
typedef unsigned short USHORT;
typedef USHORT* PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR* PUCHAR;
typedef char* PSZ;

#define WAIT_OBJECT_0	0			// Return from WaitForSingleObject

HANDLE CreateMutex(PUC i_puc_Inutile, UC i_b_initialOwner, PUC i_puc_Name);
int CloseHandle(HANDLE i_h_Handle);
int WaitForSingleObject(HANDLE i_h_Handle, DWORD i_dw_Timeout);
int ReleaseMutex(HANDLE i_h_Handle);

#endif


#ifndef TRUE
#define TRUE	(1)
#endif
#ifndef FALSE
#define FALSE	(0)
#endif
#ifndef NULL
#define NULL	(0)
#endif

// debug.c
#define TCHAR		char
#define _T(a)		a
#define _stprintf	sprintf
#define _stricmp	stricmp
#define _strnicmp	strnicmp
#define strnicmp	strncasecmp
#define stricmp		strcasecmp

#ifndef min
	#define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef INFINITE
    #define INFINITE 0xFFFFFFFF		// Infinite timeout
#endif

#define Sleep(a)		usleep(a*1000)	// Sleep(ms)	usleep(micro sec)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

