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


DDDS_EXPORT dDDS_String _dDDS_Object_json(dDDS_Object sample);
#define dDDS_Object_json(sample) _dDDS_Object_json(sample)

DDDS_EXPORT dDDS_String _dDDS_Object_nameof(dDDS_Object _this);
#define dDDS_Object_nameof(_this) _dDDS_Object_nameof(_this)

DDDS_EXPORT dDDS_Object _dDDS_Object_new(dDDS_Struct type, corto_string json);
#define dDDS_Object_new(type, json) _dDDS_Object_new(dDDS_Struct(type), json)

DDDS_EXPORT dDDS_Object _dDDS_Object_parentof(dDDS_Object _this);
#define dDDS_Object_parentof(_this) _dDDS_Object_parentof(_this)

DDDS_EXPORT corto_void _dDDS_Object_set(dDDS_Object sample, corto_string json);
#define dDDS_Object_set(sample, json) _dDDS_Object_set(sample, json)

DDDS_EXPORT corto_type _dDDS_Object_typeof(dDDS_Object _this);
#define dDDS_Object_typeof(_this) _dDDS_Object_typeof(_this)

#ifdef __cplusplus
}
#endif
#endif

