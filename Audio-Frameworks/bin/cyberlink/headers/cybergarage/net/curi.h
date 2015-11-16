/******************************************************************
*
*	CyberUtil for C
*
*	Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: curl.h
*
*	Revision:
*
*	03/09/05
*		- first revision
*
******************************************************************/

#ifndef _CG_NET_CURI_H_
#define _CG_NET_CURI_H_

#include <cybergarage/typedef.h>

#include <cybergarage/util/cstring.h>
#include <cybergarage/util/cdictionary.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#define CG_NET_URI_KNKOWN_PORT (-1)
#define CG_NET_URI_DEFAULT_HTTP_PORT 80
#define CG_NET_URI_DEFAULT_FTP_PORT 21
#define CG_NET_URI_DEFAULT_PATH "/"
#define CG_NET_URI_MAXLEN 256

#define CG_NET_URI_PROTOCOL_DELIM "://"
#define CG_NET_URI_USER_DELIM "@"
#define CG_NET_URI_COLON_DELIM ":"
#define CG_NET_URI_SLASH_DELIM "/"
#define CG_NET_URI_SBLACET_DELIM "["
#define CG_NET_URI_EBLACET_DELIM "]"
#define CG_NET_URI_SHARP_DELIM "#"
#define CG_NET_URI_QUESTION_DELIM "?"
#define CG_NET_URI_ESCAPING_CHAR "%"
#define CG_NET_URI_AMP_DELIM "&"
#define CG_NET_URI_EQ_DELIM "="

#define CG_NET_URI_PROTOCOL_HTTP "http"
#define CG_NET_URI_PROTOCOL_FTP "ftp"

/****************************************
* Data Type
****************************************/

typedef struct _CgNetURI {
	CgString *uri;
	CgString *protocol;
	CgString *user;
	CgString *password;
	CgString *host;
	int port;
	CgString *path;
	CgString *query;
	CgString *fragment;
	CgString *request;
	CgDictionary *queryDictionary;
} CgNetURI;

/****************************************
* Function
****************************************/

CgNetURI *cg_net_uri_new();
void cg_net_uri_delete(CgNetURI *uri);
void cg_net_uri_clear(CgNetURI *uri);
void cg_net_uri_setvalue(CgNetURI *uri, char *value);
void cg_net_uri_rebuild(CgNetURI *uri);
char *cg_net_uri_getvalue(CgNetURI *uri);

#define cg_net_uri_set(urip, value) cg_net_uri_setvalue(urip, value)
	
#define cg_net_uri_seturi(urip, value) cg_string_setvalue(urip->uri, value)
#define cg_net_uri_setprotocol(urip, value) cg_string_setvalue(urip->protocol, value)
#define cg_net_uri_setuser(urip, value) cg_string_setvalue(urip->user, value)
#define cg_net_uri_setpassword(urip, value) cg_string_setvalue(urip->password, value)
#define cg_net_uri_sethost(urip, value) cg_string_setvalue(urip->host, value)
#define cg_net_uri_setport(urip, value) (urip->port = value)
#define cg_net_uri_setpath(urip, value) cg_string_setvalue(urip->path, value)
#define cg_net_uri_addpath(urip, value) cg_string_addvalue(urip->path, value)
#define cg_net_uri_setquery(urip, value) cg_string_setvalue(urip->query, value)
#define cg_net_uri_setfragment(urip, value) cg_string_setvalue(urip->fragment, value)
    
#define cg_net_uri_geturi(urip) cg_string_getvalue(urip->uri)
#define cg_net_uri_getprotocol(urip) cg_string_getvalue(urip->protocol)
#define cg_net_uri_getuser(urip) cg_string_getvalue(urip->user)
#define cg_net_uri_getpassword(urip) cg_string_getvalue(urip->password)
#define cg_net_uri_gethost(urip) cg_string_getvalue(urip->host)
#define cg_net_uri_getport(urip) (urip->port)
#define cg_net_uri_getpath(urip) cg_string_getvalue(urip->path)
#define cg_net_uri_getquery(urip) cg_string_getvalue(urip->query)
#define cg_net_uri_getfragment(urip) cg_string_getvalue(urip->fragment)

char *cg_net_uri_getrequest(CgNetURI *uri);
char *cg_net_uri_getupnpbasepath(CgNetURI *locationURL);

#define cg_net_uri_hasuri(urip) ((0 < cg_string_length(urip->uri)) ? TRUE : FALSE)
#define cg_net_uri_hasprotocol(urip) ((0 < cg_string_length(urip->protocol)) ? TRUE : FALSE)
#define cg_net_uri_hasuser(urip) ((0 < cg_string_length(urip->user)) ? TRUE : FALSE)
#define cg_net_uri_haspassword(urip) ((0 < cg_string_length(urip->password)) ? TRUE : FALSE)
#define cg_net_uri_hashost(urip) ((0 < cg_string_length(urip->host)) ? TRUE : FALSE)
#define cg_net_uri_hasport(urip) ((0 < urip->port) ? TRUE : FALSE)
#define cg_net_uri_haspath(urip) ((0 < cg_string_length(urip->path)) ? TRUE : FALSE)
#define cg_net_uri_hasquery(urip) ((0 < cg_string_length(urip->query)) ? TRUE : FALSE)
#define cg_net_uri_hasfragment(urip) ((0 < cg_string_length(urip->fragment)) ? TRUE : FALSE)
#define cg_net_uri_hasrequest(urip) cg_net_uri_haspath(urip)

#define cg_net_uri_ishttpprotocol(urip) cg_streq(cg_string_getvalue(urip->protocol), CG_NET_URI_PROTOCOL_HTTP)

#define cg_net_uri_isabsolute(urip) cg_net_uri_hasprotocol(urip)
#define cg_net_uri_isabsolutepath(uripath) (( *uripath == '/' ) ? TRUE : FALSE )
#define cg_net_uri_isrelative(urip) ((cg_net_uri_hasprotocol(urip) == TRUE) ? FALSE : TRUE)

BOOL cg_net_uri_isreservedchar(char c);
BOOL cg_net_uri_isunreservedchar(char c);
BOOL cg_net_uri_isescapechar(char c);
BOOL cg_net_uri_isalphanumchar(char c);

BOOL cg_net_uri_isescapedstring(char *buf, int bufSize);
char *cg_net_uri_escapestring(char *buf, int bufSize, CgString *retBuf);
char *cg_net_uri_unescapestring(char *buf, int bufSize, CgString *retBuf);
BOOL cg_net_uri_isequivalent(char *url, char *relative_url);

CgDictionary *cg_net_uri_getquerydictionary(CgNetURI *uri);

#ifdef  __cplusplus
}
#endif

#endif
