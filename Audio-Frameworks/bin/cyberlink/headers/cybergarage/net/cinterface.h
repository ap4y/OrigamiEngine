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
*	File: cinterface.h
*
*	Revision:
*
*	02/09/05
*		- first revision
*	03/23/06 Theo Beisch
*		- added localhost constants
*	09/12/07
*		- Added the following functions to get MAC address.
*		  cg_net_interface_setmacaddress(), cg_net_interface_getmacaddress()
*		- Changed cg_net_gethostinterfaces() to get the MAC address using GetAdaptersInfo() as default on Windows platform.
*		- Changed cg_net_gethostinterfaces() to get the MAC address using getifaddrs() on UNIX platform.
*		   Note : Other platforms might not support to get this functions yet. 
*
******************************************************************/

#ifndef _CG_NET_CINTERFACE_H_
#define _CG_NET_CINTERFACE_H_

#include <cybergarage/typedef.h>
#include <cybergarage/util/cstring.h>
#include <cybergarage/util/clist.h>

#if !defined(WIN32)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#define CG_NET_IPV4_ADDRSTRING_MAXSIZE ((3*4)+(1*3)+1)
#define CG_NET_IPV6_ADDRSTRING_MAXSIZE (1+(8*4)+(1*7)+1+1)

#define CG_NET_IPV4_LOOPBACK "127.0.0.1"
#define CG_NET_IPV6_LOOPBACK "fixmelater"
#define CG_NET_MACADDR_SIZE 6

#if defined(BTRON) || defined(TENGINE)
#define CG_NET_DEFAULT_IFNAME "Neta"
#endif

/****************************************
* Data Type
****************************************/

typedef struct _CgNetworkInterface {
	BOOL headFlag;
	struct _CgNetworkInterface *prev;
	struct _CgNetworkInterface *next;
	CgString *name;
	CgString *ipaddr;
	CgString *netmask;
	CgByte macaddr[CG_NET_MACADDR_SIZE];
	int index;
} CgNetworkInterface, CgNetworkInterfaceList;

/****************************************
* Function (NetworkInterface)
****************************************/

CgNetworkInterface *cg_net_interface_new();
void cg_net_interface_delete(CgNetworkInterface *netIf);
CgNetworkInterface* cg_net_interface_getany();

#define cg_net_interface_next(netIf) (CgNetworkInterface *)cg_list_next((CgList *)netIf)
#define cg_net_interface_remove(netIf) cg_list_remove((CgList *)netIf)

void cg_net_interface_setname(CgNetworkInterface *netIf, char *name);
char *cg_net_interface_getname(CgNetworkInterface *netIf);
void cg_net_interface_setaddress(CgNetworkInterface *netIf, char *ipaddr);
char *cg_net_interface_getaddress(CgNetworkInterface *netIf);
void cg_net_interface_setnetmask(CgNetworkInterface *netIf, char *ipaddr);
char *cg_net_interface_getnetmask(CgNetworkInterface *netIf);
char *cg_net_selectaddr(struct sockaddr *remoteaddr);

#define cg_net_interface_setmacaddress(netIf, value) memcpy(netIf->macaddr, value, CG_NET_MACADDR_SIZE)
#define cg_net_interface_getmacaddress(netIf, buf) memcpy(buf, netIf->macaddr, CG_NET_MACADDR_SIZE)

#define cg_net_interface_setindex(netIf, value) (netIf->index = value)
#define cg_net_interface_getindex(netIf, buf) (netIf->index)

/**
 * Compares two interfaces based on IP-address.
 */
int cg_net_interface_cmp(CgNetworkInterface *netIfA, 
			 CgNetworkInterface *netIfB);

/****************************************
* Function (NetworkInterfaceList)
****************************************/

CgNetworkInterfaceList *cg_net_interfacelist_new();
void cg_net_interfacelist_delete(CgNetworkInterfaceList *netIfList);

#define cg_net_interfacelist_clear(netIfList) cg_list_clear((CgList *)netIfList, (CG_LIST_DESTRUCTORFUNC)cg_net_interface_delete)
#define cg_net_interfacelist_size(netIfList) cg_list_size((CgList *)netIfList)
#define cg_net_interfacelist_gets(netIfList) (CgNetworkInterface *)cg_list_next((CgList *)netIfList)
#define cg_net_interfacelist_add(netIfList,netIf) cg_list_add((CgList *)netIfList, (CgList *)netIf)

CgNetworkInterface *cg_net_interfacelist_get(CgNetworkInterfaceList *netIfList, char *name);

/**
 * Gets changes in the two given (aka old and new) interface lists. Changes
 * are resolved based on IP-addresses.
 * 
 * @param netIfListOld List of the old interfaces. It is changed!
 * @param netIfListNew List of the new interfaces. It is changed!
 * @param netIfListAdded List to store interfaces, which were in netIfListNew,
 *			 but were not in netIfListOld. May be NULL.
 * @param netIfListRemoved List to store interfaces, which were in 
 *			   netIfListOld, but were not in netIfListNew.
 */
void cg_net_interfacelist_getchanges(CgNetworkInterfaceList *netIfListOld,
				     CgNetworkInterfaceList *netIfListNew,
				     CgNetworkInterfaceList *netIfListAdded,
				     CgNetworkInterfaceList *netIfListRemoved);

/****************************************
* Function
****************************************/

#if defined(ITRON)
void cg_net_setinterface(char *ifaddr);
#endif

int cg_net_gethostinterfaces(CgNetworkInterfaceList *netIfList);

BOOL cg_net_isipv6address(char *addr);
int cg_net_getipv6scopeid(char *addr);

#ifdef  __cplusplus
}
#endif

#endif

