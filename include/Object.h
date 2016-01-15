/* Object.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef DDDS_OBJECT_H
#define DDDS_OBJECT_H

#include "corto/corto.h"
#include "dDDS/_type.h"
#include "dDDS/_api.h"
#include "dDDS/_meta.h"
#include "dDDS/_interface.h"

#ifdef __cplusplus
extern "C" {
#endif


DDDS_EXPORT dDDS_Object _dDDS_Object_new(dDDS_Struct type, corto_string json);
#define dDDS_Object_new(type, json) _dDDS_Object_new(dDDS_Struct(type), json)

DDDS_EXPORT corto_void _dDDS_Object_set(dDDS_Object sample, corto_string json);
#define dDDS_Object_set(sample, json) _dDDS_Object_set(sample, json)

#ifdef __cplusplus
}
#endif
#endif

