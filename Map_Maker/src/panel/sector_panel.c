#include "demo.h"
#include "list.h"
#include "sector.h"

static int	sector_panel_op = 0;
struct nk_style_button *button;

void	add_sector(t_map_interface *draw_mode);
void	fill_lines(t_map_interface *draw_mode);

void	edit_sector(t_map_interface *draw_mode);

void    sector_panel(t_map_interface *draw_mode)
{
	if (draw_mode->bank->selected) {
		if (draw_mode->bank->selected->line)
			draw_mode->bank->selected->color = LINE_COLOR;
		else draw_mode->bank->selected->color = THING_COLOR;
	}
	struct nk_rect size = nk_rect(draw_mode->win_w - ((draw_mode->win_w / 15) * 3) + (WINDOW_OFFSET * 2),
	((draw_mode->win_h * 2) / 5) + (WINDOW_OFFSET * 3),
	((draw_mode->win_w * 5) / 32) + (WINDOW_OFFSET * 5),
	(draw_mode->win_h * 5) / 9);

	struct nk_context *ctx = draw_mode->ctx;
	nk_window_set_focus(ctx, "Sector");
	if (nk_begin(ctx, "Sector", size, NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE)) {
		if (!draw_mode->bank->head_line ||
		!draw_mode->bank->head_line->next ||
		!draw_mode->bank->head_line->next->next) {
			nk_layout_row_dynamic(ctx, 45, 1);
			nk_label_wrap(ctx, "Need at least 3 lines to make a sector");
			nk_end(ctx);
			return ;
		}
		
		nk_layout_row_dynamic(ctx, 20, 2);
		/* color style for buttons */
		button = &ctx->style.button;
		button->active = nk_style_item_color(BUTTON_DEFAULT);
		button->hover = nk_style_item_color(nk_rgba(150, 150, 150, 80));

		// colors for selected item type
		if (sector_panel_op == SECTOR_ADD) button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		else button->normal = nk_style_item_color(BUTTON_DEFAULT);
		if (nk_button_label(ctx, "Add Sector"))
			sector_panel_op = SECTOR_ADD;

		if (sector_panel_op == SECTOR_EDIT) button->normal = nk_style_item_color(nk_rgba(170, 170, 170, 80));
		else button->normal = nk_style_item_color(BUTTON_DEFAULT);
		if (nk_button_label(ctx, "Edit Sector"))
			sector_panel_op = SECTOR_EDIT;	
	}
	button->normal = nk_style_item_color(BUTTON_DEFAULT);
	if (sector_panel_op == SECTOR_ADD) add_sector(draw_mode);
	if (sector_panel_op == SECTOR_EDIT) edit_sector(draw_mode);
	nk_end(ctx);
}

void	add_sector(t_map_interface *draw_mode)
{
	//fill lines
	fill_lines(draw_mode);
	//get sector number
	// suggest_sector_number();
	//confirm valid sector
	// confirm_sector();
	//add to sector bank
	
}

void	edit_sector(t_map_interface *draw_mode)
{
	(void)draw_mode;
}

void	fill_lines(t_map_interface *draw_mode)
{
	//choose the lines to add to the new sector
	(void)draw_mode;
}
