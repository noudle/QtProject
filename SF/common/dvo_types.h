#ifndef __DVO_TYPE_H__
#define __DVO_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


typedef unsigned char           DVO_U8;
typedef unsigned short          DVO_U16;
typedef unsigned int            DVO_U32;

typedef signed char             DVO_S8;
typedef short                   DVO_S16;
typedef int                     DVO_S32;

typedef float                   DVO_F32;
typedef double                  DVO_F64;

#ifdef WIN32
    typedef __int64             DVO_U64;
    typedef __int64             DVO_S64;
#else
    typedef unsigned long long  DVO_U64;
    typedef long long           DVO_S64;
#endif

typedef char                    DVO_CHAR;
#define DVO_VOID                void

/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/
typedef enum {
    DVO_FALSE = 0,
    DVO_TRUE  = 1,
} DVO_BOOL;

#ifdef WIN32
#define PACK(__align) __declspec(align(__align))
#else
#define PACK(__align) __attribute__((aligned(__align)))
#endif


#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

