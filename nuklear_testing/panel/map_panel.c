#include "demo.h"
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

struct nk_command_buffer *canvas;
static struct nk_rect cursor = { .w = 4, .h = 4};
static struct nk_rect end_point = { .w = 4, .h = 4};
static int count = 0;
static int count2 = 0;
static int show_grid = nk_true;
static int show_about = nk_false;
static int save_as = nk_false;
static int open_map = nk_false;

void    draw_lines(t_map_interface *draw_mode);
void    draw_things(t_map_interface *draw_mode);
void    draw_grid(struct nk_rect size);
void    draw_menu(t_map_interface *draw_mode);
void    draw_about(struct nk_context *ctx);
void    save_map_name(t_map_interface *draw_mode);
void    open_map_list(t_map_interface *draw_mode);

void    map_pannel(t_map_interface *draw_mode)
{
	struct nk_rect size;

	/* pannel size nk_rect(5, 5, 1200, 800) */
	size = nk_rect(WINDOW_OFFSET, WINDOW_OFFSET,
	WINDOW_WIDTH - (WINDOW_WIDTH / 4),
	WINDOW_HEIGHT - (WINDOW_OFFSET * 2));
	char name[32] = "Map Maker - ";
	strcat(name, draw_mode->map_name[0] ? draw_mode->map_name : "Unspecified");
	if (nk_begin(draw_mode->ctx, name, size,
		NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MINIMIZABLE))
		{
			canvas = nk_window_get_canvas(draw_mode->ctx);
			if (show_grid) draw_grid(nk_window_get_content_region(draw_mode->ctx));
			draw_mode->ctx->style.button.normal = nk_style_item_color(BUTTON_DEFAULT);
			SDL_ShowCursor(SDL_ENABLE);
			if ((draw_mode->tool_op) == LINE) {
				// follow cursor for start point of line
				nk_fill_circle(canvas, cursor, nk_rgb(255, 0, 0));
				draw_lines(draw_mode);
			}
			else if ((draw_mode->tool_op) == THING) {
				// follow cursor for thing location
				nk_fill_circle(canvas, cursor, nk_rgb(0, 255, 0));
				draw_things(draw_mode);
			}
			/* fill items on map */
			for (t_item_node *temp = draw_mode->bank->head_line; temp; temp = temp->next)
				stroke_my_line(canvas, temp);
			for (t_item_node *temp = draw_mode->bank->head_thing; temp; temp = temp->next)
				stroke_box(canvas, temp);
			/* menu bar */
			if (draw_mode->tool_op == NK_FILE) draw_menu(draw_mode);
			/* about map-maker */
			if (show_about) draw_about(draw_mode->ctx);
		
		}
		nk_end(draw_mode->ctx);
	/* Window for naming map file */
	if (save_as)
		save_map_name(draw_mode);
	/* Window for opening mapfile */
   if (open_map)
	   open_map_list(draw_mode);

}

void    draw_lines(t_map_interface *draw_mode)
{
	static struct nk_vec2 line_start;
	static struct nk_vec2 line_end;

	if (draw_mode->ctx->input.mouse.pos.x >= 5 &&
	draw_mode->ctx->input.mouse.pos.x <=
	(WINDOW_WIDTH - (WINDOW_WIDTH / 4) + WINDOW_OFFSET) &&
	draw_mode->ctx->input.mouse.pos.y >= 33 &&
	draw_mode->ctx->input.mouse.pos.y <=
	(WINDOW_HEIGHT - (WINDOW_OFFSET * 2)) + WINDOW_OFFSET) {
		SDL_ShowCursor(SDL_DISABLE);

		if (nk_input_mouse_clicked(&draw_mode->ctx->input,
		NK_BUTTON_RIGHT, nk_window_get_bounds(draw_mode->ctx)))
				count = 0;
		if (nk_input_mouse_clicked(&draw_mode->ctx->input,
		NK_BUTTON_LEFT, nk_window_get_bounds(draw_mode->ctx)))
				count++;
		if (count == 0) {
			// set coordinates for beginning of line
			line_start = draw_mode->ctx->input.mouse.pos;
			cursor.x = line_start.x;
			cursor.y = line_start.y;
		}
		if (count == 1) {
			// set coordinates for end of line
			line_end = draw_mode->ctx->input.mouse.pos;
			end_point.x = line_end.x;
			end_point.y = line_end.y;
			// follow cursor with circle
			nk_fill_circle(canvas, end_point, nk_rgb(171, 128, 255));
		}
		if (count == 2) {
			add_line(draw_mode->bank, line_start, line_end);
			count = 0;
		}
	}
}

