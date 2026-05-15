#ifndef ENN_OPTIONAL_INTERNAL_H
#define ENN_OPTIONAL_INTERNAL_H

#ifdef __cplusplus
        extern "C" {
#endif

#include "config.h"
#include "Optional/opt.h"

#ifdef ENN_INCLUDE_OPTIONAL

struct _CoreState {
        bool            _running;

#ifdef ENN_MULTI_WINDOW
        vector(Window)  _window_arr;
#else 
        Window          _window_main;
#endif
        f64             _time_current;
        f64             _time_delta;
};

extern struct _CoreState _core_state;

#endif


#ifdef __cplusplus
        }
#endif

#endif
