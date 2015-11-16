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
*	File: cproperty.h
*
*	Revision:
*
*	06/21/05
*		- first revision
*
******************************************************************/

#ifndef _CG_UPNP_CPROPERTY_H_
#define _CG_UPNP_CPROPERTY_H_

#include <cybergarage/typedef.h>

#include <cybergarage/util/clist.h>
#include <cybergarage/util/cstring.h>
#include <cybergarage/net/curl.h>

#include <cybergarage/upnp/cstatevariable.h>

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * Type definition for UPnP property
 */
typedef struct _CgUpnpProperty {
	BOOL headFlag;
	struct _CgUpnpProperty *prev;
	struct _CgUpnpProperty *next;

	CgString *name;
	CgString *value;
	CgString *sid;
	long seq;
} CgUpnpProperty, CgUpnpPropertyList;

/****************************************
* Function (Property)
****************************************/

/**
 * Construct new UPnP property object
 */
CgUpnpProperty *cg_upnp_property_new();

/**
 * Delete UPnP property object
 *
 * @param prop UPnP property
 */
void cg_upnp_property_delete(CgUpnpProperty *prop);

/**
 * Clear UPnP property
 *
 * @param prop UPnP property
 */
void cg_upnp_property_clear(CgUpnpProperty *prop);

/**
 * Fetches next UPnP property from property list
 *
 * @param prop UPnP property
 */
#define cg_upnp_property_next(prop) (CgUpnpProperty *)cg_list_next((CgList *)prop)

/**** name ****/
/**
 * Set property name
 *
 * @param prop UPnP property
 * @param value Name
 */
#define cg_upnp_property_setname(prop, value) cg_string_setvalue(prop->name, value)

/**
 * Get property name
 *
 * @param prop UPnP property
 */
#define cg_upnp_property_getname(prop) cg_string_getvalue(prop->name)

/**** value ****/

/**
 * Set value for UPnP property
 *
 * @param prop UPnP property
 * @param nvalue Value
 */
#define cg_upnp_property_setvalue(prop, nvalue) cg_string_setvalue(prop->value, nvalue)

/**
 * Get value from property
 *
 * @param prop UPnP property
 */
#define cg_upnp_property_getvalue(prop) cg_string_getvalue(prop->value)

/**** sid ****/

/**
 * Set SID for UPnP property
 *
 * @param prop UPnP property
 * @param value Value
 */
#define cg_upnp_property_setsid(prop, value) cg_string_setvalue(prop->sid, value)

/**
 * Get SID from UPnP property
 *
 * @param prop UPnP property
 */
#define cg_upnp_property_getsid(prop) cg_string_getvalue(prop->sid)

/**** seq ****/

/**
 * Set sequence number for UPnP property
 *
 * @param prop UPnP property
 * @param value
 */
#define cg_upnp_property_setseq(prop, value) (prop->seq = value)

/**
 * Get sequence number from UPnP property
 *
 * @param prop UPnP property
 */
#define cg_upnp_property_getseq(prop) (prop->seq)

/****************************************
* Function (PropertyList)
****************************************/

/**
 * Create new UPnP property list
 */
CgUpnpPropertyList *cg_upnp_propertylist_new();

/**
 * Delete UPnP property list
 *
 * @param propertylist UPnP property list
 */
void cg_upnp_propertylist_delete(CgUpnpPropertyList *propertylist);

/**
 * Clear UPnP propertylist
 *
 * @param propList UPnP property list
 */
#define cg_upnp_propertylist_clear(propList) cg_list_clear((CgList *)propList, (CG_LIST_DESTRUCTORFUNC)cg_upnp_property_delete)

/**
 * Get size of property list
 *
 * @param propList UPnP property list
 */
#define cg_upnp_propertylist_size(propList) cg_list_size((CgList *)propList)

/**
 * Get next list element from property list
 *
 * @param propList UPnP property list
 */
#define cg_upnp_propertylist_gets(propList) (CgUpnpProperty *)cg_list_next((CgList *)propList)

/**
 * Add new property into property list
 *
 * @param propList UPnP property list
 * @param prop UPnP property
 */
#define cg_upnp_propertylist_add(propList, prop) cg_list_add((CgList *)propList, (CgList *)prop)

#ifdef  __cplusplus
}
#endif

#endif
