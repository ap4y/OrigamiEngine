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
*	File: cservice.h
*
*	Revision:
*
*	02/14/05
*		- first revision
*
*	09-Jan-06 Heikki Junnila
*		- Added service type parsing functions:
*		  cg_upnp_servicetype_getidentifier
*		  cg_upnp_servicetype_geturn
*		  cg_upnp_servicetype_getservice
*		  cg_upnp_servicetype_gettype
*		  cg_upnp_servicetype_getversion
*
*	10-Jan-06 Heikki Junnila
*		- Added cg_upnp_servicetype_getschematype() and fixed
*		  existing servicetype functions to accept 1-character strings
*
*	11-Jan-06 Heikki Junnila
*		- Removed cg_upnp_service_isname() and _getname() because
*		  according to UPnP specs, services have only types, not names
*
*	17-Jan-06 Heikki Junnila
*		- Added API comments
*	04/05/06 Theo Beisch
*		- changed timeout to use subscriber object
*		  when used by ControlPoint 
*		  modified execution data accordingly
*
******************************************************************/

#ifndef _CG_UPNP_CSERVICE_H_
#define _CG_UPNP_CSERVICE_H_

#include <cybergarage/typedef.h>
#include <cybergarage/xml/cxml.h>
#include <cybergarage/util/ctime.h>
#include <cybergarage/util/clist.h>
#include <cybergarage/util/cstring.h>
#include <cybergarage/util/cmutex.h>
#include <cybergarage/net/curl.h>

#include <cybergarage/upnp/caction.h>
#include <cybergarage/upnp/cstatevariable.h>
#include <cybergarage/upnp/ssdp/cssdp_server.h>
#include <cybergarage/upnp/event/csubscriber.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************************************************
* Define
*****************************************************************************/

#define CG_UPNP_SERVICE_ELEM_NAME "service"
#define CG_UPNP_SERVICELIST_ELEM_NAME "serviceList"

#define CG_UPNP_SERVICE_TYPE "serviceType"
#define CG_UPNP_SERVICE_ID "serviceId"
#define CG_UPNP_SERVICE_SCPDURL "SCPDURL"
#define CG_UPNP_SERVICE_CONTROL_URL "controlURL"
#define CG_UPNP_SERVICE_EVENT_SUB_URL "eventSubURL"
#define CG_UPNP_SERVICE_EVENT_KEY_INVALID 0
#define CG_UPNP_SERVICE_NOTIFY_WAITTIME 1000

/****************************************************************************
* Data Type
*****************************************************************************/

typedef struct _CgUpnpService {
	BOOL headFlag;
	struct _CgUpnpService *prev;
	struct _CgUpnpService *next;
	CgXmlNodeList *scpdNodeList;
	CgXmlNode *serviceNode;
	CgUpnpActionList *actionList;
	CgUpnpServiceStateTable *serviceStateTable;
	void *parentDevice;
	CgMutex *mutex;
#if !defined(CG_UPNP_NOUSE_SUBSCRIPTION)
	CgUpnpSubscriberList *subscriberList;
#endif
	BOOL parsed;
	/**** Execution Data ****/
	CgString *subscriptionSid;
	long subscriptionTimeout;

	void *userData;
	unsigned int eventKey;
	CgSysTime subscriptionTimestamp;
} CgUpnpService, CgUpnpServiceList;

/****************************************
* Function (Service)
****************************************/

/**
 * Create a new UPnP service
 */
CgUpnpService *cg_upnp_service_new();

/**
 * Destroy a UPnP service
 *
 * @param service The service to destroy
 */
void cg_upnp_service_delete(CgUpnpService *service);

/**
 * Clear the contents of a UPnP service
 *
 * @param service The service to clear
 */
void cg_upnp_service_clear(CgUpnpService *service);

/**
 * Get the next service in a service list. Use as an iterator.
 *
 * @param service Current service
 */
#define cg_upnp_service_next(service) (CgUpnpService *)cg_list_next((CgList *)service)

