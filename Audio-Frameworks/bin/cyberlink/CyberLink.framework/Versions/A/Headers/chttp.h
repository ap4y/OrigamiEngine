/******************************************************************
*
*	CyberNet for C
*
*	Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: chttp.h
*
*	Revision:
*
*	01/25/05
*		- first revision
*
*	10/31/05
*		- Changed CHttpHeader* pointers to CHttpServer* pointers
*		  in CHttpServer struct.
*	
*	16-Jan-06 Aapo Makela
*		- Fixed header comparisons to be case-insensitive
*		- Fixed iskeepaliveconnection to check "Connection:close"
*		- Added M-POST specific stuff
*	04/03/06 Theo Beisch
*		- added (CgHttpHeaderList*) cast (see below)
*		- added LONGLONG macro for platform independence
*		- some re-sorting of get/set macros (see below)
******************************************************************/

#ifndef _CG_HTTP_CHTTP_H_
#define _CG_HTTP_CHTTP_H_

#include <cybergarage/typedef.h>
#include <cybergarage/util/cstring.h>
#include <cybergarage/util/clist.h>
#include <cybergarage/util/cthread.h>
#include <cybergarage/util/ctime.h>
#include <cybergarage/util/cmutex.h>
#include <cybergarage/net/csocket.h>
#include <cybergarage/net/curl.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/


#define CG_HTTP_READLINE_BUFSIZE 512
#define CG_HTTP_SEVERNAME_MAXLEN 64
#define CG_HTTP_DATE_MAXLEN 128

#define CG_HTTP_REQUESTLINE_DELIM "\r\n "
#define CG_HTTP_STATUSLINE_DELIM "\r\n "
#define CG_HTTP_HEADERLINE_DELIM "\r\n :"

#define CG_HTTP_CR "\r"
#define CG_HTTP_LF "\n"
#define CG_HTTP_CRLF "\r\n"
#define CG_HTTP_SP " "
#define CG_HTTP_COLON ":"

#define CG_HTTP_VER10 "HTTP/1.0"
#define CG_HTTP_VER11 "HTTP/1.1"

#if !defined(CG_HTTP_CURL)
#define CG_HTTP_USERAGENT_DEFAULT "CyberGarage-HTTP/1.0"
#else
#define CG_HTTP_USERAGENT_DEFAULT "libcurl-agent/1.0 (Cyberlink for C)"
#endif

#define CG_HTTP_DEFAULT_PORT 80
#define CG_HTTS_DEFAULT_PORT 443

#define CG_HTTP_STATUS_CONTINUE 100
#define CG_HTTP_STATUS_OK 200
#define CG_HTTP_STATUS_PARTIAL_CONTENT 206
#define CG_HTTP_STATUS_BAD_REQUEST 400
#define CG_HTTP_STATUS_NOT_FOUND 404
#define CG_HTTP_STATUS_METHOD_NOT_ALLOWED 405
#define CG_HTTP_STATUS_PRECONDITION_FAILED 412
#define CG_HTTP_STATUS_INVALID_RANGE 416
#define CG_HTTP_STATUS_INTERNAL_SERVER_ERROR 500

#define CG_HTTP_POST "POST"
#define CG_HTTP_GET "GET"
#define CG_HTTP_HEAD "HEAD"

#define CG_HTTP_HOST "HOST"
#define CG_HTTP_DATE "Date"
#define CG_HTTP_CACHE_CONTROL "Cache-Control"
	#define CG_HTTP_NO_CACHE "no-cache"
	#define CG_HTTP_MAX_AGE "max-age"
#define CG_HTTP_CONNECTION "Connection"
	#define CG_HTTP_CLOSE "close"
	#define CG_HTTP_KEEP_ALIVE "Keep-Alive"
#define CG_HTTP_CONTENT_TYPE "Content-Type"
#define CG_HTTP_CONTENT_LENGTH "Content-Length"
#define CG_HTTP_CONTENT_RANGE "Content-Range"
	#define CG_HTTP_CONTENT_RANGE_BYTES "bytes" 
#define CG_HTTP_LOCATION "Location"
#define CG_HTTP_SERVER "Server"
#define CG_HTTP_RANGE "Range"
#define CG_HTTP_TRANSFER_ENCODING "Transfer-Encoding"
	#define CG_HTTP_CHUNKED "Chunked"
#define CG_HTTP_USERAGENT "User-Agent"

/**** SOAP Extention ****/
#define CG_HTTP_SOAP_ACTION "SOAPACTION"
#define CG_HTTP_SOAP_ACTION_WITH_NS "01-SOAPACTION"
#define CG_HTTP_SOAP_MAN_VALUE "\"http://schemas.xmlsoap.org/soap/envelope/\"; ns=01"

