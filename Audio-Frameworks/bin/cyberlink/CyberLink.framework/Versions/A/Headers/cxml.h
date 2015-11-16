/******************************************************************
*
*	CyberNet for C
*
*	Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006-2007 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: cxml.h
*
*	Revision:
*
*	02/07/05
*		- first revision
*	10/22/07 Aapo Makela
*		- Added NULL check to cg_xml_node_getchildnode() macro
*
******************************************************************/

#ifndef _CG_XML_CXML_H_
#define _CG_XML_CXML_H_

#include <cybergarage/typedef.h>
#include <cybergarage/util/cstring.h>
#include <cybergarage/util/clist.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#define CG_XML_INDENT_STRING " "
#define CG_XML_VERSION_HEADER "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
#define CG_XML_CONTENT_TYPE "text/xml; charset=\"utf-8\""
#define CG_XML_CONTENT_LF "\n"

typedef void (*CG_XML_NODE_USERDATA_DESTRUCTORFUNC)(void *);

/****************************************
* Data Type
****************************************/

typedef struct _CgXmlAttribute {
	BOOL headFlag;
	struct _CgXmlAttribute *prev;
	struct _CgXmlAttribute *next;
	CgString *name;
	CgString *value;
} CgXmlAttribute, CgXmlAttributeList;

typedef struct _CgXmlNode {
	BOOL headFlag;
	struct _CgXmlNode *prev;
	struct _CgXmlNode *next;
	CgString *name;
	CgString *value;
	struct _CgXmlAttribute *attrList;
	struct _CgXmlNode *nodeList;
	struct _CgXmlNode *parentNode;
	void *userData;
	CG_XML_NODE_USERDATA_DESTRUCTORFUNC userDataDestructorFunc;
} CgXmlNode, CgXmlNodeList;

typedef struct _CgXmlParser {
	BOOL parseResult;
} CgXmlParser;

/****************************************
* Function (AttribureList)
****************************************/

CgXmlAttributeList *cg_xml_attributelist_new();
void cg_xml_attributelist_delete(CgXmlAttributeList *attrList);

#define cg_xml_attributelist_clear(attrList) cg_list_clear((CgList *)attrList, (CG_LIST_DESTRUCTORFUNC)cg_xml_attribute_delete)
#define cg_xml_attributelist_size(attrList) cg_list_size((CgList *)attrList)
#define cg_xml_attributelist_gets(attrList) (CgXmlAttribute *)cg_list_next((CgList *)attrList)
#define cg_xml_attributelist_add(attrList, attr) cg_list_add((CgList *)attrList, (CgList *)attr)

void cg_xml_attributelist_set(CgXmlAttributeList *attrList, char *name, char *value);
CgXmlAttribute *cg_xml_attributelist_get(CgXmlAttributeList *attrList, char *name);
char *cg_xml_attributelist_getvalue(CgXmlAttributeList *attrList, char *name);

/****************************************
* Function (Attributes)
****************************************/

CgXmlAttribute *cg_xml_attribute_new();
void cg_xml_attribute_delete(CgXmlAttribute *attr);

#define cg_xml_attribute_next(attr) (CgXmlAttribute *)cg_list_next((CgList *)attr)
#define cg_xml_attribute_remove(attr) cg_list_remove((CgList *)attr)

void cg_xml_attribute_setname(CgXmlAttribute *attr, char *name);
char *cg_xml_attribute_getname(CgXmlAttribute *attr);
BOOL cg_xml_attribute_isname(CgXmlAttribute *attr, char *name);

void cg_xml_attribute_setvalue(CgXmlAttribute *attr, char *value);
char *cg_xml_attribute_getvalue(CgXmlAttribute *attr);

/****************************************
* Function (NodeList)
****************************************/

CgXmlNodeList *cg_xml_nodelist_new();
void cg_xml_nodelist_delete(CgXmlNodeList *nodeList);

#define cg_xml_nodelist_clear(nodeList) cg_list_clear((CgList *)(nodeList), (CG_LIST_DESTRUCTORFUNC)cg_xml_node_delete)
#define cg_xml_nodelist_size(nodeList) cg_list_size((CgList *)(nodeList))
#define cg_xml_nodelist_gets(nodeList) (CgXmlNode *)cg_list_next((CgList *)(nodeList))
#define cg_xml_nodelist_add(nodeList, node) cg_list_add((CgList *)(nodeList), (CgList *)(node))

/* Deprecated : Use cg_xml_nodelist_getbyname */
#define cg_xml_nodelist_get(nodeList, name) cg_xml_nodelist_getbyname(nodeList, name)
CgXmlNode *cg_xml_nodelist_getbyname(CgXmlNodeList *nodeList, char *name);
CgXmlNode *cg_xml_nodelist_getbyxpath(CgXmlNodeList *nodeList, char *xpath);

/****************************************
* Function (Node)
****************************************/

CgXmlNode *cg_xml_node_new();
void cg_xml_node_delete(CgXmlNode *node);

#define cg_xml_node_next(node) (CgXmlNode *)cg_list_next((CgList *)node)
#define cg_xml_node_remove(node) cg_list_remove((CgList *)node)

