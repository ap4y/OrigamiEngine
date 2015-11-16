/******************************************************************
*
*	CyberUtil for C
*
*	Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: cthread.h
*
*	Revision:
*
*	02/07/05
*		- first revision
*
*	10/31/05
*		- Modified the thread struct to use the cg_list concept
*	02/13/06 Theo Beisch
*		- added WINCE support
*	03/20/06 Theo Beisch
*		- added thread->deletePending and thread->selfDelete (WINCE only)
*		  so that a thread can delete its own cg_thread context when truly
*		  finished (as cg_thread_stop is not reliable under WINCE)
*		- added cg_threadlist_remove
*	06/13/07 Fabrice Fontaine Orange
*		- Bug correction : Variable used to wait for thread termination by sleeping instead of joining.
*
******************************************************************/

#ifndef _CG_UTIL_CTHREAD_H_
#define _CG_UTIL_CTHREAD_H_

#include <cybergarage/typedef.h>
#include <cybergarage/util/clist.h>
#include <cybergarage/util/ccond.h>

#include <cybergarage/util/ctime.h>
#if defined(WIN32) && !defined(ITRON)
#include <windows.h>
#elif defined(BTRON)
#include <btron/proctask.h>
#elif defined(ITRON)
#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
#include <tk/tkernel.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
#include <btron/proctask.h>
#else
#include <pthread.h>
#include <signal.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#if defined (WINCE)
#define CG_THREAD_SHUTDOWN_ATTEMPTS 10
#define CG_THREAD_MIN_SLEEP 1000
/* ADD Fabrice Fontaine Orange 24/04/2007 */
/* Bug correction : Variable used to wait for thread termination by sleeping */
/* instead of joining */
#else
#define CG_THREAD_MIN_SLEEP 1000
/* ADD END Fabrice Fontaine Orange 24/04/2007 */ 
#endif

/****************************************
 * Data Type
 ****************************************/

/**
 * \brief The generic wrapper struct for CyberLinkC's threads.
 *
 * This wrapper has been created to enable 100% code
 * compatibility between different platforms (Linux, Win32 etc..)
 */
typedef struct _CgThread {
	BOOL headFlag;
	struct _CgThread *prev;
	struct _CgThread *next;
		
	/** Indicates whether this thread is ready to run */
	BOOL runnableFlag;
#if defined WINCE
	/** serves as look ahead to have the thread manage its own delete(thread) on exit */
	BOOL isRunning;
	BOOL deletePending;
	CgSysTime sleep;
#endif //WINCE

#if defined DEBUG
	char friendlyName[32];
#endif

#if defined(WIN32) && !defined(ITRON)
	HANDLE hThread;
	DWORD threadID;
#elif defined(BTRON)
	W taskID;
#elif defined(ITRON)
	ER_ID taskID;
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	ID taskID;
#elif defined(TENGINE) && defined(PROCESS_BASE)
 	WERR taskID;
#else

	/** The POSIX thread handle */
	pthread_t pThread;

#endif

	/** Thread's worker function */
	void (*action)(struct _CgThread *);

	/** Arbitrary data pointer */
	void *userData;
} CgThread, CgThreadList;

/**
 * Prototype for the threads' worker functions 
 */
typedef void (*CG_THREAD_FUNC)(CgThread *);

/****************************************
* Function
****************************************/

/**
 * Create a new thread
 */
CgThread *cg_thread_new();

/**
 * Get a self reference to thread.
 */

CgThread *cg_thread_self();

/**
 * Stop and destroy a thread.
 *
 * \param thread Thread to destroy
 */
BOOL cg_thread_delete(CgThread *thread);

/**
 * Start a thread (must be created first with ch_thread_new())
 *
 * \param thread Thread to start
 */
BOOL cg_thread_start(CgThread *thread);

/**
 * Stop a running thread.
 *
 * \param thread Thread to stop
 */
BOOL cg_thread_stop(CgThread *thread);

/**
 * Stop the running thread and signal the given CGCond.
 */
BOOL cg_thread_stop_with_cond(CgThread *thread, CgCond *cond);

/**
 * Restart a thread. Essentially calls cg_thread_stop() and cg_thread_start()
 *
 * \param thread Thread to restart
 */
BOOL cg_thread_restart(CgThread *thread);

/**
 * Check if a thread has been started
 *
 * \param thread Thread to check
 */
BOOL cg_thread_isrunnable(CgThread *thread);

/**
 * Set the thread's worker function.
 *
 * \param thread Thread struct
 * \param actionFunc Function pointer to set as the worker function
 */
void cg_thread_setaction(CgThread *thread, CG_THREAD_FUNC actionFunc);

/**
 * Set the user data pointer
 *
 * \param thread Thread struct
 * \param data Pointer to user data
 */
void cg_thread_setuserdata(CgThread *thread, void *data);

/**
 * Get the user data pointer
 *
 * \param thread Thread from which to get the pointer
 */
void *cg_thread_getuserdata(CgThread *thread);

#if defined (WINCE)
void cg_thread_sleep(CgThread *thread); 
void cg_thread_exit(DWORD exitCode);
#if defined DEBUG_MEM
void cg_thread_monitor(CgThread *thread);
#endif //DEBUG_MEM
#endif //WIN32_WCE

#define cg_thread_next(thread) (CgThread *)cg_list_next((CgList *)thread)
#define cg_thread_remove(thread) cg_list_remove((CgList *)thread)

/****************************************
* Function (Thread List)
****************************************/

/**
 * Create a new thread list
 *
 * \return Thread list
 */
CgThreadList *cg_threadlist_new();

/**
 * Destroy a thread list
 *
 * \param threadList The thread list in question
 */
void cg_threadlist_delete(CgThreadList *threadList);

/**
 * Clear the contents of a thread list
 *
 * \param threadList Thread list in question
 */
#define cg_threadlist_clear(threadList) cg_list_clear((CgList *)threadList, (CG_LIST_DESTRUCTORFUNC)cg_thread_delete)

/**
 * Get the size of a thread list
 *
 * \param threadList The thread list in question
 */
#define cg_threadlist_size(threadList) cg_list_size((CgList *)threadList)

/**
 * Get the first actual item from a thread list to use as an iterator
 *
 * \param threadList The thread list in question
 */
#define cg_threadlist_gets(threadList) (CgThread *)cg_list_next((CgList *)threadList)

/**
 * Add a thread into a thread list
 *
 * \param threadList The thread list in question
 * \param thread The thread to add to the list
 */
#define cg_threadlist_add(threadList, thread) cg_list_add((CgList *)threadList, (CgList *)thread)

/**
 * Remove a thread from thread list
 *
 * \param threadList The thread list in question
 * \param thread The thread to be removed 
 */
#define cg_threadlist_remove(thread) cg_list_remove((CgList *)thread)

/**

 * Start all threads in the thread list
 *
 * \param threadList The thread list in question
 */
BOOL cg_threadlist_start(CgThreadList *threadList);

/**
 * Stop all threads in the thread list
 *
 * \param threadList The thread list in question
 */
BOOL cg_threadlist_stop(CgThreadList *threadList);

#ifdef  __cplusplus

} /* extern "C" */

#endif

#endif