/**** UPnP Extention ****/
#define CG_HTTP_MPOST "M-POST"
#define CG_HTTP_MSEARCH "M-SEARCH"
#define CG_HTTP_NOTIFY "NOTIFY"
#define CG_HTTP_SUBSCRIBE "SUBSCRIBE"
#define CG_HTTP_UNSUBSCRIBE "UNSUBSCRIBE"	

#define CG_HTTP_ST "ST"
#define CG_HTTP_MX "MX"
#define CG_HTTP_MAN "MAN"
#define CG_HTTP_NT "NT"
#define CG_HTTP_NTS "NTS"
#define CG_HTTP_USN "USN"
#define CG_HTTP_EXT "EXT"
#define CG_HTTP_SID "SID"
#define CG_HTTP_SEQ "SEQ"
#define CG_HTTP_CALLBACK "CALLBACK"
#define CG_HTTP_TIMEOUT "TIMEOUT"

/**** CURL connection timeout: 2 seconds ****/
#define CG_HTTP_CURL_CONNECTTIMEOUT 2

/**** CURL total timeout: 30 seconds (UPnP DA: whole transfer: 30sec ****/
#define CG_HTTP_CONN_TIMEOUT 30

/* HTTP server - client thread blocking timeout */
#define CG_HTTP_SERVER_READ_TIMEOUT 120

/**** HTTP Status code reason phrases ****/
#define CG_HTTP_REASON_100 "Continue"
#define CG_HTTP_REASON_101 "Switching Protocols"
#define CG_HTTP_REASON_200 "OK"
#define CG_HTTP_REASON_201 "Created"
#define CG_HTTP_REASON_202 "Accepted"
#define CG_HTTP_REASON_203 "Non-Authoritative Information"
#define CG_HTTP_REASON_204 "No Content"
#define CG_HTTP_REASON_205 "Reset Content"
#define CG_HTTP_REASON_206 "Partial Content"
#define CG_HTTP_REASON_300 "Multiple Choices"
#define CG_HTTP_REASON_301 "Moved Permanently"
#define CG_HTTP_REASON_302 "Found"
#define CG_HTTP_REASON_303 "See Other"
#define CG_HTTP_REASON_304 "Not Modified"
#define CG_HTTP_REASON_305 "Use Proxy"
#define CG_HTTP_REASON_307 "Temporary Redirect"
#define CG_HTTP_REASON_400 "Bad Request"
#define CG_HTTP_REASON_401 "Unauthorized"
#define CG_HTTP_REASON_402 "Payment Required"
#define CG_HTTP_REASON_403 "Forbidden"
#define CG_HTTP_REASON_404 "Not Found"
#define CG_HTTP_REASON_405 "Method Not Allowed"
#define CG_HTTP_REASON_406 "Not Acceptable"
#define CG_HTTP_REASON_407 "Proxy Authentication Required"
#define CG_HTTP_REASON_408 "Request Time-out"
#define CG_HTTP_REASON_409 "Conflict"
#define CG_HTTP_REASON_410 "Gone"
#define CG_HTTP_REASON_411 "Length Required"
#define CG_HTTP_REASON_412 "Precondition Failed"
#define CG_HTTP_REASON_413 "Request Entity Too Large"
#define CG_HTTP_REASON_414 "Request-URI Too Large"
#define CG_HTTP_REASON_415 "Unsupported Media Type"
#define CG_HTTP_REASON_416 "Requested range not satisfiable"
#define CG_HTTP_REASON_417 "Expectation Failed"
#define CG_HTTP_REASON_500 "Internal Server Error"
#define CG_HTTP_REASON_501 "Not Implemented"
#define CG_HTTP_REASON_502 "Bad Gateway"
#define CG_HTTP_REASON_503 "Service Unavailable"
#define CG_HTTP_REASON_504 "Gateway Time-out"
#define CG_HTTP_REASON_505 "HTTP Version not supported"

/****************************************
* Data Type
****************************************/

typedef struct _CgHttpHeader {
	BOOL headFlag;
	struct _CgHttpHeader *prev;
	struct _CgHttpHeader *next;
	CgString *name;
	CgString *value;
} CgHttpHeader, CgHttpHeaderList;

typedef struct _CgHttpPacket {
	CgHttpHeaderList *headerList;
	CgString *content;
} CgHttpPacket;

