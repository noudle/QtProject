#ifndef __DVO_TIME_COMMON_H__
#define __DVO_TIME_COMMON_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include <time.h>
#include <sys/time.h>
#include "dvo_types.h"
#include "dvo_trace.h"

//todo:there is no c file to these params
extern DVO_CHAR g_szTimeZoneName[32];
extern DVO_S32  g_s32TimeZoneOffsetSeconds;

#define DVO_TIMET time_t
#define DVO_TIMEV struct timeval

typedef struct maDVO_TIMEM
{
	DVO_U32 u32Year;   //1970~ depends system
	DVO_U32 u32Month;  //1~12
	DVO_U32 u32Day;    //1~ depends month
	DVO_U32 u32Hour;   //0~23
	DVO_U32 u32Minute; //0~59
	DVO_U32 u32Second; //0~59
	DVO_U32 u32Weekday;
	DVO_U32 u32Padding;
} PACK(8) DVO_TIMEM;

// by wz 20160906
typedef struct maDVO_TIMEZONE
{
	DVO_CHAR g_szTimeZoneName[32];
    DVO_S32 g_s32TimeZoneOffsetSeconds;
}DVO_TIMEZONE;


static inline DVO_TIMEV DVO_TIME_COMMON_GetLocalRealtime();
static inline DVO_TIMEV DVO_TIME_COMMON_GetUTCRealtime();
static inline DVO_TIMEV DVO_TIME_COMMON_GetMonotonic();

static inline DVO_TIMEV DVO_TIME_COMMON_TimevAdd(DVO_TIMEV _stTimeEnd, DVO_TIMEV _stTimeBegin);
static inline DVO_TIMEV DVO_TIME_COMMON_TimevSub(DVO_TIMEV _stTimeEnd, DVO_TIMEV _stTimeBegin);

static inline DVO_TIMEM DVO_TIME_COMMON_Second2Time(DVO_TIMET _tSecond);
static inline DVO_TIMEM DVO_TIME_COMMON_Second2LocalTime(DVO_TIMET _tSecond);

static inline DVO_TIMET DVO_TIME_COMMON_Time2Second(DVO_TIMEM _stTime);

static inline DVO_BOOL DVO_TIME_COMMON_GetTimeZone(DVO_CHAR* _szRetName, DVO_S32* _ps32RetOffsetSeconds);
static inline DVO_BOOL DVO_TIME_COMMON_SetTimeZone(DVO_CHAR* _szName, DVO_S32 _s32OffsetSeconds);

static inline DVO_TIMEM DVO_TIME_COMMON_GetLocalTime();
static inline DVO_TIMEM DVO_TIME_COMMON_GetUTCTime();
static inline DVO_BOOL DVO_TIME_COMMON_SetLocalTime(DVO_TIMEM _stTime);
static inline DVO_BOOL DVO_TIME_COMMON_SetUTCTime(DVO_TIMEM _stTime);



static inline DVO_TIMEV _ClockGettime(clockid_t _tclock_id)
{
	struct timespec stTime;
	clock_gettime(_tclock_id, &stTime);
	DVO_TIMEV stRet;
	stRet.tv_sec = stTime.tv_sec;
	stRet.tv_usec = stTime.tv_nsec/1000;
	return stRet;
}

//add 2020/02/19 by zyg  将utc的相对时间转换为local的时间格式
static inline DVO_TIMEM _Second2LocalTime(DVO_TIMET _tSecond)
{
	if(_tSecond < 0)
	{
		_tSecond = 0;
	}

	DVO_TIMEM stRet;
	memset(&stRet, 0, sizeof(DVO_TIMEM));
	struct tm stTm;
	struct tm* pstTm = localtime_r(&_tSecond, &stTm);
	
	DVO_ASSERT(pstTm != NULL, "_tSecond %ld is invalid", _tSecond) ;

	stRet.u32Year = stTm.tm_year + 1900;
	stRet.u32Month = stTm.tm_mon + 1;
	stRet.u32Day = stTm.tm_mday;
	stRet.u32Hour = stTm.tm_hour;
	stRet.u32Minute = stTm.tm_min;
	stRet.u32Second = stTm.tm_sec;
	stRet.u32Weekday = stTm.tm_wday;
	return stRet;
}



