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
*	File: cssdp_server.h
*
*	Revision:
*
*	05/26/05
*		- first revision
*
******************************************************************/

#ifndef _CG_UPNP_CSSDP_SERVER_H_
#define _CG_UPNP_CSSDP_SERVER_H_

#include <cybergarage/typedef.h>

#include <cybergarage/upnp/ssdp/cssdp.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

/****************************************
* Data Type
****************************************/

typedef CgSocket CgUpnpHttpMuSocket;
typedef CgSocket CgUpnpHttpUSocket;

typedef struct _CgUpnpSSDPPacket {
	CgDatagramPacket *dgmPkt;
	CgHttpHeaderList *headerList;
	void *userData;
	CgSysTime timeStamp;

	/* These are used when filtering out duplicate
	 * M-SEARCHes
	 */
	CgSysTime *timestamps;
        int initialized;
} CgUpnpSSDPPacket;

typedef void (*CG_UPNP_SSDP_LISTNER)(CgUpnpSSDPPacket *);

typedef struct _CgUpnpSSDPServer {
	BOOL headFlag;
	struct _CgUpnpSSDPServer *prev;
	struct _CgUpnpSSDPServer *next;
	CgUpnpHttpMuSocket *httpmuSock;
	CgThread *recvThread;
	CG_UPNP_SSDP_LISTNER listener;
	void *userData;
} CgUpnpSSDPServer, CgUpnpSSDPServerList;

typedef void (*CG_UPNP_SSDP_RESPONSE_LISTNER)(CgUpnpSSDPPacket *);

typedef struct _CgUpnpSSDPResponseServer {
	BOOL headFlag;
	struct _CgUpnpSSDPServer *prev;
	struct _CgUpnpSSDPServer *next;
	CgUpnpHttpUSocket *httpuSock;
	CgThread *recvThread;
	CG_UPNP_SSDP_RESPONSE_LISTNER listener;
	void *userData;
} CgUpnpSSDPResponseServer, CgUpnpSSDPResponseServerList;

/****************************************
* Function (HTTPMU)
****************************************/

#define cg_upnp_httpmu_socket_new() cg_socket_dgram_new()
#define cg_upnp_httpmu_socket_delete(sock) cg_socket_delete(sock)
#define cg_upnp_httpmu_socket_close(sock) cg_socket_close(sock)

BOOL cg_upnp_httpmu_socket_bind(CgUpnpHttpMuSocket *sock, char *mcastAddr, int port, char *bindAddr);
int cg_upnp_httpmu_socket_recv(CgUpnpHttpMuSocket *sock, CgUpnpSSDPPacket *ssdpPkt);

/****************************************
* Function (HTTPU)
****************************************/

#define cg_upnp_httpu_socket_new() cg_socket_dgram_new()
#define cg_upnp_httpu_socket_delete(sock) cg_socket_delete(sock)
#define cg_upnp_httpu_socket_close(sock) cg_socket_close(sock)
#define cg_upnp_httpu_socket_new() cg_socket_dgram_new()
#define cg_upnp_httpu_socket_bind(sock, bindPort, bindAddr) cg_socket_bind(sock, bindPort, bindAddr, FALSE, FALSE)
int cg_upnp_httpu_socket_recv(CgUpnpHttpUSocket *sock, CgUpnpSSDPPacket *ssdpPkt);

/****************************************
* Function (SSDPPacket)
****************************************/

CgUpnpSSDPPacket *cg_upnp_ssdp_packet_new();
void cg_upnp_ssdp_packet_delete(CgUpnpSSDPPacket *ssdpPkt);
void cg_upnp_ssdp_packet_clear(CgUpnpSSDPPacket *ssdpPkt);

#define cg_upnp_ssdp_packet_setdatagrampacket(ssdpPkt, value) (ssdpPkt->dgmPkt = value)
#define cg_upnp_ssdp_packet_getdatagrampacket(ssdpPkt) (ssdpPkt->dgmPkt)
#define cg_upnp_ssdp_packet_setdata(ssdpPkt, value) cg_string_setvalue(ssdpPkt->dgmPkt->data, value)
#define cg_upnp_ssdp_packet_getdata(ssdpPkt) cg_string_getvalue(ssdpPkt->dgmPkt->data)
#define cg_upnp_ssdp_packet_setuserdata(ssdpPkt, data) (ssdpPkt->userData = data)
#define cg_upnp_ssdp_packet_getuserdata(ssdpPkt) (ssdpPkt->userData)
#define cg_upnp_ssdp_packet_settimestamp(ssdpPkt, value) (ssdpPkt->timeStamp = value)
#define cg_upnp_ssdp_packet_gettimestamp(ssdpPkt) (ssdpPkt->timeStamp)
#define cg_upnp_ssdp_packet_getexpiration(ssdpPkt) (cg_upnp_ssdp_packet_getmaxage(ssdpPkt) == 0 ? 0 : cg_upnp_ssdp_packet_gettimestamp(ssdpPkt) + cg_upnp_ssdp_packet_getmaxage(ssdpPkt))