typedef struct _CgHttpResponse {
	CgHttpHeaderList *headerList;
	CgString *content;
	CgString *version;
	int statusCode;
	CgString *reasonPhrase;
	void *userData;
	int timeout;
} CgHttpResponse;

typedef struct _CgHttpRequest {
	CgHttpHeaderList *headerList;
	CgString *content;
	CgString *method;
	CgString *uri;
	CgString *version;
	CgString *userAgent;
	CgHttpResponse *httpRes;
	CgSocket *sock;
	CgNetURL *postURL;
	void *userData;
	int timeout;
} CgHttpRequest;

typedef void (*CG_HTTP_LISTENER)(CgHttpRequest *);

typedef struct _CgHttpServer {
	BOOL headFlag;
	struct _CgHttpServer *prev;
	struct _CgHttpServer *next;
	CgSocket *sock;
	CgThread *acceptThread;
	CgThreadList *clientThreads;
	CG_HTTP_LISTENER listener;
	void *userData;
	int timeout;
	CgMutex *mutex;
} CgHttpServer, CgHttpServerList;

/****************************************
* Function 
****************************************/

/**
 * Convert an HTTP status code to a reason phrase (ex. 200 -> "200 OK")
 *
 * @param code The HTTP status code to convert
 * @return The reason phrase (do not modify the pointer's contents)
 */
const char *cg_http_statuscode2reasonphrase(const int code);

/****************************************
* Function (Header)
****************************************/

CgHttpHeader *cg_http_header_new();
void cg_http_header_delete(CgHttpHeader *header);

#define cg_http_header_next(header) (CgHttpHeader *)cg_list_next((CgList *)header)
          
void cg_http_header_setname(CgHttpHeader *header, char *name);
char *cg_http_header_getname(CgHttpHeader *header);
void cg_http_header_setvalue(CgHttpHeader *header, char *value);
char *cg_http_header_getvalue(CgHttpHeader *header);

/****************************************
* Function (Header List)
****************************************/

CgHttpHeaderList *cg_http_headerlist_new();
void cg_http_headerlist_delete(CgHttpHeaderList *headerList);

#define cg_http_headerlist_clear(headerList) cg_list_clear((CgList *)headerList, (CG_LIST_DESTRUCTORFUNC)cg_http_header_delete)
#define cg_http_headerlist_size(headerList) cg_list_size((CgList *)headerList)
#define cg_http_headerlist_gets(headerList) (CgHttpHeader *)cg_list_next((CgList *)headerList)
#define cg_http_headerlist_add(headerList, header) cg_list_add((CgList *)headerList, (CgList *)header)

CgHttpHeader *cg_http_headerlist_get(CgHttpHeaderList *headerList, char *name);
void cg_http_headerlist_set(CgHttpHeaderList *headerList, char *name, char *value);
char *cg_http_headerlist_getvalue(CgHttpHeaderList *headerList, char *name);

/****************************************
* Function (Packet)
****************************************/

CgHttpPacket *cg_http_packet_new();
void cg_http_packet_delete(CgHttpPacket *httpPkt);
void cg_http_packet_init(CgHttpPacket *httpPkt);
void cg_http_packet_clean(CgHttpPacket *httpPkt);
void cg_http_packet_clear(CgHttpPacket *httpPkt);

#define cg_http_packet_deleteallheaders(httpPkt) cg_http_headerlist_gets(httpPkt->headerList)
#define cg_http_packet_getnheaders(httpPkt) cg_http_headerlist_size(httpPkt->headerList)
#define cg_http_packet_getheaders(httpPkt) cg_http_headerlist_gets(httpPkt->headerList)
//Theo Beisch - added (CgHttpHeaderList*) cast - as _get is function call
#define cg_http_packet_getheader(httpPkt, name) cg_http_headerlist_get((CgHttpHeaderList*)httpPkt->headerList, name)
#define cg_http_packet_hasheader(httpPkt, name) ((cg_http_headerlist_get((CgHttpHeaderList*)httpPkt->headerList, name) != NULL) ? TRUE : FALSE)

#define cg_http_packet_hasheadervalue(httpPkt, header, value) (cg_streq(cg_http_packet_getheadervalue((CgHttpPacket*)httpPkt, header),value))

#define cg_http_packet_addheader(httpPkt, header) cg_http_headerlist_add(httpPkt->headerList, header)

