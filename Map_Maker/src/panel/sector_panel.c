#include "demo.h"
#include "list.h"
#include "sector.h"

static int  texture_popup = nk_false;
static int  texture_set = 0;
static int	sector_panel_op = 0;
struct nk_style_button *button;
struct nk_rect size;
struct nk_context *ctx;

void	add_sector(t_map_interface *draw_mode);
void	fill_lines(t_map_interface *draw_mode);
int		suggest_sector_number(t_map_interface *draw_mode);
void	finish_sector(t_map_interface *draw_mode, t_sector *new_sector);
void	edit_sector(t_map_interface *draw_mode);
void    list_floor_textures(t_map_interface *draw_mode, t_sector_info *info, t_resource_table *floor);

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
		if (nk_button_label(ctx, "Add Sector")) {
			// printf("sector pointer %p\n", (void*)draw_mode->sectors);
			// printf("sector[0] %p\n", (void*)draw_mode->sectors->sectors);
			// printf("selected %d\n", draw_mode->sectors->selected);
			sector_panel_op = SECTOR_ADD;
		}

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
int		confirm_sector(t_sector *sector, t_item_node *head_line, t_sector *new_sector)
{
	/* check if more than 3 lines are used */
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
	/* make sure no duplicate sector number */
	for (int i = 0; i < SECTOR_MAX; i++) {
		if (new_sector->sector_num == sector[i].sector_num)
			return 3;
	}
	/* create new sector */
	int line_index = 0;
	new_sector->sector_lines = malloc(sizeof(*new_sector->sector_lines) * new_sector->line_count);
	for (t_item_node *lines = head_line; lines; lines = lines->next) {
		t_linedef *line = lines->line;
		for (int j = 0; j < 2; j++) {
			if (line->sectorized[j]) {
				if (!j) {
					new_sector->sector_lines[line_index].start.x = line->start_vertex.x;
					new_sector->sector_lines[line_index].start.y = line->start_vertex.y;
					new_sector->sector_lines[line_index].end.x = line->end_vertex.x;
					new_sector->sector_lines[line_index].end.y = line->end_vertex.y;
				} else {
					new_sector->sector_lines[line_index].start.x = line->end_vertex.x;
					new_sector->sector_lines[line_index].start.y = line->end_vertex.y;
					new_sector->sector_lines[line_index].end.x = line->start_vertex.x;
					new_sector->sector_lines[line_index].end.y = line->start_vertex.y;
				}
				++line_index;
			}
		}
	}
	/* validate sector */
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
			if (!(ret = confirm_sector(draw_mode->sectors->sectors, draw_mode->bank->head_line, new_sector))) {
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
				if (ret == 3)
					dprintf(2, "Sector Number duplicate found\n");
			}
		}
	}
	nk_end(ctx);
}
int		sector_number(struct nk_input *input)
{

	static int sector_num = 1;
	char	buffer[8];
	int		len;
	nk_layout_row_begin(ctx, NK_STATIC, 20, 5);
	nk_layout_row_push(ctx, 45);
	nk_label(ctx, "List ", NK_TEXT_RIGHT);
	nk_layout_row_push(ctx, 30);
	if ((nk_input_is_key_pressed(input, NK_KEY_LEFT)) ||
	(nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_LEFT, "", NK_TEXT_LEFT))) {
		if (sector_num > 1) --sector_num;
	}
	len = snprintf(buffer, 7, "%d", sector_num);
	nk_layout_row_push(ctx, 40);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 4, nk_filter_decimal);
	buffer[len] = 0;
	sector_num = atoi(buffer);
	nk_layout_row_push(ctx, 30);
	if ((nk_input_is_key_pressed(input, NK_KEY_RIGHT)) ||
	(nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_RIGHT, " ", NK_TEXT_RIGHT))) {
		if (sector_num != (SECTOR_MAX)) ++sector_num;
	}
	if (sector_num < 1) sector_num = 1;
	if (sector_num > SECTOR_MAX) sector_num = SECTOR_MAX;
	nk_layout_row_end(ctx);
	return sector_num - 1;
}
/* panel functions for editing sector info */
void	edit_sector(t_map_interface *draw_mode)
{
	char	buffer[8];
	int		len;
	size.y += 80;
	size.h = draw_mode->win_h * 0.35;
	nk_window_set_focus(ctx, "edit sector");
	if (nk_begin(ctx, "edit sector", size, NK_WINDOW_BORDER)) {
		/* menubar to choose which sector to edit */
		nk_menubar_begin(ctx);
		draw_mode->sectors->selected = sector_number(&draw_mode->ctx->input);
		nk_menubar_end(ctx);
		for (t_item_node *list = draw_mode->bank->head_line; list; list = list->next)
			list->color = LINE_COLOR;
		if (!draw_mode->sectors->sectors[draw_mode->sectors->selected].line_count) {
			nk_end(ctx);
			return ;
		}
		// draw sector lines colored
		int i = 0;
		for (t_item_node *list = draw_mode->bank->head_line; list; list = list->next) {
			if (i >= draw_mode->sectors->sectors[draw_mode->sectors->selected].line_count)
				continue ;
			t_line edge = draw_mode->sectors->sectors[draw_mode->sectors->selected].sector_lines[i];
			t_linedef *line = list->line;
			if ((line->start_vertex.x == edge.start.x &&
			line->start_vertex.y == edge.start.y &&
			line->end_vertex.x == edge.end.x &&
			line->end_vertex.y == edge.end.y) ||
			(line->start_vertex.x == edge.end.x &&
			line->start_vertex.y == edge.end.y &&
			line->end_vertex.x == edge.start.x &&
			line->end_vertex.y == edge.start.y)) {
				list->color = HIGHLIGHT;
				++i;	
			}
			else list->color = LINE_COLOR;
		}
		char *sections[ ] = {"Ceiling: ", "Floor: "};
		t_sector_info *info = &draw_mode->sectors->sectors[draw_mode->sectors->selected].sector_info;
		/* Change height of sector floor and ceiling */
		nk_layout_row_dynamic(ctx, 25, 2);
		nk_label(ctx, "Sector :", NK_TEXT_LEFT);
		{
		char num[8];
		sprintf(num, "%d", draw_mode->sectors->sectors[draw_mode->sectors->selected].sector_num);
		nk_label(ctx, num, NK_TEXT_LEFT);
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Sector heights", NK_TEXT_LEFT);
		nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
		for (int i = 0; i < 2; i++) {
			nk_layout_row_push(ctx, 55);
			nk_label(ctx, sections[i], NK_TEXT_LEFT);
			nk_layout_row_push(ctx, 40);
			len = snprintf(buffer, 8, "%d", !i ? info->room_heights.x : info->room_heights.y);
			nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 7, nk_filter_decimal);
			if (!i) info->room_heights.x = atoi(buffer);
			else info->room_heights.y = atoi(buffer);
		}
		nk_layout_row_end(ctx);

		/* add textures to floor and ceiling of sector */
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Textures", NK_TEXT_LEFT);
		nk_layout_row_begin(ctx, NK_STATIC, 30, 3);
		for (int i = 0; i < 2; i++) {
			nk_layout_row_push(ctx, 60);
			nk_label(ctx, sections[i], NK_TEXT_LEFT);
			nk_layout_row_push(ctx, 100);
			if (nk_button_label(ctx, info->flats[i])) {
				texture_popup = nk_true;
				texture_set = i;
			}
			nk_layout_row_push(ctx, 50);
			if (nk_button_label(ctx, "clear"))
				memset(info->flats[i], 0, sizeof(info->flats[i]));
		}
		nk_layout_row_end(ctx);

		/* Edit light special and tag values */
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Additional info", NK_TEXT_LEFT);
		nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
		char *infos[] = {"Light: ", "Special: ", "Tag: "};
		for (int i = 0; i < 3; i++) {
			nk_layout_row_push(ctx, 70);
			nk_label(ctx, infos[i], NK_TEXT_LEFT);
			nk_layout_row_push(ctx, 40);
			if (i == 0) len = snprintf(buffer, 7, "%d", info->light);
			else if (i == 1) len = snprintf(buffer, 7, "%d", info->special);
			else len = snprintf(buffer, 7, "%d", info->tag);
			nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 23, nk_filter_decimal);
			buffer[len] = 0;
			if (i == 0) info->light = atoi(buffer);
			else if (i == 1) info->special = atoi(buffer);
			else info->tag = atoi(buffer);
		}
		/* delete sector */
		nk_layout_row_dynamic(ctx, 40, 1);
		if (nk_button_label(ctx, "Clear")) {
			// unsectorize line choices from sector to clear
			t_sector *to_clear = &draw_mode->sectors->sectors[draw_mode->sectors->selected];
			int line_index = 0;
			for (t_item_node *list = draw_mode->bank->head_line; list && line_index < to_clear->line_count; list = list->next) {
				t_linedef *line = list->line;
				// check right side of line
				if (line->start_vertex.x == to_clear->sector_lines[line_index].start.x &&
				line->start_vertex.y == to_clear->sector_lines[line_index].start.y &&
				line->end_vertex.x == to_clear->sector_lines[line_index].end.x &&
				line->end_vertex.y == to_clear->sector_lines[line_index].end.y) {
					line->sectorized[0] = 0;
					line->sides[0].sector_num = 0;
					++line_index;
				}
				// check left side of line
				else if (line->start_vertex.x == to_clear->sector_lines[line_index].end.x &&
				line->start_vertex.y == to_clear->sector_lines[line_index].end.y &&
				line->end_vertex.x == to_clear->sector_lines[line_index].start.x &&
				line->end_vertex.y == to_clear->sector_lines[line_index].start.y) {
					line->sides[1].sector_num = 0;
					line->sectorized[1] = 0;
					++line_index;
				}
			}
			// free sector
			free(to_clear->sector_lines);
			bzero(to_clear, sizeof(*to_clear));
			--draw_mode->sectors->selected;
			if (draw_mode->sectors->selected < 1) draw_mode->sectors->selected = 1;
		}
		/* Texture Select window */
    	if (texture_popup) {
			struct nk_rect s = {-40, -90,
			draw_mode->win_w / 5, draw_mode->win_h / 2};
			if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Texture Select", 0, s))
			{
				nk_menubar_begin(ctx);
				nk_layout_row_begin(ctx, NK_STATIC, 25, 3);
				nk_layout_row_push(ctx, 130);
				nk_label(ctx, "Select Texture for ", NK_TEXT_LEFT);
				nk_layout_row_push(ctx, 60);
				nk_label(ctx, sections[texture_set], NK_TEXT_LEFT);
				if (nk_button_label(ctx, "Cancel")) {
					texture_popup = nk_false;
					nk_popup_close(ctx);
				}
				// hit enter to close the popup window
				if (nk_input_is_key_pressed(&ctx->input, NK_KEY_ENTER)) {
					texture_popup = nk_false;
					nk_popup_close(ctx);
				}
				nk_menubar_end(ctx);
				/* list texture options */
				list_floor_textures(draw_mode, info, draw_mode->floor);
				nk_popup_end(ctx);
			} else texture_popup = nk_false;
		}
	}
	nk_end(ctx);
}

