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

struct nk_style_button *button;

static void	lines(struct nk_context *ctx, t_line_bank *linebank)
{
	t_line_node		*nodes = linebank->tail;

	while (nodes) {
		char buffer[24];
		t_linedef line = nodes->line;

		nk_layout_row_begin(ctx, NK_STATIC, 30, 3);

		if (linebank->selected == nodes) {
			button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		} else {
			button->normal = nk_style_item_color(nk_rgb(50, 50, 50));
		}

		nk_layout_row_push(ctx, 55);
		if (nk_button_label(ctx, "Select")) {
			linebank->selected->color = nk_rgb(10, 10, 10);
			nodes->color = nk_rgb(255, 160, 40);
			linebank->selected = nodes;
		}

		nk_layout_row_push(ctx, 130);
		snprintf(buffer, 24, " start (%.0f, %.0f)", line.start_vertex.x, line.start_vertex.y);
		nk_label(ctx, buffer, NK_TEXT_LEFT);

		nk_layout_row_push(ctx, 130);
		snprintf(buffer, 24, "end (%.0f, %.0f)", line.end_vertex.x, line.end_vertex.y);
		nk_label(ctx, buffer, NK_TEXT_RIGHT);

		nk_layout_row_end(ctx);

		/* add padding between each line */
		nk_layout_row_begin(ctx, NK_STATIC, 10, 1);
		nk_layout_row_push(ctx, 10);
		nk_label(ctx, " ", 0);
		nk_layout_row_end(ctx);

		nodes = nodes->prev;
	}
}
static void	things(struct nk_context *ctx, t_thing_bank *thingbank)
{
	t_thing_node	*nodes = thingbank->tail;

	while (nodes) {
		char buffer[24];
		t_thing thing = nodes->thing;

		nk_layout_row_begin(ctx, NK_STATIC, 30, 3);

		if (thingbank->selected == nodes) {
			button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		} else {
			button->normal = nk_style_item_color(nk_rgb(50, 50, 50));
		}

		nk_layout_row_push(ctx, 55);
		if (nk_button_label(ctx, "Select")) {
			thingbank->selected->color = nk_rgb(32, 32, 180);
			nodes->color = nk_rgb(255, 0, 0);
			thingbank->selected = nodes;
		}

		nk_layout_row_push(ctx, 130);
		snprintf(buffer, 24, " (%.0f, %.0f)", thing.pos.x, thing.pos.y);
		nk_label(ctx, buffer, NK_TEXT_LEFT);

		nk_layout_row_push(ctx, 130);
		snprintf(buffer, 24, " '%s' ", thing.name);
		nk_label(ctx, buffer, NK_TEXT_RIGHT);

		nk_layout_row_end(ctx);

		/* add padding between each line */
		nk_layout_row_begin(ctx, NK_STATIC, 10, 1);
		nk_layout_row_push(ctx, 10);
		nk_label(ctx, " ", 0);
		nk_layout_row_end(ctx);

		nodes = nodes->prev;
	}
}

void    list_pannel(t_map_interface draw_mode)
{
	struct nk_context *ctx = draw_mode.ctx;
	t_line_bank *linebank = draw_mode.linebank;
	t_thing_bank *thingbank = draw_mode.thingbank;
	static int		item = 0;
	/* pannel size nk_rect(1210, 5, 385, 400) */
	struct nk_rect size = nk_rect(WINDOW_WIDTH - (WINDOW_WIDTH / 4) + (WINDOW_OFFSET * 2),
	WINDOW_OFFSET, WINDOW_WIDTH - ((WINDOW_WIDTH / 4) * 3) - (WINDOW_OFFSET * 3),
    WINDOW_HEIGHT - ((WINDOW_HEIGHT * 5) / 9));
	if (nk_begin(ctx, "List", size, NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE)) {
		/* color style for buttons */
		button = &ctx->style.button;
		button->active = nk_style_item_color(nk_rgb(50, 50, 50));
		button->hover = nk_style_item_color(nk_rgba(150, 150, 150, 80));

		/* menu bar buttons */
		nk_menubar_begin(ctx);
		nk_layout_row_dynamic(ctx, 30, 2);
	// colors for selected item type
		if (item == 0) button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		else button->normal = nk_style_item_color(nk_rgb(50, 50, 50));
		if (nk_button_label(ctx, "Lines")) item = 0;

		if (item == 1) button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		else button->normal = nk_style_item_color(nk_rgb(50, 50, 50));
		if (nk_button_label(ctx, "Things")) item = 1;
		nk_menubar_end(ctx);

		if (item == 0) {
			
			lines(ctx, linebank);
		}
		else {
			
			things(ctx, thingbank);
		}
	}
	nk_end(ctx);
}
