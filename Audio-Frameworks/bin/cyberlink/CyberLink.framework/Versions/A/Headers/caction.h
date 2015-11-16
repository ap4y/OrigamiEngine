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
*	File: caction.h
*
*	Revision:
*
*	02/23/05
*		- first revision
*
*	18-Jan-06 Heikki Junnila
*		- Added API comments
*
******************************************************************/

#ifndef _CG_UPNP_CACTION_H_
#define _CG_UPNP_CACTION_H_

#include <cybergarage/typedef.h>
#include <cybergarage/xml/cxml.h>
#include <cybergarage/util/clist.h>
#include <cybergarage/util/cstring.h>

#include <cybergarage/upnp/cargument.h>
#include <cybergarage/upnp/cupnp_status.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#define CG_UPNP_ACTION_ELEM_NAME "action"
#define CG_UPNP_ACTIONLIST_ELEM_NAME "actionList"

#define CG_UPNP_ACTION_NAME "name"

/****************************************
* Data Type
****************************************/

typedef struct _CgUpnpAction {
	BOOL headFlag;
	struct _CgUpnpAction *prev;
	struct _CgUpnpAction *next;
	void *parentService;
	CgXmlNode *actionNode;
	CgUpnpArgumentList *argumentList;
	/**** Execution Data ****/
	BOOL (*listener)(struct _CgUpnpAction *);
	CgUpnpStatus *upnpStatus;
	void *userData;
} CgUpnpAction, CgUpnpActionList;

typedef BOOL (*CG_UPNP_ACTION_LISTNER)(CgUpnpAction *);

/****************************************************************************
 * Function (Action)
 ****************************************************************************/

/**
 * Create a new action
 *
 * @return CgUpnpAction*
 */
CgUpnpAction *cg_upnp_action_new();

/**
 * Destroy an action
 *
 * @param action The action to destroy
 */
void cg_upnp_action_delete(CgUpnpAction *action);

/**
 * Get the next action in a list of actions. Use as an iterator.
 *
 * @param action The current action
 */
#define cg_upnp_action_next(action) (CgUpnpAction *)cg_list_next((CgList *)action)

/**
 * Check, whether the given XML node is an action node
 * 
 * @param node The CgXmlNode*
 */
#define cg_upnp_action_isactionnode(node) cg_xml_node_isname(node, CG_UPNP_ACTION_ELEM_NAME)

/****************************************************************************
 * XML Node
 ****************************************************************************/

/**
 * Set the action's XML node (i.e. the XML representation of the action)
 *
 * @param action The action in question
 * @param node The CgXmlNode*
 */
void cg_upnp_action_setactionnode(CgUpnpAction *action, CgXmlNode *node);

/**
 * Get the action's XML node (i.e. the XML representation of the action)
 *
 * @param action The action in question
 * @return CgXmlNode*
 */
#define cg_upnp_action_getactionnode(action) (action->actionNode)

/****************************************************************************
 * Service
 ****************************************************************************/

/**
 * Set the action's parent service
 *
 * @param action The action in question
 * @param service The action's parent service
 */
#define cg_upnp_action_setservice(action, service) (action->parentService = service)

/**
 * Get the action's parent service
 *
 * @param action The action in question
 * @return CgUpnpService*
 */
#define cg_upnp_action_getservice(action) ((CgUpnpService *)action->parentService)

/****************************************************************************
 * Name
 ****************************************************************************/

/**
 * Set the action's name
 *
 * @param action The action in question
 * @param value The action's name
 */
#define cg_upnp_action_setname(action, value) cg_xml_node_setchildnode(cg_upnp_action_getactionnode(action), CG_UPNP_ACTION_NAME, value)

/**
 * Get the action's name
 *
 * @param action The action in question
 * @return char*
 */
#define cg_upnp_action_getname(action) cg_xml_node_getchildnodevalue(cg_upnp_action_getactionnode(action), CG_UPNP_ACTION_NAME)

/**
 * Check, whether the action's name matches the one given as parameter.
 *
 * @param action The action in question
 * @param name The name string to compare
 */
#define cg_upnp_action_isname(action, name) cg_streq(cg_upnp_action_getname(action), name)

/****************************************************************************
 * Argument
 ****************************************************************************/

/**
 * Get the action's list of arguments
 *
 * @param action The action in question
 * @return CgUpnpArgumentList*
 */
#define cg_upnp_action_getargumentlist(action) (action->argumentList)

/**
 * Get the first argument from the action's list of arguments. Use as the
 * starting point in iteration loops.
 *
 * @param action The action in question
 * @return CgUpnpArgument*
 */
#define cg_upnp_action_getarguments(action) ((CgUpnpArgument *)cg_list_next((CgList *)action->argumentList))

/**
 * Find an argument from the action by the argument's name.
 *
 * @param action The action in question
 * @param name The name of the argument to look for
 * @return CgUpnpArgument* if successfull; otherwise NULL
 */