void    draw_things(t_map_interface *draw_mode)
{
	static struct nk_vec2 thing_pos;
	struct nk_rect s = nk_rect(
	(WINDOW_WIDTH / 2) - 150, (WINDOW_HEIGHT / 2) - 100, 250, 120);

	if (draw_mode->ctx->input.mouse.pos.x >= 5 &&
	draw_mode->ctx->input.mouse.pos.x <=
	(WINDOW_WIDTH - (WINDOW_WIDTH / 4) + WINDOW_OFFSET) &&
	draw_mode->ctx->input.mouse.pos.y >= 33 &&
	draw_mode->ctx->input.mouse.pos.y <=
	(WINDOW_HEIGHT - (WINDOW_OFFSET * 2)) + WINDOW_OFFSET) {
		SDL_ShowCursor(SDL_DISABLE);
		// set coordinates for thing
		if (count2 == 0) {
			thing_pos = draw_mode->ctx->input.mouse.pos;
			cursor.x = thing_pos.x;
			cursor.y = thing_pos.y;
		}
		if (nk_input_mouse_clicked(&draw_mode->ctx->input,
		NK_BUTTON_LEFT, nk_window_get_bounds(draw_mode->ctx)))
			count2++;
		if (count2) {
			if (nk_popup_begin(draw_mode->ctx, NK_POPUP_STATIC, "", 0, s))
			{
				SDL_ShowCursor(SDL_ENABLE);
				// submit buffer
				static char buffer2[16];
				//edit buffer
				char buffer[16];
				/* add submit buffer to edit buffer */
				int len = snprintf(buffer, 16, "%s", buffer2);
				/* create rows to display the edit buffer */
				nk_layout_row_dynamic(draw_mode->ctx, 25, 1);
				nk_label(draw_mode->ctx, "Name Thing", NK_TEXT_CENTERED);

				nk_layout_row_begin(draw_mode->ctx, NK_STATIC, 25, 2);
				nk_layout_row_push(draw_mode->ctx, 168);
				nk_edit_string(draw_mode->ctx, NK_EDIT_SIMPLE, buffer, &len, 16, nk_filter_ascii);

				nk_layout_row_push(draw_mode->ctx, 50);
				if (nk_button_label(draw_mode->ctx, "clear")) {
					memset(buffer, 0, 16);
					memset(buffer2, 0, 16);
				}
				nk_layout_row_end(draw_mode->ctx);
				
				/* copy edit back to submit buffer */
				buffer[len] = 0;
				memcpy(buffer2, buffer, 16);
				// spacing between rows
				nk_layout_row_dynamic(draw_mode->ctx, 10, 1);
				nk_label(draw_mode->ctx, " ", 1);
				/* buttons to cancel, or add thing with or without name */
				nk_layout_row_dynamic(draw_mode->ctx, 25, 3);
				if (nk_button_label(draw_mode->ctx, "OK")) {
					add_thing(draw_mode->bank, thing_pos, buffer2);
					memset(buffer2, 0, 16);
					count2 = -1;
				}
				if (nk_button_label(draw_mode->ctx, "No Name")) {
					add_thing(draw_mode->bank, thing_pos, "");
					memset(buffer2, 0, 16);
					count2 = -1;
				}
				if (nk_button_label(draw_mode->ctx, "Cancel")) {
					memset(buffer2, 0, 16);
					count2 = -1;
				}
				nk_popup_end(draw_mode->ctx);
			}
		}
	}
}

