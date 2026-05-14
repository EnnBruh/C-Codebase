#ifndef ENN_OPTIONAL_H
#define ENN_OPTIONAL_H

#ifdef __cplusplus
        extern "C" {
#endif


#include "config.h"
#include "Base/base.h"


#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLAD/gl.h"
#include "STB/stb_image.h"

#ifndef ENN_ASSETS_PATH
#       define ENN_ASSETS_PATH "/Assets/"
#endif

/* ---------- Event System ---------- */

typedef i32     EventID;


typedef enum {
        ENN_WINDOW_REGISTER_EVENT,

        ENN_WINDOW_CLOSE_EVENT,
        ENN_WINDOW_RESIZE_EVENT,
        ENN_WINDOW_MOVE_EVENT,

        ENN_INPUT_KEY_EVENT,
        ENN_INPUT_MOUSE_BUTTON_EVENT,
        ENN_INPUT_MOUSE_SCROLL_EVENT,
        ENN_INPUT_MOUSE_MOVE_EVENT,
        ENN_INPUT_TEXT_EVENT,
 #ifdef ENN_USER_DEFINED_EVENTS
        ENN_USER_DEFINED_EVENTS,
 #else
        ENN_LAST_EVENT
 #endif
} ENN_EVENT_TYPE;

typedef struct Event {
        EventID         id;
        ENN_EVENT_TYPE  type;
        void*           data;
        bool            handled;
} Event;

/* ---------- Layer System ---------- */

typedef i32     LayerID;

typedef struct Layer {
        LayerID         id;
        i32             priority;
        bool            active;

        void (*init)(void);
        void (*term)(void);

        void (*on_event)(Event* event);
        void (*on_update)(f64 dt);
        void (*on_render)(void);
} Layer;

#define LAYER_DEFINE(name)                                                            \
        extern LayerID EXPAND(JOIN(name, _layer_id));                                 \
        ENNDEF_PRIVATE void EXPAND(JOIN(name, _layer_init))(void);                    \
        ENNDEF_PRIVATE void EXPAND(JOIN(name, _layer_term))(void);                    \
        ENNDEF_PRIVATE void EXPAND(JOIN(name, _layer_on_render))(void);               \
        ENNDEF_PRIVATE void EXPAND(JOIN(name, _layer_on_event))(Event* event);        \
        ENNDEF_PRIVATE void EXPAND(JOIN(name, _layer_on_update))(f64 dt)             

#define LAYER_ASSIGN(name)                                                            \
        .init = EXPAND(JOIN(name, _layer_init)),                                      \
        .term = EXPAND(JOIN(name, _layer_term)),                                      \
        .on_render = EXPAND(JOIN(name, _layer_on_render)),                            \
        .on_update = EXPAND(JOIN(name, _layer_on_update)),                            \
        .on_event  = EXPAND(JOIN(name, _layer_on_event))


ENNDEF_PUBLIC ENN_CMP __layer_compare(Layer a, Layer b) {
        if (!a.active) return ENN_BIGGER;
        if (!b.active) return ENN_SMALLER;

        if (a.priority < b.priority) return ENN_BIGGER;
        if (a.priority > b.priority) return ENN_SMALLER;

        if (a.id < b.id) return ENN_SMALLER;
        if (a.id > b.id) return ENN_BIGGER;
        return ENN_EQUAL;
}


/* ---------- Render System ---------- */

#ifndef ENN_SHADERS_PATH
#       define ENN_SHADERS_PATH "/Assets/Shaders/"
#endif

#ifndef ENN_TEXTURES_PATH
#       define ENN_TEXTURES_PATH "/Assets/Textures/"
#endif

#ifndef ENN_FONTS_PATH
#       define ENN_FONTS_PATH "/Assets/Fonts/"
#endif

typedef GLuint  ShaderID;
typedef GLuint  VBOID;
typedef GLuint  VAOID;
typedef GLuint  FBOID;
typedef GLuint  EBOID;

typedef GLuint  TextureID;

#ifndef ENN_USE_CUSTOM_RENDERING

        typedef i32 TextureAtlasID;
        typedef struct Sprite {
                TextureAtlasID  atlas;
                f32vec2         sprite_pos;
        } Sprite;

#       ifndef ENN_DEFAULT_RENDERER_VERTEX_BUFF_SIZE
#               define ENN_DEFAULT_RENDERER_VERTEX_BUFF_SIZE 2048
#       endif
        struct DefaultRenderer {
                struct BasicRenderer {
                        struct BasicVertex {
                                f32vec2 pos;
                                u32 color;
                        }                       buff[ENN_DEFAULT_RENDERER_VERTEX_BUFF_SIZE];
                        VBOID                   vbo;
                        VAOID                   vao;
                        ShaderID                shader;
                }                               basic_render;

                struct SpriteRenderer {
                        vector(struct {
                                struct SpriteVertex {
                                        f32vec2         pos;
                                        u32             color;
                                        f32vec2         texture_pos;
                                }                       buff[ENN_DEFAULT_RENDERER_VERTEX_BUFF_SIZE];
                                VBOID                   vbo;
                                VAOID                   vao;
                                TextureID               texture_atlas;
                        })                              sprite_buffs;
                        ShaderID                        shader;
                }                                       sprite_render;
        };
#endif

        typedef struct Image
        {
                i32 width;
                i32 height;
                i32 channels;
                byte *data;
        } Image;

#define ENN_SHADER_ERR -1

ENNDEF_PRIVATE ShaderID render_shader_compile(const char* vertex_shader_filename, const char* fragment_shader_filename);

ENNDEF_PRIVATE void render_image_create(Image* img);
ENNDEF_PRIVATE void render_image_destroy(Image* img);

#ifndef ENN_IMAGE_FLIP
#       define ENN_IMAGE_FLIP true
#endif

ENNDEF_PRIVATE void render_image_read_2(Image* img, const char* filepath);
ENNDEF_PRIVATE void render_image_read_3(Image* img, const char* filepath, bool flip_vertical);
#define render_image_read(...) EXPAND(JOIN(render_image_read_, GET_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__))

typedef enum ENN_WRAP_ALGORITHM {
        ENN_CLAMP_TO_EDGES = GL_CLAMP_TO_EDGE,
        ENN_REPEAT         = GL_REPEAT
} ENN_WRAP_ALGORITHM;

typedef enum ENN_SCALING_ALGORITHM {
        ENN_NEAREST        = GL_NEAREST,
        ENN_LINEAR         = GL_LINEAR
} ENN_SCALING_ALGORITHM;

ENNDEF_PRIVATE TextureID        render_texture_create_image(Image* img, ENN_WRAP_ALGORITHM wrap_algo, ENN_SCALING_ALGORITHM scale_algo);
ENNDEF_PRIVATE TextureID        render_texture_create_file(const char* filename, ENN_WRAP_ALGORITHM wrap_algo, ENN_SCALING_ALGORITHM scale_algo);
ENNDEF_PRIVATE void             render_texture_destroy(TextureID id);

#ifndef ENN_USE_CUSTOM_RENDERING

#       ifdef ENN_MULTI_WINDOW

                ENNDEF_PRIVATE TextureAtlasID render_texture_atlas_create(WindowID window, const char* filename);

                ENNDEF_PRIVATE void render_rectangle_draw(WindowID window, f32vec2 top_left, f32vec2 bott_right, u32 color);
                ENNDEF_PRIVATE void render_line_draw(WindowID window, f32vec2 pos1, f32vec2 pos2, f32 width, u32 color);
                ENNDEF_PRIVATE void render_sprite(WindowID window, f32vec2 top_left, f32vec2 bott_right, u32 color, Sprite sprite);
                
#       else
                ENNDEF_PRIVATE TextureAtlasID render_texture_atlas_create(const char* filename);

                ENNDEF_PRIVATE void render_rectangle_draw(f32vec2 top_left, f32vec2 bott_right, u32 color);
                ENNDEF_PRIVATE void render_line_draw(f32vec2 pos1, f32vec2 pos2, f32 width, u32 color);
                ENNDEF_PRIVATE void render_sprite_draw(f32vec2 top_left, f32vec2 bott_right, u32 color, Sprite sprite);
#       endif

#endif

/* ---------- Font System ---------- */

#ifndef ENN_USE_CUSTOM_RENDERING
        typedef i32     FontID;
        typedef struct Font {
                FontID          id;
                TextureID       base_texture;
                i32             char_width;
                i32             char_height;
                i32             texture_width;
                i32             texture_height;
        } Font;

        typedef struct FontSpecification {
                i32     char_width;
                i32     char_height;
                i32     texture_width;
                i32     texture_height;
        } FontSpecification;

#       ifdef ENN_MULTI_WINDOW
                ENNDEF_PRIVATE FontId   font_read_bitmap(WindowID window, const char* texture_path, FontSpecification* spec);
                ENNDEF_PRIVATE Font*    font_get_by_id(WindowID window, FontID id);
#       else
                ENNDEF_PRIVATE FontID   font_read_bitmap(const char* texture_path, FontSpecification* spec);
                ENNDEF_PRIVATE Font*    font_get_by_id(FontID id);
#       endif

        typedef enum ENN_TEXT_ALIGN {
                ENN_LEFT,
                ENN_RIGHT,
                ENN_CENTER
        } ENN_TEXT_ALIGN;

        typedef enum ENN_TEXT_BOX_BEHAVIOUR {
                ENN_RESIZE_TEXT_WITH_BOX,
                ENN_RESIZE_BOX_WITH_TEXT,
                ENN_WRAP_TEXT_IN_BOX
        } ENN_TEXT_BOX_BEHAVIOUR;

        typedef struct TextSpecification {
                ENN_TEXT_ALIGN          text_align;
                ENN_TEXT_BOX_BEHAVIOUR  text_box_behaviour;
                i32                     text_size;
                bool                    text_box_border;
        } TextSpecifications;

#       ifndef ENN_FONT_ESCAPE_CHARACTER
#              define ENN_FONT_ESCAPE_CHARACTER '$'
#       endif

#       ifdef ENN_MULTI_WINDOW
                ENNDEF_PRIVATE void font_text_draw(WindowID window, FontID id, const char* text, f32vec2 text_box_top_left, f32vec2 text_box_bottom_right, TextSpecifications* spec);
#       else
                ENNDEF_PRIVATE void font_text_draw(FontID id, const char* text, f32vec2 text_box_top_left, f32vec2 text_box_bottom_right, TextSpecifications* spec);
#       endif
#endif

/* ---------- UI System ---------- */


// typedef struct UIElement {
// } UIElement;

/* ---------- Window System ---------- */

typedef i64     WindowID;


#ifndef ENN_WINDOW_MIN_WIDTH
#       define ENN_WINDOW_MIN_WIDTH 320
#endif

#ifndef ENN_WINDOW_MIN_HEIGHT
#       define ENN_WINDOW_MIN_HEIGHT 180
#endif

typedef struct WindowSpecification {
        i32     width;
        i32     height;
        char*   name;

        bool    is_fullscreen;
        bool    is_resizable;
        bool    has_vsync;
        bool    has_msaa;

        u32     buffer_clear_color;
} WindowSpecification;

#define ENN_WINDOW_INVALID_ID           0x0

#define ENN_WINDOW_FULLSCREEN_BIT       1
#define ENN_WINDOW_VSYNC_BIT            2
#define ENN_WINDOW_RESIZE_BIT           3
#define ENN_WINDOW_MSAA_BIT             4
#define ENN_WINDOW_FOCUS_BIT            5

typedef struct Window {
        WindowID        id;
        GLFWwindow*     handle;

        i32vec2         framebuff_dim;

        f32vec4         buffer_clear_color;
        char*           name;
        byte            flags;

        vector(Layer)   layer_stack;
        i32             num_inactive_layers;
#       ifndef ENN_USE_CUSTOM_RENDERING
                struct DefaultRenderer  renderer;
                vector(Font)            fonts;
#       endif
} Window;

#ifdef ENN_MULTI_WINDOW

        ENNDEF_PRIVATE WindowID window_create(WindowSpecification* specification);
        ENNDEF_PRIVATE void     window_destroy(WindowID id);

        ENNDEF_PRIVATE Window*  window_get_by_id(WindowID id);

        ENNDEF_PRIVATE char*    window_get_name(WindowID id);
        ENNDEF_PRIVATE i32vec2  window_get_framebuff_dim(WindowID id);
        ENNDEF_PRIVATE i32      window_get_framebuff_width(WindowID id);
        ENNDEF_PRIVATE i32      window_get_framebuff_height(WindowID id);
        ENNDEF_PRIVATE bool     window_is_fullscreen(WindowID id);
        ENNDEF_PRIVATE bool     window_has_vsync(WindowID id);

        ENNDEF_PRIVATE LayerID  window_push_layer(WindowID id, Layer* layer);
        ENNDEF_PRIVATE void     window_remove_layer(WindowID id, LayerID layer);

        ENNDEF_PRIVATE void     window_set_fullscreen(WindowID id, bool state);
        ENNDEF_PRIVATE void     window_set_vsync(WindowID id, bool state);
        ENNDEF_PRIVATE void     window_set_buffer_clear_color(WindowID id, u32 color_hex);

        ENNDEF_PRIVATE void     window_flip_fullscreen(WindowID id);
        ENNDEF_PRIVATE void     window_flip_vsync(WindowID id);

        ENNDEF_PRIVATE void     window_make_context(WindowID id);
        ENNDEF_PRIVATE void     window_set_viewport(WindowID id, i32 x, i32 y, i32 width, i32 height);

#else

        ENNDEF_PRIVATE void     window_create(WindowSpecification* specification);
        ENNDEF_PRIVATE void     window_destroy(void);

        ENNDEF_PRIVATE char*    window_get_name(void);
        ENNDEF_PRIVATE i32vec2  window_get_framebuff_dim(void);
        ENNDEF_PRIVATE i32      window_get_framebuff_width(void);
        ENNDEF_PRIVATE i32      window_get_framebuff_height(void);
        ENNDEF_PRIVATE bool     window_is_fullscreen(void);
        ENNDEF_PRIVATE bool     window_has_vsync(void);

        ENNDEF_PRIVATE LayerID  window_push_layer(Layer* layer);
        ENNDEF_PRIVATE void     layer_set_active(LayerID id);
        ENNDEF_PRIVATE void     layer_set_inactive(LayerID id);
        ENNDEF_PRIVATE bool     layer_is_active(LayerID id);

        ENNDEF_PRIVATE void     window_set_fullscreen(bool state);
        ENNDEF_PRIVATE void     window_set_vsync(bool state);
        ENNDEF_PRIVATE void     window_set_buffer_clear_color(u32 color_hex);

        ENNDEF_PRIVATE void     window_flip_fullscreen(void);
        ENNDEF_PRIVATE void     window_flip_vsync(void);

        ENNDEF_PRIVATE void     window_make_context(void);
        ENNDEF_PRIVATE void     window_set_viewport(i32 x, i32 y, i32 width, i32 height);
#endif


/* ---------- Network Helpers ---------- */

#if ENN_PLATFORM == ENN_WINDOWS

#       ifndef _WIN32_WINNT
#               define _WIN32_WINNT 0x0600
#       endif

#       include <winsock2.h>
#       include <ws2tcpip.h>

        typedef SOCKET Socket;

#       define ENN_IS_SOCKET_INVALID(_sock) ((_sock) == INVALID_SOCKET)
#       define ENN_SOCKET_ERR SOCKET_ERROR

#else

#       include <sys/types.h>
#       include <sys/socket.h>
#       include <netinet/in.h>
#       include <arpa/inet.h>
#       include <netdb.h>
#       include <unistd.h>
#       include <errno.h>
#       include <sys/select.h>

        typedef int Socket;

#       define ENN_IS_SOCKET_INVALID(_sock) ((_sock) < 0)
#       define ENN_SOCKET_ERR (-1)
#endif

ENNDEF_PRIVATE bool     network_init();
ENNDEF_PRIVATE void     network_term();
ENNDEF_PRIVATE void     network_socket_close(Socket sock);

#ifdef __cplusplus
        }
#endif

#endif