/**
 * Check, whether the given XML node is a service root node
 *
 * @param node CgXmlNode*
 * @return TRUE if the node is a service root node; otherwise FALSE
 */
#define cg_upnp_service_isservicenode(node) cg_xml_node_isname(node, CG_UPNP_SERVICE_ELEM_NAME)

/**
 * Get the service's SCPD node
 *
 * @param service The service in question
 */
#define cg_upnp_service_getscpdnode(service) cg_xml_nodelist_gets(service->scpdNodeList)

/**
 * Set the root XML node for the service
 *
 * @param service The service in question
 * @param node The node to set
 */
#define cg_upnp_service_setservicenode(service,node) (service->serviceNode = node)

/**
 * Get the root XML node for the service
 *
 * @param service The service in question
 */
#define cg_upnp_service_getservicenode(service) (service->serviceNode)

/**
 * Get the parent device's XML node for the service
 *
 * @param service The service in question
 */
#define cg_upnp_service_getdevicenode(service) cg_xml_node_getparentnode(cg_xml_node_getparentnode(service->serviceNode))

/**
 * Create the service's contents from the given XML document
 *
 * @param service The service to create
 * @param description The XML document to parse
 * @param descriptionLen The length of the XML document
 */
BOOL cg_upnp_service_parsedescription(CgUpnpService *service, char *description, int descriptionLen);

/**
 * Create the service's contents from the given URL. Gets the XML document
 * from the URL and passes it again to @ref cg_upnp_service_parsedescription
 * 
 * @param service The service to create
 * @param url The URL of the document to parse
 */
BOOL cg_upnp_service_parsedescriptionurl(CgUpnpService *service, CgNetURL *url);

/**
 * Get the service's description XML nodes as a string
 *
 * @param service The service in question
 * @param descStr The description string
 */
char *cg_upnp_service_getdescription(CgUpnpService *service, CgString *descStr);

/**
 * Get the service's parsing status (i.e. whether the service has been
 * completely constructed). Useful mainly in optimized control point mode.
 *
 * @param service The service in question
 */
#define cg_upnp_service_isparsed(service) (service->parsed)

/****************************************************************************
 * serviceType
 ****************************************************************************/

/**
 * Set the service's complete type string
 * (ex. "urn:schemas-upnp-org:service:FooService:1")
 *
 * @param service The service in question
 * @param value The type string
 */
#define cg_upnp_service_setservicetype(service, value) cg_xml_node_setchildnode(cg_upnp_service_getservicenode(service), CG_UPNP_SERVICE_TYPE, value)

/**
 * Get the service's complete type string
 * (ex. "urn:schemas-upnp-org:service:FooService:1")
 *
 * @param service The service in question
 * @return char*
 */
#define cg_upnp_service_getservicetype(service) cg_xml_node_getchildnodevalue(cg_upnp_service_getservicenode(service), CG_UPNP_SERVICE_TYPE)

/** 
 * Get the identifier-part of a service type string (usually "urn") 
 *
 * @param serviceType A service type string (usually the result from
 *	  \ref cg_upnp_service_getservicetype)
 *
 * @return A newly-created char* if successful; otherwise NULL
 */
char* cg_upnp_servicetype_getidentifier(char* serviceType);

/** 
 * Get the URN part of a service type string (usually "schemas-upnp-org") 
 *
 * @param serviceType A service type string (usually the result from
 *	  \ref cg_upnp_service_getservicetype)
 *
 * @return A newly-created char* if successful; otherwise NULL
 */
char* cg_upnp_servicetype_geturn(char* serviceType);

/** 
 * Get the service part of a service type string (usually just "service")
 *
 * @param serviceType A service type string (usually the result from
 *	  \ref cg_upnp_service_getservicetype)
 *
 * @return A newly-created char* if successful; otherwise NULL
 */
char* cg_upnp_servicetype_getservice(char* serviceType);

