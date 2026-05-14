#define ENN_DEBUG_MEMORY_IMPL
#define STB_IMAGE_IMPLEMENTATION
#include "Optional/opt_internal.h"

ShaderID render_shader_compile(const char* vertex_shader_filename, const char* fragment_shader_filename) {
        DEBUG_TRACE();
        DEBUG_ASSERT(vertex_shader_filename != NULL);
        DEBUG_ASSERT(fragment_shader_filename != NULL);

        i32 _filepath_len = snprintf(NULL, 0, ENN_APP_DIRECTORY ENN_SHADERS_PATH "%s", vertex_shader_filename);
        char vertex_shader_filepath[_filepath_len];
        sprintf(vertex_shader_filepath, ENN_APP_DIRECTORY ENN_SHADERS_PATH "%s", vertex_shader_filename);

        _filepath_len = snprintf(NULL, 0, ENN_APP_DIRECTORY ENN_SHADERS_PATH "%s", fragment_shader_filename);
        char fragment_shader_filepath[_filepath_len];
        sprintf(fragment_shader_filepath, ENN_APP_DIRECTORY ENN_SHADERS_PATH "%s", fragment_shader_filename);

        char* vertex_shader_src;
        file_read_cstring(vertex_shader_filepath, &vertex_shader_src);

        ShaderID vertex_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_id, 1, (const char**)&vertex_shader_src, NULL);
        glCompileShader(vertex_id);

        i32 compile_status;
        glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &compile_status);
        if (!compile_status) {
                i32 info_len = 0;
                glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &info_len);

                char* info = calloc(info_len, (sizeof (char)));
                glGetShaderInfoLog(vertex_id, info_len * (sizeof (char)), NULL, info);

        LOG_WARN("[OpenGL] Failed to compile Vertex Shader from source '%s'\n\t%s", vertex_shader_filepath, info);

                free(vertex_shader_src);
                free(info);

                DEBUG_UNTRACE();
                return ENN_SHADER_ERR;
        }

        char* fragment_shader_src;
        file_read_cstring(fragment_shader_filepath, &fragment_shader_src);

        ShaderID fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_id, 1, (const char**)&fragment_shader_src, NULL);
        glCompileShader(fragment_id);

        glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &compile_status);
        if (!compile_status) {
                i32 info_len = 0;
                glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &info_len);

                char* info = calloc(info_len, (sizeof (char)));
                glGetShaderInfoLog(fragment_id, info_len * (sizeof (char)), NULL, info);

                LOG_WARN("[OpenGL] Failed to compile Fragment Shader from source '%s'\n\t%s", fragment_shader_filepath, info);

                free(vertex_shader_src);
                free(fragment_shader_src);
                free(info);

                DEBUG_UNTRACE();
                return ENN_SHADER_ERR;
        }

        ShaderID final_shader = glCreateProgram();
        glAttachShader(final_shader, vertex_id);
        glAttachShader(final_shader, fragment_id);
        glLinkProgram(final_shader);

        glGetShaderiv(final_shader, GL_LINK_STATUS, &compile_status);
        if (!compile_status) {
                i32 info_len = 0;
                glGetShaderiv(final_shader, GL_INFO_LOG_LENGTH, &info_len);

                char* info = calloc(info_len, (sizeof (char)));
                glGetShaderInfoLog(final_shader, info_len * (sizeof (char)), NULL, info);

                LOG_WARN("[OpenGL] Failed to link shaders from sources '%s' '%s'\n\t%s", fragment_shader_filepath, vertex_shader_filepath, info);

                free(vertex_shader_src);
                free(fragment_shader_src);
                free(info);

                DEBUG_UNTRACE();
                return ENN_SHADER_ERR;
        }

        free(vertex_shader_src);
        free(fragment_shader_src);

        DEBUG_UNTRACE();
        return final_shader;
}

void render_image_create(Image* img) {
        DEBUG_TRACE();
        memset(img, 0x0, (sizeof (Image)));
        DEBUG_UNTRACE();
}

