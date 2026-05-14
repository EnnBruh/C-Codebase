#include "Optional/opt_internal.h"

static EventID  _event_id_generator  = 0x1;
static LayerID  _layer_id_generator  = 0x1;

#ifdef ENN_MULTI_WINDOW

static WindowID _window_id_generator = 0x1;

ENNDEF_PUBLIC void window_on_focus(GLFWwindow* handle, i32 focused) {
        DEBUG_TRACE();
        Window* window = window_get_by_id((WindowID)glfwGetWindowUserPointer(handle));
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, (WindowID)glfwGetWindowUserPointer(handle));

        if (focused) {
                window -> flags |= (1 << ENN_WINDOW_FOCUS_BIT);
                DEBUG_LOG("[Core] Window %#" PRIx64 " with name '%s' came into focus", window -> id, window -> name);
        } else {
                window -> flags &= ~(1 << ENN_WINDOW_FOCUS_BIT);
                DEBUG_LOG("[Core] Window %#" PRIx64 " with name '%s' has gone out of focus", window -> id, window -> name);
        }
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_framebuff_resize(GLFWwindow* handle, i32 new_width, i32 new_height) {
        DEBUG_TRACE();
        Window* window = window_get_by_id((WindowID)glfwGetWindowUserPointer(handle));
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, (WindowID)glfwGetWindowUserPointer(handle));

        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_WINDOW_RESIZE_EVENT,
                .handled = false,
        };

        window -> framebuff_dim.x = new_width; window -> framebuff_dim.y = new_height;
        struct { i32 width, height; } data = { new_width, new_height };
        event.data = (void*)&data;

        i32 i;

        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active && !event.handled; ++i)
                window -> layer_stack.data[i].on_event(&event);
        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active; ++i)
                window -> layer_stack.data[i].on_update(_core_state._time_delta);

        glfwMakeContextCurrent(handle);
        glClear(GL_COLOR_BUFFER_BIT);
        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active; ++i)
                window -> layer_stack.data[i].on_render();
        glfwSwapBuffers(handle);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_close(GLFWwindow* handle) {
        DEBUG_TRACE();
        Window* window = window_get_by_id((WindowID)glfwGetWindowUserPointer(handle));
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, (WindowID)glfwGetWindowUserPointer(handle));

        if (vector_size(_core_state._window_arr) == 1)  _core_state._running = false;
        window_destroy(window -> id);

        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_WINDOW_CLOSE_EVENT,
                .handled = false,
        };

        i32 i;
        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active && !event.handled; ++i)
                window -> layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_move(GLFWwindow* handle, i32 xPos, i32 yPos) {
        DEBUG_TRACE();
        Window* window = window_get_by_id((WindowID)glfwGetWindowUserPointer(handle));
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, (WindowID)glfwGetWindowUserPointer(handle));

        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_WINDOW_MOVE_EVENT,
                .handled = false,
        };

        struct { i32 x, y; } data = { xPos , yPos };
        event.data = (void*)&data;

        i32 i;
        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active && !event.handled; ++i)
                window -> layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_key(GLFWwindow* handle, i32 key, i32 scan, i32 action, i32 mods) {
        DEBUG_TRACE();
        Window* window = window_get_by_id((WindowID)glfwGetWindowUserPointer(handle));
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, (WindowID)glfwGetWindowUserPointer(handle));

        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_KEY_EVENT,
                .handled = false,
        };

        struct { i32 key, action; } data = { key, action };
        event.data = (void*)&data;

        i32 i;
        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active && !event.handled; ++i)
                window -> layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_mouse_button(GLFWwindow* handle, i32 button, i32 action, i32 mods) {
        DEBUG_TRACE();
        Window* window = window_get_by_id((WindowID)glfwGetWindowUserPointer(handle));
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, (WindowID)glfwGetWindowUserPointer(handle));

        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_MOUSE_BUTTON_EVENT,
                .handled = false,
        };

        struct { i32 button, action; } data = { button, action };
        event.data = (void*)&data;

        i32 i;
        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active && !event.handled; ++i)
                window -> layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_mouse_move(GLFWwindow* handle, f64 xPos, f64 yPos) {
        DEBUG_TRACE();
        Window* window = window_get_by_id((WindowID)glfwGetWindowUserPointer(handle));
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, (WindowID)glfwGetWindowUserPointer(handle));

        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_MOUSE_MOVE_EVENT,
                .handled = false,
        };

        struct { f64 x, y; } data = { xPos, yPos };
        event.data = (void*)&data;

        i32 i;
        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active && !event.handled; ++i)
                window -> layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_scroll(GLFWwindow* handle, f64 xOffset, f64 yOffset) {
        DEBUG_TRACE();
        Window* window = window_get_by_id((WindowID)glfwGetWindowUserPointer(handle));
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, (WindowID)glfwGetWindowUserPointer(handle));

        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_MOUSE_SCROLL_EVENT,
                .handled = false,
        };

        struct { f64 offset; } data = { yOffset };
        event.data = (void*)&data;

        i32 i;
        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active && !event.handled; ++i)
                window -> layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_char(GLFWwindow* handle, u32 code) {
        DEBUG_TRACE();
        Window* window = window_get_by_id((WindowID)glfwGetWindowUserPointer(handle));
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, (WindowID)glfwGetWindowUserPointer(handle));

        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_TEXT_EVENT,
                .handled = false,
        };

        struct { u32 code; } data = { code };
        event.data = (void*)&data;

        i32 i;
        for (i = window -> layer_stack.start; i < window -> layer_stack.end && window -> layer_stack.data[i].active && !event.handled; ++i)
                window -> layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}


