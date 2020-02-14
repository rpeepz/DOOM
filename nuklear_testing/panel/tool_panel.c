#include "demo.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
// #define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
// # define NK_IMPLEMENTATION
// # define NK_SDL_GL3_IMPLEMENTATION
#include "../../Nuklear/nuklear.h"
#include "nuklear_sdl_gl3.h"

void tool_pannel(t_map_interface *draw_mode)
{
	struct nk_context *ctx = draw_mode->ctx;
	/* pannel size nk_rect(1210, 410, 200, 200) */
	struct nk_rect size = nk_rect(WINDOW_WIDTH - (WINDOW_WIDTH / 4) + (WINDOW_OFFSET * 2),
	WINDOW_HEIGHT - ((WINDOW_HEIGHT * 5) / 9) + (WINDOW_OFFSET * 2),
	(WINDOW_WIDTH / 8),
    WINDOW_HEIGHT - ((WINDOW_HEIGHT * 7) / 9));

    if (nk_begin(draw_mode->ctx, "Tools", size,
		NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MINIMIZABLE))
		{
			nk_layout_row_static(ctx, 30, 60, 1);
            if (nk_input_is_mouse_hovering_rect(&ctx->input, nk_widget_bounds(ctx)))
                nk_tooltip(ctx, "Move tooltip");
			if (nk_option_label(ctx, "Move", draw_mode->tool_op == MOVE)) draw_mode->tool_op = MOVE;
            if (nk_input_is_mouse_hovering_rect(&ctx->input, nk_widget_bounds(ctx)))
                nk_tooltip(ctx, "Edit features of existing items on the map");
			if (nk_option_label(ctx, "Edit", draw_mode->tool_op == EDIT)) draw_mode->tool_op = EDIT;
            if (nk_input_is_mouse_hovering_rect(&ctx->input, nk_widget_bounds(ctx)))
                nk_tooltip(ctx, "Add lines to the map");
			if (nk_option_label(ctx, "Line", draw_mode->tool_op == LINE)) draw_mode->tool_op = LINE;
            if (nk_input_is_mouse_hovering_rect(&ctx->input, nk_widget_bounds(ctx)))
                nk_tooltip(ctx, "Add things to the map");
			if (nk_option_label(ctx, "Thing", draw_mode->tool_op == THING)) draw_mode->tool_op = THING;
            if (nk_input_is_mouse_hovering_rect(&ctx->input, nk_widget_bounds(ctx)))
                nk_tooltip(ctx, "Sector tooltip");
			if (nk_option_label(ctx, "Sector", draw_mode->tool_op == SECTOR)) draw_mode->tool_op = SECTOR;
		}
		nk_end(ctx);
}