void render_image_destroy(Image* img) {
        DEBUG_TRACE();
        stbi_image_free(img -> data);
        memset(img, 0x0, (sizeof (Image)));
        DEBUG_UNTRACE();
}

void render_image_read_2(Image* img, const char* filepath) {
        DEBUG_TRACE();
        DEBUG_ASSERT(img != NULL);
        DEBUG_ASSERT(filepath != NULL);

        stbi_set_flip_vertically_on_load(false);
        img -> data = stbi_load(filepath, &img -> width, &img -> height, &img -> channels, 0);
        if (img -> data == NULL) LOG_WARN("[Render] Could not read image at '%s'", filepath);
        DEBUG_UNTRACE();
}

void render_image_read_3(Image* img, const char* filepath, bool flip_vertical) {
        DEBUG_TRACE();
        DEBUG_ASSERT(img != NULL);
        DEBUG_ASSERT(filepath != NULL);

        stbi_set_flip_vertically_on_load(flip_vertical);
        img -> data = stbi_load(filepath, &img -> width, &img -> height, &img -> channels, 0);
        if (img -> data == NULL) LOG_WARN("[Render] Could not read image at '%s'", filepath);
        DEBUG_UNTRACE();
}

static u32 _internal_unknown_texture[] = {
        0x000000FF, 0xFF00FFFF,
        0xFF00FFFF, 0x000000FF
};


TextureID render_texture_create_image(Image* img, ENN_WRAP_ALGORITHM wrap_algo, ENN_SCALING_ALGORITHM scale_algo) {
        DEBUG_TRACE();
        DEBUG_ASSERT(img != NULL);

        TextureID id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        if (img -> data != NULL) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scale_algo);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scale_algo);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_algo);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_algo);

                if (img -> channels == 4) {
                        DEBUG_LOG("[Render] Using RGBA Format for texture");
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img -> width, img -> height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img -> data);
                } else if (img -> channels == 3) {
                        DEBUG_LOG("[Render] Using RGB Format for texture");
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img -> width, img -> height, 0, GL_RGB, GL_UNSIGNED_BYTE, img -> data);
                }
        } else {
                DEBUG_LOG("[Render] No texture data found. Using UNKNOWN Texture");
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ENN_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ENN_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ENN_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ENN_REPEAT);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, (byte*)_internal_unknown_texture);
        }

        DEBUG_UNTRACE();
        return id;
}

TextureID render_texture_create_file(const char* filename, ENN_WRAP_ALGORITHM wrap_algo, ENN_SCALING_ALGORITHM scale_algo) {
        DEBUG_TRACE();
        DEBUG_ASSERT(filename != NULL);

        TextureID id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        i32 _filepath_len = snprintf(NULL, 0, ENN_APP_DIRECTORY ENN_TEXTURES_PATH "%s", filename);
        char filepath[_filepath_len];
        sprintf(filepath, ENN_APP_DIRECTORY ENN_TEXTURES_PATH "%s", filename);

        Image img;
        render_image_create(&img);
        render_image_read(&img, filepath, true);

        if (img.data != NULL) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scale_algo);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scale_algo);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_algo);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_algo);

                if (img.channels == 4) {
                        DEBUG_LOG("[Render] Using RGBA Format for texture at %s", filename);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
                } else if (img.channels == 3) {
                        DEBUG_LOG("[Render] Using RGB Format for texture at %s", filename);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
                }
        } else {
                DEBUG_LOG("[Render] No texture data found. Using UNKNOWN Texture");
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ENN_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ENN_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ENN_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ENN_REPEAT);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, (byte*)_internal_unknown_texture);
        }

        render_image_destroy(&img);

        DEBUG_UNTRACE();
        return id;
}

#ifndef ENN_USE_CUSTOM_RENDERING

TextureAtlasID render_texture_atlas_create(const char *filename) {
        return 0;
}

void render_rectangle_draw(f32vec2 top_left, f32vec2 bott_right, u32 color);
void render_line_draw(f32vec2 pos1, f32vec2 pos2, f32 width, u32 color);
void render_sprite_draw(f32vec2 top_left, f32vec2 bott_right, u32 color, Sprite sprite);
#endif