void    draw_grid(struct nk_rect size)
{
	float x, y;
	const float grid_size = 10.0f;
	const struct nk_color grid_color = nk_rgba(35, 35, 35, 150);
	const struct nk_color grid_color_2 = nk_rgb(50, 50, 50);
	const struct nk_color grid_color_3 = nk_rgb(30, 30, 30);
	
	/* sub grid */
	for (x = grid_size; x < size.w; x += grid_size * 2)
		nk_stroke_line(canvas, x+size.x, size.y, x+size.x, size.y+size.h, 1.0f, grid_color);
	for (y = (float)fmod(size.y, grid_size) + grid_size; y < size.h; y += grid_size * 2)
		nk_stroke_line(canvas, size.x, y+size.y, size.x+size.w, y+size.y, 1.0f, grid_color);

	/* inner grid */
	for (x = 0; x < size.w; x += grid_size * 2)
		nk_stroke_line(canvas, x+size.x, size.y, x+size.x, size.y+size.h, 1.0f, grid_color_2);
	for (y = (float)fmod(size.y, grid_size); y < size.h; y += grid_size * 2)
		nk_stroke_line(canvas, size.x, y+size.y, size.x+size.w, y+size.y, 1.0f, grid_color_2);

	/* outer grid */
	for (x = 0; x < size.w; x += grid_size*grid_size)
		nk_stroke_line(canvas, x+size.x, size.y, x+size.x, size.y+size.h, 1.5f, grid_color_3);
	for (y = -3.5; y < size.h; y += grid_size*grid_size)
		nk_stroke_line(canvas, size.x, y+size.y, size.x+size.w, y+size.y, 1.5f, grid_color_3);
}

void    draw_about(struct nk_context *ctx)
{
	static struct nk_rect s = {20, 100, 300, 250};
	if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, s))
	{
		nk_layout_row_begin(ctx, NK_STATIC, 25, 3);
		nk_layout_row_push(ctx, 50);
		nk_label(ctx, " ", NK_TEXT_CENTERED);
		nk_layout_row_push(ctx, 70);
		nk_label_colored(ctx, "DOOM ", NK_TEXT_RIGHT, nk_rgb(255,0,0));
		nk_label(ctx, " Map-Maker", NK_TEXT_LEFT);
		nk_layout_row_end(ctx);
		nk_layout_row_dynamic(ctx, 20, 2);
		nk_label(ctx, "Leader:", NK_TEXT_LEFT);
		nk_label(ctx, "Samuel Maddox", NK_TEXT_RIGHT);
		nk_label(ctx, "Designer/Developer:", NK_TEXT_LEFT);
		nk_label(ctx, "Robert Papagna", NK_TEXT_RIGHT);
		nk_label(ctx, "Research/Info:", NK_TEXT_LEFT);
		nk_label(ctx, "Steven Limon", NK_TEXT_RIGHT);
		nk_label(ctx, "Support:", NK_TEXT_LEFT);
		nk_label(ctx, "Francisco Guzman", NK_TEXT_RIGHT);

		nk_layout_row_dynamic(ctx, 10, 1);
		nk_label(ctx, " ", NK_TEXT_CENTERED);
		nk_layout_row_dynamic(ctx, 60, 1);
		nk_label_wrap(ctx, "nuklear is licensed under the public domain License.");
		nk_popup_end(ctx);
	} else show_about = nk_false;
}