WindowID window_create(WindowSpecification* specification) {
        DEBUG_TRACE();
        DEBUG_ASSERT(specification != NULL); 

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwWindowHint(GLFW_RESIZABLE, specification -> is_resizable ? GLFW_TRUE : GLFW_FALSE);
        if (specification -> has_msaa) glfwWindowHint(GLFW_SAMPLES, 8);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        GLFWwindow* handle = glfwCreateWindow(specification -> width, specification -> height, specification -> name, specification -> is_fullscreen ? monitor : NULL, NULL);

        DEBUG_ASSERT(handle != NULL, "[GLFW] Could not create window");

        glfwSetWindowSizeLimits(handle, ENN_WINDOW_MIN_WIDTH, ENN_WINDOW_MIN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
        glfwMakeContextCurrent(handle);
        glfwSwapInterval(specification -> has_vsync);

        glfwSetWindowCloseCallback(handle, window_on_close);
        glfwSetFramebufferSizeCallback(handle, window_on_framebuff_resize);
        glfwSetWindowPosCallback(handle, window_on_move);
        glfwSetWindowFocusCallback(handle, window_on_focus);


        glfwSetKeyCallback(handle, window_on_key);
        glfwSetMouseButtonCallback(handle, window_on_mouse_button);
        glfwSetCursorPosCallback(handle, window_on_mouse_move);
        glfwSetScrollCallback(handle, window_on_scroll);
        glfwSetCharCallback(handle, window_on_char);

        WindowID id = _window_id_generator++;
        glfwSetWindowUserPointer(handle, (void*)id);

        i32 window_name_size = 0;
        while (specification -> name[window_name_size++]);
        char* window_name = calloc(window_name_size + 1, (sizeof (char)));
        memcpy(window_name, specification -> name, window_name_size * (sizeof (char)));

        Window new_window = {
                .id                     = id,
                .handle                 = handle,
                .name                   = window_name,
                .flags                  = ((specification -> is_resizable       << ENN_WINDOW_RESIZE_BIT        |
                                            specification -> has_vsync          << ENN_WINDOW_VSYNC_BIT         |
                                            specification -> is_fullscreen      << ENN_WINDOW_FULLSCREEN_BIT    |
                                            specification -> has_msaa           << ENN_WINDOW_MSAA_BIT          |
                                                          1                     << ENN_WINDOW_FOCUS_BIT)),
                .layer_stack            = vector_new(),
                .buffer_clear_color     = { .x = 0, .y = 0, .z = 0, .w = 0 }
        };
        glfwGetFramebufferSize(handle, &new_window.framebuff_dim.x, &new_window.framebuff_dim.y);


        vector_push_back(_core_state._window_arr, new_window);

        i32 version = gladLoadGL(glfwGetProcAddress);
        DEBUG_LOG("[Core] Created new window %#" PRIx64 " with name '%s' and OpenGL version %" PRIi32 ".%" PRIi32, new_window.id, new_window.name, GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

        DEBUG_UNTRACE();
        return new_window.id;
}

void window_destroy(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);

        i32 i;
        for (i = window -> layer_stack.start; i < window -> layer_stack.end; ++i)
                window -> layer_stack.data[i].term();

        DEBUG_LOG("[Core] All Layers terminated for window '%s'", window -> name);

        glfwDestroyWindow(window -> handle);

        DEBUG_LOG("[Core] Destroyed window %#" PRIx64 " with name '%s'", window -> id, window -> name);
        free(window -> name);

        vector_remove_at_address(_core_state._window_arr, window);
        DEBUG_UNTRACE();
}

Window* window_get_by_id(WindowID id) {
        DEBUG_TRACE();
        if ((id - 1) >= _core_state._window_arr.start && (id - 1) < _core_state._window_arr.end
                        && id == _core_state._window_arr.data[id - 1].id) {
                DEBUG_UNTRACE();
                return &_core_state._window_arr.data[id - 1];
        }
        for (i32 i = _core_state._window_arr.start; i < _core_state._window_arr.end; ++i)
                if (_core_state._window_arr.data[i].id == id) {
                        DEBUG_UNTRACE();
                        return &_core_state._window_arr.data[i];
                }
        DEBUG_UNTRACE();
        return NULL;
}

char* window_get_name(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);
        DEBUG_UNTRACE();
        return window -> name;
}

