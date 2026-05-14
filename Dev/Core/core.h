#ifndef ENN_CORE_H
#define ENN_CORE_H

#ifdef __cplusplus
        extern "C" {
#endif

#include "config.h"
#include "Base/base.h"

#ifdef ENN_INCLUDE_OPTIONAL
#	include "Optional/opt.h"
#endif

ENNDEF_PRIVATE void core_init(void);
ENNDEF_PRIVATE void core_run(void);
ENNDEF_PRIVATE void core_term(void);
ENNDEF_PRIVATE void core_stop(void);


#ifdef __cplusplus
        }
#endif

#endif
