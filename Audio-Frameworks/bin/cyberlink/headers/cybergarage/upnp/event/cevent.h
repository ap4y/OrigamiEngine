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
*	File: cevent.h
*
*	Revision:
*
*	06/20/05
*		- first revision
*	03/28/06 Theo Beisch
*		- modified cg_upnp_eventlistenerlist_clear to use
*		  default destructor in clist
*		- added cg_upnp_event_subscription_request_hasnt
*		- changed _subscription function signature to use (service)
*		  instead of (remoteAddress)
*
******************************************************************/

#ifndef _CG_UPNP_CEVENT_H_
#define _CG_UPNP_CEVENT_H_

#include <cybergarage/typedef.h>

#include <cybergarage/http/chttp.h>
#include <cybergarage/util/cstring.h>
#include <cybergarage/util/clist.h>
#include <cybergarage/upnp/event/cproperty.h>
#include <cybergarage/upnp/cservice.h>
#include <cybergarage/upnp/cupnp_function.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

/**
 * Definition for SID size
 */
#define CG_UPNP_SUBSCRIPTION_SID_SIZE (CG_UPNP_UUID_MAX_LEN + 8)

/**
 * Definition for SID header size
 */
#define CG_UPNP_SUBSCRIPTION_SID_HEADER_SIZE (5 + CG_UPNP_SUBSCRIPTION_SID_SIZE)

/**
 * Definition for subscription xml namespace
 */
#define CG_UPNP_SUBSCRIPTION_XMLNS "urn:schemas-upnp-org:event-1-0"

/**
 * Definition for subscription timeout header prefix
 */
#define CG_UPNP_SUBSCRIPTION_TIMEOUT_HEADER "Second-"

/**
 * Definition for infinite string in subscription
 */
#define CG_UPNP_SUBSCRIPTION_INFINITE_STRING "infinite"

/**
 * Definition for infinite value in subscription
 */
#define CG_UPNP_SUBSCRIPTION_INFINITE_VALUE (-1)

/**
 * Definition for subscription uuid prefix
 */
#define CG_UPNP_SUBSCRIPTION_UUID "uuid:"

/**
 * Definition for subscription callback start tag
 */
#define CG_UPNP_SUBSCRIPTION_CALLBACK_START_WITH  "<"

/**
 * Definition for subscription callback end tag
 */
#define CG_UPNP_SUBSCRIPTION_CALLBACK_END_WITH  ">"

/**
 * Definition for subscription delay
 */
#define CG_UPNP_SUBSCRIPTION_DELAY 30

/**
 * Definition for maximum event sequence number
 */
#define CG_UPNP_EVENT_MAX_SEQ 2147483647

/**
 * Definition for maximum upnp subscription timeout
 */
#define CG_UPNP_SUBSCRIPTION_MAX_TIMEOUT 300
	
/****************************************
* Data Type
****************************************/

/**
 * Type definition for subsription request
 */
typedef CgHttpRequest CgUpnpSubscriptionRequest;

/**
 * Type definition for subsrciption response
 */
typedef CgHttpResponse CgUpnpSubscriptionResponse;

/**
 * Type definition for event listener callback
 */
typedef void (*CG_UPNP_EVENT_LISTENER)(CgUpnpProperty *);

/**
 * Type definition for event listener list
 */
typedef struct _CgUpnpEventListenerList {
	/** Used by cg_list_* functions to indicate start of list */
	BOOL headFlag;
	/** Used by cg_list_* functions to point to the previous item in list */
	struct _CgUpnpEventListenerList *prev;
	/** Used by cg_list_* functions to point to the next item in list */
	struct _CgUpnpEventListenerList *next;
 
	CG_UPNP_EVENT_LISTENER listener;
} CgUpnpEventListenerList;
 
/****************************************
* Function
****************************************/

/**
 * Create timeout header string
 *
 * @param time Timeout
 * @param buf Buffer used when creating header string
 *
 * @return Timeout header C string
 */
char *cg_upnp_event_subscription_totimeoutheaderstring(long time, CgString *buf);

/**
 * Get event subscription timeout
 *
 * @param headerValue Header value
 *
 * @return Timeout
 */
