/******************************************************************
*
*	CyberUtil for C
*
*	Copyright (C) 2006 Nokia Corporation
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: ccond.h
*
*	Revision:
*
*	16-Jan-06
*		- first revision
*
******************************************************************/

#ifndef _CG_UTIL_CCOND_H_
#define _CG_UTIL_CCOND_H_

#include <cybergarage/typedef.h>
#include <cybergarage/util/cmutex.h>

#if defined(WIN32) && !defined(ITRON)
#include <winsock2.h>
#elif defined(BTRON)
#include <btron/taskcomm.h>
#elif defined(ITRON)
#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
#include <tk/tkernel.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
#include <btron/taskcomm.h>
#else
#include <pthread.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Types
 ****************************************/

/**
 * \brief The generic wrapper struct for CyberLinkC's conds.
 *
 * This wrapper has been created to enable 100% code
 * compatibility for different platforms (Linux, Win32 etc..)
 */
typedef struct _CgCond {
#if defined(WIN32) && !defined(ITRON)
	HANDLE	condID;
#elif defined(BTRON)
	WERR	condID;
#elif defined(ITRON)
	ER_ID	condID;
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	ID condID;
#elif defined(TENGINE) && defined(PROCESS_BASE)
	WERR	condID;
#else
	/** The cond entity */
	pthread_cond_t condID;
#endif
} CgCond;

/****************************************
 * Functions
 ****************************************/

/** 
 * Create a new condition variable
 */
CgCond *cg_cond_new();

/** 
 * Destroy a condition variable
 *
 * \param cond The cond to destroy
 */
BOOL cg_cond_delete(CgCond *cond);

/** 
 * Wait for condition variable to be signalled.
 *
 * \param cond Cond to be waited
 * \param mutex Mutex used for synchronization
 * \param timeout Maximum time in seconds to wait, 0 to wait forever
 */
BOOL cg_cond_wait(CgCond *cond, CgMutex *mutex, unsigned long timeout);

/** 
 * Signal a condition variable
 *
 * \param cond Cond to be signalled
 */
BOOL cg_cond_signal(CgCond *cond);

#ifdef  __cplusplus

} /* extern "C" */

#endif

#endif