int     check_exists(const char *name);
int     save(t_map_interface *draw_mode);
void    draw_menu(t_map_interface *draw_mode)
{
	struct nk_context *ctx = draw_mode->ctx;
	nk_menubar_begin(ctx);
	nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
	nk_layout_row_push(ctx, 40);
	if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(120, 200)))
	{
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_menu_item_label(ctx, "Hide menu", NK_TEXT_LEFT)) {
			draw_mode->tool_op = LINE;
			count = -1;
		}
		if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT))
			show_about = nk_true;
		nk_checkbox_label(ctx, "Show grid", &show_grid);
		nk_menu_end(ctx);
	}
	nk_layout_row_push(ctx, 60);
	if (nk_menu_begin_label(ctx, "FILE", NK_TEXT_LEFT,
	nk_vec2(120, 200)))
	{
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_menu_item_label(ctx, "New", NK_TEXT_LEFT)) {
			/* do you want to save? */
			if (check_exists(draw_mode->map_name) < 1)
				printf("I hope you saved your work\n");
			bzero(draw_mode->bank, sizeof(*draw_mode->bank));
			memset(draw_mode->map_name, 0, sizeof(draw_mode->map_name));
		}
		/* load in a file that was previously saved */
		if (nk_menu_item_label(ctx, "Open", NK_TEXT_LEFT)) {
			open_map = nk_true;
		}
		if (nk_menu_item_label(ctx, "Save", NK_TEXT_LEFT)) {
			/* create new file name */
			if (check_exists(draw_mode->map_name) < 0)
				save_as = nk_true;
			/* save bank contents into a file */
			if (!save_as) {
				if (!save(draw_mode))
					printf("Saving to %s\n", draw_mode->map_name);
			}
		}
		if (nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT)) {
			if (check_exists(draw_mode->map_name))
				printf("Exit program without saving\n... Sorry :(\n");
			exit(0);
		}
		nk_menu_end(ctx);
	}
	nk_menubar_end(ctx);
}

int     check_exists(const char *name)
{
	if (!name[0]) return (-1);
	DIR *dir = opendir(MAP_SAVE_PATH);
	struct dirent *sd;
	while (dir && (sd = readdir(dir))) {
		if (!strcmp(name, sd->d_name))
			return (1);
	}
	if (dir) closedir(dir);
	return (0);
}

typedef struct  s_lumped
{
	t_header    head;
	t_lump      lumps[2];
}               t_lumped;

int     save(t_map_interface *draw_mode)
{
	t_item_node *item;
	t_bank      *bank = draw_mode->bank;
	t_lumped    lumped = {0};
		lumped.head.num_lumps = 2;
		lumped.head.lump_offset = 0;
	int         fd;
	int         size = 0;
	char path[strlen(MAP_SAVE_PATH) + sizeof(draw_mode->map_name)] = {0};
	strcat(path, MAP_SAVE_PATH);
	strcat(path, draw_mode->map_name);
	fd = open(path, O_RDWR | O_CREAT, 0666);
	if (fd < 3)
		return (dprintf(2, "MAP PATH ERROR\n"));
	/* write header */
	size += write(fd, "DWD\n", 4);
	size += write(fd, &lumped.head.num_lumps, sizeof(lumped.head.num_lumps));
	size += write(fd, &lumped.head.lump_offset, sizeof(lumped.head.lump_offset));
	/* write item info */
	for (int i = 0; i < 2; i++) {
		strcpy(lumped.lumps[i].lump_name, i ? "Thing" : "Line");
		lumped.lumps[i].offset = size;
		int count = i ? bank->count_thing : bank->count_line;
		/* write lump index */
		size += write(fd, &i, sizeof(i));
		/* write item count */
		size += write(fd, &count, sizeof(count));
		item = i ? bank->head_thing : bank->head_line;
		for (int j = 0 ; j < count; j++) {
			size += write(fd, &item->color, sizeof(item->color));
			if (i == 0) {
				/* write line data */
				size += write(fd, &item->line->start_vertex, sizeof(item->line->start_vertex));
				size += write(fd, &item->line->end_vertex, sizeof(item->line->end_vertex));
				size += write(fd, &item->line->special, sizeof(item->line->special));
				size += write(fd, &item->line->tag, sizeof(item->line->tag));
				size += write(fd, &item->line->flags, sizeof(item->line->flags));
				for (int k = 0; k < 2; k++) {
					size += write(fd, &item->line->sides[k].offset, sizeof(item->line->sides[0].offset));
					size += write(fd, &item->line->sides[k].textures, sizeof(item->line->sides[0].textures));
				/*
					size += write(fd, &item->line->sides[k].sector_info.flats[0], sizeof(item->line->sides[k].sector_info.flats[0]));
					size += write(fd, &item->line->sides[k].sector_info.flats[1], sizeof(item->line->sides[k].sector_info.flats[1]));
					size += write(fd, &item->line->sides[k].sector_info.light, sizeof(item->line->sides[k].sector_info.light));
					size += write(fd, &item->line->sides[k].sector_info.special, sizeof(item->line->sides[k].sector_info.special));
					size += write(fd, &item->line->sides[k].sector_info.tag, sizeof(item->line->sides[k].sector_info.tag));
					size += write(fd, &item->line->sides[k].sector_num, sizeof(item->line->sides[k].sector_num));
				*/
				}
			} else {
				// write thing data
				size += write(fd, &item->thing->pos, sizeof((item->thing->pos)));
				size += write(fd, &item->thing->angle, sizeof((item->thing->angle)));
				size += write(fd, &item->thing->type, sizeof((item->thing->type)));
				size += write(fd, &item->thing->name, sizeof((item->thing->name)));
				size += write(fd, &item->thing->flags, sizeof((item->thing->flags)));
				size += write(fd, &item->thing->color, sizeof((item->thing->color)));

			}
			item = item->next;
		}
		lumped.lumps[i].size = size - lumped.lumps[i].offset;
	}
	close(fd);
	return (0);
}