void cg_http_packet_setheadervalue(CgHttpPacket *httpPkt, char* name, char *value);
void cg_http_packet_setheaderinteger(CgHttpPacket *httpPkt, char* name, int value);
void cg_http_packet_setheaderlong(CgHttpPacket *httpPkt, char* name, long value);
char *cg_http_packet_getheadervalue(CgHttpPacket *httpPkt, char* name);
int cg_http_packet_getheaderinteger(CgHttpPacket *httpPkt, char* name);
long cg_http_packet_getheaderlong(CgHttpPacket *httpPkt, char* name);
int cg_http_packet_getheadersize(CgHttpPacket *httpPkt);

#if defined(CG_USE_INT64)
void cg_http_packet_setheaderlonglong(CgHttpPacket *httpPkt, char* name, CgInt64 value);
CgInt64 cg_http_packet_getheaderlonglong(CgHttpPacket *httpPkt, char* name);
#endif

#define cg_http_packet_setcontent(httpPkt, value) cg_string_setvalue(httpPkt->content, value)
#define cg_http_packet_setncontent(httpPkt, value, len) cg_string_setnvalue(httpPkt->content, value, len)
#define cg_http_packet_appendncontent(httpPkt, value, len) cg_string_naddvalue(httpPkt->content, value, len)
#define cg_http_packet_setcontentpointer(httpPkt, value, len) cg_string_setpointervalue(httpPkt->content, value, len)
#define cg_http_packet_getcontent(httpPkt) cg_string_getvalue(httpPkt->content)

void cg_http_packet_post(CgHttpPacket *httpPkt, CgSocket *sock);
void cg_http_packet_read_headers(CgHttpPacket *httpPkt, CgSocket *sock, char *lineBuf, int lineBufSize);
BOOL cg_http_packet_read_body(
							  CgHttpPacket *httpPkt, CgSocket *sock, char *lineBuf, int lineBufSize);
BOOL cg_http_packet_read(CgHttpPacket *httpPkt, CgSocket *sock, BOOL onlyHeader, char *lineBuf, int lineBufSize);

/**** Content-Length ****/
#if defined(CG_USE_INT64)
#define cg_http_packet_setcontentlength(httpPkt,value) cg_http_packet_setheaderlonglong(httpPkt,CG_HTTP_CONTENT_LENGTH,value)
#define cg_http_packet_getcontentlength(httpPkt) cg_http_packet_getheaderlonglong(httpPkt,CG_HTTP_CONTENT_LENGTH)
#else
#define cg_http_packet_setcontentlength(httpPkt,value) cg_http_packet_setheaderlong(httpPkt,CG_HTTP_CONTENT_LENGTH,value)
#define cg_http_packet_getcontentlength(httpPkt) cg_http_packet_getheaderlong(httpPkt,CG_HTTP_CONTENT_LENGTH)
#endif

/**** Connection ****/
#define cg_http_packet_setconnection(httpPkt, value) cg_http_packet_setheadervalue(httpPkt,CG_HTTP_CONNECTION, value)
#define cg_http_packet_getconnection(httpPkt) cg_http_packet_getheadervalue(httpPkt,CG_HTTP_CONNECTION)
#define cg_http_packet_iskeepaliveconnection(httpPkt) (cg_strcaseeq(cg_http_packet_getconnection(httpPkt), CG_HTTP_CLOSE) == FALSE)

/**** Transfer-Encoding ****/
#define cg_http_packet_settransferencoding(httpPkt, value) cg_http_packet_setheadervalue(httpPkt,CG_HTTP_TRANSFER_ENCODING, value)
#define cg_http_packet_gettransferencoding(httpPkt) cg_http_packet_getheadervalue(httpPkt,CG_HTTP_TRANSFER_ENCODING)
#define cg_http_packet_ischunked(httpPkt) cg_strcaseeq(cg_http_packet_gettransferencoding(httpPkt), CG_HTTP_CHUNKED)

/**** Host ****/
void cg_http_packet_sethost(CgHttpPacket *httpPkt, char *addr, int port);
#define cg_http_packet_gethost(httpPkt) cg_http_packet_getheaderlong(httpPkt,CG_HTTP_HOST)

/**** Copy ****/
void cg_http_packet_copy(CgHttpPacket *destHttpPkt, CgHttpPacket *srcHttpPkt);

/**** Print ****/
void cg_http_packet_print(CgHttpPacket *httpPkt);

/****************************************
* Function (Request)
****************************************/

