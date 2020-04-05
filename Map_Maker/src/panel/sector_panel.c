#include "demo.h"
#include "list.h"
#include "sector.h"

static int	sector_panel_op = 0;
struct nk_style_button *button;
struct nk_rect size;
struct nk_context *ctx;

void	add_sector(t_map_interface *draw_mode);
void	fill_lines(t_map_interface *draw_mode);
int		suggest_sector_number(t_map_interface *draw_mode);
void	finish_sector(t_map_interface *draw_mode, t_sector *new_sector);
void	edit_sector(t_map_interface *draw_mode);

void    sector_panel(t_map_interface *draw_mode)
{
	// turn off highlight for things
	if (draw_mode->bank->selected)
		if (draw_mode->bank->selected->thing)
			draw_mode->bank->selected->color = THING_COLOR;

	size = nk_rect(draw_mode->win_w - ((draw_mode->win_w / 15) * 3) + (WINDOW_OFFSET * 2),
	draw_mode->win_h - ((draw_mode->win_h * 5) / 9) + (WINDOW_OFFSET * 3),
	((draw_mode->win_w * 5) / 32) + (WINDOW_OFFSET * 5),
	100);

	ctx = draw_mode->ctx;
	if (nk_begin(ctx, "Sector", size, NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
		if (!draw_mode->bank->head_line ||
		!draw_mode->bank->head_line->next ||
		!draw_mode->bank->head_line->next->next) {
			nk_layout_row_dynamic(ctx, 45, 1);
			nk_label_wrap(ctx, "Need at least 3 lines to make a sector");
			nk_end(ctx);
			return ;
		}
		/* color style for buttons */
		button = &ctx->style.button;
		button->active = nk_style_item_color(BUTTON_DEFAULT);
		button->hover = nk_style_item_color(nk_rgba(150, 150, 150, 80));

		nk_layout_row_dynamic(ctx, 20, 2);
		// colors for selected panel option
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
	nk_end(ctx);
	if (sector_panel_op == SECTOR_ADD) add_sector(draw_mode);
	if (sector_panel_op == SECTOR_EDIT) edit_sector(draw_mode);
}

/* driver for sector panel functions */
void	add_sector(t_map_interface *draw_mode)
{
	//fill lines
	fill_lines(draw_mode);
	//get sector number
	static t_sector	new_sector = {0};
	new_sector.sector_num = suggest_sector_number(draw_mode);
	//confirm valid sector and add to sector bank
	finish_sector(draw_mode, &new_sector);
}

/* panel functions for editing sector info */
void	edit_sector(t_map_interface *draw_mode)
{
	(void)draw_mode;
}

/* choose the lines to add to the new sector */
void	fill_lines(t_map_interface *draw_mode)
{
	size.y += 80;
	size.h = draw_mode->win_h * 0.2;
	// easier to scroll in window
	nk_window_set_focus(ctx, "fill lines");
	if (nk_begin(ctx, "fill lines", size, NK_WINDOW_BORDER)) {
		char buffer[32];
		int line_counter = 0;
		for (t_item_node *list = draw_mode->bank->head_line; list; list = list->next) {
			// remove highlight for lines
			list->color = LINE_COLOR;
			memset(buffer, 0, sizeof(buffer));
			t_linedef *line = list->line;
			nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
			// list line
			sprintf(buffer, "line %d", line_counter++);
			nk_layout_row_push(ctx, 50);
			nk_label(ctx, buffer, NK_TEXT_LEFT);
			// checkbox for either side of line if applicable
			for (int i = 0; i < 2; i++) {
				int used = line->sides[i].sector_num ? 1 : 0;
				if (!(line->flags & L_TWO_SIDED) && i == 1) break ;
				if (!used) {
					if (nk_checkbox_label(ctx, "Use", &line->sectorized[i]))
						line->sectorized[i ^ 1] = 0;
					if (line->sectorized[i]) list->color = HIGHLIGHT;
				} else nk_label(ctx, " ", NK_TEXT_CENTERED);
			}
		}
	}
	nk_end(ctx);
}

/* set sector number or suggest using an unused number */
int		suggest_sector_number(t_map_interface *draw_mode)
{
	size.y += (draw_mode->win_h * 0.2);
	size.h = 50;
	static int sector_num = 1;
	if (nk_begin(ctx, "sector number", size, NK_WINDOW_BORDER)) {
		nk_layout_row_static(ctx, 2, 10, 1);
		nk_label(ctx, " ",  NK_TEXT_CENTERED);
		nk_layout_row_begin(ctx, NK_STATIC, 22, 4);
		nk_layout_row_push(ctx, 100);
		nk_label(ctx, "Sector number: ", NK_TEXT_LEFT);
		{
		char buffer[8];
		int len = sprintf(buffer, "%d", sector_num);
		nk_layout_row_push(ctx, 60);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 4, nk_filter_decimal);
		buffer[len] = 0;
		sector_num = atoi(buffer);
		}
		nk_layout_row_push(ctx, 2);
		nk_label(ctx, " ",  NK_TEXT_CENTERED);
		nk_layout_row_push(ctx, 62);
		if (nk_button_label(ctx, "Suggest")) {
			int suggest = 1;
			for (t_item_node *lines = draw_mode->bank->head_line; lines; lines = lines->next) {
				t_linedef *line = lines->line;
				for (int i = 0; i < 2; i++) {
					if (!(line->flags & L_TWO_SIDED) && i == 1) break ;
					if (line->sides[i].sector_num == suggest) ++suggest;
				}
			}
			sector_num = suggest;
		}
		nk_layout_row_end(ctx);
	}
	nk_end(ctx);
	return sector_num;
}

/* is it a valid enclosed sector */
int		check_lines_for_convex(void)
{
	// TODO
	// compare verticies of selected lines
	// to confirm an enclosed sector
	return 0;
}

/* various checks to see if the sector selection is valid */
int		confirm_sector(t_item_node *head_line, t_sector *new_sector)
{
	for (t_item_node *lines = head_line; lines; lines = lines->next) {
		t_linedef *line = lines->line;
		for (int i = 0; i < 2; i++)
			if (line->sectorized[i])
				++new_sector->line_count;
	}
	if (new_sector->line_count < 3) {
		new_sector->line_count = 0;
		return 1;
	}
	new_sector->sector_lines = malloc(sizeof(*new_sector->sector_lines) * new_sector->line_count);
	int i = 0;
	for (t_item_node *lines = head_line; lines; lines = lines->next) {
		t_linedef *line = lines->line;
		for (int j = 0; j < 2; j++) {
			if (line->sectorized[j]) {
				if (!j) {
					new_sector->sector_lines[i].start.x = line->start_vertex.x;
					new_sector->sector_lines[i].start.y = line->start_vertex.y;
					new_sector->sector_lines[i].end.x = line->end_vertex.x;
					new_sector->sector_lines[i].end.y = line->end_vertex.y;
				} else {
					new_sector->sector_lines[i].start.x = line->end_vertex.x;
					new_sector->sector_lines[i].start.y = line->end_vertex.y;
					new_sector->sector_lines[i].end.x = line->start_vertex.x;
					new_sector->sector_lines[i].end.y = line->start_vertex.y;
				}
				i++;
			}
		}
	}
	if (check_lines_for_convex()) {
		free(new_sector->sector_lines);
		bzero(new_sector, sizeof(*new_sector));
		return 2;
	}
	
	return 0;
}

/* functionality for accepting and adding the new sector */
void	finish_sector(t_map_interface *draw_mode, t_sector *new_sector)
{
	size.y += 50;
	size.h += 5;
	if (nk_begin(ctx, "finish sector", size, NK_WINDOW_BORDER)) {
		nk_layout_row_static(ctx, 5, 10, 1);
		nk_label(ctx, " ",  NK_TEXT_CENTERED);
		nk_layout_row_static(ctx, 20, draw_mode->win_w * 0.05, 3);
		nk_label(ctx, " ",  NK_TEXT_RIGHT);
		if (nk_button_label(ctx, "OK")) {
			int ret;
			if (!(ret = confirm_sector(draw_mode->bank->head_line, new_sector))) {
				int i;
				for (i = 0; i < SECTOR_MAX; i++) {
					if (draw_mode->sectors->sectors[i].line_count)
						continue ;
					// apply new sector to the next free slot in the sector array
					memcpy(&draw_mode->sectors->sectors[i], new_sector, sizeof(*new_sector));
					break ;
				}
				if (i >= SECTOR_MAX)
					dprintf(2, "UNABLE TO ADD MORE SECTORS\n");
				for (t_item_node *list = draw_mode->bank->head_line; list; list = list->next) {
					t_linedef *line = list->line;
					for (int j = 0; j < 2; j++) {
						if (line->sectorized[j]) {
							if (i < SECTOR_MAX)
								line->sides[j].sector_num = new_sector->sector_num;
							line->sectorized[j] = 0;
						}
					}
				}
				bzero(new_sector, sizeof(*new_sector));
			} else {
				// tell me why you cant
				if (ret == 1)
					dprintf(2, "Not enough lines selected to form a valid sector\n");
				if (ret == 2)
					dprintf(2, "Selected Lines do not form a valid sector\n");
			}
		}
	}
	nk_end(ctx);
}
