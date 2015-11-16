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
*	File: ccontrol.h
*
*	Re1vision:
*
*	04/19/05
*		- first revision
*
******************************************************************/

#ifndef _CG_UPNP_CCONTROL_H_
#define _CG_UPNP_CCONTROL_H_

#include <cybergarage/typedef.h>

#include <cybergarage/util/cstring.h>
#include <cybergarage/soap/csoap.h>
#include <cybergarage/upnp/cservice.h>
#include <cybergarage/upnp/caction.h>
#include <cybergarage/upnp/cargument.h>
#include <cybergarage/upnp/cstatevariable.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

/**** MAN ****/

/**
 * Definition for mandatory discover header
 */
#define CG_UPNP_MAN_DISCOVER "\"ssdp:discover\""

/**** NT ****/

/**
 * Definition for NT (Notification type) root device
 */
#define CG_UPNP_NT_ROOTDEVICE "upnp:rootdevice"

/**
 * Definition for NT event
 */
#define CG_UPNP_NT_EVENT "upnp:event"

/**** NTS ****/

/**
 * Definition for NTS alive
 */
#define CG_UPNP_NTS_ALIVE "ssdp:alive"

/**
 * Definition for NTS byebye
 */
#define CG_UPNP_NTS_BYEBYE "ssdp:byebye"

/**
 * Definition for NTS property change
 */
#define CG_UPNP_NTS_PROPCHANGE "upnp:propchange"

/**** ST ****/

/**
 * Definition for general ST (search target)
 */
#define CG_UPNP_ST_ALL_DEVICE "ssdp:all"

/**
 * Definition for root device ST
 */
#define CG_UPNP_ST_ROOT_DEVICE "upnp:rootdevice"

/**
 * Definition for ST UUID (universally unique identifier) prefix
 */
#define CG_UPNP_ST_UUID_DEVICE "uuid"

	/**
 * Definition for URN device
 */
#define CG_UPNP_ST_URN "urn"

/**
 * Definition for URN device
 */
#define CG_UPNP_ST_URN_DEVICE "urn:schemas-upnp-org:device"

/**
 * Definition for URN service
 */
#define CG_UPNP_ST_URN_SERVICE "urn:schemas-upnp-org:service"

/**** USN ****/

/**
 * Definition for USN rootdevice
 */
#define CG_UPNP_USN_ROOTDEVICE "upnp:rootdevice"

/**
 * Definition for control name space
 */
#define CG_UPNP_CONTROL_NS "u"

/**
 * Definition for soap action: state variable query
 */
#define CG_UPNP_CONTROL_QUERY_SOAPACTION "\"urn:schemas-upnp-org:control-1-0#QueryStateVariable\""

/**
 * Definition for control XML name space
 */
#define CG_UPNP_CONTROL_XMLNS "urn:schemas-upnp-org:control-1-0"

/**
 * Definition for query state variable
 */
#define CG_UPNP_CONTROL_QUERY_STATE_VARIABLE "QueryStateVariable"

/**
 * Definition for query state variable response
 */
#define CG_UPNP_CONTROL_QUERY_STATE_VARIABLE_RESPONSE "QueryStateVariableResponse"

/**
 * Definition for control variable name
 */
#define CG_UPNP_CONTROL_VAR_NAME "varName"

/**
 * Definition for control return
 */
#define CG_UPNP_CONTROL_RETURN "return"

/**
 * Definition for control fault code
 */
#define CG_UPNP_CONTROL_FAULT_CODE "Client"

/**
 * Definition for contol fault string
 */
#define CG_UPNP_CONTROL_FAULT_STRING "UPnPError"

/** 
 * Definition fro control error code
 */
#define CG_UPNP_CONTROL_ERROR_CODE "errorCode"

/**
 * Definition for control error description
 */
#define CG_UPNP_CONTROL_ERROR_DESCRIPTION "errorDescription"

/****************************************
* Data Type
****************************************/

/**** Action ****/

/**
 * Data type definition for action response
 */
typedef struct _CgUpnpActionResponse {
	CgSoapResponse *soapRes;
	BOOL isSoapResCreated;
	CgUpnpArgumentList *argList;
} CgUpnpActionResponse;