i32vec2 window_get_framebuff_dim(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);
        DEBUG_UNTRACE();
        return window -> framebuff_dim;
}

i32 window_get_framebuff_width(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);
        DEBUG_UNTRACE();
        return window -> framebuff_dim.x;
}

i32 window_get_framebuff_height(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);
        DEBUG_UNTRACE();
        return window -> framebuff_dim.y;
}
bool window_is_fullscreen(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);
        DEBUG_UNTRACE();
        return (window -> flags & (1 << ENN_WINDOW_FULLSCREEN_BIT));
}

bool window_has_vsync(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);
        DEBUG_UNTRACE();
        return (window -> flags & (1 << ENN_WINDOW_VSYNC_BIT));
}

LayerID window_push_layer(WindowID id, Layer* layer) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);
        
        layer -> id = _layer_id_generator++;
        layer -> init();
        vector_push_back(window -> layer_stack, *layer);
        vector_sort(window -> layer_stack, __layer_compare, window -> layer_stack.start, window -> layer_stack.end);

        DEBUG_UNTRACE();
        return _layer_id_generator - 1;
}

void window_remove_layer(WindowID id, LayerID layer) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);

        for (i32 i = window -> layer_stack.start; i < window -> layer_stack.end; ++i)
                if (window -> layer_stack.data[i].id == layer) {
                        vector_remove_at_index(window -> layer_stack, i);
                        DEBUG_UNTRACE();
                        return ;
                }
        // vector_sort(window -> layer_stack);

        DEBUG_UNTRACE();
}
void window_set_fullscreen(WindowID id, bool state) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);


        GLFWmonitor* monitor    = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (state) {
                window -> flags |= (1 << ENN_WINDOW_FULLSCREEN_BIT);
                glfwSetWindowMonitor(window -> handle, monitor, 0, 0, mode -> width, mode -> height, mode -> refreshRate);
                DEBUG_LOG("[Core] Fullscreen ON for window %#" PRIx64 " with name '%s'", window -> id, window -> name);
        } else {
                window -> flags &= ~(1 << ENN_WINDOW_FULLSCREEN_BIT);
                glfwSetWindowMonitor(window -> handle, NULL, (mode -> width >> 1) - (window -> framebuff_dim.x >> 1), (mode -> height >> 1) - (window -> framebuff_dim.y >> 1), window -> framebuff_dim.x, window -> framebuff_dim.y, mode -> refreshRate);
                DEBUG_LOG("[Core] Fullscreen OFF for window %#" PRIx64 " with name '%s'", window -> id, window -> name);
        }
        DEBUG_UNTRACE();
}