/** 
 * Get the type part of a service type string (ex. "ContentDirectory")
 *
 * @param serviceType A service type string (usually the result from
 *	  \ref cg_upnp_service_getservicetype)
 *
 * @return A newly-created char* if successful; otherwise NULL
 */
char* cg_upnp_servicetype_gettype(char* serviceType);

/** 
 * Get the schema type part of a service type string (without last colon)
 * (ex. "urn:schemas-upnp-org:service:ContentDirectory")
 *
 * @param serviceType A service type string (usually the result from
 *	  \ref cg_upnp_service_getservicetype)
 *
 * @return A newly-created char* if successful; otherwise NULL
 */
char* cg_upnp_servicetype_getschematype(char* serviceType);

/** 
 * Get the version part of a service type string (ex. "1")
 *
 * @param serviceType A service type string (usually the result from
 *	  \ref cg_upnp_service_getservicetype)
 *
 * @return A newly-created char* if successful; otherwise NULL
 */
char* cg_upnp_servicetype_getversion(char* serviceType);

/****************************************************************************
 * serviceID
 ****************************************************************************/

/**
 * Set the service's service ID
 *
 * @param service The service in question
 * @param value Service ID string
 */
#define cg_upnp_service_setserviceid(service, value) cg_xml_node_setchildnode(cg_upnp_service_getservicenode(service), CG_UPNP_SERVICE_ID, value)

/**
 * Get the service's service ID
 * 
 * @param service The service in question
 * @return char*
 */
#define cg_upnp_service_getserviceid(service) cg_xml_node_getchildnodevalue(cg_upnp_service_getservicenode(service), CG_UPNP_SERVICE_ID)

/****************************************************************************
 * SCPDURL
 ****************************************************************************/

/**
 * Set the service's SCPD (service description) URL
 *
 * @param service The service in question
 * @param value The SCPD URL string
 */
#define cg_upnp_service_setscpdurl(service, value) cg_xml_node_setchildnode(cg_upnp_service_getservicenode(service), CG_UPNP_SERVICE_SCPDURL, value)

/**
 * Get the service's SCPD (service description) URL
 *
 * @param service The service in question
 * @return CgNetURL Pointer to URL/URI structure
 */

CgNetURL *cg_upnp_service_getscpdurl(CgUpnpService *service);

/**
 * Compare the service's SCPD URL and the given location
 * 
 * @param service The service in question
 * @param url The URL (location) to compare
 * @return TRUE if location is found from URL; otherwise FALSE
 */
BOOL cg_upnp_service_isscpdurl(CgUpnpService *service, char *url);

/****************************************************************************
 * controlURL
 ****************************************************************************/

/**
 * Set the service's control URL
 *
 * @param service The service in question
 * @param value The control URL string
 */
#define cg_upnp_service_setcontrolurl(service, value) cg_xml_node_setchildnode(cg_upnp_service_getservicenode(service), CG_UPNP_SERVICE_CONTROL_URL, value)

/**
 * Get the service's control URL
 *
 * @param service The service in question
 * @return CgNetURL Pointer to URL/URI structure
 */
CgNetURL *cg_upnp_service_getcontrolurl(CgUpnpService *service);

/****************************************************************************
 * eventSubURL
 ****************************************************************************/

/**
 * Get the service's event subscription URL
 * 
 * @param service The service in question
 * @param value The event subscription URL string
 */
#define cg_upnp_service_seteventsuburl(service, value) cg_xml_node_setchildnode(cg_upnp_service_getservicenode(service), CG_UPNP_SERVICE_EVENT_SUB_URL, value)

/**
 * Get the service's event subscription URL
 *
 * @param service The service in question
 * @return CgNetURL Pointer to URL/URI structure
 */
CgNetURL *cg_upnp_service_geteventsuburl(CgUpnpService *service);

/****************************************************************************
 * Action
 ****************************************************************************/

/**
 * Get the service's list of actions (head of list)
 *
 * @param service The service in question
 */
CgUpnpActionList *cg_upnp_service_getactionlist(CgUpnpService *service);

