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
*	File: cstatevariable.h
*
*	Revision:
*
*	02/23/05
*		- first revision
*
******************************************************************/

#ifndef _CG_UPNP_CSTATEVARIABLE_H_
#define _CG_UPNP_CSTATEVARIABLE_H_

#include <cybergarage/typedef.h>
#include <cybergarage/xml/cxml.h>
#include <cybergarage/util/clist.h>
#include <cybergarage/util/cstring.h>

#include <cybergarage/upnp/cupnp_status.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

/**
 * Definition for state variable XML element name
 */
#define CG_UPNP_STATEVARIABLE_ELEM_NAME "stateVariable"

/**
 * Definition for service state table XML element name
 */
#define CG_UPNP_SERVICESTATETALBE_ELEM_NAME "serviceStateTable"

/**
 * Definition for state variable name element
 */
#define CG_UPNP_STATEVARIABLE_NAME "name"

/**
 * Definition for state variable data type element
 */
#define CG_UPNP_STATEVARIABLE_DATATYPE "dataType"

/**
 * Definition for state variable send events element name
 */
#define CG_UPNP_STATEVARIABLE_SENDEVENTS "sendEvents"

/**
 * Definition for state variable send events element value
 */
#define CG_UPNP_STATEVARIABLE_SENDEVENTS_YES "yes"

/**
 * Definition for state variable send events element value
 */
#define CG_UPNP_STATEVARIABLE_SENDEVENTS_NO "no"
	
/**
 * Definition for state variable allowed values list
 */	
#define CG_UPNP_STATEVARIABLE_ALLOWEDVALUELIST "allowedValueList"


/****************************************
* Data Type
****************************************/

/**
  * Data type for allowed state variables allowed values table
  */
typedef struct _CgUpnpAllowedValuesList {
	BOOL headFlag;
	struct _CgUpnpAllowedValuesList *prev;
	struct _CgUpnpAllowedValuesList *next;
	CgString *value;
} CgUpnpAllowedValuesList, CgUpnpAllowedValue;

	
/**
 * Data type for state variable and state table
 */
typedef struct _CgUpnpStateVariable {
	BOOL headFlag;
	struct _CgUpnpStateVariable *prev;
	struct _CgUpnpStateVariable *next;
	CgUpnpAllowedValuesList* allowedValueList;
	//tb: fixmelater verify type CgUpnpService* ???
	void *parentService;
	CgXmlNode *stateVarNode;
	/* Dummy pointers to maintain ABI, Please remove during the
	 * appropriate development stage */
	void *dummy1;
	void *dummy2;
	/**** Execution Data ****/
	CgString *value;
	BOOL (*listener)(struct _CgUpnpStateVariable *);
	CgUpnpStatus *upnpStatus;
	void *userData;
	
} CgUpnpStateVariable, CgUpnpServiceStateTable;

	
/**
 * Type definition for state variable listener function
 */
typedef BOOL (*CG_UPNP_STATEVARIABLE_LISTNER)(CgUpnpStateVariable *);

/****************************************
* Function (StateVariable)
****************************************/

/**
 * Create new state variable object
 */
CgUpnpStateVariable *cg_upnp_statevariable_new();

/**
 * Delete state variable object
 *
 * @param statVar state variable
 */
void cg_upnp_statevariable_delete(CgUpnpStateVariable *statVar);

/**
 * Get next element from state variable list
 *
 * @param statVar state variable list node
 */
#define cg_upnp_statevariable_next(statVar) (CgUpnpStateVariable *)cg_list_next((CgList *)statVar)

/**
 * Check if XML node is state variable node
 *
 * @param node XML node
 */
#define cg_upnp_statevariable_isstatevariablenode(node) cg_xml_node_isname(node, CG_UPNP_STATEVARIABLE_ELEM_NAME)

/**** XML Node ****/

/**
 * Set XML node for this state variable
 *
 * @param statVar State variable
 * @param node XML node
 */
void cg_upnp_statevariable_setstatevariablenode(CgUpnpStateVariable *statVar, CgXmlNode *node);

/**
 * Get XML node from this state variable
 *
 * @param statVar State variable
 */
#define cg_upnp_statevariable_getstatevariablenode(statVar) (statVar->stateVarNode)

/**** Service ****/

/**
 * Set service for this state variable
 *
 * @param statVar State variable
 * @param service Service
 */
