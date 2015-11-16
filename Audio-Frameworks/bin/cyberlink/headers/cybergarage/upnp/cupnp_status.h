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
*	File: upnp_status.h
*
*	Revision:
*
*	02/13/05
*		- first revision
*
*	10/31/05
*		- Added comments to all functions and structs
*
******************************************************************/

#ifndef _CG_UPNP_CUPNPSTATUS_H_
#define _CG_UPNP_CUPNPSTATUS_H_

#include <cybergarage/typedef.h>
#include <cybergarage/util/cstring.h>

#ifdef  __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Defines
 ********************************************************************************/

#define CG_UPNP_STATUS_INVALID_ACTION 401
#define CG_UPNP_STATUS_INVALID_ARGS 402
#define CG_UPNP_STATUS_OUT_OF_SYNC 403
#define CG_UPNP_STATUS_INVALID_VAR 404
#define CG_UPNP_STATUS_ACTION_FAILED 501

/****************************************
 * Data Type
 ****************************************/

/**
 * UPnP status struct used in action and state operations
 */
typedef struct _CgUpnpStatus {
	/** Status code */
	int code;

	/** Status description */
	CgString *description;
} CgUpnpStatus;

/******************************************************************************
 * Function
 ******************************************************************************/

/**
 * Create a new status struct
 *
 */
CgUpnpStatus *cg_upnp_status_new();

/**
 * Delete a status struct
 *
 * \param upnpStat Status struct to delete
 */
void cg_upnp_status_delete(CgUpnpStatus *upnpStat);

/**
 * Set status code for status struct
 *
 * \param upnpStat Status struct
 * \param value Status code
 */
#define cg_upnp_status_setcode(upnpStat, value) (upnpStat->code = value)

/**
 * Get Status code from a status struct
 *
 * \param upnpStat Status structure
 */
#define cg_upnp_status_getcode(upnpStat) (upnpStat->code)

/**
 * Set status description for a status struct
 *
 * \param upnpStat Status struct
 * \param value Status description
 */
#define cg_upnp_status_setdescription(upnpStat, value) cg_string_setvalue(upnpStat->description, value)

/**
 * Get status description from a status struct
 *
 * \param upnpStat Status struct
 */
#define cg_upnp_status_getdescription(upnpStat) cg_string_getvalue(upnpStat->description)

/**
 * Helper function that converts a status code into a status description
 *
 * \param code Code to convert into a description
 * \return Status description
 */
char *cg_upnp_status_code2string(int code);

#ifdef  __cplusplus

} /* extern "C" */

#endif

#endif