void    save_map_name(t_map_interface *draw_mode)
{
	struct nk_context *ctx = draw_mode->ctx;
	nk_window_set_focus(ctx, "Save as");
	static struct nk_rect s = {(WINDOW_WIDTH * .05),
	(WINDOW_HEIGHT * .15), 200, 125};
	if (nk_begin(ctx, "Save as", s, NK_WINDOW_BORDER)) {
		//submit buffer
		static char buffer2[20];
		//edit buffer
		char buffer[20];
		int len = snprintf(buffer, 20, "%s", buffer2);
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Name Map", NK_TEXT_CENTERED);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 20, nk_filter_ascii);

		buffer[len] = 0;
		memcpy(buffer2, buffer, 20);
		// spacing between rows
		nk_layout_row_dynamic(ctx, 10, 1);
		nk_label(ctx, " ", 1);
		/* buttons to cancel, or add thing with or without name */
		nk_layout_row_dynamic(ctx, 25, 2);
		if (nk_button_label(ctx, "OK")) {
			if (buffer2[0]) {
				strcpy(draw_mode->map_name, buffer2);
				memset(buffer2, 0, 16);
				save_as = nk_false;
				printf("\nSaving ...\n");
				if (!save(draw_mode))
					printf("Saved to %s\n", draw_mode->map_name);
				else printf("save error\n");
			}
		}
		if (nk_button_label(ctx, "Cancel")) {
			memset(buffer2, 0, 16);
			save_as = nk_false;
		}
	}
	nk_end(ctx);
}

