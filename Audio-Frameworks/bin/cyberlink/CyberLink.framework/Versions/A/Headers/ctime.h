/******************************************************************
*
*	CyberUtil for C
*
*	Copyright (C) Satoshi Konno 2005
*
*	File: ctime.h
*
*	Revision:
*
*	02/07/05
*		- first revision
*
******************************************************************/

#ifndef _CG_UTIL_CTIME_H_
#define _CG_UTIL_CTIME_H_

#include <cybergarage/typedef.h>

#if defined(BTRON)
#include <btron/proctask.h>
#include <btron/clk.h>
#elif defined(ITRON)
#include <kernel.h>
#else
#include <time.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#if defined(BTRON) || defined(ITRON) 
typedef long CgSysTime;
#else
typedef time_t CgSysTime;
#endif

/****************************************
* Function
****************************************/

float cg_random();

void cg_wait(CgSysTime mtime);
void cg_waitrandom(CgSysTime mtime);

#define cg_sleep(val) cg_wait(val)
#define cg_sleeprandom(val) cg_waitrandom(val)

CgSysTime cg_getcurrentsystemtime();

#ifdef  __cplusplus
}
#endif

#endif
