#include "list.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
// # define NK_IMPLEMENTATION
// # define NK_SDL_GL3_IMPLEMENTATION
#include "../../Nuklear/nuklear.h"
#include "nuklear_sdl_gl3.h"

void    edit_pannel(struct nk_context *ctx, t_line_bank *linebank)
{
    nk_window_set_focus(ctx, "Edit");
    if (nk_begin(ctx, "Edit", nk_rect(1320, 375, 260, 500), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE))
    {
        if (linebank->selected)
        {
            t_linedef *line = &linebank->selected->line;
            char buffer[256];
            int len;
            nk_layout_row_dynamic(ctx, 30, 2);
            
            nk_label(ctx, "start x :", NK_TEXT_LEFT);
            len = snprintf(buffer, 256, "%.0f", line->start_vertex.x);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_decimal);
            buffer[len] = 0;
            double x = atof(buffer);

            nk_label(ctx, "start y :", NK_TEXT_LEFT);
            len = snprintf(buffer, 256, "%.0f", line->start_vertex.y);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_decimal);
            buffer[len] = 0;
            double y = atof(buffer);
            line->start_vertex = snap((struct nk_vec2){x, y});

            nk_label(ctx, "end x :", NK_TEXT_LEFT);
            len = snprintf(buffer, 256, "%.0f", line->end_vertex.x);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_decimal);
            buffer[len] = 0;
            x = atof(buffer);

            nk_label(ctx, "end y :", NK_TEXT_LEFT);
            len = snprintf(buffer, 256, "%.0f", line->end_vertex.y);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_decimal);
            buffer[len] = 0;
            y = atof(buffer);
            line->end_vertex = snap((struct nk_vec2){x, y});

            nk_label(ctx, "special :", NK_TEXT_LEFT);
            len = snprintf(buffer, 256, "%d", line->special);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_decimal);
            buffer[len] = 0;
            line->special = atoi(buffer);

            nk_label(ctx, "tag :", NK_TEXT_LEFT);
            len = snprintf(buffer, 256, "%d", line->tag);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_decimal);
            buffer[len] = 0;
            line->tag = atoi(buffer);

            // line->flags; make checkboxes to represent the flags
            // line->sides[0];
            // line->sides[1];
        }
    }
    nk_end(ctx);
}