int     load_map(t_map_interface *draw_mode, char *name)
{
	int     fd;
	char    path[strlen(MAP_SAVE_PATH) + sizeof(draw_mode->map_name)] = {0};
	strcat(path, MAP_SAVE_PATH);
	strcat(path, name);
	printf("\nOpening %s ...\n", name);
	fd = open(path, O_RDONLY);
	if (fd < 3)
		return (dprintf(2, "MAP PATH ERROR\n"));
	t_lumped lumped = {0};
	/* read header */
	read(fd, lumped.head.name, sizeof(lumped.head.name));
	if (strncmp("DWD\n", lumped.head.name, 4))
		return (dprintf(2, "Improper header\n"));
	/* read num lumps */
	read(fd, &lumped.head.num_lumps, sizeof(lumped.head.num_lumps));
	/* read lump offset */
	read(fd, &lumped.head.lump_offset, sizeof(lumped.head.lump_offset));
	t_bank *bank = draw_mode->bank;
	bzero(bank, sizeof(*bank));
	for (int i = 0; i < 2; i++) {
		int index;
		int count;
		/* read lump index */
		read(fd, &index, sizeof(index));
		/* read item count */
		read(fd, &count, sizeof(count));
		if (count < 0 || count > 4096)
			return (dprintf(2, "Invalid %s bank count %d\n", i ? "thing" : "line", count));
		for (int j = 0; j < count; j++) {
			struct nk_color color;
			read(fd, &color, sizeof(color));
			if (i == 0) {
				struct nk_vec2 start;
				struct nk_vec2 end;
				/* read line data */
				read(fd, &start, sizeof(start));
				read(fd, &end, sizeof(end));
				add_line(bank, start, end);
				t_linedef *line = bank->tail_line->line;
				read(fd, &line->special, sizeof(line->special));
				read(fd, &line->tag, sizeof(line->tag));
				read(fd, &line->flags, sizeof(line->flags));
				for (int k = 0; k < 2; k++) {
					read(fd, &line->sides[k].offset, sizeof(line->sides[0].offset));
					read(fd, &line->sides[k].textures, sizeof(line->sides[0].textures));
				/*
					read(fd, &line->sides[k].sector_info.flats[0], sizeof(line->sides[k].sector_info.flats[0]));
					read(fd, &line->sides[k].sector_info.flats[1], sizeof(line->sides[k].sector_info.flats[1]));
					read(fd, &line->sides[k].sector_info.light, sizeof(line->sides[k].sector_info.light));
					read(fd, &line->sides[k].sector_info.special, sizeof(line->sides[k].sector_info.special));
					read(fd, &line->sides[k].sector_info.tag, sizeof(line->sides[k].sector_info.tag));
					read(fd, &line->sides[k].sector_num, sizeof(line->sides[k].sector_num));
				*/
				}
			} else {
				/* read thing data */
				struct nk_vec2 location;
				char name[16];
				read(fd, &location, sizeof((location)));
				read(fd, name, sizeof((name)));
				int q = 0;
				for (int p = 0; p < 16; p++) {
					if (name[p]) {
						name[q++] = name[p];
						name[p] = 0;
					}
				}
				add_thing(bank, location, name);
				t_thing *thing = bank->tail_thing->thing;
				read(fd, &thing->angle, sizeof((thing->angle)));
				read(fd, &thing->type, sizeof((thing->type)));
				read(fd, &thing->flags, sizeof((thing->flags)));
				read(fd, &thing->color, sizeof((thing->color)));
			}
		}
	}
	return (0);
}

void    open_map_list(t_map_interface *draw_mode)
{
	struct nk_context *ctx = draw_mode->ctx;
	nk_window_set_focus(ctx, "Open");
	static struct nk_rect s = {(WINDOW_WIDTH * .05),
	(WINDOW_HEIGHT * .15), 200, (WINDOW_HEIGHT / 3)};
	if (nk_begin(ctx, "Open", s, NK_WINDOW_BORDER)) {
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Map select", NK_TEXT_CENTERED);
		// spacing between rows
		nk_layout_row_dynamic(ctx, 10, 1);
		nk_label(ctx, " ", 1);
		/* print out map selections */
		nk_layout_row_dynamic(ctx, 25, 2);
		DIR *dir = opendir(MAP_SAVE_PATH);
		struct dirent *sd;
		while (dir && (sd = readdir(dir))) {
			if (sd->d_name[0] == '.') continue ;
			if (nk_button_label(ctx, "Select")) {
				if (!load_map(draw_mode, sd->d_name)) {
					printf("Opened %s\n", sd->d_name);
					strcpy(draw_mode->map_name, sd->d_name);
					open_map = nk_false;
					break ;
				}
			}
			nk_label(ctx, sd->d_name, NK_TEXT_CENTERED);
		}
		if (dir) closedir(dir);
		nk_layout_row_dynamic(ctx, 10, 1);
		nk_label(ctx, " ", 1);
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_button_label(ctx, "Cancel"))
			open_map = nk_false;
	}
	nk_end(ctx);

}
