/******************************************************************
*
*	CyberLink for C
*
*	Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: cupnp_function.h
*
*	Revision:
*
*	02/21/05
*		- first revision
*
******************************************************************/

#ifndef _CG_UPNP_FUNCTION_H_
#define _CG_UPNP_FUNCTION_H_

#include <cybergarage/typedef.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#define CG_UPNP_UUID_NAME "uuid"
#define CG_UPNP_UUID_MAX_LEN 128 /*(5 + ((4+1)*4) + 1)*/
#define CG_UPNP_SEVERNAME_MAXLEN 64

/****************************************
* Data Type
****************************************/

/****************************************
* Function
****************************************/

/**
 * Create a UUID (Universally Unique IDentifier) based on a semi-random value using current time.
 *
 * \param uuidBuf The UUID created by this function (must not be NULL)
 * \return The UUID created by this function (== \e uuidBuf)
 */
char *cg_upnp_createuuid(char *uuidBuf, int uuidBufSize);

/**
 * Get the server name ("OSname/OSversion/UPnP/<version> CyberLinkC/<version>")
 *
 * \param buf Buffer to store the server name
 * \param bufSize Buffer length
 * \param The server name buffer (== \e buf)
 */
char *cg_upnp_getservername(char *buf, int bufSize);

/**
 * Enable/disable NMPR (Intel Networked Media Product Requirements) mode.
 *
 * \param onFlag TRUE: enable; FALSE: disable
 */
void cg_upnp_setnmprmode(BOOL onFlag);

/**
 * Check whether NMPR (Intel Networked Media Product Requirements) mode is on.
 * 
 *
 * \return TRUE if on; otherwise FALSE
 */
BOOL cg_upnp_isnmprmode();

#ifdef  __cplusplus
  
} /* extern "C" */

#endif

#endif