void cg_upnp_ssdp_packet_setheader(CgUpnpSSDPPacket *ssdpPkt, char *ssdpMsg);
#define cg_upnp_ssdp_packet_addheader(ssdpPkt, header) cg_http_headerlist_add(ssdpPkt->headerList, header)
#define cg_upnp_ssdp_packet_getheaders(ssdpPkt) cg_http_headerlist_gets(ssdpPkt->headerList)

#define cg_upnp_ssdp_packet_gethost(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_HOST)
#define cg_upnp_ssdp_packet_getserver(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_SERVER)
#define cg_upnp_ssdp_packet_getcachecontrol(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_CACHE_CONTROL)
#define cg_upnp_ssdp_packet_getdate(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_DATE)
#define cg_upnp_ssdp_packet_getext(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_EXT)
#define cg_upnp_ssdp_packet_getlocation(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_LOCATION)
#define cg_upnp_ssdp_packet_getst(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_ST)
#define cg_upnp_ssdp_packet_getusn(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_USN)
#define cg_upnp_ssdp_packet_getmx(ssdpPkt) cg_str2int(cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_MX))
#define cg_upnp_ssdp_packet_getman(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_MAN)
#define cg_upnp_ssdp_packet_getnt(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_NT)
#define cg_upnp_ssdp_packet_getnts(ssdpPkt) cg_http_headerlist_getvalue(ssdpPkt->headerList, CG_HTTP_NTS)
long cg_upnp_ssdp_packet_getmaxage(CgUpnpSSDPPacket *ssdpPkt);

#define cg_upnp_ssdp_packet_isdiscover(ssdpPkt) cg_upnp_man_isdiscover(cg_upnp_ssdp_packet_getman(ssdpPkt))
#define cg_upnp_ssdp_packet_isalive(ssdpPkt) cg_upnp_nts_isalive(cg_upnp_ssdp_packet_getnts(ssdpPkt))
#define cg_upnp_ssdp_packet_isbyebye(ssdpPkt) cg_upnp_nts_isbyebye(cg_upnp_ssdp_packet_getnts(ssdpPkt))

BOOL cg_upnp_ssdp_packet_isrootdevice(CgUpnpSSDPPacket *ssdpPkt);

#define cg_upnp_ssdp_packet_getlocaladdress(ssdpPkt) cg_socket_datagram_packet_getlocaladdress(ssdpPkt->dgmPkt)
#define cg_upnp_ssdp_packet_getlocalport(ssdpPkt) cg_socket_datagram_packet_getlocalport(ssdpPkt->dgmPkt)
#define cg_upnp_ssdp_packet_getremoteaddress(ssdpPkt) cg_socket_datagram_packet_getremoteaddress(ssdpPkt->dgmPkt)
#define cg_upnp_ssdp_packet_getremoteport(ssdpPkt) cg_socket_datagram_packet_getremoteport(ssdpPkt->dgmPkt)

void cg_upnp_ssdp_packet_copy(CgUpnpSSDPPacket *destSsdpPkt, CgUpnpSSDPPacket *srcSsdpPkt);
void cg_upnp_ssdp_packet_print(CgUpnpSSDPPacket *ssdpPkt);

/****************************************
* Function (SSDPServer)
****************************************/

CgUpnpSSDPServer *cg_upnp_ssdp_server_new();
void cg_upnp_ssdp_server_delete(CgUpnpSSDPServer *server);

#define cg_upnp_ssdp_server_next(server) (CgUpnpSSDPServer *)cg_list_next((CgList *)server)

BOOL cg_upnp_ssdp_server_open(CgUpnpSSDPServer *server, char *bindAddr);
BOOL cg_upnp_ssdp_server_close(CgUpnpSSDPServer *server);

#define cg_upnp_ssdp_server_isopened(server) ((server->httpmuSock != NULL) ? TRUE : FALSE)
#define cg_upnp_ssdp_server_getsocket(server) (server->httpmuSock)

#define cg_upnp_ssdp_server_setlistener(server, func) (server->listener = func)
#define cg_upnp_ssdp_server_getlistener(server) (server->listener)
void cg_upnp_ssdp_server_performlistener(CgUpnpSSDPServer *server, CgUpnpSSDPPacket *ssdpPkt);

#define cg_upnp_ssdp_server_setuserdata(server, data) (server->userData = data)
#define cg_upnp_ssdp_server_getuserdata(server) (server->userData)

BOOL cg_upnp_ssdp_server_start(CgUpnpSSDPServer *server);
BOOL cg_upnp_ssdp_server_stop(CgUpnpSSDPServer *server);

/****************************************
* Function (SSDPServerList)
****************************************/

CgUpnpSSDPServerList *cg_upnp_ssdp_serverlist_new();
void cg_upnp_ssdp_serverlist_delete(CgUpnpSSDPServerList *serverList);