#define cg_upnp_statevariable_setservice(statVar, service) (statVar->parentService = service)

/**
 * Get service from state variable
 *
 * @param statVar State variable
 *
 * @return UPnP service
 */
#define cg_upnp_statevariable_getservice(statVar) ((CgUpnpService *)statVar->parentService)

/**** allowedValueList ****/

/**
 * Get allowed value list
 *
 * @param statVar State variable
 *
 * @return Allowed value list
 */
CgUpnpAllowedValuesList*  cg_upnp_statevariable_getallowedvaluelist(CgUpnpStateVariable* statVar);

/**
 * Check if state variable has allowed value list
 *
 * @param statVar State variable
 *
 * @return 0 if it does not have allowed value list integer greater than zero otherwise
 */
#define cg_upnp_statevariable_hasallowedvaluelist(statVar) cg_upnp_allowedvaluelist_size(statVar->allowedValueList)
	
/**
 * Check if state variable allowes to receive value
 *
 * @parap value value to be checked
 * @param statVar State variable
 *
 * @return 0 if it does allowed to receive value
*/
int cg_upnp_statevariable_is_allowed_value(CgUpnpStateVariable* statVar, const char* value);
	
	
/**
 * Create new allowed state variable values list
 */
CgUpnpAllowedValuesList* cg_upnp_allowedvaluelist_new();

/**** allowedValueRange ****/

/**
 * Get allowed value range for state variable
 *
 * @param statVar State variable
 *
 * @return Allowed value range
 */
#define cg_upnp_statevariable_getallowedvaluerange(statVar) (statVar->allowedValueRange)

/**
 * Checks if state variable has allowed variable range
 *
 * @param statVar State variable
 *
 * @return True if allowed value range exists, false otherwise
 */
#define cg_upnp_statevariable_hasallowedvaluerange(statVar) ((statVar->allowedValueRange != NULL) ? TRUE : FALSE)

/**** name ****/

/**
 * Set state variable name
 *
 * @param statVar State variable
 * @param value New state variable name
 */
#define cg_upnp_statevariable_setname(statVar, value) cg_xml_node_setchildnode(cg_upnp_statevariable_getstatevariablenode(statVar), CG_UPNP_STATEVARIABLE_NAME, value)

/**
 * Get state variable name
 *
 * @param statVar State variable
 *
 * @return State variable name
 */
#define cg_upnp_statevariable_getname(statVar) cg_xml_node_getchildnodevalue(cg_upnp_statevariable_getstatevariablenode(statVar), CG_UPNP_STATEVARIABLE_NAME)

/**
 * Checks if state variable has certain name
 *
 * @param statVar State variable
 * @param name Name to be checked
 *
 * @return True if State variable name is same as provided, false otherwise
 */
#define  cg_upnp_statevariable_isname(statVar, name) cg_streq(cg_upnp_statevariable_getname(statVar), name)

/**** dataType ****/

/**
 * Set data type for state variable
 *
 * @param statVar State variable
 * @param value Data type
 */
#define cg_upnp_statevariable_setdatatype(statVar, value) cg_xml_node_setchildnode(cg_upnp_statevariable_getstatevariablenode(statVar), CG_UPNP_STATEVARIABLE_DATATYPE, value)

/**
 * Get data type from state variable
 *
 * @param statVar State variable
 *
 * @return Data type
 */
#define cg_upnp_statevariable_getdatatype(statVar) cg_xml_node_getchildnodevalue(cg_upnp_statevariable_getstatevariablenode(statVar), CG_UPNP_STATEVARIABLE_DATATYPE)

/**** sendEvents ****/

/**
 * Set send events option for state variable, can be used to disable eventing 
 * for certain variables.
 *
 * @param statVar State variable
 * @param value New value
 */
#define cg_upnp_statevariable_setsendevents(statVar, value) cg_xml_node_setattribute(cg_upnp_statevariable_getstatevariablenode(statVar), CG_UPNP_STATEVARIABLE_SENDEVENTS, value)

/**
 * Get state variable event sending option value
 *
 * @param statVar State variable
 * 
 * @return Current value associated to state variable
 */
#define cg_upnp_statevariable_getsendevents(statVar) cg_xml_node_getattributevalue(cg_upnp_statevariable_getstatevariablenode(statVar), CG_UPNP_STATEVARIABLE_SENDEVENTS)

