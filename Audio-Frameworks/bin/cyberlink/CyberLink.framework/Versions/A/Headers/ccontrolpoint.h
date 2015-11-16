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
*	File: ccontrolpoint.h
*
*	Revision:
*
*	05/25/05
*		- first revision
*
*	11-Jan-06 Heikki Junnila
*		- Removed cg_upnp_device_isname and _getbyname because
*		  according to UPnP specs UDN, type or friendlyname is
*		  not the same as the device's name.
*		- Added cg_upnp_controlpoint_getdevicebyudn()
*	04/02/06 Theo Beisch
*		- added support for resubscriber 
*		  and device disposer
*		- added cg_upnp_controlpoint_getservicebysid
*		- added cg_upnp_controlpoint_waitforlock 
*	22-Apr-2008 
*		- Added cg_upnp_controlpoint_getdevice() to get a device by the index.
*
******************************************************************/

#ifndef _CG_UPNP_CCONTROLPOINT_H_
#define _CG_UPNP_CCONTROLPOINT_H_

#include <cybergarage/util/cstring.h>
#include <cybergarage/util/cmutex.h>
#include <cybergarage/util/ccond.h>
#include <cybergarage/xml/cxml.h>
#include <cybergarage/net/cinterface.h>

#include <cybergarage/upnp/cdevice.h>
#include <cybergarage/upnp/cargument.h>
#include <cybergarage/upnp/cupnp_status.h>
#include <cybergarage/upnp/ssdp/cssdp_server.h>
#include <cybergarage/upnp/event/cevent.h>
#include <cybergarage/upnp/event/cnotify.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

/** Definition for SSDP default search MX */
#define CG_UPNP_CONTROLPOINT_SSDP_DEFAULT_SEARCH_MX 3

/** Definition for control point SSDP minimum delay */
#define CG_UPNP_CONTROLPOINT_SSDP_MIN_DELAY 20

/** Definition for control point default response port */
#define CG_UPNP_CONTROLPOINT_SSDP_RESPONSE_DEFAULT_PORT 39400

/** Definition for numeric constant to specify the max tries to open a SSDP Response Port (rosfran.borges) */
#define CG_UPNP_CONTROLPOINT_SSDP_RESPONSE_PORT_MAX_TRIES_INDEX 80

/** Definition for HTTP event default port */
#define CG_UPNP_CONTROLPOINT_HTTP_EVENT_DEFAULT_PORT 39500

/** Definition for default controlpoint event callback "location" */ 
#define CG_UPNP_CONTROLPOINT_HTTP_EVENTSUB_URI "/eventSub"

/****************************************
* Data Type
****************************************/

/**
 * Device listener status parameters.
 * 
 * @ref CgUpnpDeviceStatusAdded means a device has been added to local cache.
 * @ref CgUpnpDeviceStatusUpdated means a device description has been updated
 * because its IP address or description has changed. The user should renew
 * any active subscriptions if this has been received.
 *
 * @ref CgUpnpDeviceStatusInvalid is basically the same as @ref CgUpnpDeviceStatusRemoved,
 * but @ref CgUpnpDeviceStatusRemoved is sent only when a device leaves the
 * network in a proper way (i.e. with UPnP ByeBye messages). The user should
 * immediately cease using the device, because it will be removed in both cases.
 */
typedef enum _CgUpnpDeviceStatus
{
	CgUpnpDeviceStatusAdded = 0,
	CgUpnpDeviceStatusUpdated,
	CgUpnpDeviceStatusInvalid,
	CgUpnpDeviceStatusRemoved
} CgUpnpDeviceStatus;

/**
 * Prototype for control point's device listener callback.
 * 
 * @param udn The UDN of the device, that the status update concerns
 * @param status The new status
 */
	
typedef struct _CgUpnpControlPoint {
	CgMutex *mutex;
	CgXmlNodeList *deviceRootNodeList;
	CgUpnpDeviceList *deviceList;
	CgUpnpSSDPServerList *ssdpServerList;
	CgUpnpSSDPResponseServerList *ssdpResServerList;
	CgHttpServerList *httpServerList;
	void (*deviceListener)(struct _CgUpnpControlPoint *, char*, CgUpnpDeviceStatus); /* CG_UPNP_DEVICE_LISTENER */
	CG_HTTP_LISTENER httpListener;
	CG_UPNP_SSDP_LISTNER ssdpListener;
	CG_UPNP_SSDP_RESPONSE_LISTNER ssdpResListener;
	CgUpnpEventListenerList* eventListeners;
	int ssdpResPort;
	CgString *httpEventURI;
	int httpEventPort;
	int ssdpSearchMx;
	void *userData;
	
	/* Expiration handling */
	CgThread *expThread;
	CgMutex *expMutex;
	CgCond *expCond;
	
	/** List of cached interfaces */
	CgNetworkInterfaceList *ifCache;
} CgUpnpControlPoint;

