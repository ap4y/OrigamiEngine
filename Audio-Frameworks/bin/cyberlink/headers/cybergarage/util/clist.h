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
*	File: clist.h
*
*	Revision:
*
*	01/18/05
*		- first revision
*
*	10/31/05
*		- Added comments to all functions and structs
*
******************************************************************/

#ifndef _CG_UTIL_CLIST_H_
#define _CG_UTIL_CLIST_H_

#include <cybergarage/typedef.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

/**
 * Prototype for individual list node destructor functions
 *
 */
typedef void (*CG_LIST_DESTRUCTORFUNC)(void *);

/****************************************
 * Data Type
 ****************************************/

/**
 * \brief The generic list interface in CyberLinkC.
 *
 * Each struct in CyberLinkC, that is designed to be a part of
 * a list, must have these elements in their definition before the
 * actual struct definition. The struct pointers are then cast to
 * CgList* and operated with cg_list_* functions.
 */
typedef struct _CgList {

  /** Marks the beginning of a list */
  BOOL headFlag;
  /** Pointer to the next list node */
  struct _CgList *prev;
  /** Pointer to the previous list node */
  struct _CgList *next;

} CgList;

/****************************************
 * Functions
 ****************************************/

/**
 * Initialize a list node to act as the first node
 *
 * \param list List node
 */
void cg_list_header_init(CgList *list);

/**
 * Initialize a list node to act as a regular node, part of a list.
 *
 * \param list List node
 */
void cg_list_node_init(CgList *list);

/**
 * Insert a list node or a complete list structure after the given node
 *
 * \param prevList Insert after this node
 * \param list List node or list structure to insert
 */
void cg_list_insert(CgList *prevList, CgList *list);

/**
 * \todo This works essentially like insert, although it is more error-prone?!
 * \todo There might be a bug in this function.
 *
 */
void cg_list_add(CgList *headList, CgList *list);

/**
 * Remove a node from a list. Does not free any memory, but only removes
 * the next and previous link associations regarding the given node.
 *
 * \param list List node to remove
 */
void cg_list_remove(CgList *list);

/**
 * Get the number of nodes in the current list structure. Counts forwards from the given
 * node, so if you want to get the complete size, give a header node as the parameter.
 *
 * \param headList List header
 */
int cg_list_size(CgList *headList);

/**
 * Get an item from the list by the item's index
 *
 * \param headList List header
 * \param index The index of the item to get
 */
CgList *cg_list_get(CgList *headList, int index);

/**
 * Get the previous node. Wrap around if the beginning has been reached.
 * 
 *
 * \param list Current node
 */
CgList *cg_list_prev_circular(CgList *list);

/**
 * Get the previous node. Returns NULL if beginning has been reached
 *
 * \param list Current node
 */
CgList *cg_list_prev(CgList *list);

/**
 * Get the next node. Wrap around if the end has been reached.
 *
 * \param list Current node
 */
CgList *cg_list_next_circular(CgList *list);

/**
 * Get the next node. Returns NULL if end has been reached.
 *
 * \param list Current node
 */
CgList *cg_list_next(CgList *list);

/**
 * Clear the list and delete all of its contents with \ref CG_LIST_DESTRUCTORFUNC
 *
 * \param headList List header
 * \param destructorFunc Function pointer that clears the contents of individual nodes
 */
void cg_list_clear(CgList *headList, CG_LIST_DESTRUCTORFUNC destructorFunc);

/**
 * Get the first actual item from a list for iteration
 *
 * \param headList List header
 */
#define cg_list_gets(headList) cg_list_next(headList)

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif
