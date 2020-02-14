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


void    edit_selected_line(struct nk_context *ctx, t_linedef *line)
{
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
<<<<<<< HEAD:nuklear_testing/pannel/edit_pannel.c
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
=======
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

			/* flags check boxes */

			char *labels[] = { "Block", "Mblock", "2-Sided", "Ftop", "Fbot", "Secret", "Snd Block", "No Draw" };
			static int flags[ ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
			int num_flags = sizeof(flags) / sizeof(flags[0]);
			for (int i = 0; i < num_flags; i++){
				nk_checkbox_label(ctx, labels[i], flags + i);
				/* if checkbox is set then set flag bit */ 
				if ( flags[i] )
					line->flags |= (1 << i);
				/* else make sure said bit is turned off */
				else
					line->flags &=  ~(1 << i);
			}
            nk_tree_pop(ctx);
>>>>>>> testpanel:nuklear_testing/panel/edit_panel.c
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

			char *sections[ ] = { "Top:", "Middle:", "Bottom:" };

            nk_layout_row_begin(ctx, NK_STATIC, 25, 3);
            for (int i = 0; i < 3; i++) {
                nk_layout_row_push(ctx, 50);
				nk_label(ctx, sections[i], NK_TEXT_RIGHT);
                nk_layout_row_push(ctx, 100);
                len = snprintf(buffer, 9, "%s", side->textures[i]);
                nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 8, nk_filter_default);
                buffer[len] = 0;
                memcpy(side->textures[i], buffer, 9);

                nk_layout_row_push(ctx, 50);
                if (nk_button_label(ctx, "clear"))
                    memset(side->textures[i], 0, 8);
            }
            nk_tree_pop(ctx);
        }
<<<<<<< HEAD:nuklear_testing/pannel/edit_pannel.c
    }
}

void    edit_selected_thing(struct nk_context *ctx, t_thing *thing)
{
    (void)ctx;
    (void)thing;
}

void    edit_pannel(t_map_interface *draw_mode)
{
    if (!draw_mode->bank->selected ||
    ((draw_mode->list_op == ITEM_LINE && !draw_mode->bank->selected->line) ||
    (draw_mode->list_op == ITEM_THING && !draw_mode->bank->selected->thing)))
        return ;
    struct nk_context *ctx = draw_mode->ctx;
    nk_window_set_focus(ctx, "Edit");

    /* pannel size nk_rect(1310, 375, 275, 500); */
    struct nk_rect size = nk_rect(WINDOW_WIDTH - ((WINDOW_WIDTH / 16) * 3) + (WINDOW_OFFSET * 2),
    ((WINDOW_HEIGHT * 2) / 5) + (WINDOW_OFFSET * 3),
    ((WINDOW_WIDTH * 5) / 32) + (WINDOW_OFFSET * 5),
    (WINDOW_HEIGHT * 5) / 9);

    if (nk_begin(draw_mode->ctx, "Edit", size, NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE))
    {
        if (draw_mode->list_op == ITEM_LINE)
            edit_selected_line(ctx, draw_mode->bank->selected->line);
        else if (draw_mode->list_op == ITEM_THING)
            edit_selected_thing(ctx, draw_mode->bank->selected->thing);
=======
		if ( line->flags & L_TWO_SIDED )
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
			
				char *sections[ ] = { "Top:", "Middle:", "Bottom:" };

                nk_layout_row_begin(ctx, NK_STATIC, 25, 3);
                for (int i = 0; i < 3; i++) {
                    nk_layout_row_push(ctx, 50);
					nk_label(ctx, sections[i], NK_TEXT_RIGHT);
                    nk_layout_row_push(ctx, 100);
                    len = snprintf(buffer, 9, "%s", side->textures[i]);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 8, nk_filter_default);
                    buffer[len] = 0;
                    memcpy(side->textures[i], buffer, 9);

                    nk_layout_row_push(ctx, 50);
                    if (nk_button_label(ctx, "clear"))
                        memset(side->textures[i], 0, 8);
                }
                nk_tree_pop(ctx);
            }
        }
>>>>>>> testpanel:nuklear_testing/panel/edit_panel.c
    }
    nk_end(ctx);
}