CgHttpRequest *cg_http_request_new();
void cg_http_request_delete(CgHttpRequest *httpReq);
void cg_http_request_clear(CgHttpRequest *httpReq);
void cg_http_request_setmethod(CgHttpRequest *httpReq, char *method);
char *cg_http_request_getmethod(CgHttpRequest *httpReq);
void cg_http_request_setversion(CgHttpRequest *httpReq, char *version);
char *cg_http_request_getversion(CgHttpRequest *httpReqest);
void cg_http_request_setuseragent(CgHttpRequest *httpReq, char *version);
char *cg_http_request_getuseragent(CgHttpRequest *httpReqest);
void cg_http_request_addtouseragent(CgHttpRequest *httpReq, char *value);
void cg_http_request_seturi(CgHttpRequest *httpReq, char *uri);
char *cg_http_request_geturi(CgHttpRequest *httpReq);
void cg_http_request_setsocket(CgHttpRequest *httpReq, CgSocket *sock);
CgSocket *cg_http_request_getsocket(CgHttpRequest *httpReq);
#define cg_http_request_closesocket(httpReq) cg_socket_close(cg_http_request_getsocket(httpReq))
CgHttpResponse *cg_http_request_post(CgHttpRequest *httpReq, char *ipaddr, int port);
#if defined(CG_USE_OPENSSL)
CgHttpResponse *cg_https_request_post(CgHttpRequest *httpReq, char *ipaddr, int port);
#endif
BOOL cg_http_request_read(CgHttpRequest *httpReq, CgSocket *sock);
BOOL cg_http_request_postresponse(CgHttpRequest *httpReq, CgHttpResponse *httpRes);
BOOL cg_http_request_poststatuscode(CgHttpRequest *httpReq, int httpStatCode);
BOOL cg_http_request_postdata(CgHttpRequest *httpReq, void *data, int dataLen);
BOOL cg_http_request_postchunkedsize(CgHttpRequest *httpReq, int dataLen);
BOOL cg_http_request_postchunkeddata(CgHttpRequest *httpReq, void *data, int dataLen);
BOOL cg_http_request_postlastchunk(CgHttpRequest *httpReq);
#define cg_http_request_poststring(httpReq, data) cg_http_request_postdata(httpReq, data, cg_strlen(data))
#define cg_http_request_postchunkedstring(httpReq, data) cg_http_request_postchunkeddata(httpReq, data, cg_strlen(data))

#define cg_http_request_postokrequest(httpReq) cg_http_request_poststatuscode(httpReq, CG_HTTP_STATUS_OK)
#define cg_http_request_postbadrequest(httpReq) cg_http_request_poststatuscode(httpReq, CG_HTTP_STATUS_BAD_REQUEST)

#define cg_http_request_ismethod(httpReq,value) cg_streq(cg_http_request_getmethod(httpReq), value)
#define cg_http_request_isgetrequest(httpReq) cg_streq(cg_http_request_getmethod(httpReq), CG_HTTP_GET)
#define cg_http_request_ispostrequest(httpReq) (cg_streq(cg_http_request_getmethod(httpReq), CG_HTTP_POST) || cg_streq(cg_http_request_getmethod(httpReq), CG_HTTP_MPOST))
#define cg_http_request_isheadrequest(httpReq) cg_streq(cg_http_request_getmethod(httpReq), CG_HTTP_HEAD)
#define cg_http_request_issubscriberequest(httpReq) cg_streq(cg_http_request_getmethod(httpReq), CG_HTTP_SUBSCRIBE)
#define cg_http_request_isunsubscriberequest(httpReq) cg_streq(cg_http_request_getmethod(httpReq), CG_HTTP_UNSUBSCRIBE)
#define cg_http_request_isnotifyrequest(httpReq) cg_streq(cg_http_request_getmethod(httpReq), CG_HTTP_NOTIFY)
#define cg_http_request_issoapaction(httpReq) (cg_http_packet_hasheader(((CgHttpPacket*)httpReq), CG_HTTP_SOAP_ACTION) || cg_streq(cg_http_request_getmethod(httpReq), CG_HTTP_MPOST))

