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
*	File: cupnp_limit.h
*
*	Revision:
*
*	05/17/05
*		- first revision
*	04/04/06 Theo Beisch
*		- changed max notifycount to 32bit (per UPnP spec)
*
******************************************************************/

#ifndef _CG_UPNP_CUPNPLIMIT_H_
#define _CG_UPNP_CUPNPLIMIT_H_

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#if defined(HAVE_STDINT_H)
#include <stdint.h>
#else
#include <limits.h>
#endif

#define CG_UPNP_ACTOINNAME_LEN_MAX 128

/* DLNA specifies (7.2.26.6) max UDN length to 68 bytes,
 * we just play safe here and use 128 byte buffer. */
#define CG_UPNP_UDN_LEN_MAX 128
#define CG_UPNP_LOCATION_URL_MAX 512
#define CG_UPNP_CONTROLPOINT_EVENTSUBURL_MAX 512

#if defined(HAVE_STDINT_H)
#define CG_UPNP_NOTIFY_COUNT_MAX UINT32_MAX 
#else
#define CG_UPNP_NOTIFY_COUNT_MAX UINT_MAX 
#endif

#endif