/**
 * Get the first action in the service's list of actions. Use as the
 * starting point in iteration loops.
 *
 * @param service The service in question
 */
CgUpnpAction *cg_upnp_service_getactions(CgUpnpService *service);

/**
 * Get an action from the service by the action's name
 *
 * @param service The service in question
 * @param name The name of the action to look for
 * @return CgUpnpAction* or NULL
 */
CgUpnpAction *cg_upnp_service_getactionbyname(CgUpnpService *service, char *name);

/**
 * Check, whether a service has an action by the given name
 *
 * @param service The service in question
 * @param name The name of the action
 */
#define cg_upnp_service_hasactionbyname(service, name) ((cg_upnp_service_getactionbyname(service,name) != NULL) ? TRUE : FALSE)

/****************************************************************************
 * ServiceStateTable
 ****************************************************************************/

/**
 * Get the service's state table
 *
 * @param service The service in question
 */
CgUpnpServiceStateTable *cg_upnp_service_getservicestatetable(CgUpnpService *service);

/**
 * Get the first state variable in the service's state table. Use as the
 * starting point in iteration loops.
 *
 * @param service The service in question
 */
CgUpnpStateVariable *cg_upnp_service_getstatevariables(CgUpnpService *service);

/**
 * Get a state variable from the service's state table by the variable's name
 *
 * @param service The service in question
 * @param name the name of the variable
 * @return CgUpnpStateVariable* or NULL
 */
CgUpnpStateVariable *cg_upnp_service_getstatevariablebyname(CgUpnpService *service, char *name);

/**
 * Check, whether the service has the given state variable
 *
 * @param service The service in question
 * @param name The name of the state variable
 * @return TRUE if found; otherwise FALSE
 */
#define cg_upnp_service_hasstatevariablebyname(service, name) ((cg_upnp_service_getstatevariablebyname(service,name) != NULL) ? TRUE : FALSE)

/****************************************************************************
 * Advertisement
 ****************************************************************************/

/**
 * Send a service announcement (advertisement) from the given address
 *
 * @param service The service to advertise
 * @param bindAddr The address to attach to the announcement
 */
BOOL cg_upnp_service_announcefrom(CgUpnpService *service, char *bindAddr);

/**
 * Send a byebye announcement (i.e. a cancelling advertisement) from the given
 * address
 * 
 * @param service The service to announce from
 * @param bindAddr The address to attach to the announcement
 */
BOOL cg_upnp_service_byebyefrom(CgUpnpService *service, char *bindAddr);

/****************************************************************************
 * Listener 
 ****************************************************************************/

/**
 * Set an action listener to the service
 *
 * @param service The service in question
 * @param actionListener A callback function of type CG_UPNP_ACTION_LISTNER
 */
void cg_upnp_service_setactionlistener(CgUpnpService *service, CG_UPNP_ACTION_LISTNER actionListener);

/**
 * Set a query listener to the service
 *
 * @param service The service in question
 * @param queryListener A callback function of type CG_UPNP_STATEVARIABLE_LISTNER
 */
void cg_upnp_service_setquerylistener(CgUpnpService *service, CG_UPNP_STATEVARIABLE_LISTNER queryListener);

/****************************************************************************
 * Parent Device
 ****************************************************************************/

/**
 * Set the service's parent device
 *
 * @param service The service in question
 * @param dev The parent device
 */
#define cg_upnp_service_setdevice(service, dev) (service->parentDevice = dev)

/**
 * Get the service's parent device
 *
 * @param service The service in question
 * @return CgUpnpDevice*
 */
#define cg_upnp_service_getdevice(service) ((CgUpnpDevice *)service->parentDevice)

/****************************************************************************
 * Root Device
 ****************************************************************************/

/**
 * Get the service's top-most root device
 *
 * @param service The service in question
 * @return CgUpnpDevice*
 */
#define cg_upnp_service_getrootdevice(service) cg_upnp_device_getrootdevice((CgUpnpDevice *)service->parentDevice)

/****************************************************************************
 * Subscriber
 ****************************************************************************/

