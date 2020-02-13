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
    /* TODO
        This pannel is currently in a fixed location. Change to
        dynamic placement such that the following will replace
        the call in `nk_begin`
        x, y, w, and h will all use formulas on the
        WINDOW_WIDTH / WINDOW_HEIGHT / WINDOW_OFFSET
        macros to replicate the following */
        struct nk_rect size = nk_rect(1310, 375, 275, 500);

    if (nk_begin(ctx, "Edit", size, NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE))
    {
        if (linebank->selected)
        {
            t_linedef *line = &linebank->selected->line;
            char buffer[24];
            int len;
            double x;
            double y;
            /* edit vertex values */
            nk_layout_row_begin(ctx, NK_STATIC, 30, 5);
            
            nk_layout_row_push(ctx, 70);
            nk_label(ctx, "start x", NK_TEXT_LEFT);

            nk_layout_row_push(ctx, 40);
            len = snprintf(buffer, 24, "%.0f", line->start_vertex.x);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 23, nk_filter_decimal);
            buffer[len] = 0;
            x = atof(buffer);

            nk_layout_row_push(ctx, 5);
            nk_label(ctx, " ", NK_TEXT_LEFT);

            nk_layout_row_push(ctx, 70);
            nk_label(ctx, "start y", NK_TEXT_LEFT);

            nk_layout_row_push(ctx, 40);
            len = snprintf(buffer, 24, "%.0f", line->start_vertex.y);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 23, nk_filter_decimal);
            buffer[len] = 0;
            y = atof(buffer);

            line->start_vertex = snap((struct nk_vec2){x, y});

            nk_layout_row_push(ctx, 70);
            nk_label(ctx, "end x", NK_TEXT_LEFT);

            nk_layout_row_push(ctx, 40);
            len = snprintf(buffer, 24, "%.0f", line->end_vertex.x);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 23, nk_filter_decimal);
            buffer[len] = 0;
            x = atof(buffer);

            nk_layout_row_push(ctx, 5);
            nk_label(ctx, " ", NK_TEXT_LEFT);

            nk_layout_row_push(ctx, 70);
            nk_label(ctx, "end y", NK_TEXT_LEFT);

            nk_layout_row_push(ctx, 40);
            len = snprintf(buffer, 24, "%.0f", line->end_vertex.y);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 23, nk_filter_decimal);
            buffer[len] = 0;
            y = atof(buffer);

            line->end_vertex = snap((struct nk_vec2){x, y});

            nk_layout_row_push(ctx, 70);
            nk_label(ctx, "special", NK_TEXT_LEFT);

            nk_layout_row_push(ctx, 40);
            len = snprintf(buffer, 24, "%d", line->special);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 23, nk_filter_decimal);
            buffer[len] = 0;
            line->special = atoi(buffer);

            nk_layout_row_push(ctx, 5);
            nk_label(ctx, " ", NK_TEXT_LEFT);

            nk_layout_row_push(ctx, 70);
            nk_label(ctx, "tag", NK_TEXT_LEFT);

            nk_layout_row_push(ctx, 40);
            len = snprintf(buffer, 24, "%d", line->tag);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 23, nk_filter_decimal);
            buffer[len] = 0;
            line->tag = atoi(buffer);

            /* linedef flag options */
            if (nk_tree_push(ctx, NK_TREE_TAB, "Flags", NK_MAXIMIZED))
            {
                struct nk_style_toggle *box = &ctx->style.checkbox;

                box->border = 1.0f;
                box->border_color = nk_rgb(80, 80, 80);
        // full box when 0
                /* no hover */
                box->normal = nk_style_item_color(nk_rgb(80, 80, 80));
                /* hover */
                box->hover = nk_style_item_color(nk_rgb(150, 80, 80));

        // inside of box when 1
                /* no hover */
                box->cursor_normal = nk_style_item_color(nk_rgb(140, 140, 140));
                /* hover */
                box->cursor_hover = nk_style_item_color(nk_rgb(140, 140, 140));

                nk_layout_row_dynamic(ctx, 20, 2);
                nk_checkbox_label(ctx, "Block", &line->flags.block);
                nk_checkbox_label(ctx, "Mblock", &line->flags.mblock);
                nk_checkbox_label(ctx, "2-Sided", &line->flags.two_side);
                nk_checkbox_label(ctx, "Ftop", &line->flags.ftop);
                nk_checkbox_label(ctx, "Fbot", &line->flags.fbot);
                nk_checkbox_label(ctx, "Secret", &line->flags.secret);
                nk_checkbox_label(ctx, "Snd Block", &line->flags.snd_block);
                nk_checkbox_label(ctx, "No Draw", &line->flags.no_draw);
                nk_tree_pop(ctx);
            }
            ctx->style.button.normal = nk_style_item_color(nk_rgb(50, 50, 50));
            t_sidedef *side = &line->sides[0];
            if (nk_tree_push(ctx, NK_TREE_TAB, "Sidedef - Right", NK_MAXIMIZED))
            {
                float max = 25;
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_label(ctx, "Texture offset", NK_TEXT_LEFT);

                nk_layout_row_dynamic(ctx, 25, 2);
                nk_property_float(ctx, "#x:", 0, &side->offset.x, max, 0.25f, 0.01f);
                nk_property_float(ctx, "#y:", 0, &side->offset.y, max, 0.25f, 0.01f);
            
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_label(ctx, "Textures", NK_TEXT_LEFT);

                nk_layout_row_begin(ctx, NK_STATIC, 25, 3);
                for (int i = 0; i < 3; i++) {
                    char *label;
                    nk_layout_row_push(ctx, 50);
                    if (i == 0) label = "Top:";
                    else if (i == 1) label = "Middle:";
                    else if (i == 2) label = "Bottom:";
                    nk_label(ctx, label, NK_TEXT_RIGHT);

                    nk_layout_row_push(ctx, 100);
                    len = snprintf(buffer, 9, "%s", side->textures[i]);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 8, nk_filter_default);
                    buffer[len] = 0;
                    memcpy(side->textures[i], buffer, 9);

                    nk_layout_row_push(ctx, 50);
                    if (nk_button_label(ctx, "zero"))
                        memset(side->textures[i], 0, 8);
                }
                nk_tree_pop(ctx);
            }
            if (line->flags.two_side)
            {
                side = &line->sides[1];
                if (nk_tree_push(ctx, NK_TREE_TAB, "Sidedef - Left", NK_MAXIMIZED))
                {
                    float max = 25;
                    nk_layout_row_dynamic(ctx, 25, 1);
                    nk_label(ctx, "Texture offset", NK_TEXT_LEFT);

                    nk_layout_row_dynamic(ctx, 25, 2);
                    nk_property_float(ctx, "#x:", 0, &side->offset.x, max, 0.25f, 0.01f);
                    nk_property_float(ctx, "#y:", 0, &side->offset.y, max, 0.25f, 0.01f);
                
                    nk_layout_row_dynamic(ctx, 25, 1);
                    nk_label(ctx, "Textures", NK_TEXT_LEFT);

                    nk_layout_row_begin(ctx, NK_STATIC, 25, 3);
                    for (int i = 0; i < 3; i++) {
                        char *label;
                        nk_layout_row_push(ctx, 50);
                        if (i == 0) label = "Top:";
                        else if (i == 1) label = "Middle:";
                        else if (i == 2) label = "Bottom:";
                        nk_label(ctx, label, NK_TEXT_RIGHT);

                        nk_layout_row_push(ctx, 100);
                        len = snprintf(buffer, 9, "%s", side->textures[i]);
                        nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 8, nk_filter_default);
                        buffer[len] = 0;
                        memcpy(side->textures[i], buffer, 9);

                        nk_layout_row_push(ctx, 50);
                        if (nk_button_label(ctx, "zero"))
                            memset(side->textures[i], 0, 8);
                    }
                    nk_tree_pop(ctx);
                }
            }
        }
    }
    nk_end(ctx);
}