static inline DVO_TIMEM _Second2Time(DVO_TIMET _tSecond)
{
	if(_tSecond < 0)
	{
		_tSecond = 0;
	}

	DVO_TIMEM stRet;
	memset(&stRet, 0, sizeof(DVO_TIMEM));
	struct tm stTm;
	
	struct tm* pstTm = gmtime_r(&_tSecond, &stTm);// 
	
	DVO_ASSERT(pstTm != NULL, "_tSecond %ld is invalid", _tSecond) ;

	stRet.u32Year = stTm.tm_year + 1900;
	stRet.u32Month = stTm.tm_mon + 1;
	stRet.u32Day = stTm.tm_mday;
	stRet.u32Hour = stTm.tm_hour;
	stRet.u32Minute = stTm.tm_min;
	stRet.u32Second = stTm.tm_sec;
	stRet.u32Weekday = stTm.tm_wday;

	return stRet;
}



static inline DVO_TIMET _Time2Second(DVO_TIMEM _stTime)
{
	struct tm stTm;
	struct tm stTm4Check;
	DVO_TIMET tTime;

	memset(&stTm, 0, sizeof(struct tm));
	stTm.tm_year = _stTime.u32Year-1900;
	stTm.tm_mon = _stTime.u32Month-1;
	stTm.tm_mday = _stTime.u32Day;
	stTm.tm_hour = _stTime.u32Hour;
	stTm.tm_min = _stTime.u32Minute;
	stTm.tm_sec = _stTime.u32Second;
	stTm.tm_isdst = -1;//解决时间偏差
	stTm4Check = stTm;
	tTime = mktime(&stTm);
	DVO_RETURN(tTime >= 0, 0, "%u-%u-%u %u:%u:%u is not a valid datetime.",
			_stTime.u32Year, _stTime.u32Month, _stTime.u32Day, _stTime.u32Hour, _stTime.u32Minute, _stTime.u32Second);

	DVO_RETURN(stTm.tm_year == stTm4Check.tm_year && stTm.tm_mon == stTm4Check.tm_mon && stTm.tm_mday == stTm4Check.tm_mday
			&& stTm.tm_hour == stTm4Check.tm_hour && stTm.tm_min == stTm4Check.tm_min && stTm.tm_sec == stTm4Check.tm_sec,
			0,
			"%u-%u-%u %u:%u:%u is not a valid datetime.",
			_stTime.u32Year, _stTime.u32Month, _stTime.u32Day, _stTime.u32Hour, _stTime.u32Minute, _stTime.u32Second);

	return tTime;
}

static inline DVO_BOOL _SetTime(DVO_TIMEM _stTime, DVO_S32 _s32TimeZoneOffsetSeconds)
{
	struct timespec stTimespec;
	stTimespec.tv_sec = _Time2Second(_stTime);
	stTimespec.tv_nsec = 0;

	DVO_RETURN(stTimespec.tv_sec >= 0, DVO_FALSE, "stTimespec.tv_sec %ld is invalid", stTimespec.tv_sec);

	stTimespec.tv_sec += _s32TimeZoneOffsetSeconds;
	if(stTimespec.tv_sec < 0)
	{
		stTimespec.tv_sec = 0;
	}
	
#if SERIES_DVO_GW == 1
#warning // need code 
#else
	DVO_S32 s32Ret = clock_settime(CLOCK_REALTIME, &stTimespec);
	DVO_RETURN(s32Ret != -1, DVO_FALSE, "clock_settime %d failed by %d", s32Ret, errno);
#endif
	return DVO_TRUE;
}


static inline DVO_TIMEV DVO_TIME_COMMON_GetLocalRealtime()
{
	DVO_TIMEV stTimev = _ClockGettime(CLOCK_REALTIME);
#ifndef UPGRADE_TOOL_MAIN // by wz 20160927
	//stTimev.tv_sec += g_s32TimeZoneOffsetSeconds;   //by zyh 20190911
#endif
    return stTimev;
}

static inline DVO_TIMEV DVO_TIME_COMMON_GetUTCRealtime()
{
	return _ClockGettime(CLOCK_REALTIME);
}

static inline DVO_TIMEV DVO_TIME_COMMON_GetMonotonic()
{
	return _ClockGettime(CLOCK_MONOTONIC);
}

