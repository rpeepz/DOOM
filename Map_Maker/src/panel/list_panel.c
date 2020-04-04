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

static void	lines(struct nk_context *ctx, t_bank *bank)
{
	t_item_node		*nodes = bank->tail_line;

	while (nodes) {
		char buffer[24];
		t_linedef line = *nodes->line;

		nk_layout_row_begin(ctx, NK_STATIC, 30, 3);

		if (bank->selected == nodes) {
			button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		} else {
			button->normal = nk_style_item_color(BUTTON_DEFAULT);
		}

		nk_layout_row_push(ctx, 55);
		if (nk_button_label(ctx, "Select")) {
			if (bank->selected) bank->selected->color = nk_rgb(10, 10, 10);
			nodes->color = nk_rgb(255, 160, 40);
			bank->selected = nodes;
		}
		// nk_style_default(ctx);

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

static char	*get_angle(int angle)
{
	char	*direction[8] = {
		"E", "NE", "N", "NW",
		"W", "SW", "S", "SE"
	};
	return (direction[angle / 45]);
}

static void	things(struct nk_context *ctx, t_bank *bank)
{
	t_item_node		*nodes = bank->tail_thing;

	while (nodes) {
		char buffer[24];
		t_thing thing = *nodes->thing;

		nk_layout_row_begin(ctx, NK_STATIC, 30, 5);

		if (bank->selected == nodes) {
			button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		} else {
			button->normal = nk_style_item_color(BUTTON_DEFAULT);
		}

		nk_layout_row_push(ctx, 55);
		if (nk_button_label(ctx, "Select")) {
			if (bank->selected) bank->selected->color = THING_COLOR;
			nodes->color = nk_rgb(255, 0, 0);
			bank->selected = nodes;
		}
		button->normal = nk_style_item_color(BUTTON_DEFAULT);

		nk_layout_row_push(ctx, 80);
		snprintf(buffer, 24, " (%.0f, %.0f)", thing.pos.x, thing.pos.y);
		nk_label(ctx, buffer, NK_TEXT_LEFT);

		nk_layout_row_push(ctx, 40);
		snprintf(buffer, 24, "%s", get_angle(thing.angle));
		nk_label(ctx, buffer, NK_TEXT_RIGHT);

		nk_layout_row_push(ctx, 130);
		snprintf(buffer, 24, "'%s'", thing.name);
		nk_label(ctx, buffer, NK_TEXT_CENTERED);

		nk_layout_row_push(ctx, 30);
		nk_button_color(ctx, nodes->thing->color);
		nk_layout_row_end(ctx);

		/* add padding between each line */
		nk_layout_row_begin(ctx, NK_STATIC, 10, 1);
		nk_layout_row_push(ctx, 10);
		nk_label(ctx, " ", 0);
		nk_layout_row_end(ctx);

		nodes = nodes->prev;
	}
}

void    list_panel(t_map_interface *draw_mode)
{
	struct nk_context *ctx = draw_mode->ctx;
	t_bank *bank = draw_mode->bank;
	/* panel size nk_rect(1210, 5, 385, 400) (size and offset for 1600 x 900 window) */
	struct nk_rect size = nk_rect(draw_mode->win_w - (draw_mode->win_w / 4) + (WINDOW_OFFSET * 2),
	WINDOW_OFFSET, draw_mode->win_w - ((draw_mode->win_w / 4) * 3) - (WINDOW_OFFSET * 3),
    draw_mode->win_h - ((draw_mode->win_h * 5) / 9));
	if (nk_begin(ctx, "List", size, NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE)) {
		/* color style for buttons */
		button = &ctx->style.button;
		button->active = nk_style_item_color(BUTTON_DEFAULT);
		button->hover = nk_style_item_color(nk_rgba(150, 150, 150, 80));

		/* menu bar buttons */
		nk_menubar_begin(ctx);
		nk_layout_row_dynamic(ctx, 30, 2);
	// colors for selected item type
		if (draw_mode->list_op == ITEM_LINE) button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		else button->normal = nk_style_item_color(BUTTON_DEFAULT);
		if (nk_button_label(ctx, "Lines")) draw_mode->list_op = ITEM_LINE;

		if (draw_mode->list_op == ITEM_THING) button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		else button->normal = nk_style_item_color(BUTTON_DEFAULT);
		if (nk_button_label(ctx, "Things")) draw_mode->list_op = ITEM_THING;
		nk_menubar_end(ctx);

		/* Change `bank->selected` to reflect the list option */
		if (draw_mode->list_op == ITEM_LINE) {
			if (bank->selected) {
				if (bank->selected->thing) {
					bank->selected->color = THING_COLOR;
					bank->selected = NULL;
				}
			}
			if (bank->selected == NULL)
				bank->selected = bank->tail_line;
			if (bank->selected) bank->selected->color = HIGHLIGHT;
			lines(ctx, bank);
		}
		else if (draw_mode->list_op == ITEM_THING) {
			if (bank->selected) {
				if (bank->selected->line) {
					bank->selected->color = LINE_COLOR;
					bank->selected = NULL;
				}
			}
			if (bank->selected == NULL)
				bank->selected = bank->tail_thing;
			if (bank->selected) bank->selected->color = HIGHLIGHT;
			things(ctx, bank);
		}
	}
	button->normal = nk_style_item_color(BUTTON_DEFAULT);
	nk_end(ctx);
}