/**
 * Data type definition for action request
 */
typedef struct _CgUpnpActionRequest{
	CgSoapRequest *soapReq;
	BOOL isSoapReqCreated;
	CgUpnpArgumentList *argList;
	CgUpnpActionResponse *actionRes;
} CgUpnpActionRequest;

/**** Query ****/

/**
 * Data type definition for query response
 */
typedef struct _CgUpnpQueryResponse {
	CgSoapResponse *soapRes;
	BOOL isSoapResCreated;
} CgUpnpQueryResponse;

/**
 * Data type definition for query request
 */
typedef struct _CgUpnpQueryRequest{
	CgSoapRequest *soapReq;
	BOOL isSoapReqCreated;
	CgUpnpQueryResponse *queryRes;
} CgUpnpQueryRequest;

/****************************************
* Function
****************************************/

/* After POST has failed we send M-POST and change the SOAPACTION header into 01-SOAPACTION.
 * Next ensures that also in this case query is interpreted as a query.
 */

/**
 * Check if HTTP request query is UPnP query request
 *
 * @param httpReq HTTP request
 *
 * @return True if request is UPnP query request, false othewise
 */
#define cg_upnp_control_isqueryrequest(httpReq) ( ((0 <= cg_strstr(cg_soap_request_getsoapaction(httpReq), CG_UPNP_CONTROL_QUERY_SOAPACTION)) ? TRUE : FALSE) || ((0 <= cg_strstr(cg_soap_request_getsoapactionwithns(httpReq), CG_UPNP_CONTROL_QUERY_SOAPACTION)) ? TRUE : FALSE) )

/* Check if HTTP request is UPnP action request
 *
 * @param httpReq HTTP request
 * 
 * @return True if request is UPnP action request, false otherwise
 */
#define cg_upnp_control_isactionrequest(httpReq) ((cg_upnp_control_isqueryrequest(httpReq) == TRUE) ? FALSE : TRUE)

/**
 * Initializes soap requests envelope node
 *
 * @param soapReq Soap request
 */
void cg_upnp_control_soap_request_initializeenvelopenode(CgSoapRequest *soapReq);

/**
 * Initialize soap responses envelope node
 *
 * @param soapRes Soap response
 */
void cg_upnp_control_soap_response_initializeenvelopenode(CgSoapResponse *soapRes);
CgXmlNode *cg_upnp_control_soap_response_createfaultresponsenode(int errCode, char *errDescr);

/**
 * Set failed response data
 *
 * @param soapRes Soap response
 * @param errCode Error code
 * @param errDescr Error description
 */
void cg_upnp_control_soap_response_setfaultresponse(CgSoapResponse *soapRes, int errCode, char *errDescr);

/**
 * Perform action listener
 *
 * @param action Action
 * @param actionReq Action request
 */
BOOL cg_upnp_action_performlistner(CgUpnpAction *action, CgUpnpActionRequest *actionReq);

/**
 * Perform query listener
 *
 * @param statVar State variable
 * @param queryReq Query request
 */
BOOL cg_upnp_statevariable_performlistner(CgUpnpStateVariable *statVar, CgUpnpQueryRequest *queryReq);

/**
 * Set host from service
 *
 * @param soapReq Soap request
 * @param service UPnP service
 */
void cg_upnp_control_request_sethostfromservice(CgSoapRequest *soapReq, CgUpnpService *service);

/****************************************
* Function (ActionRequest)
****************************************/

/**
 * Create new action request object
 */
CgUpnpActionRequest *cg_upnp_control_action_request_new();

/**
 * Delete action request object
 *
 * @param actionReq Action request
 */
void cg_upnp_control_action_request_delete(CgUpnpActionRequest *actionReq);

/**
 * Clear action request
 *
 * @param actionReq Clear Action request
 */
void cg_upnp_control_action_request_clear(CgUpnpActionRequest *actionReq);

/**
 * Set soap request associated with action request
 *
 * @param actionReq Action request
 * @param soapReq Soap request
 */
void cg_upnp_control_action_request_setsoaprequest(CgUpnpActionRequest *actionReq, CgSoapRequest *soapReq);