typedef void (*CG_UPNP_DEVICE_LISTENER)(CgUpnpControlPoint *ctrlPoint, char* udn, CgUpnpDeviceStatus status);
	
/****************************************************************************
 * Control Point top-level control
 ****************************************************************************/

/**
 * Create a new control point. Does not start any threads.
 *
 * @return A newly-created CgUpnpControlPoint
 */
CgUpnpControlPoint *cg_upnp_controlpoint_new();

/**
 * Destroy the given control point
 *
 * @param ctrlPoint The control point struct to destroy
 */
void cg_upnp_controlpoint_delete(CgUpnpControlPoint *ctrlPoint);

/**
 * Activate the control point. Starts listening for SSDP messages etc.
 * You must call this function before you can actually use a control point.
 *
 * @param ctrlPoint The control point to start
 *
 * @return TRUE if successful; otherwise FALSE
 *
 */
BOOL cg_upnp_controlpoint_start(CgUpnpControlPoint *ctrlPoint);

/**
 * Stop the control point. Stops sending/receiveing/responding to any messages.
 *
 * @param ctrlPoint The control point to stop
 *
 * @return TRUE if successful; otherwise FALSE
 *
 */
BOOL cg_upnp_controlpoint_stop(CgUpnpControlPoint *ctrlPoint);

/**
* Check if  the control point is activated.
*
* @param ctrlPoint The control point to stop
*
* @return TRUE if running; otherwise FALSE
*
*/
BOOL cg_upnp_controlpoint_isrunning(CgUpnpControlPoint *ctrlPoint);
	
/****************************************************************************
 * Control Point locking
 ****************************************************************************/

/**
 * Lock the control point's mutex. 
 * The control point should be ALWAYS locked, when a CgUpnpDevice*,
 * CgUpnpService*, CgUpnpAction* or other pointer has been taken into use from
 * the stack. This effectively prevents devices/services from being updated/
 * removed or added while the control point is locked. You should release the
 * lock as soon as possible with @ref cg_upnp_controlpoint_unlock
 *
 * @note Do NOT save any CgUpnp* pointers to user-space variables. Use them
 * only as local variables (inside one function) after gaining a mutex lock.
 * Release the lock as soon as you are done accessing the pointer, and then
 * discard the pointer immediately.
 *
 * @param ctrlPoint The control point in question
 */

#if defined(WITH_THREAD_LOCK_TRACE) && defined(__USE_ISOC99)
#define cg_upnp_controlpoint_lock(ctrlPoint) cg_mutex_lock_trace(__FILE__,  __LINE__, __PRETTY_FUNCTION__, ctrlPoint->mutex)
#else
BOOL cg_upnp_controlpoint_lock(CgUpnpControlPoint *ctrlPoint);
#endif
/**
 * Release a previously locked control point mutex.
 * See @ref cg_upnp_controlpoint_lock for a more detailed description on
 * the control point locking mechanism.
 *
 * @param ctrlPoint The control point in question
 */
#if defined(WITH_THREAD_LOCK_TRACE) && defined(__USE_ISOC99)
#define cg_upnp_controlpoint_unlock(ctrlPoint) cg_mutex_unlock_trace(__FILE__,  __LINE__, __PRETTY_FUNCTION__, ctrlPoint->mutex)
#else
BOOL cg_upnp_controlpoint_unlock(CgUpnpControlPoint *ctrlPoint);
#endif

/****************************************************************************
 * Device searching
 ****************************************************************************/

/**
 * Find a device from the control point by the exact type of the device.
 * This function searches for devices, whose *complete type string*
 * matches the given string, including version number. For example: 
 * "urn:schemas-upnp-org:device:FooDevice:1". If you need to disregard
 * the version, use @ref cg_upnp_controlpoint_getdevicebytype
 *
 * @param ctrlPoint Controlpoint in question
 * @param exacttype Type of the device
 *
 */
