#ifndef _PREDEF_H
#define _PREDEF_H

#include <assert.h>

#define TRUE 1
#define FALSE 0

#ifndef OK
#define OK 1
#endif

#define ERROR 0
#define INFEASIBLE -1
//#define OVERFLOW -2

typedef int Status;
typedef int BOOL;

#ifndef bool 
typedef int bool;
#endif

#define ARRAY_LEN(x) sizeof(x)/sizeof(x[0])

#endif