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
#include "../Nuklear/nuklear.h"
#include "nuklear_sdl_gl3.h"

void    list_pannel(struct nk_context *ctx, t_line_bank *linebank)
{
	t_line_node		*nodes = linebank->tail;
	int				count = linebank->count;

	 //printf("count = %d\n", linebank->count);
	// add_line(linebank, (struct nk_vec2){500.0,500.0}, (struct nk_vec2){600.0,600.0});
	// printf("count = %d\n", linebank->count);
	if (nk_begin(ctx, "line list", nk_rect(1210, 5, 350, 400), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE))
	{
		while (nodes) {
			char buffer[16];
			t_linedef line = nodes->line;
			nk_layout_row_begin(ctx, NK_STATIC, 30, 5);
			// nk_layout_row_dynamic(ctx, 30, 2);
			nk_layout_row_push(ctx, 100);
			snprintf(buffer, 16, "%.2f, %.2f", line.start_vertex.x, line.start_vertex.y);
			nk_label(ctx, buffer, NK_TEXT_LEFT);

			nk_layout_row_push(ctx, 10);
			nk_label(ctx, " ", 0);
			
			nk_layout_row_push(ctx, 100);
			snprintf(buffer, 16, "%.2f, %.2f", line.end_vertex.x, line.end_vertex.y);
			nk_label(ctx, buffer, NK_TEXT_RIGHT);

			nk_layout_row_push(ctx, 10);
			nk_label(ctx, " ", 0);

			nk_layout_row_push(ctx, 60);
			if (nk_button_label(ctx, "Select")) {
				linebank->selected->color = nk_rgb(10, 10, 10);
				nodes->color = nk_rgb(255, 160, 40);
				linebank->selected = nodes;
			}
			nk_layout_row_end(ctx);
			nodes = nodes->prev;
		}
/*
		//  testing slider display value
		nk_layout_row_dynamic(ctx, 30, 1);
		{int len; char buffer[64];
		len = snprintf(buffer, 64, "%.2f", value);
		nk_edit_string(ctx, NK_EDIT_READ_ONLY, buffer, &len, 63, nk_filter_float);
		buffer[len] = 0;
		value = atof(buffer);}

		nk_slider_float(ctx, 100, &value, 0.1f, 0.1f);
*/
	}
	nk_end(ctx);
}