static inline DVO_TIMEV DVO_TIME_COMMON_TimevAdd(DVO_TIMEV _stTimeEnd, DVO_TIMEV _stTimeBegin)
{
	DVO_TIMEV stRet;
	timeradd(&_stTimeEnd, &_stTimeBegin, &stRet);
	return stRet;
}

static inline DVO_TIMEV DVO_TIME_COMMON_TimevSub(DVO_TIMEV _stTimeEnd, DVO_TIMEV _stTimeBegin)
{
	DVO_TIMEV stRet;
	timersub(&_stTimeEnd, &_stTimeBegin, &stRet);
	return stRet;
}

static inline DVO_TIMEM DVO_TIME_COMMON_Second2Time(DVO_TIMET _tSecond)
{
	return _Second2Time(_tSecond);
}

static inline DVO_TIMEM DVO_TIME_COMMON_Second2LocalTime(DVO_TIMET _tSecond)
{
	return _Second2LocalTime(_tSecond);
}

static inline DVO_TIMET DVO_TIME_COMMON_Time2Second(DVO_TIMEM _stTime)
{
	return _Time2Second(_stTime);
}

static inline DVO_BOOL DVO_TIME_COMMON_SetTimeZone(DVO_CHAR* _szName, DVO_S32 _s32OffsetSeconds)
{
	DVO_RETURN(_szName != NULL, DVO_FALSE, "_szName is NULL");
	DVO_RETURN(strlen(_szName) < sizeof(g_szTimeZoneName), DVO_FALSE, "_szName %s is too big, expect %d", _szName, sizeof(g_szTimeZoneName) - 1);
	DVO_RETURN(_s32OffsetSeconds >= (3600 * -12) && _s32OffsetSeconds <= (3600 * 13), DVO_FALSE,
			"_s32OffsetSeconds %d is invalid, expect %d~%d", _s32OffsetSeconds, 3600 * -12, 3600 * 13);

#if 0 // by wz 20160907
	DVO_U32 i;
	DVO_BOOL bFoundError = DVO_FALSE;
	for(i = 0; i < strlen(_szName); i++)
	{
		if(!((_szName[i] >= 'a' && _szName[i] <= 'z')
				|| (_szName[i] >= 'A' && _szName[i] <= 'Z')
				|| (_szName[i] >= '0' && _szName[i] <= '9')
				|| (_szName[i] == '-' || _szName[i] == ',' || _szName[i] == '.'|| _szName[i] == '+')))
		{
			bFoundError = DVO_TRUE;
			break;
		}
	}
	DVO_RETURN(bFoundError == DVO_FALSE, DVO_FALSE, "_szName %s is invalid, expect all alpha", _szName);
#endif

	DVO_SAFE_STRCPY(g_szTimeZoneName, _szName);
	g_s32TimeZoneOffsetSeconds = _s32OffsetSeconds;
	return DVO_TRUE;
}

static inline DVO_BOOL DVO_TIME_COMMON_GetTimeZone(DVO_CHAR* _szRetName, DVO_S32* _ps32RetOffsetSeconds)
{
	DVO_RETURN(_szRetName != NULL, DVO_FALSE, "_szRetName is NULL");
	DVO_RETURN(_ps32RetOffsetSeconds != NULL, DVO_FALSE, "_ps32RetOffsetSeconds is NULL");

	strcpy(_szRetName, g_szTimeZoneName);
	*_ps32RetOffsetSeconds = g_s32TimeZoneOffsetSeconds;
	return DVO_TRUE;
}

static inline DVO_TIMEM DVO_TIME_COMMON_GetLocalTime()
{
	
	//DVO_TRACE_DEBUG("g_s32TimeZoneOffsetSeconds = %d",g_s32TimeZoneOffsetSeconds);
	return _Second2Time(_ClockGettime(CLOCK_REALTIME).tv_sec + g_s32TimeZoneOffsetSeconds);
}

static inline DVO_TIMEM DVO_TIME_COMMON_GetUTCTime()
{
	return _Second2Time(_ClockGettime(CLOCK_REALTIME).tv_sec);
}

static inline DVO_BOOL DVO_TIME_COMMON_SetLocalTime(DVO_TIMEM _stTime)
{
	return _SetTime(_stTime, 0 - g_s32TimeZoneOffsetSeconds);
}

static inline DVO_BOOL DVO_TIME_COMMON_SetUTCTime(DVO_TIMEM _stTime)
{
	return _SetTime(_stTime, 0);
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