/**
 * Check if events are sent to subscribers when this state variable changes
 *
 * @param statVar State variable
 *
 * @return True if events are sent, false otherwise
 */
#define cg_upnp_statevariable_issendevents(statVar) (cg_streq(cg_upnp_statevariable_getsendevents(statVar), CG_UPNP_STATEVARIABLE_SENDEVENTS_YES))

/****************************************
* Function (StateVariable - Execution Data)
****************************************/

/**** value ****/

/**
 * Set state variable value
 *
 * @param statVar State variable
 * @param data New value associated with this state variable, and the value is send to other devices which subscribe the service.
 */
void cg_upnp_statevariable_setvalue(CgUpnpStateVariable *statVar, char *data);

/**
 * Set state variable value
 *
 * @param statVar State variable
 * @param data New value associated with this state variable
 */
void cg_upnp_statevariable_setvaluewithoutnotify(CgUpnpStateVariable *statVar, char *data);

/**
 * Get value associated with this state variable
 *
 * @param statVar State variable
 *
 * @return Value associated with this state variable
 */
#define cg_upnp_statevariable_getvalue(statVar) cg_string_getvalue(statVar->value)

/*** listener ****/

/**
 * Set listener callback for this state variable
 *
 * @param statVar State variable
 * @param func Listener function
 */
#define cg_upnp_statevariable_setlistener(statVar, func) (statVar->listener = func)

/**
 * Get lister function associated with state variable
 *
 * @param statVar State variable
 */
#define cg_upnp_statevariable_getlistener(statVar) (statVar->listener)

/**** UPnPStatus ****/

/**
 * Set status code for state variable
 *
 * @param statVar State variable
 * @param code Status code
 */
#define cg_upnp_statevariable_setstatuscode(statVar, code) cg_upnp_status_setcode(statVar->upnpStatus, code)

/** 
 * Get status code of this state variable
 *
 * @param statVar Status variable
 */
#define cg_upnp_statevariable_getstatuscode(statVar) cg_upnp_status_getcode(statVar->upnpStatus)

/**
 * Set status description for state variable
 *
 * @param statVar State variable
 * @param value Status description
 */
#define cg_upnp_statevariable_setstatusdescription(statVar, value) cg_upnp_status_setdescription(statVar->upnpStatus, value)

/**
 * Get status description from this state variable
 *
 * @param statVar State variable
 */
#define cg_upnp_statevariable_getstatusdescription(statVar) cg_upnp_status_getdescription(statVar->upnpStatus)

/**** User Data****/

/**
 * Set arbitrary user data associated with this state variable
 *
 * @param statVar State variable
 * @param value Data
 */
#define cg_upnp_statevariable_setuserdata(statVar, value) (statVar->userData = value)

/**
 * Get user data from this state variable
 *
 * @param statVar State variable
 *
 * @return User data
 */
#define cg_upnp_statevariable_getuserdata(statVar) (statVar->userData)

/****************************************
* Function (ServiceStateTable)
****************************************/

/**
 * Create service state table object
 */
CgUpnpServiceStateTable *cg_upnp_servicestatetable_new();

/**
 * Delete service state table object
 *
 * @param servicestatetable Service state table
 */
void cg_upnp_servicestatetable_delete(CgUpnpServiceStateTable *servicestatetable);

/**
 * Clear service state table
 *
 * @param stateTable Service state table
 */
#define cg_upnp_servicestatetable_clear(stateTable) cg_list_clear((CgList *)stateTable, (CG_LIST_DESTRUCTORFUNC)cg_upnp_statevariable_delete)

/**
 * Get size of service state table
 *
 * @param stateTable Service state table
 */
#define cg_upnp_servicestatetable_size(stateTable) cg_list_size((CgList *)stateTable)

/**
 * Get next service state table from service state table list
 *
 * @param stateTable Service state table
 *
 * @return Service state table (node)
 */
#define cg_upnp_servicestatetable_gets(stateTable) (CgUpnpStateVariable *)cg_list_next((CgList *)stateTable)

/**
 * Add new service state table into service state table list
 *
 * @param stateTable Service state table
 * @param statVar State variable
 */
#define cg_upnp_servicestatetable_add(stateTable, statVar) cg_list_add((CgList *)stateTable, (CgList *)statVar)

#ifdef  __cplusplus
}
#endif

#endif
