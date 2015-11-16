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
*	File: csubscription.h
*
*	Revision:
*
*	06/20/05
*		- first revision
*
******************************************************************/

#ifndef _CG_UPNP_CSUBSCRIPTION_H_
#define _CG_UPNP_CSUBSCRIPTION_H_

#include <cybergarage/typedef.h>

#include <cybergarage/http/chttp.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/


/****************************************
* Data Type
****************************************/

/**
 * Data type for subscription request
 */
typedef CgHttpRequest CgUpnpSubscriptionRequest;

/****************************************
* Function (Request)
****************************************/

/**
 * Create new subscription request
 */
CgUpnpSubscriptionRequest *cg_upnp_event_subscription_request_new();

/**
 * Delete subscription request
 *
 * @param subReq subscription request
 */
#define cg_upnp_event_subscription_request_delete(subReq) cg_http_request_delete(subReq)

/**** NT ****/

/**
 * Set host header value into underlying HTTP packet
 *
 * @param subReq Subscription request
 * @param ipaddr Host ip address
 * @param port Host port
 */
#define cg_upnp_event_subscription_request_sethost(subReq, ipaddr, port) cg_http_request_sethost(subReq, ipaddr, port)

/**
 * Get host header value from subscription request
 *
 * @param subReq Subscription request
 * @param value Not actually used anywhere
 *
 * \bug Value not needed here! (It is even processed out from to be compiled code)
 */
#define cg_upnp_event_subscription_request_gethost(subReq, value) cg_http_request_gethost(subReq)

/**** NT ****/

/**
 * Set NT (Notification type) for subscription request
 *
 * @param subReq Subscription request
 * @param value Notification type
 */
#define cg_upnp_event_subscription_request_setnt(subReq, value) cg_http_request_setheadervalue(subReq, CG_HTTP_NT, value)

/**
 * Get NT from subscription request
 *
 * @param subReq Subscription request
 * @param value Not actually used anywhere
 *
 * \bug Value not needed here!
 */
#define cg_upnp_event_subscription_request_getnt(subReq, value) cg_http_request_getheadervalue(subReq, CG_HTTP_NT)

/**** CALLBACK ****/

/**
 * Set callback URL for eventing
 *
 * @param subReq Subscription request
 * @value value String which defines the callback URL
 */
#define cg_upnp_event_subscription_request_setcallback(subReq, value) cg_http_request_setheadervalue(subReq, CG_HTTP_CALLBACK, value)

/**
 * Get callback url from subscription request
 *
 * @subReq Subscription request
 * @value Not actually used anywhere
 *
 * \bug Value not needed here!
 */
#define cg_upnp_event_subscription_request_getcallback(subReq, value) cg_http_request_getheadervalue(subReq, CG_HTTP_CALLBACK)

/**** SID ****/

/**
 * Set SID (Subscription ID) for subscription request
 *
 * @param subReq Subscription request
 * @param value SID
 */
#define cg_upnp_event_subscription_request_setsid(subReq, value) cg_http_request_setheadervalue(subReq, CG_HTTP_SID, value)

/**
 * Get SID from subscription request object
 *
 * @param subReq Subscription request
 * @param value Not used
 *
 * \bug Value not needed here!
 */
#define cg_upnp_event_subscription_request_getsid(subReq, value) cg_http_request_getheadervalue(subReq, CG_HTTP_SID)

/**** Timeout ****/

/**
 * Set timeout for subscription request
 *
 * @param subReq Subscription request
 * @param timeout Timeout
 */
void cg_upnp_event_subscription_request_settimeout(CgUpnpSubscriptionRequest *subReq, long timeout);

/**
 * Get timeout value from subscription request
 *
 * @param subReq Subscription request
 *
 * @return Timeout
 */
long cg_upnp_event_subscription_request_gettimeout(CgUpnpSubscriptionRequest *subReq);

#ifdef  __cplusplus
}
#endif

#endif