CgUpnpDevice *cg_upnp_controlpoint_getdevicebyexacttype(CgUpnpControlPoint *ctrlPoint,
						   char *exacttype);

/**
 * Find a device from the controlpoint by the type of the device.
 * This function searches for devices, whose *type part* (i.e. not including
 * the version) of the device type string matches the given string.
 * For example: "urn:schemas-upnp-org:device:FooDevice". If you need
 * to know the version of a device, use @ref cg_upnp_devicetype_getversion
 *
 * @param ctrlPoint Controlpoint in question
 * @param type Type of the device
 *
 */
CgUpnpDevice *cg_upnp_controlpoint_getdevicebytype(CgUpnpControlPoint *ctrlPoint,
						   char *type);

/**
 * Find a device from the controlpoint by the UDN of the device.
 *
 * @param ctrlPoint Controlpoint in question
 * @param type Type of the device
 *
 */
CgUpnpDevice *cg_upnp_controlpoint_getdevicebyudn(CgUpnpControlPoint *ctrlPoint,
						   char *udn);

/****************************************************************************
 * SSDP response
 ****************************************************************************/

/**
 * Get the list of SSDP servers associated to the control point
 *
 * @param ctrlPoint The control point in question
 */
#define cg_upnp_controlpoint_getssdpserverlist(ctrlPoint) (ctrlPoint->ssdpServerList)

/**
 * Get the list of SSDP response servers associated to the control point
 *
 * @param ctrlPoint The control point in question
 */
#define cg_upnp_controlpoint_getssdpresponseserverlist(ctrlPoint) (ctrlPoint->ssdpResServerList)

/**
 * Get the list of HTTP servers associated to the control point
 *
 * @param ctrlPoint The control point in question
 */
#define cg_upnp_controlpoint_gethttpserverlist(ctrlPoint) (ctrlPoint->httpServerList)

/****************************************************************************
 * SSDP listener
 ****************************************************************************/

/**
 * Set an SSDP message listener for the control point. Use this function to
 * get SSDP messages to user-space applications.
 *
 * @param ctrlPoint The control point in question
 * @param func A callback function that is of type @ref CG_UPNP_SSDP_LISTNER
 */
#define cg_upnp_controlpoint_setssdplistener(ctrlPoint, func) (ctrlPoint->ssdpListener = func)

/**
 * Get the SSDP message listener for the control point. 
 *
 * @param ctrlPoint The control point in question
 * @return A callback function that is of type @ref CG_UPNP_SSDP_LISTNER or NULL
 */
#define cg_upnp_controlpoint_getssdplistener(ctrlPoint) (ctrlPoint->ssdpListener)

/**
 * Set an SSDP response listener for the control point. Use this function to
 * get SSDP responses to user-space applications.
 *
 * @param ctrlPoint The control point in question
 * @param func A callback function that is of type @ref CG_UPNP_SSDP_RESPONSE_LISTNER
 */
#define cg_upnp_controlpoint_setssdpresponselistener(ctrlPoint, func) (ctrlPoint->ssdpResListener = func)

/**
 * Get the SSDP response listener for the control point.
 *
 * @param ctrlPoint The control point in question
 * @return A callback function that is of type @ref CG_UPNP_SSDP_RESPONSE_LISTNER or NULL
 */
#define cg_upnp_controlpoint_getssdpresponselistener(ctrlPoint) (ctrlPoint->ssdpResListener)

/**
 * Set device listener for the control point.
 *
 * @param ctrlPoint The control point
 * @param func A callback function that is type @ref CG_UPNP_DEVICE_LISTENER or NULL
 */
#define cg_upnp_controlpoint_setdevicelistener(ctrlPoint, func) (ctrlPoint->deviceListener = func)

/**
 * Get the device listener for the control point.
 *
 * @param ctrlPoint The control point in question
 * @return A callback function that is of type @ref CG_UPNP_DEVICE_LISTENER or NULL
 */
#define cg_upnp_controlpoint_getdevicelistener(ctrlPoint) (ctrlPoint->deviceListener)

/****************************************************************************
 * Event listening
 ****************************************************************************/