long cg_upnp_event_subscription_gettimeout(char *headerValue);

/**
 * Create SID for subscription
 *
 * @param buf Character buffer where SID is created
 * @param bufSize Buffer size.
 *
 * @return Pointer to buffer containing SID
 */
char * cg_upnp_event_subscription_createsid(char *buf, int bufSize);

/**
 * Creates SID header string
 *
 * @param sid Character array containing the SID
 * @param buf A buffer where header string is created
 * @param bufSize Size of the used buffer
 *
 * @return Pointer to buffer containing SID header string
 */
char *cg_upnp_event_subscription_tosidheaderstring(char *sid, char *buf, int bufSize);

/**
 * Get SID
 *
 * @param headerValue Raw header C string
 * 
 * @return C string containing SID
 */
char *cg_upnp_event_subscription_getsid(char *headerValue);

/****************************************
* Function (Request)
****************************************/

/**
 * Subscription request constructor
 *
 * @return Pointer to newly created subsription request
 */
#define cg_upnp_event_subscription_request_new() cg_http_request_new();

/**
 * Subscription request destructor
 *
 * @param subReq Reference to the subscription request to be deleted
 */
#define cg_upnp_event_subscription_request_delete(subReq) cg_http_request_delete(subReq);

/**
 * Checks if request type is subscription request
 *
 * @param subReq The request to be checked
 *
 * @return True if request is subscription request, false otherwise
 */
#define cg_upnp_event_subscription_issubscriberequest(subReq) cg_streq(cg_http_request_getmethod(subReq), CG_HTTP_SUBSCRIBE)

/**
 * Checks if request type is unsubscription request
 *
 * @param subReq The request to be checked
 *
 * @return True if request is unsubscription request
 */
#define cg_upnp_event_subscription_isunsubscriberequest(subReq) cg_streq(cg_http_request_getmethod(subReq), CG_HTTP_UNSUBSCRIBE)

/**** SID ****/

/**
 * Set sid to subscription request
 *
 * @param subReq Subscription request
 * @param sid SID (Subscription id)
 */
void cg_upnp_event_subscription_request_setsid(CgUpnpSubscriptionRequest *subReq, char *sid);

/**
 * Get sid from subscription request
 *
 * @param subReq Subscription request
 *
 * @return Character array containing the requested SID
 */
#define cg_upnp_event_subscription_request_getsid(subReq) cg_upnp_event_subscription_getsid(cg_http_packet_getheadervalue(((CgHttpPacket*)subReq), CG_HTTP_SID))

/**
 * Checks if subscription object has SID
 *
 * @param subReq Subscription request
 *
 * @return True if subscription has SID, false otherwise
 */
#define cg_upnp_event_subscription_request_hassid(subReq) cg_http_packet_hasheader(((CgHttpPacket*)subReq), CG_HTTP_SID)

/**** NT ****/

/**
 * Set NT (Notification target) for subscription request  
 *
 * @param subReq Subscription request
 * @param value Notification type
 */
#define cg_upnp_event_subscription_request_setnt(subReq, value) cg_http_packet_setheadervalue(((CgHttpPacket*)subReq), CG_HTTP_NT, value)

/**
 * Get NT from subscription request
 *
 * @param subReq Subscription request
 *
 * @return Character array containing the NT header value
 */
#define cg_upnp_event_subscription_request_getnt(subReq) cg_http_packet_getheadervalue(((CgHttpPacket*)subReq), CG_HTTP_NT)

/**
 * Checks if subscription request has NT
 *
 * @param subReq Subscription request
 *
 * @return True if subscription request contains NT, false otherwise
 */
#define cg_upnp_event_subscription_request_hasnt(subReq) cg_http_packet_hasheader(((CgHttpPacket*)subReq), CG_HTTP_NT)

/**** Callback ****/

/**
 * Set subscription request callback URL
 *
 * @param subReq Subscription request
 * @param value Callback URL string
 */
#define cg_upnp_event_subscription_request_setcallback(subReq, value) cg_http_packet_setheadervalue(((CgHttpPacket*)subReq), CG_HTTP_CALLBACK, value)

/**
 * Get subscription callback URL
 *
 * @param subReq Subscription request
 *
 * @return Character array containing the callback URL
 */