#define cg_http_request_setcontent(httpReq,value) cg_http_packet_setcontent(((CgHttpPacket*)httpReq),value)
#define cg_http_request_setncontent(httpReq,value,len) cg_http_packet_setcontent(((CgHttpPacket*)httpReq),value,len)
#define cg_http_request_appendncontent(httpPkt, value, len) cg_http_packet_appendncontent(httpPkt, value, len)
#define cg_http_request_setcontentpointer(httpReq,value, len) cg_http_packet_setcontentpointer(((CgHttpPacket*)httpReq),value, len)
#define cg_http_request_getcontent(httpReq) cg_http_packet_getcontent(((CgHttpPacket*)httpReq))
#define cg_http_request_getheaders(httpReq) cg_http_packet_getheaders((CgHttpPacket*)httpReq)
#define cg_http_request_getheader(httpReq,name) cg_http_packet_getheader((CgHttpPacket*)httpReq,name)
#define cg_http_request_setheadervalue(httpReq,name,value) cg_http_packet_setheadervalue((CgHttpPacket*)httpReq,name,value)
#define cg_http_request_setheaderinteger(httpReq,name,value)  cg_http_packet_setheaderinteger((CgHttpPacket*)httpReq,name,value)
#define cg_http_request_setheaderlong(httpReq,name,value)  cg_http_packet_setheaderlong((CgHttpPacket*)httpReq,name,value)
#define cg_http_request_getheadervalue(httpReq,name) cg_http_packet_getheadervalue((CgHttpPacket*)httpReq,name)
#define cg_http_request_getheaderinteger(httpReq,name) cg_http_packet_getheaderinteger((CgHttpPacket*)httpReq,name)
#define cg_http_request_getheaderlong(httpReq,name) cg_http_packet_getheaderlong((CgHttpPacket*)httpReq,name)

#define cg_http_request_setuserdata(httpReq, value) (httpReq->userData = value)
#define cg_http_request_getuserdata(httpReq) (httpReq->userData)

/**** Local Address/Port ****/
#define cg_http_request_getlocaladdress(httpReq) cg_socket_getaddress(httpReq->sock)
#define cg_http_request_getlocalport(httpReq) cg_socket_getport(httpReq->sock)

/**** Content-Length ****/
#define cg_http_request_setcontentlength(httpReq,value) cg_http_packet_setcontentlength((CgHttpPacket*)httpReq, value)
#define cg_http_request_getcontentlength(httpReq) cg_http_packet_getcontentlength((CgHttpPacket*)httpReq)

/**** Content-Type ****/
#define cg_http_request_setcontenttype(httpReq,value) cg_http_packet_setheadervalue((CgHttpPacket*)httpReq,CG_HTTP_CONTENT_TYPE,value)
#define cg_http_request_getcontenttype(httpReq) cg_http_packet_getheadervalue((CgHttpPacket*)httpReq,CG_HTTP_CONTENT_TYPE)

/**** Connection ****/
#define cg_http_request_setconnection(httpReq, value) cg_http_packet_setheadervalue((CgHttpPacket*)httpReq,CG_HTTP_CONNECTION, value)
#define cg_http_request_getconnection(httpReq) cg_http_packet_getheadervalue((CgHttpPacket*)httpReq,CG_HTTP_CONNECTION)
#define cg_http_request_iskeepaliveconnection(httpReq) cg_http_packet_iskeepaliveconnection((CgHttpPacket*)httpReq)

/**** Transfer-Encoding ****/
#define cg_http_request_settransferencoding(httpReq, value) cg_http_packet_settransferencoding((CgHttpPacket*)httpReq, value)
#define cg_http_request_gettransferencoding(httpReq) cg_http_packet_gettransferencoding((CgHttpPacket*)httpReq)
#define cg_http_request_ischunked(httpReq) cg_http_packet_ischunked((CgHttpPacket*)httpReq)

/**** Host ****/
#define cg_http_request_sethost(httpReq,addr,port) cg_http_packet_sethost((CgHttpPacket*)httpReq,addr,port)
#define cg_http_request_gethost(httpReq) cg_http_packet_getheadervalue((CgHttpPacket*)httpReq,CG_HTTP_HOST)

/**** Date ****/
#define cg_http_request_setdate(httpReq,value) cg_http_packet_setheadervalue((CgHttpPacket*)httpReq,CG_HTTP_DATE,value)
#define cg_http_request_getdate(httpReq) cg_http_packet_getheadervalue((CgHttpPacket*)httpReq,CG_HTTP_DATE)

/**** Copy ****/
void cg_http_request_copy(CgHttpRequest *destHttpReq, CgHttpRequest *srcHttpReq);

/*** PostURL ****/
#define cg_http_request_getposturl(httpReq) (httpReq->postURL)

/**** Print ****/
void cg_http_request_print(CgHttpRequest *httpReq);

/**** Timeout ****/
#define cg_http_request_settimeout(httpReq,value) (httpReq->timeout = value)
#define cg_http_request_gettimeout(httpReq) (httpReq->timeout)

/****************************************
* Function (Response)
****************************************/

