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
*	File: csubscriber.h
*
*	Revision:
*
*	06/21/05
*		- first revision
*
*	12-Jan-06 Heikki Junnila
*		- Added API comments
*	04/05/06 
*		- changed timing values to type CgSysTime
*		- added cg_upnp_subscriberlist_get(CgUpnpSubscriberList *subscriberList, char *sid)
******************************************************************/

#ifndef _CG_UPNP_CSUBSCRIBER_H_
#define _CG_UPNP_CSUBSCRIBER_H_

#include <cybergarage/typedef.h>

#include <cybergarage/util/clist.h>
#include <cybergarage/util/cstring.h>
#include <cybergarage/net/curl.h>
#include <cybergarage/util/ctime.h>

#include <cybergarage/upnp/cstatevariable.h>

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
 * The device-side structure for an event subscriber
 */
typedef struct _CgUpnpSubscriber {
	BOOL headFlag;
	struct _CgUpnpSubscriber *prev;
	struct _CgUpnpSubscriber *next;

	CgString *sid;
	CgString *ifAddr;
	CgNetURL *deliveryURL;
	
	CgSysTime timeout;
	long notifyCount;
	CgSysTime subscriptionTime;
} CgUpnpSubscriber, CgUpnpSubscriberList;

/****************************************
* Function (Subscriber)
****************************************/

/**
 * Create a new event subscriber
 */
CgUpnpSubscriber *cg_upnp_subscriber_new();

/**
 * Destroy an event subscriber
 *
 * @param sub The event subscriber
 */
void cg_upnp_subscriber_delete(CgUpnpSubscriber *sub);

/**
 * Clear the contents of an event subscriber
 *
 * @todo Lacks implementation (is it necessary?)
 *
 * @param sub The event subscriber
 */
void cg_upnp_subscriber_clear(CgUpnpSubscriber *sub);

/**
 * Renew a subscription. Essentially sets subscription time (duration) 
 * to zero and resets notify count (== event key).
 *
 * @param sub The event subscriber
 */
void cg_upnp_subscriber_renew(CgUpnpSubscriber *sub);

/**
 * Get the next subscriber in a subscriber list. Use as an iterator.
 *
 * @param sub The current event subscriber
 * @return Next subscriber or NULL
 */
#define cg_upnp_subscriber_next(sub) (CgUpnpSubscriber *)cg_list_next((CgList *)sub)

/**
 * Remove the event subscriber from a subscriber list.
 *
 * @param sub The event subscriber to remove
 */
#define cg_upnp_subscriber_remove(sub) cg_list_remove((CgList *)sub)

/****************************************************************************
 * sid
 ****************************************************************************/

/**
 * Set the subscriber's SID (Subscription IDentifier) value
 *
 * @param sub The event subscriber
 * @param value The SID value
 */
#define cg_upnp_subscriber_setsid(sub, value) cg_string_setvalue(sub->sid, value)

/**
 * Get the subscriber's SID (Subscription IDentifier) value
 *
 * @param sub The current event subscriber
 * @return Subscriber's SID
 */
#define cg_upnp_subscriber_getsid(sub) cg_string_getvalue(sub->sid)

/****************************************************************************
 * timeout
 ****************************************************************************/

/**
 * Set the subscription timeout (expiration time) to a certain value.
 *
 * @param sub The event subscriber
 * @param value Timeout
 */
#define cg_upnp_subscriber_settimeout(sub, value) (sub->timeout = value)

/**
 * Get the subscription timeout (expiration time)
 *
 * @param sub The event subscriber
 * @return Timeout
 */
#define cg_upnp_subscriber_gettimeout(sub) (sub->timeout)

/****************************************************************************
 * subscriptionTime
 ****************************************************************************/

/**
 * Set the subscription time (duration) to a certain value.
 *
 * @param sub The event subscriber
 * @param value Subscription time
 */
#define cg_upnp_subscriber_setsubscriptiontime(sub, value) (sub->subscriptionTime = value)

/**
 * Get the subscription time (duration)
 *
 * @param sub The event subscriber
 * @return Subscription time
 */