#define cg_upnp_event_subscription_request_getcallback(subReq) cg_http_packet_getheadervalue(((CgHttpPacket*)subReq), CG_HTTP_CALLBACK)

/**
 * Checks if subscription request has callback URL
 *
 * @param subReq Subscription request
 *
 * @return True if subscription request has callback URL, false otherwise
 */
#define cg_upnp_event_subscription_request_hascallback(subReq) cg_http_packet_hasheader(((CgHttpPacket*)subReq), CG_HTTP_CALLBACK)

/**** Timeout ****/

/**
 * Set timeout for subscription request
 *
 * @param subReq Subscription request
 * @param timeout Timeout value for subscription request
 */
void cg_upnp_event_subscription_request_settimeout(CgUpnpSubscriptionRequest *subReq, long timeout);

/**
 * Get timeout value from subscription request
 *
 * @param subReq Subscription request
 */
#define cg_upnp_event_subscription_request_gettimeout(subReq) cg_upnp_event_subscription_gettimeout(cg_http_packet_getheadervalue(((CgHttpPacket*)subReq), CG_HTTP_TIMEOUT))

/**** Subscription Content ****/

/**
 * (Re)initializes subscription request
 *
 * @param subReq Subscription request
 * @param service Service to be subscribed
 * @param callback URL to be called when service state changes
 * @param timeout Subscription timeout
 */
void cg_upnp_event_subscription_request_setnewsubscription(CgUpnpSubscriptionRequest *subReq, CgUpnpService *service, char *callback, long timeout);

/**
 * (Re)initializes subscription request to be renewal request
 *
 * @param subReq Subscription request
 * @param service Service to be subscribed
 * @param uuid Devices unique identification string
 * @param timeout Subscription timeout
 */
void cg_upnp_event_subscription_request_setrenewsubscription(CgUpnpSubscriptionRequest *subReq, CgUpnpService *service, char *uuid, long timeout);

/**
 * (Re)initializes subscription request to be unsubscription request
 *
 * @param subReq Subscription request
 * @param service Service where to be unsubscribed
 */
void cg_upnp_event_subscription_request_setunsubscription(CgUpnpSubscriptionRequest *subReq, CgUpnpService *service);

/**** Host ****/

/**
 * Set HTTP host header value
 *
 * @param subReq Subscription request
 * @param value HTTP Host: header content
 */
#define cg_upnp_event_subscription_request_sethost(subReq, value) cg_http_packet_setheadervalue(((CgHttpPacket*)subReq), CG_HTTP_HOST, value)

/**
 * Get host header value from subscription request
 *
 * @param subReq Subscription request
 */
#define cg_upnp_event_subscription_request_gethost(subReq) cg_http_packet_getheadervalue(((CgHttpPacket*)subReq), CG_HTTP_HOST)

/**
 * Send subscription request to device
 *
 * @param subReq Subscription request
 */
#define cg_upnp_event_subscription_request_post(subReq) cg_http_request_post(subReq, cg_net_url_gethost(cg_http_request_getposturl(subReq)), cg_net_url_getport(cg_http_request_getposturl(subReq)))

/**
 * Send response to subscription request
 *
 * @param subReq Subscription request
 * @param subRes Subscription response
 */
#define cg_upnp_event_subscription_request_postresponse(subReq, subRes) cg_http_request_postresponse(subReq, subRes)

/****************************************
* Function (Response)
****************************************/

/**
 * Constructor for subscription response
 */
#define cg_upnp_event_subscription_response_new() cg_http_response_new();

/**
 * Destructor for subscription response
 *
 * @param subRes Subscription response
 */
#define cg_upnp_event_subscription_response_delete(subRes) cg_http_response_delete(subRes);

/**
 * Checks if subscription was successfull by checking status code from subscription response
 *
 * @param subRes Subscription response
 */
#define cg_upnp_event_subscription_response_issuccessful(subRes) cg_http_response_issuccessful(subRes)

/**** Status Code ****/

/**
 * Set status code for subscription response
 *
 * @param subRes Subscription response
 * @param code Status code
 */
#define cg_upnp_event_subscription_response_setstatuscode(subRes, code) cg_http_response_setstatuscode(subRes, code)