CgHttpResponse *cg_http_response_new();
void cg_http_response_delete(CgHttpResponse *httpRes);
void cg_http_response_clear(CgHttpResponse *httpRes);
void cg_http_response_setversion(CgHttpResponse *httpRes, char *version);
char *cg_http_response_getversion(CgHttpResponse *httpRes);
void cg_http_response_setreasonphrase(CgHttpResponse *httpRes, char *value);
char *cg_http_response_getreasonphrase(CgHttpResponse *httpRes);
void cg_http_response_setstatuscode(CgHttpResponse *httpRes, int code);
int cg_http_response_getstatuscode(CgHttpResponse *httpRes);
BOOL cg_http_response_read(CgHttpResponse *httpRes, CgSocket *sock, BOOL onlyHeader);

/*#define cg_http_response_issuccessful(httpRes) ((cg_http_response_getstatuscode(httpRes) == CG_HTTP_STATUS_OK) ? TRUE : FALSE)*/
#define cg_http_response_issuccessful(httpRes) (cg_http_response_getstatuscode(httpRes) >= 200 && cg_http_response_getstatuscode(httpRes) < 300 )

#define cg_http_response_setcontent(httpRes,value) cg_http_packet_setcontent(((CgHttpPacket*)httpRes),value)
#define cg_http_response_setncontent(httpRes,value, len) cg_http_packet_setncontent(((CgHttpPacket*)httpRes),value, len)
#define cg_http_response_appendncontent(httpRes, value, len) cg_http_packet_appendncontent(((CgHttpPacket*) httpRes), value, len)

#define cg_http_response_setcontentpointer(httpRes,value,len) cg_http_packet_setcontentpointer(((CgHttpPacket*)httpRes),value,len)
#define cg_http_response_getcontent(httpRes) cg_http_packet_getcontent(((CgHttpPacket*)httpRes))
#define cg_http_response_getheaders(p) cg_http_packet_getheaders((CgHttpPacket*)p)
#define cg_http_response_getheader(p,n) cg_http_packet_getheader((CgHttpPacket*)p,n)
#define cg_http_response_setheadervalue(httpRes,n,value) cg_http_packet_setheadervalue((CgHttpPacket*)httpRes,n,value)
#define cg_http_response_setheaderinteger(httpRes,n,value)  cg_http_packet_setheaderinteger((CgHttpPacket*)httpRes,n,value)
#define cg_http_response_setheaderlong(httpRes,n,value)  cg_http_packet_setheaderlong((CgHttpPacket*)httpRes,n,value)
#define cg_http_response_getheadervalue(httpRes,n) cg_http_packet_getheadervalue((CgHttpPacket*)httpRes,n)
#define cg_http_response_getheaderinteger(httpRes,n) cg_http_packet_getheaderinteger((CgHttpPacket*)httpRes,n)
#define cg_http_response_getheaderlong(httpRes,n) cg_http_packet_getheaderlong((CgHttpPacket*)httpRes,n)

#define cg_http_response_setuserdata(httpRes, value) (httpRes->userData = value)
#define cg_http_response_getuserdata(httpRes) (httpRes->userData)

/**** Content-Length ****/
#define cg_http_response_setcontentlength(httpRes,value) cg_http_packet_setcontentlength((CgHttpPacket*)httpRes, value)
#define cg_http_response_getcontentlength(httpRes) cg_http_packet_getcontentlength((CgHttpPacket*)httpRes)

/**** Content-Type ****/
#define cg_http_response_setcontenttype(httpRes,value) cg_http_packet_setheadervalue((CgHttpPacket*)httpRes,CG_HTTP_CONTENT_TYPE,value)
#define cg_http_response_getcontenttype(httpRes) cg_http_packet_getheadervalue((CgHttpPacket*)httpRes,CG_HTTP_CONTENT_TYPE)

/**** connection ****/
#define cg_http_response_setconnection(httpRes, value) cg_http_packet_setheadervalue((CgHttpPacket*)httpRes,CG_HTTP_CONNECTION, value)
#define cg_http_response_getconnection(httpRes) cg_http_packet_getheadervalue((CgHttpPacket*)httpRes, CG_HTTP_CONNECTION)
#define cg_http_response_iskeepaliveconnection(httpRes) cg_http_packet_iskeepaliveconnection((CgHttpPacket*)httpRes)

/**** Host ****/
#define cg_http_response_sethost(httpRes,addr,port) cg_http_packet_sethost((CgHttpPacket*)httpRes,addr,port)
#define cg_http_response_gethost(httpRes) cg_http_packet_getheadervalue((CgHttpPacket*)httpRes,CG_HTTP_HOST)

/**** Date ****/
#define cg_http_response_setdate(httpRes,value) cg_http_packet_setheadervalue((CgHttpPacket*)httpRes,CG_HTTP_DATE,value)
#define cg_http_response_getdate(httpRes) cg_http_packet_getheadervalue((CgHttpPacket*)httpRes,CG_HTTP_DATE)