#define cg_xml_node_setname(node, nodename) cg_string_setvalue(node->name, nodename)
#define cg_xml_node_getname(node) cg_string_getvalue(node->name)
#define cg_xml_node_isname(node, nodename) (cg_strcmp(cg_xml_node_getname(node), nodename) == 0 ? TRUE : FALSE)

#define cg_xml_node_setvalue(node, nodevalue) cg_string_setvalue(node->value, nodevalue)
#define cg_xml_node_setintvalue(node, nodevalue) cg_string_setintvalue(node->value, nodevalue)
#define cg_xml_node_setlongvalue(node, nodevalue) cg_string_setlongvalue(node->value, nodevalue)
#define cg_xml_node_setnvalue(node, nodevalue, nodevaluelen) cg_string_setnvalue(node->value, nodevalue, nodevaluelen)
#define cg_xml_node_addvalue(node, nodevalue) cg_string_addvalue(node->value, nodevalue)
#define cg_xml_node_naddvalue(node, nodevalue, len) cg_string_naddvalue(node->value, nodevalue, len)
#define cg_xml_node_getvalue(node) cg_string_getvalue(node->value)
#define cg_xml_node_getintvalue(node) cg_string_getintvalue(node->value)
#define cg_xml_node_getlongvalue(node) cg_string_getlongvalue(node->value)

#define cg_xml_node_setparentnode(node, pnode) (node->parentNode = pnode)
#define cg_xml_node_getparentnode(node) (node->parentNode)
CgXmlNode *cg_xml_node_getrootnode(CgXmlNode *node);

#define cg_xml_node_getchildnodelist(node) (node->nodeList)
void cg_xml_node_addchildnode(CgXmlNode *node, CgXmlNode *cnode);
void cg_xml_node_setchildnode(CgXmlNode *node, char *name, char *value);
char *cg_xml_node_getchildnodevalue(CgXmlNode *node, char *name);
#define cg_xml_node_getchildnodeintvalue(node, name) cg_str2int(cg_xml_node_getchildnodevalue(node, name))
#define cg_xml_node_getchildnodelongvalue(node, name) cg_str2long(cg_xml_node_getchildnodevalue(node, name))
	
#define cg_xml_node_setuserdata(node, data) (node->userData = data)
#define cg_xml_node_getuserdata(node) (node->userData)
#define cg_xml_node_setuserdatadestructor(node, func) (node->userDataDestructorFunc = func)

#define cg_xml_node_getchildnodes(node) cg_xml_nodelist_gets(node->nodeList)
#define cg_xml_node_haschildnodes(node) ((cg_xml_node_getchildnodes(node) != NULL) ? TRUE : FALSE) 
/* Deprecated : Use cg_xml_node_getchildnodebyname */
#define cg_xml_node_getchildnode(node,name) ((node != NULL) ? cg_xml_nodelist_getbyname(node->nodeList,name) : NULL)
#define cg_xml_node_getchildnodebyname(node,name) ((node != NULL) ? cg_xml_nodelist_getbyname(node->nodeList,name) : NULL)
#define cg_xml_node_getchildnodebyxpath(node,xpath) ((node != NULL) ? cg_xml_nodelist_getbyxpath(node->nodeList,xpath) : NULL)

/* Get childnode with some specific namespace prefix, or ignore namespace prefix.
   If ns is NULL, name string must match completely */
CgXmlNode *cg_xml_node_getchildnodewithnamespace(CgXmlNode *node, char *name, char *ns, BOOL ignoreNs);

#define cg_xml_node_getattributes(node) cg_xml_attributelist_gets(node->attrList)
#define cg_xml_node_getattribute(node, name) cg_xml_attributelist_get(node->attrList,name)
#define cg_xml_node_addattribute(node, attr) cg_xml_attributelist_add(node->attrList,attr)
#define cg_xml_node_setattribute(node, name, value) cg_xml_attributelist_set(node->attrList,name,value)
#define cg_xml_node_getattributevalue(node, name) cg_xml_attributelist_getvalue(node->attrList, name)
void cg_xml_node_removeattribute(CgXmlNode *node, char *name);

#define cg_xml_node_setnamespace(node, ns, value) cg_xml_node_setattribute(node, "xmlns:" ns , value)

char *cg_xml_node_tostring(CgXmlNode *node, BOOL withChildNode, CgString *str);
void  cg_xml_node_print(CgXmlNode *node);

void cg_xml_node_copy(CgXmlNode *dstNode, CgXmlNode *srcNode);

/****************************************
* Function (Parser)
****************************************/

CgXmlParser *cg_xml_parser_new();
void cg_xml_parser_delete(CgXmlParser *parser);
BOOL cg_xml_parse(CgXmlParser *parser, CgXmlNodeList *nodeList, char *data, int len);

/****************************************
* Function (Other)
****************************************/

char *cg_xml_escapechars(CgString *str);
char *cg_xml_unescapechars(CgString *str);


#ifdef  __cplusplus
}
#endif

#endif
