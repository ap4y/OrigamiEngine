/******************************************************************
*
*	CyberSOAP for C
*
*	Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: csoap.h
*
*	Revision:
*
*	02/13/05
*		- first revision
*
******************************************************************/

#ifndef _CG_SOAP_CSOAP_H_
#define _CG_SOAP_CSOAP_H_

#include <cybergarage/typedef.h>
#include <cybergarage/http/chttp.h>
#include <cybergarage/xml/cxml.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#define CG_SOAP_VERSION_HEADER "<?xml version=\"1.0\" encoding=\"utf-8\"?>"

#define CG_HTTP_SOAP_ACTION "SOAPACTION"
#define CG_HTTP_SOAP_URN_DELIM ":"

#define CG_SOAP_ENVELOPE "Envelope"
#define CG_SOAP_BODY "Body"
#define CG_SOAP_RESPONSE "Response"
#define CG_SOAP_FAULT "Fault"
#define CG_SOAP_FAULT_CODE "faultcode"
#define CG_SOAP_FAULT_STRING "faultstring"
#define CG_SOAP_FAULTACTOR "faultactor"
#define CG_SOAP_DETAIL "detail"

#define CG_SOAP_ATTRIBUTE_XMLNS "xmlns"

#define CG_SOAP_XMLNS "s"
#define CG_SOAP_METHODNS "u"
#define CG_SOAP_DELIM ":"

#define CG_SOAP_XMLNS_URL "http://schemas.xmlsoap.org/soap/envelope/"
#define CG_SOAP_ENCSTYLE_URL "http://schemas.xmlsoap.org/soap/encoding/"
	
#define CG_SOAP_CONTENT_TYPE "text/xml; charset=\"utf-8\""
#define CG_SOAP_ENCORDING "encodingStyle"

/****************************************
* Data Type
****************************************/

typedef struct _CgSoapResponse {
	CgHttpResponse *httpRes;
	BOOL isHttpResCreated;
	CgXmlNodeList *rootNodeList;
	void *userData;
} CgSoapResponse;

typedef struct _CgSoapRequest{
	CgHttpRequest *httpReq;
	BOOL isHttpReqCreated;
	CgXmlNode *rootNodeList;
	CgSoapResponse *soapRes;
	void *userData;
} CgSoapRequest;

/****************************************
* Function (Request)
****************************************/

CgSoapRequest *cg_soap_request_new();
void cg_soap_request_delete(CgSoapRequest *soapReq);
void cg_soap_request_clear(CgSoapRequest *soapReq);

BOOL cg_soap_request_sethttprequest(CgSoapRequest *soapReq, CgHttpRequest *httpReq);
#define cg_soap_request_gethttprequest(soapReq) (soapReq->httpReq)

#define cg_soap_request_setsoapaction(soapReq, name) cg_http_packet_setheadervalue((CgHttpPacket*)(soapReq->httpReq), CG_HTTP_SOAP_ACTION, name)
#define cg_soap_request_getsoapaction(soapReq) cg_http_packet_getheadervalue((CgHttpPacket*)(soapReq->httpReq), CG_HTTP_SOAP_ACTION)
#define cg_soap_request_getsoapactionwithns(soapReq) cg_http_packet_getheadervalue((CgHttpPacket*)(soapReq->httpReq), CG_HTTP_SOAP_ACTION_WITH_NS)

BOOL cg_soap_request_parsemessage(CgSoapRequest *soapReq, char *msg, int msgLen);
CgSoapResponse *cg_soap_request_post(CgSoapRequest *soapReq, char *ipaddr, int port);
#define cg_soap_request_getsoapresponse(soapReq) (soapReq->soapRes)

#define cg_soap_request_getrootnoodelist(soapReq) (soapReq->rootNodeList)
#define cg_soap_request_getrootnoode(soapReq) cg_xml_nodelist_gets(soapReq->rootNodeList)
#define cg_soap_request_getenvelopenode(soapReq) cg_xml_nodelist_gets(soapReq->rootNodeList)
CgXmlNode *cg_soap_request_getbodynode(CgSoapRequest *soapReq);

void cg_soap_request_setcontent(CgSoapRequest *soapReq, CgXmlNode *node);
#define cg_soap_request_createcontent(soapReq) cg_soap_request_setcontent(soapReq, cg_soap_request_getenvelopenode(soapReq))

/**** User Data ****/
#define cg_soap_request_setuserdata(soapReq, value) (soapReq->userData = value)
#define cg_soap_request_getuserdata(soapReq) (soapReq->userData)

/****************************************
* Function (Response)
****************************************/

CgSoapResponse *cg_soap_response_new();
void cg_soap_response_delete(CgSoapResponse *soapRes);
void cg_soap_response_clear(CgSoapResponse *soapRes);

void cg_soap_response_sethttpresponse(CgSoapResponse *soapRes, CgHttpResponse *httpRes);
#define cg_soap_response_gethttpresponse(soapRes) (soapRes->httpRes)

#define cg_soap_response_getrootnoodelist(soapRes) (soapRes->rootNodeList);
#define cg_soap_response_getrootnoode(soapRes) cg_xml_nodelist_gets(soapRes->rootNodeList);
#define cg_soap_response_getenvelopenode(soapRes) cg_xml_nodelist_gets(soapRes->rootNodeList);
CgXmlNode *cg_soap_response_getbodynode(CgSoapResponse *soapRes);

void cg_soap_response_setcontent(CgSoapResponse *soapRes, CgXmlNode *node);
#define cg_soap_response_createcontent(soapRes) cg_soap_response_setcontent(soapRes, cg_soap_response_getenvelopenode(soapRes))

#define cg_soap_response_getstatuscode(soapRes) cg_http_response_getstatuscode(soapRes->httpRes)
#define cg_soap_response_issuccessful(soapRes) cg_http_response_issuccessful(soapRes->httpRes)

/**** User Data ****/
#define cg_soap_response_setuserdata(soapRes, value) (soapRes->userData = value)
#define cg_soap_response_getuserdata(soapRes) (soapRes->userData)

/****************************************
* Function
****************************************/

CgXmlNode *cg_soap_createenvelopebodynode();

#ifdef  __cplusplus
}
#endif

#endif