/**** Transfer-Encoding ****/
#define cg_http_response_settransferencoding(httpReq, value) cg_http_packet_settransferencoding((CgHttpPacket*)httpReq, value)
#define cg_http_response_gettransferencoding(httpReq) cg_http_packet_gettransferencoding((CgHttpPacket*)httpReq)
	
/**** Copy ****/
void cg_http_response_copy(CgHttpResponse *destHttpRes, CgHttpResponse *srcHttpRes);

/**** Print ****/
void cg_http_response_print(CgHttpResponse *httpRes);

/**** Timeout ****/
#define cg_http_response_settimeout(httpRes,value) (httpRes->timeout = value)
#define cg_http_response_gettimeout(httpRes) (httpRes->timeout)

/****************************************
* Function (Server)
****************************************/

CgHttpServer *cg_http_server_new();
void cg_http_server_delete(CgHttpServer *httpServer);
BOOL cg_http_server_open(CgHttpServer *httpServer, int bindPort, char *bindAddr);
BOOL cg_http_server_close(CgHttpServer *httpServer);
BOOL cg_http_server_accept(CgHttpServer *httpServer);
BOOL cg_http_server_start(CgHttpServer *httpServer);
BOOL cg_http_server_stop(CgHttpServer *httpServer);
void cg_http_server_setlistener(CgHttpServer *httpServer, CG_HTTP_LISTENER listener);

#define cg_http_server_next(httpServer) (CgHttpServer *)cg_list_next((CgList *)httpServer)

#define cg_http_server_getsocket(httpServer) (httpServer->sock)
#define cg_http_server_isopened(httpServer) ((httpServer->sock != NULL) ? TRUE : FALSE)

#define cg_http_server_setuserdata(httpServer, value) (httpServer->userData = value)
#define cg_http_server_getuserdata(httpServer) (httpServer->userData)

char *cg_http_getservername(char *buf, int bufSize);

/**** Timeout ****/
#define cg_http_server_settimeout(httpServer,value) (httpServer->timeout = value)
#define cg_http_server_gettimeout(httpServer) (httpServer->timeout)

/**** Mutex ****/
#define cg_http_server_lock(httpServer) cg_mutex_lock(httpServer->mutex)
#define cg_http_server_unlock(httpServer) cg_mutex_unlock(httpServer->mutex)

/****************************************
* Function (Server List)
****************************************/

CgHttpServerList *cg_http_serverlist_new();
void cg_http_serverlist_delete(CgHttpServerList *httpServerList);

#define cg_http_serverlist_clear(httpServerList) cg_list_clear((CgList *)httpServerList, (CG_LIST_DESTRUCTORFUNC)cg_http_server_delete)
#define cg_http_serverlist_size(httpServerList) cg_list_size((CgList *)httpServerList)
#define cg_http_serverlist_gets(httpServerList) (CgHttpServer *)cg_list_next((CgList *)httpServerList)
#define cg_http_serverlist_add(httpServerList, httpServer) cg_list_add((CgList *)httpServerList, (CgList *)httpServer)

BOOL cg_http_serverlist_open(CgHttpServerList *httpServerList, int port);
BOOL cg_http_serverlist_close(CgHttpServerList *httpServerList);
BOOL cg_http_serverlist_start(CgHttpServerList *httpServerList);
BOOL cg_http_serverlist_stop(CgHttpServerList *httpServerList);
void cg_http_serverlist_setlistener(CgHttpServerList *httpServerList, CG_HTTP_LISTENER listener);
void cg_http_serverlist_setuserdata(CgHttpServerList *httpServerList, void *value);

/****************************************
* Function (Date)
****************************************/

char *cg_http_getdate(CgSysTime sysTime, char *buf, int bufSize);

/****************************************
* Persistent connection cache
****************************************/

/** Initialize persistent connection cache. Can be called many times */
BOOL cg_http_persistentconnection_init(void);
/** Clear persistent connection cache and free all memory */
void cg_http_persistentconnection_clear(void);
/** Lock persistent connection cache. Required for getting, putting and using
    persistent connection from cache */
void cg_http_persistentconnection_lock(void);
/** Unlock persistent connection cache. */
void cg_http_persistentconnection_unlock(void);
/** Get persistent connection from cache */
void *cg_http_persistentconnection_get(char *host, int port);
/** Put connection to persistent connection cache */
BOOL cg_http_persistentconnection_put(char *host, int port, void *data);

#ifdef  __cplusplus
}
#endif

#endif