/**
 * Get soap request associated with action request
 *
 * @param actionReq Action request
 *
 * @return Soap request
 */
#define cg_upnp_control_action_request_getsoaprequest(actionReq) (actionReq->soapReq)

/**
 * Get argument list from action request
 *
 * @param actionReq Action request
 *
 * @return Argument list
 */
#define cg_upnp_control_action_request_getargumentlist(actionReq) (actionReq->argList)

/**
 * Get XML action node from action request
 *
 * @param actionReq
 *
 * @return Action request XML node
 */
CgXmlNode *cg_upnp_control_action_request_getactionnode(CgUpnpActionRequest *actionReq);

/**
 * Get action name from action request
 *
 * @param actionReq Action request
 *
 * @return Action name
 */
char *cg_upnp_control_action_request_getactionname(CgUpnpActionRequest *actionReq);

/**
 * Set action to action request
 *
 * @param actionReq Action request
 * @param action UPnP action
 */
void cg_upnp_control_action_request_setaction(CgUpnpActionRequest *actionReq, CgUpnpAction *action);

/**
 * Get action response from action request
 *
 * @param actionReq Action request
 *
 * @return Action response
 */
#define cg_upnp_control_action_request_getactionresponse(actionReq) (actionReq->actionRes)

/**
 * Send action request
 *
 * @param actionReq Action request
 */
CgUpnpActionResponse *cg_upnp_control_action_request_post(CgUpnpActionRequest *actionReq);

/****************************************
* Function (ActionResponse)
****************************************/

/**
 * Create new action response
 */
CgUpnpActionResponse *cg_upnp_control_action_response_new();

/**
 * Delete action response
 *
 * @param actionReq Action response
 */
void cg_upnp_control_action_response_delete(CgUpnpActionResponse *actionReq);

/**
 * Clear action response
 *
 * @param actionReq Action response
 */
void cg_upnp_control_action_response_clear(CgUpnpActionResponse *actionReq);

/**
 * Set soap response to action response
 *
 * @param actionRes Action response
 * @param soapRes Soap reaponse
 */
void cg_upnp_control_action_response_setsoapresponse(CgUpnpActionResponse *actionRes, CgSoapResponse *soapRes);

/**
 * Get soap response from action response
 *
 * @param actionRes Action response
 *
 * @return Soap response
 */
#define cg_upnp_control_action_response_getsoapresponse(actionRes) (actionRes->soapRes)

/**
 * Set action response to action
 *
 * @param actionRes Action response
 * @param action UPnP action
 */
void cg_upnp_control_action_response_setresponse(CgUpnpActionResponse *actionRes, CgUpnpAction *action);

/**
 * Check if action response indicates that action invoke was successfull
 *
 * @param actionRes Action response
 *
 * @return True if action has been successful, false otherwise
 */
#define cg_upnp_control_action_response_issuccessful(actionRes) cg_soap_response_issuccessful(actionRes->soapRes)

/** 
 * Get action response XML node from action response
 *
 * @param actionRes Action response
 *
 * @return Action response XML node
 */
CgXmlNode *cg_upnp_control_action_response_getactionresponsenode(CgUpnpActionResponse *actionRes);

/**
 * Get result from action response
 * \todo Check this out more carefully
 *
 * @param actionRes Action response
 * @param action UPnP action
 */
BOOL cg_upnp_control_action_response_getresult(CgUpnpActionResponse *actionRes, CgUpnpAction *action);

/**
 * Get error associated to action response
 * \todo Check this out more carefully
 * @param actionRes Action response
 * @param action UPnP action
 */
BOOL cg_upnp_control_action_response_geterror(CgUpnpActionResponse *actionRes, CgUpnpAction *action);

/****************************************
* Function (QueryRequest)
****************************************/

/**
 * Create new query request object
 */
CgUpnpQueryRequest *cg_upnp_control_query_request_new();

/**
 * Delete query request
 *
 * @param queryReq Query request
 */
void cg_upnp_control_query_request_delete(CgUpnpQueryRequest *queryReq);

/**
 * Clear query request
 *
 * @param queryReq Query request
 */
void cg_upnp_control_query_request_clear(CgUpnpQueryRequest *queryReq);

