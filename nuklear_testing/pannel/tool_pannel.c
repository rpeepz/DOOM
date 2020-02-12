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

void tool_pannel(struct nk_context *ctx, int *tool_op)
{
    if (nk_begin(ctx, "Tools", nk_rect(1210, 410, 200, 200), NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE))
		{
            // static int property = 20;
			nk_layout_row_static(ctx, 30, 100, 1);
			if (nk_option_label(ctx, "Move", *tool_op == MOVE)) *tool_op = MOVE;
			if (nk_option_label(ctx, "Edit", *tool_op == EDIT)) *tool_op = EDIT;
			if (nk_option_label(ctx, "Line", *tool_op == LINE)) *tool_op = LINE;
			if (nk_option_label(ctx, "Thing", *tool_op == THING)) *tool_op = THING;
			if (nk_option_label(ctx, "Sector", *tool_op == SECTOR)) *tool_op = SECTOR;
		}
		nk_end(ctx);
}