/**
 * Get the first node in the service's list of event subscribers. Use as the
 * starting point in iteration loops.
 *
 * @param service The service in question
 */
#define cg_upnp_service_getsubscribers(service) cg_upnp_subscriberlist_gets(service->subscriberList)

/**
 * Get the number of subscribers for the service
 * 
 * @param service The service in question
 * @return int
 */
#define cg_upnp_service_getnsubscribers(service) cg_upnp_subscriberlist_size(service->subscriberList)

/**
 * Add a subscriber to the service
 *
 * @param service The service in question
 * @param sub The subscriber
 */
BOOL cg_upnp_service_addsubscriber(CgUpnpService *service, CgUpnpSubscriber *sub);

/**
 * Remove a subscriber from the service
 *
 * @param service The service in question
 * @param sub The subscriber to remove
 */
BOOL cg_upnp_service_removesubscriber(CgUpnpService *service, CgUpnpSubscriber *sub);

/**
 * Find a subscriber from the service's list of subscribers by its SID
 * (Subscription ID)
 *
 * @param service The service in question
 * @param sid The SID
 */
CgUpnpSubscriber *cg_upnp_service_getsubscriberbysid(CgUpnpService *service, char *sid);

/****************************************************************************
 * Announce
 ****************************************************************************/

char *cg_upnp_service_getnotifyservicetypent(CgUpnpService *service, char *buf, int bufSize);
char *cg_upnp_service_getnotifyservicetypeusn(CgUpnpService *service, char *buf, int bufSize);

/****************************************************************************
 * Notify
 ****************************************************************************/

/**
 * Send a notification of a service's changed state variable
 *
 * @param service The service in question
 * @param statVar The changed state variable
 */
BOOL cg_upnp_service_notify(CgUpnpService *service, CgUpnpStateVariable *statVar);

/**
 * Send a notification message to all of the service's subscribers
 *
 * @param service The service in question
 */
BOOL cg_upnp_service_notifyall(CgUpnpService *service, BOOL doBracket );

/**
* Create a new thread to send a notification message to all of the service's subscribers
*
 * @param service The service in question
*  @param waitTime The wait time to send
*/
void cg_upnp_service_createnotifyallthread(CgUpnpService *service, CgSysTime waitTime);

/**
 * Send a notification message to all of the service's subscribers
 *
 * @param service The service in question
 * @deprecated Use cg_upnp_service_notifyall().
 */
#define cg_upnp_service_notifyallstatevariables(service) cg_upnp_service_notifyall(service, FALSE);

/****************************************************************************
 * Mutex
 ****************************************************************************/

/**
 * Lock the service's mutex
 *
 * @param service The service in question
 */
#define cg_upnp_service_lock(service) cg_mutex_lock(service->mutex)

/**
 * Unlock the service's mutex
 *
 * @param service The service in question
 */
#define cg_upnp_service_unlock(service) cg_mutex_unlock(service->mutex)

/****************************************************************************
 * SID
 ****************************************************************************/

/**
 * Set the service's subscription's SID (subscription ID)
 *
 * @param service The service in question
 * @param value The SID
 */
#define cg_upnp_service_setsubscriptionsid(service, value) cg_string_setvalue(service->subscriptionSid, value)

/**
 * Get the service's SID (subscription ID)
 *
 * @param service The service in question
 * @return char*
 */
#define cg_upnp_service_getsubscriptionsid(service) cg_string_getvalue(service->subscriptionSid)

/**
 * Check, whether the service has any subscribers
 *
 * @param service The service in question
 * @return TRUE if at least one subscriber is found; otherwise FALSE
 */
#define cg_upnp_service_issubscribed(service) ((0 < cg_string_length(service->subscriptionSid)) ? TRUE : FALSE)

/**
 * Clear (i.e. empty) the service's SID (subscription ID)
 *
 * @param service The service in question
 */
#define cg_upnp_service_clearsubscriptionsid(service) cg_string_setvalue(service->subscriptionSid, "")

