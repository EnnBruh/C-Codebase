#include "core_internal.h"

_CoreState _core_state;

#ifdef ENN_INCLUDE_OPTIONAL
ENNDEF_PUBLIC void glfw_err(i32 error, const char* description) {
        DEBUG_TRACE();
        LOG("[GLFW] Error code %" PRIi32, error);
        LOG("[GLFW] Error %s", description);
        DEBUG_UNTRACE();
}
#endif

void core_init(void) {
        DEBUG_TRACE();

        directory_create(ENN_APP_DIRECTORY);
        directory_create(ENN_APP_DIRECTORY ENN_DATA_PATH);
        directory_create(ENN_APP_DIRECTORY ENN_LOGS_PATH);

#       ifdef ENN_INCLUDE_OPTIONAL
                directory_create(ENN_APP_DIRECTORY ENN_ASSETS_PATH);
                directory_create(ENN_APP_DIRECTORY ENN_SHADERS_PATH);
                directory_create(ENN_APP_DIRECTORY ENN_FONTS_PATH);
                directory_create(ENN_APP_DIRECTORY ENN_TEXTURES_PATH);
#       endif

        debug_init();
        LOG("[Core] Initializing all processes");

#       ifdef ENN_INCLUDE_OPTIONAL
                ASSERT(network_init(), "[Core] Could not initialize network. Windows Startup error");
                LOG("[Core] Network succesfully initialized");
#       endif


#       ifdef ENN_INCLUDE_OPTIONAL
                glfwSetErrorCallback(glfw_err);
                ASSERT(glfwInit(), "[Core] GLFW Could not be initialized");
                LOG("[Core] GLFW Succesfully initialized");
#       endif

        DEBUG_UNTRACE();
}

#ifdef ENN_MULTI_WINDOW

void core_run(void) {
        DEBUG_TRACE();
        i32 i, j;

        for (i = _core_state._window_arr.start; i < _core_state._window_arr.end; ++i) {
                glfwMakeContextCurrent(_core_state._window_arr.data[i].handle);
                glClearColor(_core_state._window_arr.data[i].buffer_clear_color.x, _core_state._window_arr.data[i].buffer_clear_color.y, _core_state._window_arr.data[i].buffer_clear_color.z, _core_state._window_arr.data[i].buffer_clear_color.w);

                Event event = {
                        .id        = 0x0,
                        .type      = ENN_WINDOW_REGISTER_EVENT,
                        .handled   = false,
                };

                struct { WindowID id; } data = { .id = _core_state._window_arr.data[i].id };
                event.data = (void*)&data;

                for (j = _core_state._window_arr.data[i].layer_stack.start; j < _core_state._window_arr.data[i].layer_stack.end; ++j)
                        _core_state._window_arr.data[i].layer_stack.data[j].on_event(&event);

                LOG("[Core] All Layers succesfully initialized for window %#" PRIx64 " with name '%s'", _core_state._window_arr.data[i].id, _core_state._window_arr.data[i].name);
        }
        LOG("[Core] All Windows succesfully initialized");

        if (vector_size(_core_state._window_arr) > 0) _core_state._running = true;
        f64 last_frame = 0.0;

        while (_core_state._running) {
                _core_state._time_current = glfwGetTime();
                _core_state._time_delta = _core_state._time_current - last_frame;
                last_frame = _core_state._time_current;

                for (i = _core_state._window_arr.start; i < _core_state._window_arr.end; ++i) {
                        for (j = _core_state._window_arr.data[i].layer_stack.start; j < _core_state._window_arr.data[i].layer_stack.end && _core_state._window_arr.data[i].layer_stack.data[j].active; ++j)
                                        _core_state._window_arr.data[i].layer_stack.data[j].on_update(_core_state._time_current);
                        
#                       ifdef ENN_FOCUS_WINDOW_OPTIMIZAION
                                if ((_core_state._window_arr.data[i].flags & (1 << ENN_WINDOW_FOCUS_BIT))) {
#                       endif
                        glfwMakeContextCurrent(_core_state._window_arr.data[i].handle);
                                glClear(GL_COLOR_BUFFER_BIT);

                                for (j = _core_state._window_arr.data[i].layer_stack.start; j < _core_state._window_arr.data[i].layer_stack.end && _core_state._window_arr.data[i].layer_stack.data[j].active; ++j)
                                        _core_state._window_arr.data[i].layer_stack.data[j].on_render();
                                
                                glfwSwapBuffers(_core_state._window_arr.data[i].handle);
#                       ifdef ENN_FOCUS_WINDOW_OPTIMIZATION
                        }
#                       endif
                }

                glfwPollEvents();
        }

        DEBUG_UNTRACE();
}

void core_term(void) {
        DEBUG_TRACE();

#       ifdef ENN_INCLUDE_OPTIONAL
        while (vector_size(_core_state._window_arr) > 0)
                window_destroy(_core_state._window_arr.data[0].id);

        vector_destroy(_core_state._window_arr);

        LOG("[Core] All windows terminated");

        glfwTerminate();
        LOG("[Core] GLFW Terminated");

        network_term();
        LOG("[Core] Network Terminated");
#       endif

        LOG("[Core] All processes terminated\n");
        debug_term();
        DEBUG_UNTRACE();
}

#else


void core_run(void) {
        DEBUG_TRACE();
        i32 i;

        if (_core_state._window_main.id != ENN_WINDOW_INVALID_ID) {
                glfwMakeContextCurrent(_core_state._window_main.handle);
                glClearColor(_core_state._window_main.buffer_clear_color.x, _core_state._window_main.buffer_clear_color.y, _core_state._window_main.buffer_clear_color.z, _core_state._window_main.buffer_clear_color.w);


                LOG("[Core] All Layers succesfully initialized for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);

                _core_state._running = true;
                f64 last_frame = 0.0;


                while (_core_state._running) {
                        _core_state._time_current = glfwGetTime();
                        _core_state._time_delta = _core_state._time_current - last_frame;
                        last_frame = _core_state._time_current;

                        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers; ++i)
                                        _core_state._window_main.layer_stack.data[i].on_update(_core_state._time_delta);

#                       ifdef ENN_FOCUS_WINDOW_OPTIMIZATION
                        if ((_core_state._window_main.flags & (1 << ENN_WINDOW_FOCUS_BIT))) {
#                       endif
                                glClear(GL_COLOR_BUFFER_BIT);

                                for (i = _core_state._window_main.layer_stack.end - 1 - _core_state._window_main.num_inactive_layers; i >= _core_state._window_main.layer_stack.start; --i)
                                        _core_state._window_main.layer_stack.data[i].on_render();

                                glfwSwapBuffers(_core_state._window_main.handle);
#                       ifdef ENN_FOCUS_WINDOW_OPTIMIZATION
                        }
#                       endif

                glfwPollEvents();
                }
        }
        DEBUG_UNTRACE();
}

void core_term(void) {
        DEBUG_TRACE();

#       ifdef ENN_INCLUDE_OPTIONAL
        if (_core_state._window_main.id != ENN_WINDOW_INVALID_ID)
        window_destroy();

        LOG("[Core] All Windows Terminated");

        glfwTerminate();
        LOG("[Core] GLFW Terminated");
        network_term();
        LOG("[Core] Network Terminated");
#       endif

        LOG("[Core] All Processes Terminated\n");
        debug_term();
        DEBUG_UNTRACE();
}

#endif

void core_stop(void) {
        DEBUG_TRACE();
        LOG("[Core] Core Stop request recieved");
        _core_state._running = false;
        DEBUG_UNTRACE();
}