/**
 * Set an event listener for the control point. Use this function to
 * get event notifications to user-space applications.
 *
 * @param ctrlPoint The control point in question
 * @param listener A callback function that is of type @ref CG_UPNP_EVENT_LISTENER
 */
#define cg_upnp_controlpoint_addeventlistener(ctrlPoint, listener) (cg_upnp_eventlistenerlist_add(ctrlPoint->eventListeners, listener))

/**
 * Remove an event listener from the control point.
 *
 * @param ctrlPoint The control point in question
 * @param listener The callback function to remove, that is of type @ref CG_UPNP_EVENT_LISTENER
 */
#define cg_upnp_controlpoint_removeeventlistener(ctrlPoint, listener) (cg_upnp_eventlistenerlist_remove(ctrlPoint->eventListeners, listener))

/**
 * Set a single event listener for the control point. Use this function to
 * get event notifications to user-space applications if you need only one listener.
 *
 * @param ctrlPoint The control point in question
 * @param evlistener The callback function to set, that is of type @ref CG_UPNP_EVENT_LISTENER, or NULL
 */
#define cg_upnp_controlpoint_seteventlistener(ctrlPoint, evlistener) \
	do {\
		if (evlistener == NULL) \
			cg_upnp_eventlistenerlist_remove(ctrlPoint->eventListeners, ctrlPoint->eventListeners->next->listener); \
		else \
			cg_upnp_eventlistenerlist_add(ctrlPoint->eventListeners, evlistener); \
	} while(0)

/**
 * Get the single event listener for the control point.
 *
 * @param ctrlPoint The control point in question
 * @return The callback function, that is of type @ref CG_UPNP_EVENT_LISTENER,
 *         or NULL if there is no listener
 */
#define cg_upnp_controlpoint_geteventlistener(ctrlPoint) (ctrlPoint->eventListeners->next->listener)

/**
 * Get the list of event listeners for the control point.
 *
 * @param ctrlPoint The control point in question
 * @return List of @ref CG_UPNP_EVENT_LISTENER functions
 */
#define cg_upnp_controlpoint_geteventlisteners(ctrlPoint) (ctrlPoint->eventListeners)

/****************************************************************************
 * SSDP Response port
 ****************************************************************************/

/**
 * Set the IP port number used for the control point's SSDP responses
 *
 * @param ctrlPoint The control point in question
 * @param port The IP port number
 */
#define cg_upnp_controlpoint_setssdpresponseport(ctrlPoint, port) (ctrlPoint->ssdpResPort = port)

/**
 * Get the IP port number used for the control point's SSDP responses
 *
 * @param ctrlPoint The control point in question
 */
#define cg_upnp_controlpoint_getssdpresponseport(ctrlPoint) (ctrlPoint->ssdpResPort)

/****************************************************************************
 * Eventing
 ****************************************************************************/

/**
 * Set the IP port number for the control point's event reception
 *
 * @param ctrlPoint The control point in question
 * @param port The IP port number
 */
#define cg_upnp_controlpoint_seteventport(ctrlPoint, port) (ctrlPoint->httpEventPort = port)

/**
 * Get the IP port number for the control point's event reception
 *
 * @param ctrlPoint The control point in question
 */
#define cg_upnp_controlpoint_geteventport(ctrlPoint) (ctrlPoint->httpEventPort)

/**
 * Set the URI used for the control point's event subscription notifications
 *
 * @param ctrlPoint The control point in question
 * @param uri The URI to set
 */
#define cg_upnp_controlpoint_seteventsuburi(ctrlPoint, uri) cg_string_setvalue(ctrlPoint->httpEventURI, uri)

/**
 * Get the URI used for the control point's event subscription notifications
 *
 * @param ctrlPoint The control point in question
 */
#define cg_upnp_controlpoint_geteventsuburi(ctrlPoint) cg_string_getvalue(ctrlPoint->httpEventURI)

/****************************************************************************
 * M-SEARCH
 ****************************************************************************/

/**
 * Do an M-SEARCH to look for devices in the network.
 *
 * @param ctrlPoint The control point in question
 * @param target The Search Target parameter (ex. "ssdp:all")
 */
BOOL cg_upnp_controlpoint_search(CgUpnpControlPoint *ctrlPoint, char *target);

/**
 * Set the MX-parameter used for SSDP searches i.e. Set the time to wait 
 * (in seconds) for device responses to an M-SEARCH
 *
 * @param ctrlPoint The control point in question
 * @param value MX; Time to wait
 */