#define cg_upnp_ssdp_serverlist_clear(serverList) cg_list_clear((CgList *)serverList, (CG_LIST_DESTRUCTORFUNC)cg_upnp_ssdp_server_delete)
#define cg_upnp_ssdp_serverlist_size(serverList) cg_list_size((CgList *)serverList)
#define cg_upnp_ssdp_serverlist_gets(serverList) (CgUpnpSSDPServer *)cg_list_next((CgList *)serverList)
#define cg_upnp_ssdp_serverlist_add(serverList, server) cg_list_add((CgList *)serverList, (CgList *)server)

BOOL cg_upnp_ssdp_serverlist_open(CgUpnpSSDPServerList *ssdpServerList);
BOOL cg_upnp_ssdp_serverlist_close(CgUpnpSSDPServerList *ssdpServerList);
BOOL cg_upnp_ssdp_serverlist_start(CgUpnpSSDPServerList *ssdpServerList);
BOOL cg_upnp_ssdp_serverlist_stop(CgUpnpSSDPServerList *ssdpServerList);
void cg_upnp_ssdp_serverlist_setlistener(CgUpnpSSDPServerList *ssdpServerList, CG_UPNP_SSDP_LISTNER listener);
void cg_upnp_ssdp_serverlist_setuserdata(CgUpnpSSDPServerList *ssdpServerList, void *data);

/****************************************
* Function (SSDPResponseServer)
****************************************/

CgUpnpSSDPResponseServer *cg_upnp_ssdpresponse_server_new();
void cg_upnp_ssdpresponse_server_delete(CgUpnpSSDPResponseServer *server);

#define cg_upnp_ssdpresponse_server_next(server) (CgUpnpSSDPResponseServer *)cg_list_next((CgList *)server)

BOOL cg_upnp_ssdpresponse_server_open(CgUpnpSSDPResponseServer *server, int bindPort, char *bindAddr);
BOOL cg_upnp_ssdpresponse_server_close(CgUpnpSSDPResponseServer *server);

#define cg_upnp_ssdpresponse_server_isopened(server) ((server->httpuSock != NULL) ? TRUE : FALSE)
#define cg_upnp_ssdpresponse_server_getsocket(server) (server->httpuSock)

#define cg_upnp_ssdpresponse_server_setlistener(server, func) (server->listener = func)
#define cg_upnp_ssdpresponse_server_getlistener(server) (server->listener)
void cg_upnp_ssdpresponse_server_performlistener(CgUpnpSSDPResponseServer *server, CgUpnpSSDPPacket *ssdpPkt);

#define cg_upnp_ssdpresponse_server_setuserdata(server, data) (server->userData = data)
#define cg_upnp_ssdpresponse_server_getuserdata(server) (server->userData)

BOOL cg_upnp_ssdpresponse_server_start(CgUpnpSSDPResponseServer *server);
BOOL cg_upnp_ssdpresponse_server_stop(CgUpnpSSDPResponseServer *server);

BOOL cg_upnp_ssdpresponse_server_post(CgUpnpSSDPResponseServer *server, CgUpnpSSDPRequest *ssdpReq);

/****************************************
* Function (SSDPResponseServerList)
****************************************/

CgUpnpSSDPResponseServerList *cg_upnp_ssdpresponse_serverlist_new();
void cg_upnp_ssdpresponse_serverlist_delete(CgUpnpSSDPResponseServerList *serverList);

#define cg_upnp_ssdpresponse_serverlist_clear(serverList) cg_list_clear((CgList *)serverList, (CG_LIST_DESTRUCTORFUNC)cg_upnp_ssdpresponse_server_delete)
#define cg_upnp_ssdpresponse_serverlist_size(serverList) cg_list_size((CgList *)serverList)
#define cg_upnp_ssdpresponse_serverlist_gets(serverList) (CgUpnpSSDPResponseServer *)cg_list_next((CgList *)serverList)
#define cg_upnp_ssdpresponse_serverlist_add(serverList, server) cg_list_add((CgList *)serverList, (CgList *)server)

BOOL cg_upnp_ssdpresponse_serverlist_open(CgUpnpSSDPResponseServerList *ssdpServerList, int bindPort);
BOOL cg_upnp_ssdpresponse_serverlist_close(CgUpnpSSDPResponseServerList *ssdpServerList);
BOOL cg_upnp_ssdpresponse_serverlist_start(CgUpnpSSDPResponseServerList *ssdpServerList);
BOOL cg_upnp_ssdpresponse_serverlist_stop(CgUpnpSSDPResponseServerList *ssdpServerList);
void cg_upnp_ssdpresponse_serverlist_setlistener(CgUpnpSSDPResponseServerList *ssdpServerList, CG_UPNP_SSDP_LISTNER listener);
void cg_upnp_ssdpresponse_serverlist_setuserdata(CgUpnpSSDPResponseServerList *ssdpServerList, void *data);
BOOL cg_upnp_ssdpresponse_serverlist_post(CgUpnpSSDPResponseServerList *ssdpServerList, CgUpnpSSDPRequest *ssdpReq);

#ifdef  __cplusplus
}
#endif

#endif
