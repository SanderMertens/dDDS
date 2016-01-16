/* dDDS.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef DDDS_H
#define DDDS_H

#include "corto/corto.h"
#include "dDDS/_type.h"
#include "dDDS/_api.h"
#include "dDDS/_meta.h"
#include "dDDS/_interface.h"

#ifdef __cplusplus
extern "C" {
#endif


/* $header() */

/* Free heap object. Use dDDS_delete for dDDS_Object and entities.  */
DDDS_EXPORT void dDDS_free(void* ptr);

/* Report error to stderr */
DDDS_EXPORT void dDDS_error(const char* fmt, ...);

/* Set error, don't report to stderr */
DDDS_EXPORT void dDDS_seterr(const char* fmt, ...);

/* Retrieve last error set by seterr */
DDDS_EXPORT const char* dDDS_lasterr(void);

/* $end */

DDDS_EXPORT corto_void _dDDS_delete(dDDS_Object object);
#define dDDS_delete(object) _dDDS_delete(object)
#include "dDDS/Array.h"
#include "dDDS/DataReader.h"
#include "dDDS/DataWriter.h"
#include "dDDS/DomainParticipant.h"
#include "dDDS/Enum.h"
#include "dDDS/Member.h"
#include "dDDS/Module.h"
#include "dDDS/Object.h"
#include "dDDS/Publisher.h"
#include "dDDS/Sequence.h"
#include "dDDS/Struct.h"
#include "dDDS/Subscriber.h"
#include "dDDS/Topic.h"

#ifdef __cplusplus
}
#endif
#endif