#define cg_upnp_controlpoint_setssdpsearchmx(ctrlPoint, value) (ctrlPoint->ssdpSearchMx = value)

/**
 * Get the MX-parameter used for SSDP searches
 *
 * @param ctrlPoint The control point in question
 * @return value MX; Time to wait
 */
#define cg_upnp_controlpoint_getssdpsearchmx(ctrlPoint) (ctrlPoint->ssdpSearchMx)

/****************************************************************************
 * HTTP listener
 ****************************************************************************/

/**
 * Set an HTTP listener function to the control point. Use this function to
 * get HTTP requests to user-space applications.
 *
 * @param ctrlPoint The control point in question
 * @param func The listener function, that is of type @ref CG_HTTP_LISTENER
 */
#define cg_upnp_controlpoint_sethttplistener(ctrlPoint, func) (ctrlPoint->httpListener = func)

/**
 * Get the HTTP listener function for the control point.
 *
 * @param ctrlPoint The control point in question
 * @return The listener function, that is of type @ref CG_HTTP_LISTENER
 */
#define cg_upnp_controlpoint_gethttplistener(ctrlPoint) (ctrlPoint->httpListener)

/**
 * The function that calls all HTTP listener callback functions. Do not call
 * this from applications.
 *
 * @param httpReq The received HTTP request
 */
void cg_upnp_controlpoint_httprequestreceived(CgHttpRequest *httpReq);

/**
 * When an event is received, update also the associated service's
 * state table to include the evented state variable.
 *
 * @param service The service, whose state table to update
 * @param prop The evented property from which to update
 */
void cg_upnp_controlpoint_updatestatetablefromproperty(CgUpnpService* service,
						       CgUpnpProperty* prop);
						       
/****************************************************************************
 * User Data
 ****************************************************************************/
 
/**
 * Set the user data pointer (arbitrary user data) to the control point.
 * 
 * @param ctrlPoint The control point in question
 * @param value Arbitrary user data
 */
#define cg_upnp_controlpoint_setuserdata(ctrlPoint, value) (ctrlPoint->userData = value)

/**
 * Get the user data pointer (arbitrary user data) from the control point.
 * 
 * @param dev The control point in question
 * @return Pointer to user data or NULL
 */
#define cg_upnp_controlpoint_getuserdata(ctrlPoint) (ctrlPoint->userData)

/****************************************************************************
 * Service SCPD
 ****************************************************************************/

/**
 * Parse the service description from the service's SCPD URL. Do not call
 * this from user applications.
 * 
 * @param service The service in question
 * @return TRUE if successful; otherwise FALSE
 */
BOOL cg_upnp_controlpoint_parsescservicescpd(CgUpnpService *service);

/**
 * Parse the device's services using the received SSDP packet. Do not call this
 * from user applications.
 *
 * @param dev The device in question
 * @param ssdpPkt An SSDP packet
 */
BOOL cg_upnp_controlpoint_parseservicesfordevice(CgUpnpDevice *dev, CgUpnpSSDPPacket *ssdpPkt);

/****************************************************************************
 * Device adding/removal by SSDP packets
 ****************************************************************************/

/**
 * Add a device on the basis of an SSDP packet. Do not call this from user
 * applications.
 *
 * @param ctrlPoint The control point, whose device list to handle
 * @param ssdpPkt The received SSDP packet
 */
void cg_upnp_controlpoint_adddevicebyssdppacket(CgUpnpControlPoint *ctrlPoint,
						CgUpnpSSDPPacket *ssdpPkt);

/**
 * Remove a device on the basis of an SSDP packet. Do not call this from user
 * applications.
 *
 * @param ctrlPoint The control point, whose device list to handle
 * @param ssdpPkt The received SSDP packet
 */
void cg_upnp_controlpoint_removedevicebyssdppacket(CgUpnpControlPoint *ctrlPoint,
						   CgUpnpSSDPPacket *ssdpPkt);

/****************************************
 * Subscription
 ****************************************/

/**
 * Subscribe to a service's events
 * 
 * @param ctrlPoint The control point in use
 * @param service The service to subscribe to
 * @param timeout Timeout for subscription expiration/renewal
 * @return TRUE if successful; otherwise FALSE
 */