/**
 * Set soap request for query request
 *
 * @param queryReq Query request
 * @param soapReq Soap request
 */
void cg_upnp_control_query_request_setsoaprequest(CgUpnpQueryRequest *queryReq, CgSoapRequest *soapReq);

/**
 * Get soap request from query request
 *
 * @param queryReq
 *
 * @return Soap request
 */
#define cg_upnp_control_query_request_getsoaprequest(queryReq) (queryReq->soapReq)

/**
 * Get state variable XML node
 *
 * @param queryReq
 *
 * @return State variable XML node
 */
CgXmlNode *cg_upnp_control_query_request_getvarnamenode(CgUpnpQueryRequest *queryReq);

/**
 * Get state variable name associated with this query request
 *
 * @param queryReq Query request
 *
 * @return State variable variable name
 */
char *cg_upnp_control_query_request_getvarname(CgUpnpQueryRequest *queryReq);

/**
 * Set state variable to query request
 *
 * @param queryReq Query request
 * @param statVar State variable
 */
void cg_upnp_control_query_request_setstatevariable(CgUpnpQueryRequest *queryReq, CgUpnpStateVariable *statVar);

/**
 * Get response from query request
 *
 * @param queryReq Query request
 *
 * @return Query response
 */
#define cg_upnp_control_query_request_getqueryresponse(queryReq) (queryReq->queryRes)

/**
 * Send query request
 *
 * @param queryReq Query request
 *
 * @return Query response
 */
CgUpnpQueryResponse *cg_upnp_control_query_request_post(CgUpnpQueryRequest *queryReq);

/****************************************
* Function (QueryResponse)
****************************************/

/**
 * Create new query response
 */
CgUpnpQueryResponse *cg_upnp_control_query_response_new();

/**
 * Delete query response
 *
 * @param queryReq Query request
 */
void cg_upnp_control_query_response_delete(CgUpnpQueryResponse *queryReq);

/**
 * Clear query response
 *
 * @param queryReq Query request
 */
void cg_upnp_control_query_response_clear(CgUpnpQueryResponse *queryReq);

/**
 * Set soap response for query response
 *
 * @param queryRes Query response
 * @param soapRes Soap response
 */
void cg_upnp_control_query_response_setsoapresponse(CgUpnpQueryResponse *queryRes, CgSoapResponse *soapRes);

/**
 * Get soap response from query response
 *
 * @param queryRes Query response
 *
 * @return Soap response
 */
#define cg_upnp_control_query_response_getsoapresponse(queryRes) (queryRes->soapRes)

/**
 * Create and set XML response data into query response
 *
 * @param queryRes Query response
 * @param statVar State variable
 */
void cg_upnp_control_query_response_setresponse(CgUpnpQueryResponse *queryRes, CgUpnpStateVariable *statVar);

/**
 * Check if query response indicates that query request has succeeded
 *
 * @param queryRes Query response
 */
#define cg_upnp_control_query_response_issuccessful(queryRes) cg_soap_response_issuccessful(queryRes->soapRes)

/**
 * Get XML return node from query response
 *
 * @param queryRes
 *
 * @return XML return node
 */
CgXmlNode *cg_upnp_control_query_response_getreturnnode(CgUpnpQueryResponse *queryRes);

/**
 * Get return valuse from query response
 *
 * @param queryRes Query response
 *
 * @return UPnP return value
 */
char *cg_upnp_control_query_response_getreturnvalue(CgUpnpQueryResponse *queryRes);

/****************************************
* Function (MAN)
****************************************/

/**
 * Check if string is correct mandatory header value as specified
 * in UPnP Device Architecture 1.2.2
 *
 * @param str String to be checked
 *
 * @return True if string is exactly the required one, false otherwise
 */
#define cg_upnp_man_isdiscover(str) ((0 == cg_strstr(str, CG_UPNP_MAN_DISCOVER)) ? TRUE : FALSE)

/****************************************
* Function (ST)
****************************************/

/**
 * Check if device is of type "all"
 *
 * @param str String to be checked
 * 
 * @return True if device type is "all", false otherwise
 */
