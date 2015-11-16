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
*	File: cdictionary.h
*
*	Revision:
*
*	05/26/09
*		- first revision
*
******************************************************************/

#ifndef _CG_UTIL_CDICTIONARY_H_
#define _CG_UTIL_CDICTIONARY_H_

#include <cybergarage/typedef.h>
#include <cybergarage/util/clist.h>
#include <cybergarage/util/cstring.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

/****************************************
 * Data Type
 ****************************************/

/**
 * \brief The generic wrapper struct for CyberLinkC's dictionarys.
 *
 * This wrapper has been created to enable 100% code
 * compatibility between different platforms (Linux, Win32 etc..)
 */
typedef struct _CgDictionary {
	BOOL headFlag;
	struct _CgDictionary *prev;
	struct _CgDictionary *next;
	CgString *key;
	CgString *value;
} CgDictionaryElement, CgDictionary;

/****************************************
* Function
****************************************/

/**
 * Create a new dictionary
 */
CgDictionaryElement *cg_dictionary_element_new();

/**
 * Destroy a dictionary.
 *
 * \param dirElem Dictionary to destroy
 */
BOOL cg_dictionary_element_delete(CgDictionaryElement *dirElem);

/**
 * Set a key
 *
 * \param dirElem Dictionary struct
 * \param name String to set 
 */
#define cg_dictionary_element_setkey(dirElem, name) cg_string_setvalue(dirElem->key, name)
/**
 * Get a key
 *
 * \param dirElem Dictionary struct
 */
#define cg_dictionary_element_getkey(dirElem) cg_string_getvalue(dirElem->key)

/**
 * Set a value
 *
 * \param dirElem Dictionary struct
 * \param value String to set
 */
#define cg_dictionary_element_setvalue(dirElem, val) cg_string_setvalue(dirElem->value, val)
/**
 * Get a value
 *
 * \param dirElem Dictionary struct
 */
#define cg_dictionary_element_getvalue(dirElem) cg_string_getvalue(dirElem->value)

#define cg_dictionary_element_next(dirElem) (CgDictionaryElement *)cg_list_next((CgList *)dirElem)
#define cg_dictionary_element_remove(dirElem) cg_list_remove((CgList *)dirElem)

/****************************************
* Function (Dictionary List)
****************************************/

/**
 * Create a new dictionary list
 *
 * \return Dictionary list
 */
CgDictionary *cg_dictionary_new();

/**
 * Destroy a dictionary list
 *
 * \param dir The dictionary list in question
 */
void cg_dictionary_delete(CgDictionary *dir);

/**
 * Clear the contents of a dictionary list
 *
 * \param dir Dictionary list in question
 */
#define cg_dictionary_clear(dir) cg_list_clear((CgList *)dir, (CG_LIST_DESTRUCTORFUNC)cg_dictionary_element_delete)

/**
 * Get the size of a dictionary list
 *
 * \param dir The dictionary list in question
 */
#define cg_dictionary_size(dir) cg_list_size((CgList *)dir)

/**
 * Get the first actual item from a dictionary list to use as an iterator
 *
 * \param dir The dictionary list in question
 */
#define cg_dictionary_gets(dir) (CgDictionaryElement *)cg_list_next((CgList *)dir)

/**
 * Add a dictionary into a dictionary list
 *
 * \param dir The dictionary list in question
 * \param dirElem The dictionary to add to the list
 */
#define cg_dictionary_add(dir, dirElem) cg_list_add((CgList *)dir, (CgList *)dirElem)

/**
 * Remove a dictionary from dictionary list
 *
 * \param dirElem The dictionary to be removed 
 */
#define cg_dictionary_remove(dirElem) cg_list_remove((CgList *)dirElem)

/**
 * Get a element of the specified key
 *
 * \param dir The dictionary list in question
 * \param key Name to search.
 * \return Element of the specified key.
 */
CgDictionaryElement *cg_dictionary_get(CgDictionary *dir, char *key);

/**
 * Set a new element into a dictionary 
 *
 * \param dir The dictionary list in question
 * \param key Name to set.
 * \param value Value to set.
 */
void cg_dictionary_setvalue(CgDictionary *dir, char *key, char *value);

/**
 * Get a value of the specified key
 *
 * \param dir The dictionary list in question
 * \param key Name to search.
 * \return Value of the specified key.
 */
char *cg_dictionary_getvalue(CgDictionary *dir, char *key);

#ifdef  __cplusplus

} /* extern "C" */

#endif

#endif