#define cg_upnp_subscriber_getsubscriptiontime(sub) (sub->subscriptionTime)

/****************************************************************************
 * notifycount
 ****************************************************************************/

/**
 * Set the notify count to a certain value.
 *
 * @param sub The event subscriber
 * @param value Notify count
 */
#define cg_upnp_subscriber_setnotifycount(sub, value) (sub->notifyCount = value)

/**
 * Get the notify count
 *
 * @param sub The event subscriber
 * @return Notify count
 */
#define cg_upnp_subscriber_getnotifycount(sub) (sub->notifyCount)

/**
 * Increment the event notify count by one
 *
 * @param sub The event subscriber
 * @return The new notify count
 */
long cg_upnp_subscriber_incrementnotifycount(CgUpnpSubscriber *sub);

/****************************************************************************
 * Delivery URL
 ****************************************************************************/

/**
 * Set the subscriber's delivery URL
 *
 * @param sub The event subscriber
 * @param value The delivery URL
 */
#define cg_upnp_subscriber_setdeliveryurl(sub, value) cg_net_url_set(sub->deliveryURL, value)

/**
 * Set the subscriber's delivery host
 *
 * @param sub The event subscriber
 */
#define cg_upnp_subscriber_getdeliveryhost(sub) cg_net_url_gethost(sub->deliveryURL)

/**
 * Set the subscriber's delivery path
 *
 * @param sub The event subscriber
 */
#define cg_upnp_subscriber_getdeliverypath(sub) cg_net_url_getpath(sub->deliveryURL)

/**
 * Set the subscriber's delivery port
 *
 * @param sub The event subscriber
 */
#define cg_upnp_subscriber_getdeliveryport(sub) cg_net_url_getport(sub->deliveryURL)

/**
 * Check, whether a subscriber's event subscription has been expired
 *
 * @param sub The subscriber
 * @return TRUE if the subscription has been expired; otherwise FALSE
 */
BOOL cg_upnp_subscriber_isexpired(CgUpnpSubscriber *sub);

/**
 * Post a notification to an event subscriber. This is called in a device.
 *
 * @param sub The event subscriber
 * @param statVar The evented state variable
 * @return TRUE if succesful; otherwise FALSE
 */
BOOL cg_upnp_subscriber_notify(CgUpnpSubscriber *sub, CgUpnpStateVariable *statVar);

/**
 * Post a notification to an event subscriber. This is called in a device.
 *
 * @param sub The event subscriber
 * @param service The evented service
 * @return TRUE if succesful; otherwise FALSE
 */
BOOL cg_upnp_subscriber_notifyall(CgUpnpSubscriber *sub, void *service);

/****************************************
* Function (SubscriberList)
****************************************/

/**
 * Create a new event subscriber list
 */
CgUpnpSubscriberList *cg_upnp_subscriberlist_new();

/**
 * Destroy an event subscriber list
 *
 * @param subscriberlist The list to destroy
 */
void cg_upnp_subscriberlist_delete(CgUpnpSubscriberList *subscriberlist);

/**
 * Clear the contents of a subscriber list
 *
 * @param subList The subscriber list to clear
 */
#define cg_upnp_subscriberlist_clear(subList) cg_list_clear((CgList *)subList, (CG_LIST_DESTRUCTORFUNC)cg_upnp_subscriber_delete)

/**
 * Get the size (node count) of a subscriber list
 *
 * @param subList The subscriber list to evaluate
 */
#define cg_upnp_subscriberlist_size(subList) cg_list_size((CgList *)subList)

/**
 * Get the first item in a subscriber list (use as the first step in iteration).
 *
 * @param subList The subscriber list to evaluate
 */
#define cg_upnp_subscriberlist_gets(subList) (CgUpnpSubscriber *)cg_list_next((CgList *)subList)

/**
 * Add a new event subscriber to a subscriber list
 *
 * @param subList The subscriber list to add to
 * @param sub The subscriber to add
 */
#define cg_upnp_subscriberlist_add(subList, sub) cg_list_add((CgList *)subList, (CgList *)sub)

#ifdef  __cplusplus
}
#endif

#endif