void window_set_vsync(WindowID id, bool state) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);


        if (state) {
                window -> flags |= (1 << ENN_WINDOW_VSYNC_BIT);
                glfwSwapInterval(1);
                DEBUG_LOG("[Core] VSYNC ON for window %#" PRIx64 " with name '%s'", window -> id, window -> name);
        } else {
                window -> flags &= ~(1 << ENN_WINDOW_VSYNC_BIT);
                glfwSwapInterval(0);
                DEBUG_LOG("[Core] VSYNC OFF for window %#" PRIx64 " with name '%s'", window -> id, window -> name);
        }
        DEBUG_UNTRACE();
}

void window_flip_fullscreen(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);

        GLFWmonitor* monitor    = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (!(window -> flags & (1 << ENN_WINDOW_FULLSCREEN_BIT))) {
                window -> flags |= (1 << ENN_WINDOW_FULLSCREEN_BIT);
                glfwSetWindowMonitor(window -> handle, monitor, 0, 0, mode -> width, mode -> height, mode -> refreshRate);
                DEBUG_LOG("[Core] Fullscreen ON for window %#" PRIx64 " with name '%s'", window -> id, window -> name);
        } else {
                window -> flags &= ~(1 << ENN_WINDOW_FULLSCREEN_BIT);
                glfwSetWindowMonitor(window -> handle, NULL, (mode -> width >> 1) - (window -> framebuff_dim.x >> 1), (mode -> height >> 1) - (window -> framebuff_dim.y >> 1), window -> framebuff_dim.x, window -> framebuff_dim.y, mode -> refreshRate);
                DEBUG_LOG("[Core] Fullscreen OFF for window %#" PRIx64 " with name '%s'", window -> id, window -> name);
        }

        DEBUG_UNTRACE();
}

void window_flip_vsync(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);

        if (!(window -> flags & (1 << ENN_WINDOW_VSYNC_BIT))) {
                window -> flags |= (1 << ENN_WINDOW_VSYNC_BIT);
                glfwSwapInterval(1);
                DEBUG_LOG("[Core] VSYNC ON for window %#" PRIx64 " with name '%s'", window -> id, window -> name);
        } else {
                window -> flags &= ~(1 << ENN_WINDOW_VSYNC_BIT);
                glfwSwapInterval(0);
                DEBUG_LOG("[Core] VSYNC OFF for window %#" PRIx64 " with name '%s'", window -> id, window -> name);
        }

        DEBUG_UNTRACE();
}

void window_set_buffer_clear_color(WindowID id, u32 color_hex) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);

        window -> buffer_clear_color = (f32vec4) {
                .x = ((color_hex >> 24) & 0xff) / 255.0,
                .y = ((color_hex >> 16) & 0xff) / 255.0,
                .z = ((color_hex >> 8)  & 0xff) / 255.0,
                .w = ((color_hex)       & 0xff) / 255.0
        };
        DEBUG_UNTRACE();
}

void window_make_context(WindowID id) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);

        glfwMakeContextCurrent(window -> handle);
        DEBUG_UNTRACE();
}

