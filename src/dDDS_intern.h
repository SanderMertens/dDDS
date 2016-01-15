/* dDDS_intern.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef DDDS_INTERN_H
#define DDDS_INTERN_H

#include "dds_dcps.h"

/* Object local storage handle to attach DDS entities to Corto objects */
extern corto_int16 DDDS_ENTITY_HANDLE;

#define dDDS_getEntity(o, type) _dDDS_getEntity(o, corto_type(type))
void* _dDDS_getEntity(corto_object o, corto_type type);

#endif