/****************************************************************************
 * Timeout
 ****************************************************************************/

/**
 * Set the service's subscription timeout (expiration) value
 *
 * @param service The service in question
 * @param value The timeout value
 */
#define cg_upnp_service_setsubscriptiontimeout(service, value) (service->subscriptionTimeout = value)

/**
 * Get the service's subscription timeout (expiration) value
 *
 * @param service The service in question
 * @return long
 */
#define cg_upnp_service_getsubscriptiontimeout(service) (service->subscriptionTimeout)

/****************************************************************************
 * User Data
 ****************************************************************************/

/**
 * Set the service's arbitrary user data pointer
 *
 * @param service The service in question
 * @param value The user data pointer
 */
#define cg_upnp_service_setuserdata(service, value) (service->userData = value)

/**
 * Get the service's arbitrary user data pointer
 *
 * @param service The service in question
 * @return void*
 */
#define cg_upnp_service_getuserdata(service) (service->userData)

/****************************************************************************
 * Event Key
 ****************************************************************************/

/**
 * Set the service's event key (i.e. the sequential number used in events)
 *
 * @param service The service in question
 * @param value The event key value (uint)
 */
#define cg_upnp_service_seteventkey(service, value) (service->eventKey = value)

/**
 * Get the service's event key
 *
 * @param service The service in question
 * @return uint
 */
#define cg_upnp_service_geteventkey(service) (service->eventKey)

/****************************************************************************
 * Timestamp
 ****************************************************************************/

/**
 * Set the service's subscription time stamp
 *
 * @param service The service in question
 * @param value The time stamp
 */
#define cg_upnp_service_setsubscriptiontimestamp(service, value) (service->subscriptionTimestamp = value)

/**
 * Get the service's subscription time stamp
 *
 * @param service The service in question
 * @return CgSysTime
 */
#define cg_upnp_service_getsubscriptiontimestamp(service) (service->subscriptionTimestamp)

/**
 * Get the service's subscription expiration time
 *
 * @param service The service in question
 * @return CgSysTime
 */
#define cg_upnp_service_getsubscriptionexpiration(service) (service->subscriptionTimestamp + service->subscriptionTimeout - CG_UPNP_SUBSCRIPTION_DELAY)


/****************************************************************************
 * Function (ServiceList)
 ****************************************************************************/

/**
 * Create a new list of services
 */
CgUpnpServiceList *cg_upnp_servicelist_new();

/**
 * Destroy a list of services
 *
 * @param serviceList The list of services to destroy
 */
void cg_upnp_servicelist_delete(CgUpnpServiceList *serviceList);


/**
 * Clear the contents of a list of services
 *
 * @param serviceList The list of services to clear
 */
#define cg_upnp_servicelist_clear(serviceList) cg_list_clear((CgList *)serviceList, (CG_LIST_DESTRUCTORFUNC)cg_upnp_service_delete)

/**
 * Get the number of services in a list of services
 *
 * @param serviceList The list of services
 * @return int
 */
#define cg_upnp_servicelist_size(serviceList) cg_list_size((CgList *)serviceList)

/**
 * Get the first service in a list of services. Use as the starting point in
 * iteration loops.
 *
 * @param serviceList The service list
 */
#define cg_upnp_servicelist_gets(serviceList) (CgUpnpService *)cg_list_next((CgList *)serviceList)

/**
 * Add a service to a list of services
 * 
 * @param serviceList The list to add the service to
 * @param service The service to add to the list
 */
#define cg_upnp_servicelist_add(serviceList, service) cg_list_add((CgList *)serviceList, (CgList *)service)

/****************************************************************************
 * Function (SSDP)
 ****************************************************************************/

/**
 * The service's handler function that deals with received SSDP messages.
 *
 * @param service The service in question
 * @param ssdpPkt The received SSDP packet
 */
void cg_upnp_service_ssdpmessagereceived(CgUpnpService *service, CgUpnpSSDPPacket *ssdpPkt);

#ifdef  __cplusplus
}
#endif

#endif