/**
 * Get status code from subscription response object
 *
 * @param subRes Subscription response
 * @param code Status code
 *
 * @return Status code
 *
 * \bug Status code is not needed here! (It is not even passed to the wrapped function)
 */
#define cg_upnp_event_subscription_response_getstatuscode(subRes, code) cg_http_response_getstatuscode(subRes)

/**** SID ****/

/**
 * Set SID (Subscription ID) for response
 *
 * @param subRes Subscription response
 * @param sid Subscription ID
 */
void cg_upnp_event_subscription_response_setsid(CgUpnpSubscriptionResponse *subRes, char *sid);

/**
 * Get SID
 *
 * @param subRes Subscription request
 *
 * @return C string containing SID
 */
#define cg_upnp_event_subscription_response_getsid(subRes) cg_upnp_event_subscription_getsid(cg_http_packet_getheadervalue(((CgHttpPacket*)subRes), CG_HTTP_SID))

/**** Timeout ****/

/**
 * Set HTTP timeout for subscription response
 *
 * @param subRes Subscription response
 * @param value Timeout
 */
void cg_upnp_event_subscription_response_settimeout(CgUpnpSubscriptionResponse *subRes, long value);

/**
 * Get timeout value from subscription response
 *
 * @param subRes Subscription response
 *
 * @return C string containing the timeout
 */
#define cg_upnp_event_subscription_response_gettimeout(subRes) cg_upnp_event_subscription_gettimeout(cg_http_packet_getheadervalue(((CgHttpPacket*)subRes), CG_HTTP_TIMEOUT))

/**
 * Set response code for subscription response
 *
 * @param subRes Subscription response
 * @param code Response code
 */
void cg_upnp_event_subscription_subscriberesponse_setresponse(CgUpnpSubscriptionResponse *subRes, int code);
/* TODO getter? */

/****************************************
* Function (Eventlistener list)
****************************************/
 
/**
* Create a new event listener list
*
*/
CgUpnpEventListenerList *cg_upnp_eventlistenerlist_new();
 
/**
* Delete a event listener list.
*
* \param eventListenerList The event listener list to delete
*
*/
void cg_upnp_eventlistenerlist_delete(CgUpnpEventListenerList *eventListenerList);
 
/**
* Clear the contents of a event listener list.
*
* \param eventListenerList The device list to clear
*
*/
#define cg_upnp_eventlistenerlist_clear(eventListenerList) cg_list_clear((CgList *)eventListenerList, (CG_LIST_DESTRUCTORFUNC)free)
 
/**
* Get the size of the device list
*
* \param eventListenerList The device list
*
*/
#define cg_upnp_eventlistenerlist_size(eventListenerList) cg_list_size((CgList *)eventListenerList)
 
/**
* Fetches next list element from event listener list
*
* @param eventListenerList Event listener list
*/
#define cg_upnp_eventlistenerlist_gets(eventListenerList) (CgUpnpEventListenerList*)cg_list_next((CgList *)eventListenerList)

/**
* Fetches next list element from event listener list
*
* @param eventListenerList Event listener list
*/
#define cg_upnp_eventlistenerlist_next(eventListenerList) (CgUpnpEventListenerList*)cg_list_next((CgList *)eventListenerList)
 
/**
* Remove a listener from the event listener list
*
* \param eventListenerList The event listener list
* \param listener The listener to remove
*
*/
void cg_upnp_eventlistenerlist_remove(CgUpnpEventListenerList* eventListenerList, CG_UPNP_EVENT_LISTENER listener);
 
/**
* Add a listener to the event listener list
*
* \param eventListenerList The event listener list
* \param listener The listener to add
*
*/
void cg_upnp_eventlistenerlist_add(CgUpnpEventListenerList* eventListenerList, CG_UPNP_EVENT_LISTENER listener);
 
/**
 * Call all event listeners in the list with the given data.
 *
 * @param eventListenerList The list to iterate thru
 * @param property The property that has been evented
 */
void cg_upnp_eventlistenerlist_notify(CgUpnpEventListenerList* eventListenerList, CgUpnpProperty *property);


#ifdef  __cplusplus
}
#endif

#endif