void    list_floor_textures(t_map_interface *draw_mode, t_sector_info *info, t_resource_table *floor)
{
    static int low = 0;
    nk_layout_row_begin(ctx, NK_STATIC, 34, 5);
    if (floor->size < MAX_RESOURCE_COUNT && floor->size % 10) {
        int nearest_ten = (int)floor->size % 10;
        floor->size = floor->size + (10 - nearest_ten);
    }
    for (int i = low; i < low + 10 && i < (int)floor->size; i++) {
        t_resource texture = floor->table[i];
        nk_layout_row_push(ctx, 55);
        if (nk_button_label(ctx, "Select")) {
            if (texture.name[0]) {
                strcpy(info->flats[texture_set], texture.name);
                texture_popup = nk_false;
                nk_popup_close(ctx);
            }
        }
        nk_layout_row_push(ctx, 10);
        nk_label(ctx, " ", 1);

        nk_layout_row_push(ctx, 80);
        nk_label(ctx, texture.name, NK_TEXT_RIGHT);

        nk_layout_row_push(ctx, 30);
        nk_label(ctx, " ", 1);

//      nk_label_wrap(ctx, "placeholder texture location"); //replace with image of texture
        /* cheap workaround for not knowing how to render a png in nuklear */
        nk_layout_row_push(ctx, 80);
        if (nk_button_label(ctx, "Preview")) {
            if (texture.name[0]) {
                char buffer[32] = "open assets/floor/"; //open on macos
                strcat(buffer, texture.name);
                system (buffer);
            }
        }
		// memset(buffer, 0, sizeof(buffer));   <-- DELETE THIS MAYBE
    }
    nk_layout_row_end(ctx);

    /* cycle thru 10 textures at a time */
    nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
    nk_layout_row_push(ctx, 60);
    nk_label(ctx, " ", 1);

    nk_layout_row_push(ctx, 80);
    if (nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_LEFT, "PREV", NK_TEXT_RIGHT)) {
        if (low >= 10)
            low -= 10;
    }

    if (nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_RIGHT, "NEXT", NK_TEXT_LEFT)) {
        if (low < (int)floor->size - 10)
            low += 10;
    }
    nk_layout_row_end(ctx);
}