#define cg_upnp_st_isalldevice(str) ((0 <= cg_strstr(str, CG_UPNP_ST_ALL_DEVICE)) ? TRUE : FALSE)

/**
 * Check if device string is from root device
 *
 * @param str String to be checked
 *
 * @return True if string is from device type "root device", false otherwise
 */
#define cg_upnp_st_isrootdevice(str) ((0 <= cg_strstr(str, CG_UPNP_ST_ROOT_DEVICE)) ? TRUE : FALSE)

/**
 * Check if string is from uuid device
 *
 * @param str String to be checked
 *
 * @return True if string is from uuid device, false otherwise
 */
#define cg_upnp_st_isuuiddevice(str) ((0 <= cg_strstr(str, CG_UPNP_ST_UUID_DEVICE)) ? TRUE : FALSE)

/**
 * Check if string is from urn
 *
 * @param str String to be checked
 *
 * @return True if string is from urn, false otherwise
 */
#define cg_upnp_st_isurn(str) ((0 <= cg_strstr(str, CG_UPNP_ST_URN)) ? TRUE : FALSE)

/**
 * Check if string is from urn device
 *
 * @param str String to be checked
 *
 * @return True if string is from urn device, false otherwise
 */
#define cg_upnp_st_isurndevice(str) ((0 <= cg_strstr(str, CG_UPNP_ST_URN_DEVICE)) ? TRUE : FALSE)

/**
 * Check if string is from urn service
 *
 * @param str String to be checked
 *
 * @return True if string is from urn service, false otherwise
 */
#define cg_upnp_st_isurnservice(str) ((0 <= cg_strstr(str, CG_UPNP_ST_URN_SERVICE)) ? TRUE : FALSE)

/****************************************
* Function (NT)
****************************************/

/**
 * Check if NT (notification target) is for root device
 *
 * @param str String to be checked
 *
 * @return True if NT is for root device, false otherwise
 */
#define cg_upnp_nt_isrootdevice(str) ((0 <= cg_strstr(str, CG_UPNP_NT_ROOTDEVICE)) ? TRUE : FALSE)

/****************************************
* Function (NTS)
****************************************/

/**
 * Check if NTS (notification sub target) is of type "alive"
 *
 * @param str String to be checked
 *
 * @return True if string is of type "alive", false otherwise
 */
#define cg_upnp_nts_isalive(str) ((0 <= cg_strstr(str, CG_UPNP_NTS_ALIVE)) ? TRUE : FALSE)

/**
 * Check if string type is "byebye"
 *
 * @param str String to be checked
 *
 * @return True if string is of type "byebye", false othewise
 */
#define cg_upnp_nts_isbyebye(str) ((0 <= cg_strstr(str, CG_UPNP_NTS_BYEBYE)) ? TRUE : FALSE)

/**
 * Check if string type is "property change"
 *
 * @param str String to be checked
 *
 * @return True if string type is "property change"
 */
#define cg_upnp_nts_ispropchange(str) ((0 <= cg_strstr(str, CG_UPNP_NTS_PROPCHANGE)) ? TRUE : FALSE)

/****************************************
* Function (USN)
****************************************/

/**
 * Check if usn is from root device
 *
 * @param str String to be checked
 *
 * @return True if usn associates to root device, false otherwise
 */
#define cg_upnp_usn_isrootdevice(str) ((0 <= cg_strstr(str, CG_UPNP_USN_ROOTDEVICE)) ? TRUE : FALSE)

/**
 * Get udn from usn
 *
 * @param usn Universal serial number
 * @param udnBuf Buffer where udn is stored after successfull execution
 * @param udnBufLen Buffer length for udnBuf
 *
 * @return Pointer to beginning of udnBuf.
 */
char *cg_upnp_usn_getudn(char *usn, char *udnBuf, int udnBufLen);

/****************************************
* Function (Action)
****************************************/

/**
 * Send action
 *
 * @param action Action
 */
BOOL cg_upnp_action_post(CgUpnpAction *action);

/****************************************
* Function (Query)
****************************************/

/**
 * Send query
 *
 * @param statVar State variable
 */
BOOL cg_upnp_statevariable_post(CgUpnpStateVariable *statVar);

#ifdef  __cplusplus
}
#endif

#endif
