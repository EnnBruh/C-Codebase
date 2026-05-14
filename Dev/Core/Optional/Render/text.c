#include "Optional/opt_internal.h"

#ifndef ENN_USE_CUSTOM_RENDERING

static FontID _internal_font_generator = 0x1;

#ifndef ENN_MULTI_WINDOW
FontID font_read_bitmap(const char* texture_path, FontSpecification* spec) {
        DEBUG_TRACE();
        DEBUG_ASSERT(texture_path != NULL);
        DEBUG_ASSERT(spec != NULL);

        Image img; render_image_create(&img);
        render_image_read(&img, texture_path, ENN_IMAGE_FLIP);
        TextureID base_texture = render_texture_create_image(&img, ENN_NEAREST, ENN_CLAMP_TO_EDGES);

        Font new_font = {
                .id             = _internal_font_generator++,
                .base_texture   = base_texture,
                .char_width     = spec -> char_width,
                .char_height    = spec -> char_height,
                .texture_width  = spec -> texture_width != 0 ? spec -> texture_width : img.width,
                .texture_height = spec -> texture_height != 0 ? spec -> texture_height : img.height
        };

        vector_push_back(_core_state._window_main.fonts, new_font);

        render_image_destroy(&img);
        DEBUG_UNTRACE();
        return new_font.id;
}

Font* font_get_by_id(FontID id) {
        DEBUG_TRACE();
        if ((id - 1) >= _core_state._window_main.fonts.start && (id - 1) < _core_state._window_main.fonts.end &&
                id == _core_state._window_main.fonts.data[id - 1].id) {
                        DEBUG_UNTRACE();
                        return &_core_state._window_main.fonts.data[id - 1];
                }

        for (i32 i = _core_state._window_main.fonts.start; i < _core_state._window_main.fonts.end; ++i)
                if (id == _core_state._window_main.fonts.data[i].id) {
                        DEBUG_UNTRACE();
                        return &_core_state._window_main.fonts.data[i];
                }

        DEBUG_UNTRACE();
        return NULL;
}

void font_text_draw(FontID id, const char* text, f32vec2 text_box_top_left, f32vec2 text_box_bottom_right, TextSpecifications* spec) {
        DEBUG_TRACE();
        DEBUG_ASSERT(text != NULL);

        Font* font = font_get_by_id(id);
        DEBUG_ASSERT(font != NULL, "Could not find font with id %#" PRIx32, id);

        DEBUG_UNTRACE();
}
#else
#endif


#endif