BOOL cg_upnp_controlpoint_subscribe(CgUpnpControlPoint *ctrlPoint, CgUpnpService *service, long timeout);

/**
 * Re-subscribe to a service's events (i.e. renew subscription)
 * 
 * @param ctrlPoint The control point in use
 * @param service The service to subscribe to
 * @param timeout Timeout for subscription expiration/renewal
 * @return TRUE if successful; otherwise FALSE
 */
BOOL cg_upnp_controlpoint_resubscribe(CgUpnpControlPoint *ctrlPoint, CgUpnpService *service, long timeout);

/**
 * Unsubscribe to a service's events (i.e. cancel subscription)
 * 
 * @param ctrlPoint The control point in use
 * @param service The service to unsubscribe to
 * @return TRUE if successful; otherwise FALSE
 */
BOOL cg_upnp_controlpoint_unsubscribe(CgUpnpControlPoint *ctrlPoint, CgUpnpService *service);

/**
 * Subscribe to all of the device's services' events
 * 
 * @param ctrlPoint The control point in use
 * @param dev The device to subscribe to
 * @param timeout Timeout for subscription expiration/renewal
 * @return TRUE if successful; otherwise FALSE
 */
BOOL cg_upnp_controlpoint_subscribeall(CgUpnpControlPoint *ctrlPoint, CgUpnpDevice *dev, long timeout);

/**
 * Re-subscribe to all of the device's services' events (i.e. renew subscription)
 * 
 * @param ctrlPoint The control point in use
 * @param dev The device to subscribe to
 * @param timeout Timeout for subscription expiration/renewal
 * @return TRUE if successful; otherwise FALSE
 */
BOOL cg_upnp_controlpoint_resubscribeall(CgUpnpControlPoint *ctrlPoint, CgUpnpDevice *dev, long timeout);

/**
 * Unsubscribe to all of the device's services' events (i.e. cancel subscription)
 * 
 * @param ctrlPoint The control point in use
 * @param dev The device to unsubscribe to
 * @return TRUE if successful; otherwise FALSE
 */
BOOL cg_upnp_controlpoint_unsubscribeall(CgUpnpControlPoint *ctrlPoint, CgUpnpDevice *dev);

/****************************************
 * Function (DeviceList)
 ****************************************/

/**
 * Get the head of the control point's list of devices (use for iteration)
 * 
 * @param ctrlPoint The control point in use
 * @return The head of the device list
 */
#define cg_upnp_controlpoint_getdevices(ctrlPoint) cg_upnp_devicelist_gets(ctrlPoint->deviceList)

/**
 * Get the number of devices known by the control point
 * 
 * @param ctrlPoint The control point in use
 * @return The number of devices in the control point's device list
 */
#define cg_upnp_controlpoint_getndevices(ctrlPoint) cg_upnp_devicelist_size(ctrlPoint->deviceList)

/**
 * Get the head of the control point's list of devices (use for iteration)
 * 
 * @param ctrlPoint The control point in use
 * @param index The index of the device to get
 * @return The specified device
 */
#define cg_upnp_controlpoint_getdevice(ctrlPoint, index) cg_upnp_devicelist_get(ctrlPoint->deviceList, index)

/**
 * Add a device to the control point's list of devices. Do not call this from
 * user applications.
 * 
 * @param ctrlPoint The control point in use
 * @param dev The device to add
 */
#define cg_upnp_controlpoint_adddevice(ctrlPoint, dev) cg_upnp_devicelist_add(ctrlPoint->deviceList, dev)

/****************************************
 * Environment handling
 ****************************************/

/**
 * Worker thread for handling expirations.
 *
 * \param thread the thread in question
 */
void cg_upnp_controlpoint_expirationhandler(CgThread *thread);

/**
 * Notify the control point that any IP of the host has been changed.
 *
 * \param ctrlpoint The control point in use
 * \return success of changing used interfaces
 */
BOOL cg_upnp_controlpoint_ipchanged(CgUpnpControlPoint *ctrlpoint);


/**
 * Get the event subscription callback URI
 *
 * @param ctrlPoint The control point in question
 */
char *cg_upnp_controlpoint_geteventsubcallbackurl(CgUpnpControlPoint *ctrlPoint, char *ifaddr, char *buf, int bufLen);

#ifdef  __cplusplus
}
#endif

#endif /* _CG_UPNP_CCONTROLPOINT_H_ */