void window_set_viewport(WindowID id, i32 x, i32 y, i32 width, i32 height) {
        DEBUG_TRACE();
        Window* window = window_get_by_id(id);
        DEBUG_ASSERT(window != NULL, "Could not find window with id %#" PRIx64, id);

        glfwMakeContextCurrent(window -> handle);
        glViewport(x, y, width, height);
        DEBUG_UNTRACE();
}
  
#else 


ENNDEF_PUBLIC void window_on_focus(GLFWwindow* handle, i32 focused) {
        DEBUG_TRACE();
        if (focused) {
                _core_state._window_main.flags |= (1 << ENN_WINDOW_FOCUS_BIT);
                DEBUG_LOG("[Core] Window %#" PRIx64 " with name '%s' came into focus", _core_state._window_main.id, _core_state._window_main.name);
        } else {
                _core_state._window_main.flags &= ~(1 << ENN_WINDOW_FOCUS_BIT);
                DEBUG_LOG("[Core] Window %#" PRIx64 " with name '%s' has gone out of focus", _core_state._window_main.id, _core_state._window_main.name);
        }
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_framebuff_resize(GLFWwindow* handle, i32 new_width, i32 new_height) {
        DEBUG_TRACE();
        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_WINDOW_RESIZE_EVENT,
                .handled = false,
        };

        // _core_state._window_main.framebuff_dim.x= new_width; _core_state._window_main.framebuff_dim.y = new_height;
        struct { i32 width, height; } data = { new_width, new_height };
        event.data = (void*)&data;

        i32 i;
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers && !event.handled; ++i)
                _core_state._window_main.layer_stack.data[i].on_event(&event);
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers; ++i)
                _core_state._window_main.layer_stack.data[i].on_update(_core_state._time_delta);

        glfwMakeContextCurrent(handle);
        glClear(GL_COLOR_BUFFER_BIT);
        for (i = _core_state._window_main.layer_stack.end - 1 - _core_state._window_main.num_inactive_layers; i >= _core_state._window_main.layer_stack.start; --i)
                _core_state._window_main.layer_stack.data[i].on_render();
        glfwSwapBuffers(handle);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_close(GLFWwindow* handle) {
        DEBUG_TRACE();
        _core_state._running = false;
        window_destroy();

        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_WINDOW_CLOSE_EVENT,
                .handled = false,
        };

        i32 i;
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers && !event.handled; ++i)
                _core_state._window_main.layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_move(GLFWwindow* handle, i32 xPos, i32 yPos) {
        DEBUG_TRACE();
        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_WINDOW_MOVE_EVENT,
                .handled = false,
        };

        struct { i32 x, y; } data = { xPos , yPos };
        event.data = (void*)&data;

        i32 i;
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers && !event.handled; ++i)
                _core_state._window_main.layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_key(GLFWwindow* handle, i32 key, i32 scan, i32 action, i32 mods) {
        DEBUG_TRACE();
        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_KEY_EVENT,
                .handled = false,
        };

        struct { i32 key, action; } data = { key, action };
        event.data = (void*)&data;

        i32 i;
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers && !event.handled; ++i)
                _core_state._window_main.layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_mouse_button(GLFWwindow* handle, i32 button, i32 action, i32 mods) {
        DEBUG_TRACE();
        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_MOUSE_BUTTON_EVENT,
                .handled = false,
        };

        struct { i32 button, action; } data = { button, action };
        event.data = (void*)&data;

        i32 i;
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers && !event.handled; ++i)
                _core_state._window_main.layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_mouse_move(GLFWwindow* handle, f64 xPos, f64 yPos) {
        DEBUG_TRACE();
        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_MOUSE_MOVE_EVENT,
                .handled = false,
        };

        struct { f64 x, y; } data = { xPos, yPos };
        event.data = (void*)&data;

        i32 i;
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers && !event.handled; ++i)
                _core_state._window_main.layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_scroll(GLFWwindow* handle, f64 xOffset, f64 yOffset) {
        DEBUG_TRACE();
        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_MOUSE_SCROLL_EVENT,
                .handled = false,
        };

        struct { f64 offset; } data = { yOffset };
        event.data = (void*)&data;

        i32 i;
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers && !event.handled; ++i)
                _core_state._window_main.layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

