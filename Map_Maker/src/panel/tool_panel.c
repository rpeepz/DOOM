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

struct s_option_label{
	char *label;
	char *tooltip;
	int  mode;
};

void tool_pannel(t_map_interface *draw_mode)
{
	struct nk_context *ctx = draw_mode->ctx;
		struct s_option_label options[ ] = {
		{ .label = "Move", .tooltip = "Move tooltip", .mode = MOVE },
		{ .label = "Edit", .tooltip = "Edit items on map", .mode = EDIT },
		{ .label = "Line", .tooltip = "Add lines to map", .mode = LINE },
		{ .label = "Thing", .tooltip = "Add things to map", .mode = THING },
		{ .label = "Sector", .tooltip = "Sector tooltip", .mode = SECTOR },
		{ .label = "File", .tooltip = "File tooltip", .mode = NK_FILE }};

	int num_options = sizeof(options) / sizeof(options[0]);

	struct nk_rect size = nk_rect(WINDOW_WIDTH - (WINDOW_WIDTH / 4) + (WINDOW_OFFSET * 2),
	WINDOW_HEIGHT - ((WINDOW_HEIGHT * 5) / 9) + (WINDOW_OFFSET * 2),
	(WINDOW_WIDTH / 7), WINDOW_HEIGHT / 2.75);
	
    if (nk_begin(draw_mode->ctx, "Tools", size,
		NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE))
		{
			nk_layout_row_static(ctx, 30, 60, 1);

			for ( int i = 0; i < num_options; ++i){
				if (nk_input_is_mouse_hovering_rect(&ctx->input, nk_widget_bounds(ctx)))
					nk_tooltip(ctx, options[i].tooltip);
				if (nk_option_label(ctx, options[i].label, draw_mode->tool_op == options[i].mode))
					draw_mode->tool_op = options[i].mode;
			}
	}
		nk_end(ctx);
}