CgUpnpArgument *cg_upnp_action_getargumentbyname(CgUpnpAction *action, char *name);

/**
 * Find out, whether the action has an argument by the given name
 *
 * @param action The action in question
 * @param name The name to look for
 * @return TRUE if the action has the argument; otherwise FALSE
 */
#define cg_upnp_action_hasargumentbyname(action,name) ((cg_upnp_action_getargumentbyname(action,name) != NULL) ? TRUE : FALSE)

/**
 * Get a value of the specified argument from the action by the argument's name directly.
 *
 * @param action The action in question
 * @param name The name of the argument to look for
 * @return char* if successfull; otherwise NULL
 */
char *cg_upnp_action_getargumentvaluebyname(CgUpnpAction *action, char *name);

/**
 * Set a value of the specified argument from the action by the argument's name directly.
 *
 * @param action The action in question
 * @param name The name of the argument to look for
 * @param value The value to set
 * @return TRUE if successfull; otherwise NO
 */
BOOL cg_upnp_action_setargumentvaluebyname(CgUpnpAction *action, char *name, char *value);

/****************************************************************************
 * Function (Action - Execution Data)
 ****************************************************************************/

/****************************************************************************
 * UPnPStatus
 ****************************************************************************/

/**
 * Set the action's latest status code
 *
 * @param action The action in question
 * @param code The status code (int)
 */
#define cg_upnp_action_setstatuscode(action, code) cg_upnp_status_setcode(action->upnpStatus, code)

/**
 * Get the action's latest status code
 *
 * @param action The action in question
 * @return int
 */
#define cg_upnp_action_getstatuscode(action) cg_upnp_status_getcode(action->upnpStatus)

/**
 * Set the action's latest status description
 *
 * @param action The action in question
 * @param value The status description string
 */
#define cg_upnp_action_setstatusdescription(action, value) cg_upnp_status_setdescription(action->upnpStatus, value)

/**
 * Get the action's latest status description
 *
 * @param action The action in question
 * @return char*
 */
#define cg_upnp_action_getstatusdescription(action) cg_upnp_status_getdescription(action->upnpStatus)

/****************************************************************************
 * Listener
 ****************************************************************************/

/**
 * Set the action's listener function
 *
 * @param action The action in question
 * @param func The listener function of type: BOOL (*CG_UPNP_ACTION_LISTNER)(CgUpnpAction *)
 */
#define cg_upnp_action_setlistener(action, func) (action->listener = func)

/**
 * Get the action's listener function
 *
 * @param action The action in question
 * @return BOOL (*CG_UPNP_ACTION_LISTNER)(CgUpnpAction *)
 */
#define cg_upnp_action_getlistener(action) (action->listener)

/**
 * @deprecated Compatibility macro for typo correction
 */
#define cg_upnp_action_setlistner cg_upnp_action_setlistener

/**
 * @deprecated Compatibility macro for typo correction
 */
#define cg_upnp_action_getlistner cg_upnp_action_getlistener

/****************************************************************************
 * User Data
 ****************************************************************************/

/**
 * Set the action's arbitrary user data pointer
 *
 * @param action The action in question
 * @param value The user data pointer (void*)
 */
#define cg_upnp_action_setuserdata(action, value) (action->userData = value)

/**
 * Get the action's arbitrary user data pointer
 *
 * @param action The action in question
 * @return void*
 */
#define cg_upnp_action_getuserdata(action) (action->userData)

/****************************************************************************
 * Function (ActionList)
 ****************************************************************************/

/**
 * Create a new list of actions
 *
 * @return CgUpnpActionList*
 */
CgUpnpActionList *cg_upnp_actionlist_new();

/**
 * Destroy a list of actions
 *
 * @param actionList The list of actions to destroy
 */
void cg_upnp_actionlist_delete(CgUpnpActionList *actionList);

/**
 * Clear the contents of a list of actions
 *
 * @param actionList The list of actions to clear
 */
#define cg_upnp_actionlist_clear(actionList) cg_list_clear((CgList *)actionList, (CG_LIST_DESTRUCTORFUNC)cg_upnp_action_delete)

/**
 * Get the number of actions in a list of actions
 *
 * @param actionList The list in question
 * @return int
 */
#define cg_upnp_actionlist_size(actionList) cg_list_size((CgList *)actionList)

/**
 * Get the first action in a list of actions. Use as the starting point in
 * iteration loops.
 *
 * @param actionList The list in question
 * @return CgUpnpAction*
 */
#define cg_upnp_actionlist_gets(actionList) (CgUpnpAction *)cg_list_next((CgList *)actionList)

/**
 * Add an action to a list of actions
 *
 * @param actionList The list in question
 * @param action The action to add to the list
 */
#define cg_upnp_actionlist_add(actionList, action) cg_list_add((CgList *)actionList, (CgList *)action)

#ifdef  __cplusplus
}
#endif

#endif
