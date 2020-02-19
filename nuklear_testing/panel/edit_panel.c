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

static int texture_popup = 0;
char buffer[24];
int len;
void    sidedef_edit(struct nk_context *ctx, t_sidedef *side);
void    edit_selected_line(struct nk_context *ctx, t_linedef *line);
void    edit_selected_thing(struct nk_context *ctx, t_item_node *item);

void    edit_pannel(t_map_interface *draw_mode)
{
    if (!draw_mode->bank->selected ||
    ((draw_mode->list_op == ITEM_LINE && !draw_mode->bank->selected->line) ||
    (draw_mode->list_op == ITEM_THING && !draw_mode->bank->selected->thing)))
        return ;
    struct nk_context *ctx = draw_mode->ctx;
    
    if (texture_popup) {
        static struct nk_rect s = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
        200, 200};
        if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error", 0, s))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "A terrible error as occured", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 2);
            if (nk_button_label(ctx, "OK")) {
                texture_popup = nk_false;
                nk_popup_close(ctx);
            }
            if (nk_button_label(ctx, "Cancel")) {
                texture_popup = nk_false;
                nk_popup_close(ctx);
            }
            nk_popup_end(ctx);
        } else texture_popup = nk_false;
    }
    else
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
            edit_selected_thing(ctx, draw_mode->bank->selected);
    }
    nk_end(ctx);
}

void    edit_selected_line(struct nk_context *ctx, t_linedef *line)
{
    double x;
    double y;
    /* edit vertex values */
    nk_layout_row_begin(ctx, NK_STATIC, 30, 5);

    for (int i = 0; i < 2; i++) {
        nk_layout_row_push(ctx, 70);
        nk_label(ctx, !i ? "start x" : "end x" , NK_TEXT_LEFT);

        nk_layout_row_push(ctx, 40);
        len = snprintf(buffer, 24, "%.0f", !i ? line->start_vertex.x : line->end_vertex.x);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 23, nk_filter_decimal);
        buffer[len] = 0;
        x = atof(buffer);

        nk_layout_row_push(ctx, 5);
        nk_label(ctx, " ", NK_TEXT_LEFT);

        nk_layout_row_push(ctx, 70);
        nk_label(ctx, !i ? "start y" : "end y", NK_TEXT_LEFT);

        nk_layout_row_push(ctx, 40);
        len = snprintf(buffer, 24, "%.0f", !i ? line->start_vertex.y : line->end_vertex.y);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 23, nk_filter_decimal);
        buffer[len] = 0;
        y = atof(buffer);

        if (!i) line->start_vertex = snap((struct nk_vec2){x, y});
        else line->end_vertex = snap((struct nk_vec2){x, y});
    }
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
    if (nk_tree_push(ctx, NK_TREE_TAB, "Flags", NK_MAXIMIZED)) {
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
    }
    ctx->style.button.normal = nk_style_item_color(nk_rgb(50, 50, 50));
    t_sidedef *side = &line->sides[0];
    if (nk_tree_push(ctx, NK_TREE_TAB, "Sidedef - Right", NK_MAXIMIZED)) {
        sidedef_edit(ctx, side);
        nk_tree_pop(ctx);
    }
    if (line->flags & L_TWO_SIDED) {
        side = &line->sides[1];
        if (nk_tree_push(ctx, NK_TREE_TAB, "Sidedef - Left", NK_MAXIMIZED)) {
            sidedef_edit(ctx, side);
            nk_tree_pop(ctx);
        }
    }
    nk_style_default(ctx);
}

void    sidedef_edit(struct nk_context *ctx, t_sidedef *side)
{
    static int texture_popup = nk_false;
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
        if (nk_button_label(ctx, side->textures[i]))
            texture_popup = nk_true;

        nk_layout_row_push(ctx, 50);
        if (nk_button_label(ctx, "clear"))
            memset(side->textures[i], 0, 8);
    }
}

static int  get_angle(char *angle)
{
    char	*direction[8] = {
        "E", "NE", "N", "NW",
        "W", "SW", "S", "SE"};
	for (int i = 0; i < 8; i++){
        if (!strcmp(angle, direction[i]))
            return (i * 45);
    }
    return (0);
}

void    edit_selected_thing(struct nk_context *ctx, t_item_node *item)
{
    t_thing *thing = item->thing;
    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, "Direction", NK_TEXT_LEFT);
    nk_style_default(ctx);
    nk_layout_row_static(ctx, 25, 30, 3);
    char	*direction[9] = {
		"NW", "N", "NE",
		"W",  " ", "E",
        "SW", "S", "SE"
        };
    for (int i = 0; i < 9; i++) {
        if (nk_button_label(ctx, direction[i])) {
            if (i == 4)
                continue ;
            thing->angle = get_angle(direction[i]);
        }
    }
    // spacing
    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, " ", 0);

    char *sections[ ] = { "Angle:", "Type:", "Name:" };
    nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
    for (int i = 0; i < 3; i++) {
        nk_layout_row_push(ctx, 50);
        nk_label(ctx, sections[i], NK_TEXT_RIGHT);
        nk_layout_row_push(ctx, 150);
        if (!i) len = snprintf(buffer, 16, "%d", thing->angle);
        else if (i == 1) len = snprintf(buffer, 16, "%d", thing->type);
        else len = snprintf(buffer, 16, "%s", thing->name);
        if (i < 2) nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 5, nk_filter_decimal);
        else nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 16, nk_filter_default);
        buffer[len] = 0;
        if (!i) {
            int angle = atoi(buffer);
            if (angle >= 360 || angle < 0) angle = 0;
            thing->angle = angle;
        }
        else if (i == 1) thing->type = atoi(buffer);
        else memcpy(thing->name, buffer, 16);
    }

    /* check box style */
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

    /* thing flag options */
    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, " ", 0);
    nk_layout_row_dynamic(ctx, 20, 3);
    char *labels[] = { "Easy", "Normal", "Hard", "Ambush", "Network", "Single_P", "Co-op", "Friendly" };
    static int flags[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int i;
    for (i = 0; i < 8; i ++) {
        if (i == 3 || i == 4)
            continue ;
        nk_checkbox_label(ctx, labels[i], flags + i);
        if (flags[i])
            thing->flags |= (1 << i);
        else
            thing->flags &= ~(1 << i);
    }
    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, " ", 0);
    /* slider color combobox */
    if (nk_combo_begin_color(ctx, item->thing->color, nk_vec2(200,200))) {
        float ratios[] = {0.15f, 0.85f};
        nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratios);
        nk_label(ctx, "R:", NK_TEXT_LEFT);
        item->thing->color.r = (nk_byte)nk_slide_int(ctx, 0, item->thing->color.r, 255, 5);
        nk_label(ctx, "G:", NK_TEXT_LEFT);
        item->thing->color.g = (nk_byte)nk_slide_int(ctx, 0, item->thing->color.g, 255, 5);
        nk_label(ctx, "B:", NK_TEXT_LEFT);
        item->thing->color.b = (nk_byte)nk_slide_int(ctx, 0, item->thing->color.b, 255, 5);
        nk_combo_end(ctx);
    }
    nk_style_default(ctx);
}
