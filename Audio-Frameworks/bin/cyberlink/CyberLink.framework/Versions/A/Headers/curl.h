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

#ifndef _CG_NET_CURL_H_
#define _CG_NET_CURL_H_

#include <cybergarage/typedef.h>

#include <cybergarage/util/cstring.h>
#include <cybergarage/net/curi.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

/****************************************
* Data Type
****************************************/

typedef struct _CgNetURI CgNetURL;

/****************************************
* Function
****************************************/

#define cg_net_url_new() cg_net_uri_new()
#define cg_net_url_delete(urip) cg_net_uri_delete(urip);
#define cg_net_url_clear(urip) cg_net_uri_clear(urip);

#define cg_net_url_set(urip, value) cg_net_uri_set(urip, value)
#define cg_net_url_setvalue(urip, value) cg_net_uri_setvalue(urip, value)
#define cg_net_url_rebuild(urip) cg_net_uri_rebuild(urip)
#define cg_net_url_getvalue(urip) cg_net_uri_getvalue(urip)

#define cg_net_url_seturi(urip, value) cg_string_setvalue(urip->uri, value)
#define cg_net_url_setprotocol(urip, value) cg_string_setvalue(urip->protocol, value)
#define cg_net_url_setuser(urip, value) cg_string_setvalue(urip->user, value)
#define cg_net_url_setpassword(urip, value) cg_string_setvalue(urip->password, value)
#define cg_net_url_sethost(urip, value) cg_string_setvalue(urip->host, value)
#define cg_net_url_setport(urip, value) (urip->port = value)
#define cg_net_url_setpath(urip, value) cg_string_setvalue(urip->path, value)
#define cg_net_url_addpath(urip, value) cg_net_uri_addpath(urip, value)
#define cg_net_url_setquery(urip, value) cg_string_setvalue(urip->query, value)
#define cg_net_url_setfragment(urip, value) cg_string_setvalue(urip->fragment, value)

#define cg_net_url_geturi(urip) cg_string_getvalue(urip->uri)
#define cg_net_url_getprotocol(urip) cg_string_getvalue(urip->protocol)
#define cg_net_url_getuser(urip) cg_string_getvalue(urip->user)
#define cg_net_url_getpassword(urip) cg_string_getvalue(urip->password)
#define cg_net_url_gethost(urip) cg_string_getvalue(urip->host)
#define cg_net_url_getport(urip) (urip->port)
#define cg_net_url_getpath(urip) cg_string_getvalue(urip->path)
#define cg_net_url_getquery(urip) cg_string_getvalue(urip->query)
#define cg_net_url_getfragment(urip) cg_string_getvalue(urip->fragment)
#define cg_net_url_getrequest(urip) cg_net_uri_getrequest(urip)

#define cg_net_url_hasuri(urip) ((0 < cg_string_length(urip->uri)) ? TRUE : FALSE)
#define cg_net_url_hasprotocol(urip) ((0 < cg_string_length(urip->protocol)) ? TRUE : FALSE)
#define cg_net_url_hasuser(urip) ((0 < cg_string_length(urip->user)) ? TRUE : FALSE)
#define cg_net_url_haspassword(urip) ((0 < cg_string_length(urip->password)) ? TRUE : FALSE)
#define cg_net_url_hashost(urip) ((0 < cg_string_length(urip->host)) ? TRUE : FALSE)
#define cg_net_url_hasport(urip) ((0 < urip->port) ? TRUE : FALSE)
#define cg_net_url_haspath(urip) ((0 < cg_string_length(urip->path)) ? TRUE : FALSE)
#define cg_net_url_hasquery(urip) ((0 < cg_string_length(urip->query)) ? TRUE : FALSE)
#define cg_net_url_hasfragment(urip) ((0 < cg_string_length(urip->fragment)) ? TRUE : FALSE)
#define cg_net_url_hasrequest(urip) cg_net_uri_hasrequest(urip)

#define cg_net_url_ishttpprotocol(urip) cg_streq(cg_string_getvalue(urip->protocol), CG_NET_URI_PROTOCOL_HTTP)

#define cg_net_url_isabsolute(uripath) cg_net_uri_hasprotocol(uripath)
#define cg_net_url_isrelative(urip) ((cg_net_uri_hasprotocol(urip) == TRUE) ? FALSE : TRUE)
#define cg_net_url_isabsolutepath(uripath) cg_net_uri_isabsolutepath(uripath)

#define cg_net_url_getupnpbasepath(urip) cg_net_uri_getupnpbasepath(urip);

char *cg_net_gethosturl(char *host, int port, char *uri, char *buf, int bufSize);
char *cg_net_getmodifierhosturl(char *host, int port, char *uri, char *buf, int bufSize, char *begin, char *end);

#define cg_net_url_getquerydictionary(urip) cg_net_uri_getquerydictionary(urip);

#ifdef  __cplusplus
}
#endif

#endif