ENNDEF_PUBLIC void window_on_char(GLFWwindow* handle, u32 code) {
        DEBUG_TRACE();
        Event event = {
                .id      = _event_id_generator++,
                .type    = ENN_INPUT_TEXT_EVENT,
                .handled = false,
        };

        struct { u32 code; } data = { code };
        event.data = (void*)&data;

        i32 i;
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end - _core_state._window_main.num_inactive_layers && !event.handled; ++i)
                _core_state._window_main.layer_stack.data[i].on_event(&event);
        DEBUG_UNTRACE();
}

void window_create(WindowSpecification* specification) {
        DEBUG_TRACE();
        DEBUG_ASSERT(specification != NULL);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwWindowHint(GLFW_RESIZABLE, specification -> is_resizable ? GLFW_TRUE : GLFW_FALSE);
        if (specification -> has_msaa) glfwWindowHint(GLFW_SAMPLES, 8);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        DEBUG_ASSERT(monitor != NULL, "[GLFW] Could not get primary monitor");

        if (specification -> name == NULL) specification -> name = "Placeholder Window Name";
        GLFWwindow* handle = glfwCreateWindow(specification -> width, specification -> height, specification -> name, specification -> is_fullscreen ? monitor : NULL, NULL);

        ASSERT(handle != NULL, "[GLFW] Could not create window");

        glfwSetWindowSizeLimits(handle, ENN_WINDOW_MIN_WIDTH, ENN_WINDOW_MIN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
        glfwMakeContextCurrent(handle);
        glfwSwapInterval(specification -> has_vsync);

        glfwSetWindowCloseCallback(handle, window_on_close);
        glfwSetFramebufferSizeCallback(handle, window_on_framebuff_resize);
        glfwSetWindowPosCallback(handle, window_on_move);
        glfwSetWindowFocusCallback(handle, window_on_focus);


        glfwSetKeyCallback(handle, window_on_key);
        glfwSetMouseButtonCallback(handle, window_on_mouse_button);
        glfwSetCursorPosCallback(handle, window_on_mouse_move);
        glfwSetScrollCallback(handle, window_on_scroll);
        glfwSetCharCallback(handle, window_on_char);

        i32 window_name_size = 0;
        while (specification -> name[window_name_size++]);
        char* window_name = calloc(window_name_size + 1, (sizeof (char)));
        memcpy(window_name, specification -> name, window_name_size * (sizeof (char)));

        _core_state._window_main = (Window) {
                .id                     = 0x1,
                .handle                 = handle,
                .name                   = window_name,
                .flags                  = ((specification -> is_resizable       << ENN_WINDOW_RESIZE_BIT        |
                                            specification -> has_vsync          << ENN_WINDOW_VSYNC_BIT         |
                                            specification -> is_fullscreen      << ENN_WINDOW_FULLSCREEN_BIT    |
                                            specification -> has_msaa           << ENN_WINDOW_MSAA_BIT          |
                                                          1                     << ENN_WINDOW_FOCUS_BIT)),
                .layer_stack            = vector_new(),
                .buffer_clear_color     = { .x = 0, .y = 0, .z = 0, .w = 0 }
        };
        glfwGetFramebufferSize(handle, &_core_state._window_main.framebuff_dim.x, &_core_state._window_main.framebuff_dim.y);

        i32 version = gladLoadGL(glfwGetProcAddress);
        LOG("[Core] Created new window %#" PRIx64 " with name '%s' and OpenGL version %" PRIi32 ".%" PRIi32, _core_state._window_main.id, _core_state._window_main.name, GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

        DEBUG_UNTRACE();
}

void window_destroy(void) {
        DEBUG_TRACE();
        i32 i;
        for (i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end; ++i)
                _core_state._window_main.layer_stack.data[i].term();

        vector_destroy(_core_state._window_main.layer_stack);
        LOG("[Core] All Layers terminated for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);

        glfwDestroyWindow(_core_state._window_main.handle);

        LOG("[Core] Destroyed window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);
        free(_core_state._window_main.name);
        memset(&_core_state._window_main, 0x0, (sizeof (Window)));
        DEBUG_UNTRACE();
}

char* window_get_name(void) {
        return _core_state._window_main.name;
}

i32vec2 window_get_framebuff_dim(void) {
        return _core_state._window_main.framebuff_dim;
}

i32 window_get_framebuff_width(void) {
        return _core_state._window_main.framebuff_dim.x;
}

i32 window_get_framebuff_height(void) {
        return _core_state._window_main.framebuff_dim.y;
}

bool window_is_fullscreen(void) {
        return _core_state._window_main.flags & (1 << ENN_WINDOW_FULLSCREEN_BIT);
}

bool window_has_vsync(void) {
        return _core_state._window_main.flags & (1 << ENN_WINDOW_VSYNC_BIT);
}

LayerID window_push_layer(Layer* layer) {
        DEBUG_TRACE();
        DEBUG_ASSERT(layer != NULL);
        layer -> id = _layer_id_generator++;
        layer -> init();
        vector_push_back(_core_state._window_main.layer_stack, *layer);
        vector_sort(_core_state._window_main.layer_stack, __layer_compare, _core_state._window_main.layer_stack.start, _core_state._window_main.layer_stack.end);
        if (!layer -> active) ++_core_state._window_main.num_inactive_layers;

        DEBUG_UNTRACE();
        return layer -> id;
}

void layer_set_active(LayerID id) {
        DEBUG_TRACE();

        for (i32 i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end; ++i)
                if (id == _core_state._window_main.layer_stack.data[i].id) {
                        if (!_core_state._window_main.layer_stack.data[i].active) {
                                _core_state._window_main.layer_stack.data[i].active = true;
                                vector_sort(_core_state._window_main.layer_stack, __layer_compare, _core_state._window_main.layer_stack.start, _core_state._window_main.layer_stack.end);
                                --_core_state._window_main.num_inactive_layers;
                        }
                        DEBUG_UNTRACE();
                        return;
                }


        DEBUG_UNTRACE();
}

void layer_set_inactive(LayerID id) {
        DEBUG_TRACE();

        for (i32 i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end; ++i)
                if (id == _core_state._window_main.layer_stack.data[i].id) {
                        if (_core_state._window_main.layer_stack.data[i].active) {
                                _core_state._window_main.layer_stack.data[i].active = false;
                                vector_sort(_core_state._window_main.layer_stack, __layer_compare, _core_state._window_main.layer_stack.start, _core_state._window_main.layer_stack.end);
                                ++_core_state._window_main.num_inactive_layers;
                        }
                        DEBUG_UNTRACE();
                        return;
                }

        DEBUG_UNTRACE();
}

bool layer_is_active(LayerID id) {
        DEBUG_TRACE();
        for (i32 i = _core_state._window_main.layer_stack.start; i < _core_state._window_main.layer_stack.end; ++i)
                if (id == _core_state._window_main.layer_stack.data[i].id) {
                        DEBUG_UNTRACE();
                        return _core_state._window_main.layer_stack.data[i].active;
                }
        
        return false;
        DEBUG_UNTRACE();
}

void window_set_fullscreen(bool state) {
        DEBUG_TRACE();
        GLFWmonitor* monitor    = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (state) {
                _core_state._window_main.flags |= (1 << ENN_WINDOW_FULLSCREEN_BIT);
                glfwSetWindowMonitor(_core_state._window_main.handle, monitor, 0, 0, mode -> width, mode -> height, mode -> refreshRate);
                DEBUG_LOG("[Core] Fullscreen ON for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);
        } else {
                _core_state._window_main.flags &= ~(1 << ENN_WINDOW_FULLSCREEN_BIT);
                glfwSetWindowMonitor(_core_state._window_main.handle, NULL, (mode -> width >> 1) - (_core_state._window_main.framebuff_dim.x >> 1), (mode -> height >> 1) - (_core_state._window_main.framebuff_dim.y >> 1), _core_state._window_main.framebuff_dim.x, _core_state._window_main.framebuff_dim.y, mode -> refreshRate);
                DEBUG_LOG("[Core] Fullscreen OFF for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);
        }
        DEBUG_UNTRACE();
}

void window_set_vsync(bool state) {
        DEBUG_TRACE();
        if (state) {
                _core_state._window_main.flags |= (1 << ENN_WINDOW_VSYNC_BIT);
                glfwSwapInterval(1);
                DEBUG_LOG("[Core] VSYNC ON for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);
        } else {
                _core_state._window_main.flags &= ~(1 << ENN_WINDOW_VSYNC_BIT);
                glfwSwapInterval(0);
                DEBUG_LOG("[Core] VSYNC OFF for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);
        }
        DEBUG_UNTRACE();
}


void window_flip_fullscreen(void) {
        DEBUG_TRACE();
        GLFWmonitor* monitor    = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (!(_core_state._window_main.flags & (1 << ENN_WINDOW_FULLSCREEN_BIT))) {
                _core_state._window_main.flags |= (1 << ENN_WINDOW_FULLSCREEN_BIT);
                glfwSetWindowMonitor(_core_state._window_main.handle, monitor, 0, 0, mode -> width, mode -> height, mode -> refreshRate);
                DEBUG_LOG("[Core] Fullscreen ON for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);
        } else {
                _core_state._window_main.flags &= ~(1 << ENN_WINDOW_FULLSCREEN_BIT);
                glfwSetWindowMonitor(_core_state._window_main.handle, NULL, (mode -> width >> 1) - (_core_state._window_main.framebuff_dim.x >> 1), (mode -> height >> 1) - (_core_state._window_main.framebuff_dim.y >> 1), _core_state._window_main.framebuff_dim.x, _core_state._window_main.framebuff_dim.y, mode -> refreshRate);
                DEBUG_LOG("[Core] Fullscreen OFF for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);
        }

        DEBUG_UNTRACE();
}

void window_flip_vsync(void) {
        DEBUG_TRACE();

        if (!(_core_state._window_main.flags & (1 << ENN_WINDOW_VSYNC_BIT))) {
                _core_state._window_main.flags |= (1 << ENN_WINDOW_VSYNC_BIT);
                glfwSwapInterval(1);
                DEBUG_LOG("[Core] VSYNC ON for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);
        } else {
                _core_state._window_main.flags &= ~(1 << ENN_WINDOW_VSYNC_BIT);
                glfwSwapInterval(0);
                DEBUG_LOG("[Core] VSYNC OFF for window %#" PRIx64 " with name '%s'", _core_state._window_main.id, _core_state._window_main.name);
        }

        DEBUG_UNTRACE();
}


void window_set_buffer_clear_color(u32 color_hex) {
        DEBUG_TRACE();
        _core_state._window_main.buffer_clear_color = (f32vec4) {
                .x = ((color_hex >> 24) & 0xff) / 255.0,
                .y = ((color_hex >> 16) & 0xff) / 255.0,
                .z = ((color_hex >> 8)  & 0xff) / 255.0,
                .w = ((color_hex)       & 0xff) / 255.0
        };
        DEBUG_UNTRACE();
}

void window_make_context(void) {
        DEBUG_TRACE();
        glfwMakeContextCurrent(_core_state._window_main.handle);
        DEBUG_UNTRACE();
}

void window_set_viewport(i32 x, i32 y, i32 width, i32 height) {
        DEBUG_TRACE();
        glfwMakeContextCurrent(_core_state._window_main.handle);
        glViewport(x, y, width, height);
        DEBUG_UNTRACE();
}

#endif
