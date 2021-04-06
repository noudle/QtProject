#ifndef __DVO_TRACE_H__
#define __DVO_TRACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/prctl.h>

#include <sys/stat.h>

#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"

#define GETPID() \
({ \
	long int __ret = syscall(224); \
	if(__ret == -1) \
	{ \
		__ret = (long int)getpid(); \
	} \
	__ret; \
})

#define DVO_THREAD_UNREG_ALL() \
do \
{ \
	system("rm -rf /tmp/thread_*"); \
} \
while(0)

#define DVO_THREAD_REG(fmt, ...) \
do \
{ \
	char __szTmp[256]; \
	char __szTrdName[256]; \
	sprintf(__szTmp, "/tmp/thread_%ld", GETPID()); \
	FILE* __fp = fopen(__szTmp, "w+"); \
	if(__fp != NULL) \
	{ \
	    sprintf(__szTrdName, fmt, ##__VA_ARGS__); \
	    prctl(PR_SET_NAME, __szTrdName); \
		sprintf(__szTmp, "%ld:%p:" fmt "\n", GETPID(), __szTmp, ##__VA_ARGS__); \
		fwrite(__szTmp, 1, strlen(__szTmp), __fp); \
		fclose(__fp); \
	} \
} \
while(0)

//todo:make it in c function
#define DVO_THREAD_UNREG() \
do \
{ \
	DVO_CHAR __szTmp[128]; \
	sprintf(__szTmp, "/tmp/thread_%ld", GETPID()); \
	remove(__szTmp); \
} \
while(0)


#define MAX_OUTPUT_COUNT (32)
extern unsigned int g_u32OutputStart;
extern char g_szOutput[MAX_OUTPUT_COUNT][512];
void MA_Trace(const char* _szColor, const char* _szPrefix, const char* _szFile, unsigned int _u32Line, const char* _szFunction, const char* _szFmt, ...);
#define DVO_TRACE_FAILED(fmt, ...) MA_Trace(LIGHT_RED, "FAILED", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define DVO_TRACE_INFO(fmt, ...) MA_Trace(YELLOW, "INFO", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define DVO_TRACE_DEBUG(fmt, ...) MA_Trace(GREEN, "DEBUG", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define DVO_TRACE_ERROR(fmt, ...) MA_Trace(RED, "ERROR", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);

void MA_TraceNoLog(const char* _szColor, const char* _szPrefix, const char* _szFile, unsigned int _u32Line, const char* _szFunction, const char* _szFmt, ...);
#define DVO_TRACE_FAILED_NOLOG(fmt, ...) MA_Trace(LIGHT_RED, "FAILED", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define DVO_TRACE_INFO_NOLOG(fmt, ...) MA_Trace(YELLOW, "INFO", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define DVO_TRACE_DEBUG_NOLOG(fmt, ...) MA_Trace(GREEN, "DEBUG", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);

#define DEBUG_PRINTF (0)
#if DEBUG_PRINTF 
#define DVO_LAGRE_SIZE_DEBUG(fmt, ...) printf(fmt, ##__VA_ARGS__);		
#else
#define DVO_LAGRE_SIZE_DEBUG(fmt, ...) MA_Trace(GREEN, "DEBUG", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);	
#endif



#define DVO_ASSERT_FILE_TXT "./configs/assert.txt"
#define DVO_ASSERT_FILE_FTP "./configs/assert.ftp"
#define DVO_ASSERT(express, fmt, ...) \
do \
{ \
	if(!(express)) \
	{ \
		MA_Trace(BLUE, "ERROR", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__); \
		unsigned int i; \
		FILE* pTxtFp = fopen(DVO_ASSERT_FILE_TXT, "w+"); \
		FILE* pFtpFp = fopen(DVO_ASSERT_FILE_FTP, "w+"); \
		if(pTxtFp != NULL && pFtpFp != NULL) \
		{ \
			for(i = g_u32OutputStart; i < MAX_OUTPUT_COUNT; i++) \
			{ \
				fwrite(g_szOutput[i], 1, strlen(g_szOutput[i]), pTxtFp); \
				fwrite(g_szOutput[i], 1, strlen(g_szOutput[i]), pFtpFp); \
			} \
			for(i = 0; i < g_u32OutputStart; i++) \
			{ \
				fwrite(g_szOutput[i], 1, strlen(g_szOutput[i]), pTxtFp); \
				fwrite(g_szOutput[i], 1, strlen(g_szOutput[i]), pFtpFp); \
			} \
			fclose(pTxtFp); \
			fclose(pFtpFp); \
		} \
		assert(0); \
	} \
}while(0)


#define DVO_ASSERT_NOLOG(express, fmt, ...) \
do \
{ \
    if(!(express)) \
    { \
        MA_TraceNoLog(BLUE, "ERROR", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__); \
        unsigned int i; \
        FILE* pTxtFp = fopen(DVO_ASSERT_FILE_TXT, "w+"); \
        FILE* pFtpFp = fopen(DVO_ASSERT_FILE_FTP, "w+"); \
        if(pTxtFp != NULL && pFtpFp != NULL) \
        { \
            for(i = g_u32OutputStart; i < MAX_OUTPUT_COUNT; i++) \
            { \
                fwrite(g_szOutput[i], 1, strlen(g_szOutput[i]), pTxtFp); \
                fwrite(g_szOutput[i], 1, strlen(g_szOutput[i]), pFtpFp); \
            } \
            for(i = 0; i < g_u32OutputStart; i++) \
            { \
                fwrite(g_szOutput[i], 1, strlen(g_szOutput[i]), pTxtFp); \
                fwrite(g_szOutput[i], 1, strlen(g_szOutput[i]), pFtpFp); \
            } \
            fclose(pTxtFp); \
            fclose(pFtpFp); \
        } \
        assert(0); \
    } \
}while(0)


#define DVO_TRACE_BUFFER(__p, __size, __align) \
do \
{ \
	int __i; \
	unsigned char* p = (unsigned char*)(__p); \
	printf("%s:(%d)\n", #__p, (__size)); \
	printf("%04x:", 0); \
	for(__i = 0; __i < (int)(__size); __i++) \
	{ \
		printf("%02x ", *(p+__i)); \
		if(__i > 0 && __i < (int)(__size)-1 && ((__i+1) % __align) == 0) \
		{ \
			printf("\n%04x:", __i); \
		} \
	} \
	printf("\n"); \
}while(0)

#define DVO_TRACE_BUFFER2(__p, __size) \
do \
{ \
	int __i; \
	unsigned char* p = (unsigned char*)(__p); \
	for(__i = 0; __i < __size; __i++) \
	{ \
		printf("%02x ", *(p+__i)); \
	} \
	printf("\n"); \
}while(0)



#define MA_TRACE_TIMER_START(key) \
struct timeval __stStart##key; \
struct timeval __stEnd##key; \
struct timeval __stSub##key; \
gettimeofday(&__stStart##key, NULL)

#define MA_TRACE_TIMER_STOP(key) \
gettimeofday(&__stEnd##key, NULL); \
timersub(&__stEnd##key, &__stStart##key, &__stSub##key); \
DVO_TRACE_DEBUG(#key" using %ld.%06ld s", __stSub##key.tv_sec, __stSub##key.tv_usec)


//place in loop begin, not in loop.
#define MA_TRACE_LOOP_COUNTER_INIT() \
const DVO_U32 __u32LoopCounterSeconds = 4; \
DVO_U32 __u32LoopCounterCounts[__u32LoopCounterSeconds]; \
DVO_U32 __u32LoopCounterLastIndex; \
DVO_U32 __u32LoopCounterIndex; \
DVO_U32 __u32LoopCounterTotalCounts; \
DVO_U32 __LoopCounteri; \
DVO_TIMEV __stLoopCounterBaseTimev; \
DVO_TIMEV __stLoopCounterCurrTimev; \
DVO_TIMEV __stLoopCounterSubTimev; \
DVO_TIMEV __stLoopCounterLastPrintTimev; \
memset(&__u32LoopCounterCounts, 0, sizeof(__u32LoopCounterCounts)); \
__u32LoopCounterLastIndex = 0; \
__u32LoopCounterIndex = 0; \
__stLoopCounterBaseTimev = DVO_TIME_COMMON_GetMonotonic(); \

//place in loop.
#define MA_TRACE_LOOP_COUNTER_PRINT() \
__stLoopCounterCurrTimev = DVO_TIME_COMMON_GetMonotonic(); \
__stLoopCounterSubTimev = DVO_TIME_COMMON_TimevSub(__stLoopCounterCurrTimev, __stLoopCounterBaseTimev); \
if(__stLoopCounterSubTimev.tv_sec > 0) \
{ \
	__u32LoopCounterIndex = __stLoopCounterSubTimev.tv_sec % __u32LoopCounterSeconds; \
	if(__u32LoopCounterIndex != __u32LoopCounterLastIndex) \
	{ \
		__u32LoopCounterLastIndex = __u32LoopCounterIndex; \
		__u32LoopCounterCounts[__u32LoopCounterIndex] = 0; \
	} \
	__u32LoopCounterCounts[__u32LoopCounterIndex]++; \
} \
__stLoopCounterSubTimev = DVO_TIME_COMMON_TimevSub(__stLoopCounterCurrTimev, __stLoopCounterLastPrintTimev); \
if(__stLoopCounterSubTimev.tv_sec > 0) \
{ \
	__stLoopCounterLastPrintTimev = __stLoopCounterCurrTimev; \
	__u32LoopCounterTotalCounts = 0; \
	for(__LoopCounteri = 0; __LoopCounteri < __u32LoopCounterSeconds; __LoopCounteri++) \
	{ \
		__u32LoopCounterTotalCounts += __u32LoopCounterCounts[__LoopCounteri]; \
	} \
	DVO_TRACE_DEBUG("LoopCounter:%u", __u32LoopCounterTotalCounts/__u32LoopCounterSeconds); \
} \

//#define DVO_ASSERT(express, fmt, ...) assert(((express) ? 1 : (fprintf(stderr, fmt"\t", ##__VA_ARGS__),0)))

#define DVO_RETURN(express, ret, fmt, ...) \
do \
{\
	if(!(express)) \
	{ \
		DVO_TRACE_FAILED(fmt, ##__VA_ARGS__); \
		return (ret); \
	} \
}while(0)

#define DVO_RETURN_NOLOG(express, ret, fmt, ...) \
do \
{\
    if(!(express)) \
    { \
        DVO_TRACE_FAILED_NOLOG(fmt, ##__VA_ARGS__); \
        return (ret); \
    } \
}while(0)


#define MA_PRINTF(express, fmt, ...) \
do \
{\
	if(!(express)) \
	{ \
		DVO_TRACE_FAILED(fmt, ##__VA_ARGS__); \
	} \
}while(0)

#define DVO_GOTO(express, ret_key, ret, go, fmt, ...) \
do \
{\
	if(!(express)) \
	{ \
		DVO_TRACE_FAILED(fmt, ##__VA_ARGS__); \
		ret_key = (ret); \
		goto go; \
	} \
}while(0)

#define DVO_GOTO_NOLOG(express, ret_key, ret, go, fmt, ...) \
do \
{\
    if(!(express)) \
    { \
        DVO_TRACE_FAILED_NOLOG(fmt, ##__VA_ARGS__); \
        ret_key = (ret); \
        goto go; \
    } \
}while(0)


#define DVO_BREAK(express, ret_key, ret, fmt, ...) \
if(!(express)) \
{ \
	DVO_TRACE_FAILED(fmt, ##__VA_ARGS__); \
	ret_key = (ret); \
	break; \
}

//todo:using macro to rewrite
static inline char* MA_FORMAT_DATE_STRING(char* _szBuf, char _cDelimiter)
{
	time_t timet = time(NULL);
	struct tm* ptimetm = localtime(&timet);
	sprintf(_szBuf, "%04d%c%02d%c%02d", ptimetm->tm_year+1900, _cDelimiter, ptimetm->tm_mon+1, _cDelimiter, ptimetm->tm_mday);
	return _szBuf;
}

//todo:using macro to rewrite
static inline char* MA_FORMAT_TIME_STRING(char* _szBuf, char _cDelimiter)
{
	time_t timet = time(NULL);
	struct tm* ptimetm = localtime(&timet);
	sprintf(_szBuf, "%02d%c%02d%c%02d", ptimetm->tm_hour, _cDelimiter, ptimetm->tm_min, _cDelimiter, ptimetm->tm_sec);
	return _szBuf;
}

//todo:using macro to rewrite
static inline char* AJ_FORMAT_COMPILE_DATE_STRING(char* _szBuf)
{
	char szMonth[8];
	int s32Year;
	int s32Month;
	int s32Day;
	int s32Hour;
	int s32Minute;
	int s32Second;
	int s32Ret;
	int i;
	s32Ret = sscanf(__DATE__, "%s %d %d", szMonth, &s32Day, &s32Year);
	const char* szMonthCollection[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	s32Month = 0;
	for(i = 0; i < 12; i++)
	{
		if(strcmp(szMonth, szMonthCollection[i]) == 0)
		{
			s32Month = i + 1;
			break;
		}
	}
	s32Ret = sscanf(__TIME__, "%d:%d:%d", &s32Hour, &s32Minute, &s32Second);
	sprintf(_szBuf, "%04d-%02d-%02d %02d:%02d:%02d.000", s32Year, s32Month, s32Day, s32Hour, s32Minute, s32Second);
	s32Ret = s32Ret;
	return _szBuf;
}

static inline char* MA_FORMAT_COMPILE_DATE_STRING(char* _szBuf, char _cDelimiter)
{
	char szMonth[8];
	int s32Year;
	int s32Month;
	int s32Day;
	int s32Ret;
	int i;
	s32Ret = sscanf(__DATE__, "%s %d %d", szMonth, &s32Day, &s32Year);
	const char* szMonthCollection[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	s32Month = 0;
	for(i = 0; i < 12; i++)
	{
		if(strcmp(szMonth, szMonthCollection[i]) == 0)
		{
			s32Month = i + 1;
			break;
		}
	}
	sprintf(_szBuf, "%02d%c%02d%c%04d", s32Day, _cDelimiter, s32Month, _cDelimiter, s32Year);
	s32Ret = s32Ret;
	return _szBuf;
}

#define DVO_HASBIT(__u32Value, __u32Bit) \
({ \
	DVO_BOOL __bRet = DVO_FALSE; \
	if(((__u32Value) & (1u << (__u32Bit))) == (1u << (__u32Bit))) \
	{ \
		__bRet = DVO_TRUE; \
	} \
	__bRet; \
})

#define DVO_SETBIT(__u32Value, __u32Bit) \
do \
{ \
	(__u32Value) |= (1 << (__u32Bit)); \
}while(0)

#define DVO_CLRBIT(__u32Value, __u32Bit) \
do \
{ \
	(__u32Value) &= ~(1 << (__u32Bit)); \
}while(0)

#define DVO_SAFE_STRCPY(__dest, __src) \
do \
{ \
	DVO_ASSERT(sizeof(__dest) != 4, #__dest" is a point?"); \
	__dest[sizeof(__dest) - 1] = 0; \
	strncpy(((char*)&(__dest)), __src, sizeof(__dest) - 1); \
}while(0)

#define DVO_SAFE_ALLOCA_STRING(...) \
({ \
	DVO_CHAR* __szString; \
	DVO_S32 __s32Len; \
	__s32Len = snprintf(NULL, 0, ##__VA_ARGS__); \
	DVO_ASSERT(__s32Len >= 0, "%p, __s32Len is %d", (DVO_VOID*)NULL, __s32Len); \
	__szString = (DVO_CHAR*)alloca(__s32Len); \
	__s32Len = sprintf(__szString, ##__VA_ARGS__); \
	DVO_ASSERT(__s32Len >= 0, "%p, __s32Len is %d", __szString, __s32Len); \
	__szString; \
})



//from kernel
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define DVO_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

