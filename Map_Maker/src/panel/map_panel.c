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
static int show_help = nk_true;
static int save_as = nk_false;
static int open_map = nk_false;
static int show_resources = nk_false;

void    draw_lines(t_map_interface *draw_mode);
void    draw_things(t_map_interface *draw_mode);
void    draw_grid(struct nk_rect size);
void    draw_menu(t_map_interface *draw_mode);
void    draw_about(struct nk_context *ctx);
void    draw_help(t_map_interface *draw_mode);
void    draw_resources(t_map_interface *draw_mode);

void    map_panel(t_map_interface *draw_mode, int *launch_help)
{
	if (!(*launch_help)) show_help = nk_false;
	struct nk_rect size;
	/* panel size nk_rect(5, 5, 1200, 800) (size and offset for 1600 x 900 window) */
	size = nk_rect(WINDOW_OFFSET, WINDOW_OFFSET,
	draw_mode->win_w - (draw_mode->win_w / 4),
	draw_mode->win_h - (WINDOW_OFFSET * 2));
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
			/* help info */
			if (show_help) draw_help(draw_mode);
			/* all resources */
			if (show_resources) draw_resources(draw_mode);
		
		}
		nk_end(draw_mode->ctx);
	/* Window for naming map file */
	if (save_as)
		save_map_name(draw_mode, &save_as);
	/* Window for opening mapfile */
   if (open_map)
	   open_map_list(draw_mode, &open_map);

}

void    draw_lines(t_map_interface *draw_mode)
{
	static struct nk_vec2 line_start;
	static struct nk_vec2 line_end;

	if (draw_mode->ctx->input.mouse.pos.x >= 5 &&
	draw_mode->ctx->input.mouse.pos.x <=
	(draw_mode->win_w - (draw_mode->win_w / 4) + WINDOW_OFFSET) &&
	draw_mode->ctx->input.mouse.pos.y >= 33 &&
	draw_mode->ctx->input.mouse.pos.y <=
	(draw_mode->win_h - (WINDOW_OFFSET * 2)) + WINDOW_OFFSET) {
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
			add_line(draw_mode, line_start, line_end);
			count = 0;
		}
	}
}

void    draw_things(t_map_interface *draw_mode)
{
	static struct nk_vec2 thing_pos;
	struct nk_rect s = nk_rect(
	(draw_mode->win_w / 2) - 150, (draw_mode->win_h / 2) - 100, 250, 120);

	if (draw_mode->ctx->input.mouse.pos.x >= 5 &&
	draw_mode->ctx->input.mouse.pos.x <=
	(draw_mode->win_w - (draw_mode->win_w / 4) + WINDOW_OFFSET) &&
	draw_mode->ctx->input.mouse.pos.y >= 33 &&
	draw_mode->ctx->input.mouse.pos.y <=
	(draw_mode->win_h - (WINDOW_OFFSET * 2)) + WINDOW_OFFSET) {
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
					add_thing(draw_mode, thing_pos, buffer2);
					memset(buffer2, 0, 16);
					count2 = -1;
				}
				if (nk_button_label(draw_mode->ctx, "No Name")) {
					add_thing(draw_mode, thing_pos, "");
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
		nk_label(ctx, " Map Maker", NK_TEXT_LEFT);
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

void	draw_help(t_map_interface *draw_mode)
{
	struct nk_context *ctx = draw_mode->ctx;
	struct nk_rect s = {(draw_mode->win_w / 3), (150), 250, 350};
	if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Help", NK_WINDOW_CLOSABLE|NK_WINDOW_MOVABLE, s))
	{
		nk_layout_row_dynamic(ctx, 6, 1);
		nk_label(ctx, " ", 1);
		nk_layout_row_dynamic(ctx, 60, 1);
		nk_label_colored_wrap(ctx, "Help can always be accessed in the MENU dropdown which appears after selecting FILE in the tool panel.", nk_rgb(255,255,0));

		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Map:", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 30, 1);
		nk_label_wrap(ctx, "Displays Lines and Things in the current map.");
		nk_layout_row_dynamic(ctx, 45, 1);
		nk_label_wrap(ctx, "Lines are drawn in black and things varry by color, but are gray by default.");
		nk_layout_row_dynamic(ctx, 55, 1);
		nk_label_wrap(ctx, "A single item will be highlighted in oragne to indicate the current selection for later editing.");

		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Tools:", NK_TEXT_LEFT);
			nk_label(ctx, "- Move -", NK_TEXT_LEFT);
				nk_layout_row_dynamic(ctx, 40, 1);
				nk_label_wrap(ctx, "Unused at this time of development");
			nk_layout_row_dynamic(ctx, 25, 1);
			nk_label(ctx, "- Edit -", NK_TEXT_LEFT);
				nk_layout_row_dynamic(ctx, 80, 1);
				nk_label_wrap(ctx, "Opens a window that allows for editing data regarding the current slected item, identified by the selected orange item, or in list panel on the right.");
			nk_layout_row_dynamic(ctx, 25, 1);
			nk_label(ctx, "- Line -", NK_TEXT_LEFT);
				nk_layout_row_dynamic(ctx, 60, 1);
				nk_label_wrap(ctx, "Changes the cursor to a red dot while hovering in the map window. Left click to select the start and and end points.");
				nk_layout_row_dynamic(ctx, 50, 1);
				nk_label_wrap(ctx, "The purple dot indicates when the second point is ready to be selected.");
				nk_layout_row_dynamic(ctx, 60, 1);
				nk_label_wrap(ctx, "When the second point has been selected, a line will appear in the map panel showing the current position of the line.");
				nk_label_wrap(ctx, "An entry will appear in the List panel, along with any other lines that have been created.");
				nk_layout_row_dynamic(ctx, 45, 1);
				nk_label_wrap(ctx, "Right click can be used to cancel the placement of the starting point.");
			nk_layout_row_dynamic(ctx, 25, 1);
			nk_label(ctx, "- Thing -", NK_TEXT_LEFT);
				nk_layout_row_dynamic(ctx, 60, 1);
				nk_label_wrap(ctx, "Changes the cursor to a green dot while hovering in the map window. Click to select the position.");
				nk_label_wrap(ctx, "A popup will appear with options to give a name or continue with no name, or cancel the placement.");
			nk_layout_row_dynamic(ctx, 25, 1);
			nk_label(ctx, "- Sector	 -", NK_TEXT_LEFT);
				nk_layout_row_dynamic(ctx, 40, 1);
				nk_label_wrap(ctx, "Unused at this time of development");
			nk_layout_row_dynamic(ctx, 25, 1);
			nk_label(ctx, "- File -", NK_TEXT_LEFT);
				nk_layout_row_dynamic(ctx, 45, 1);
				nk_label_wrap(ctx, "A menu bar appears at the top left corner with the Menu and File options.");
				nk_layout_row_dynamic(ctx, 85, 1);
				nk_label_wrap(ctx, "The Menu tab is where you can view the info about this map maker, this very help screen, the option to hide the map grid, and to close the menu bar and revert to the Line tool.");
				nk_layout_row_dynamic(ctx, 110, 1);
				nk_label_wrap(ctx, "The File tab is where you can 'save' your current progress in the map editor, 'open' other maps that have been made with this program, start a 'new' map, 'export' your map as a finished WAD file, or 'exit' the program.");
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "List:", NK_TEXT_LEFT);
			nk_layout_row_dynamic(ctx, 80, 1);
			nk_label_wrap(ctx, "Just as it sounds, the list panel lists all items in the current map. There are two tabs which you can click to view items in each category.");
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_popup_end(ctx);
	} else show_help = nk_false;

}

void	my_callback(void *userdata, uint8_t *stream, int len);
static uint8_t	*audio_pos;
static uint32_t	audio_len;
void	test_audio(t_resource *resource)
{
	static uint8_t	*wav_buffer;
	static uint32_t	wav_length;
	static SDL_AudioSpec wav_spec;
	
	SDL_LoadWAV(resource->full_path, &wav_spec, &wav_buffer, &wav_length);
	// wav_spec = load_wav(resource);
	// wav_buffer = resource->raw_data + 44;
	// wav_length = resource->size - 44;
	wav_spec.callback = my_callback;
	wav_spec.userdata = NULL;
	audio_pos = wav_buffer;
	audio_len = wav_length;
	if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
		printf("Failed to open %s\n", SDL_GetError());
		return ;
	}
	SDL_PauseAudio(0);

	while (audio_len > 0)
		SDL_Delay(100);
	SDL_CloseAudio();
}
void	my_callback(void *userdata, uint8_t *stream, int len)
{
	if (audio_len == 0)
		return ;
	len = (len > audio_len ? audio_len : len);
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

void    draw_resources(t_map_interface *draw_mode)
{
	struct nk_context *ctx = draw_mode->ctx;
	struct nk_rect s = {draw_mode->win_w / 4, 100, 300, 400};
	if (nk_popup_begin(ctx, NK_POPUP_STATIC, "All resources", NK_WINDOW_CLOSABLE, s)) {
		char *resources[] = {"Floors", "Walls", "Sounds", "Music"};
		const int len = NK_LEN(resources);
		enum menu_state {MENU_NONE, MENU_FLOORS, MENU_WALLS, MENU_SOUNDS, MENU_MUSIC};
		static enum menu_state menu_state = MENU_NONE;
		enum nk_collapse_states state;
		
		static int index[NK_LEN(resources)];
		state = (menu_state == MENU_FLOORS) ? NK_MAXIMIZED : NK_MINIMIZED;
		if (nk_tree_state_push(ctx, NK_TREE_TAB, resources[0], &state)) {
			menu_state = MENU_FLOORS;
			nk_layout_row_static(ctx, 20, 45, 5);
			nk_label(ctx, "Page: ", NK_TEXT_LEFT);
			{
				char num[8];
				sprintf(num, "%d", index[0] + 1);
				nk_label(ctx, num, NK_TEXT_CENTERED);
			}
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT)) {
				if (index[0] > 0) --index[0];
			}
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT)) {
				if (index[0] < (MAX_RESOURCE_COUNT - 1) / 10) ++index[0];
			}
			nk_layout_row_dynamic(ctx, 25, 2);
			t_resource_table *resource = draw_mode->floor;
			for (int k = (index[0] * 10); k - (index[0] * 10) < 10; k++) {
				nk_label(ctx, resource->table[k].name, NK_TEXT_LEFT);
				if (nk_button_label(ctx, "preview")) {
					char command[64] = "open ";
					strcat(command, resource->table[k].full_path);
					system(command);
				}
			}
			nk_tree_pop(ctx);
		} else menu_state = (menu_state == MENU_FLOORS) ? MENU_NONE : menu_state;

		state = (menu_state == MENU_WALLS) ? NK_MAXIMIZED : NK_MINIMIZED;
		if (nk_tree_state_push(ctx, NK_TREE_TAB, resources[1], &state)) {
			menu_state = MENU_WALLS;
			nk_layout_row_static(ctx, 20, 45, 5);
			nk_label(ctx, "Page: ", NK_TEXT_LEFT);
			{
				char num[8];
				sprintf(num, "%d", index[1] + 1);
				nk_label(ctx, num, NK_TEXT_CENTERED);
			}
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT)) {
				if (index[1] > 0) --index[1];
			}
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT)) {
				if (index[1] < (MAX_RESOURCE_COUNT - 1) / 10) ++index[1];
			}
			nk_layout_row_dynamic(ctx, 25, 2);
			t_resource_table *resource = draw_mode->wall;
			for (int k = (index[1] * 10); k - (index[1] * 10) < 10; k++) {
				nk_label(ctx, resource->table[k].name, NK_TEXT_LEFT);
				if (nk_button_label(ctx, "preview")) {
					char command[64] = "open ";
					strcat(command, resource->table[k].full_path);
					system(command);
				}
			}
			nk_tree_pop(ctx);
		} else menu_state = (menu_state == MENU_WALLS) ? MENU_NONE : menu_state;
			
		state = (menu_state == MENU_SOUNDS) ? NK_MAXIMIZED : NK_MINIMIZED;
		if (nk_tree_state_push(ctx, NK_TREE_TAB, resources[2], &state)) {
			menu_state = MENU_SOUNDS;
			nk_layout_row_static(ctx, 20, 45, 5);
			nk_label(ctx, "Page: ", NK_TEXT_LEFT);
			{
				char num[8];
				sprintf(num, "%d", index[2] + 1);
				nk_label(ctx, num, NK_TEXT_CENTERED);
			}
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT)) {
				if (index[2] > 0) --index[2];
			}
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT)) {
				if (index[2] < (MAX_RESOURCE_COUNT - 1) / 10) ++index[2];
			}
			nk_layout_row_dynamic(ctx, 25, 2);
			t_resource_table *resource = draw_mode->sounds;
			for (int k = (index[2] * 10); k - (index[2] * 10) < 10; k++) {
				nk_label(ctx, resource->table[k].name, NK_TEXT_LEFT);
				if (nk_button_label(ctx, "preview")) {
					// display audio file in bytes skipping the header
					// write(1, resource->table[k].raw_data + 44, resource->table[k].size - 44);
					// write(1, "\n\n", 2);
					test_audio(&resource->table[k]);
					// char command[64] = "afplay ";
					// strcat(command, resource->table[k].full_path);
					// system(command);
				}
			}
			nk_tree_pop(ctx);
		} else menu_state = (menu_state == MENU_SOUNDS) ? MENU_NONE : menu_state;
			
		state = (menu_state == MENU_MUSIC) ? NK_MAXIMIZED : NK_MINIMIZED;
		if (nk_tree_state_push(ctx, NK_TREE_TAB, resources[3], &state)) {
			menu_state = MENU_MUSIC;
			nk_layout_row_static(ctx, 20, 45, 5);
			nk_label(ctx, "Page: ", NK_TEXT_LEFT);
			{
				char num[8];
				sprintf(num, "%d", index[3] + 1);
				nk_label(ctx, num, NK_TEXT_CENTERED);
			}
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT)) {
				if (index[3] > 0) --index[3];
			}
			if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT)) {
				if (index[3] < (MAX_RESOURCE_COUNT - 1) / 10) ++index[3];
			}
			nk_layout_row_dynamic(ctx, 25, 2);
			t_resource_table *resource = draw_mode->music;
			for (int k = (index[3] * 10); k - (index[3] * 10) < 10; k++) {
				nk_label(ctx, resource->table[k].name, NK_TEXT_LEFT);
				if (nk_button_label(ctx, "preview")) {
					char command[64] = "afplay ";
					strcat(command, resource->table[k].full_path);
					system(command);
				}
			}
			nk_tree_pop(ctx);
		} else menu_state = (menu_state == MENU_MUSIC) ? MENU_NONE : menu_state;
		nk_popup_end(ctx);
	} else show_resources = nk_false;
}

void    draw_menu(t_map_interface *draw_mode)
{
	struct nk_context *ctx = draw_mode->ctx;
	nk_menubar_begin(ctx);
	nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
	nk_layout_row_push(ctx, 45);
	nk_window_set_focus(ctx, "MENU");
	if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(120, 200)))
	{
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT))
			show_about = nk_true;
		if (nk_menu_item_label(ctx, "Help", NK_TEXT_LEFT))
			show_help = nk_true;
		nk_checkbox_label(ctx, "Show grid", &show_grid);
		if (nk_menu_item_label(ctx, "Hide menu", NK_TEXT_LEFT)) {
			draw_mode->tool_op = LINE;
			count = -1;
		}
		if (nk_menu_item_label(ctx, "Show resources", NK_TEXT_LEFT)) {
			show_resources = nk_true;
		}
		nk_menu_end(ctx);
	}
	nk_layout_row_push(ctx, 60);
	if (nk_menu_begin_label(ctx, "FILE", NK_TEXT_LEFT,
	nk_vec2(120, 200)))
	{
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_menu_item_label(ctx, "New", NK_TEXT_LEFT)) {
			save_as = nk_false;
			open_map = nk_false;
			/* do you want to save? */
			if (check_exists(draw_mode->map_name, MAP) < 1)
				printf("I hope you saved your work\n");
			// clear bank
			bzero(draw_mode->bank, sizeof(*draw_mode->bank));
			// clear name
			memset(draw_mode->map_name, 0, sizeof(draw_mode->map_name));
			// clear sectors
			for (int i = 0; i < SECTOR_MAX; i++) {
				if (draw_mode->sectors->sectors[i].sector_num) {
					free(draw_mode->sectors->sectors[i].sector_lines);
					bzero(&draw_mode->sectors->sectors[i], sizeof(draw_mode->sectors->sectors[i]));
				}
			}
			draw_mode->sectors->selected = 1;
		}
		/* load in a file that was previously saved */
		if (nk_menu_item_label(ctx, "Open", NK_TEXT_LEFT)) {
			save_as = nk_false;
			open_map = nk_true;
		}
		if (nk_menu_item_label(ctx, "Save", NK_TEXT_LEFT)) {
			open_map = nk_false;
			/* create new file name */
			if (check_exists(draw_mode->map_name, MAP) < 0)
				save_as = nk_true;
			/* save bank contents into a file */
			if (!save_as) {
				if (!save(draw_mode))
					printf("Saving to %s\n", draw_mode->map_name);
			}
		}
		if (nk_menu_item_label(ctx, "Export", NK_TEXT_LEFT)) {
			/* export map as WAD */
				export_wad(draw_mode);
		}
		if (nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT)) {
			if (check_exists(draw_mode->map_name, MAP))
				printf("Exit program without saving\n... Sorry :(\n");
			exit(0);
		}
		nk_menu_end(ctx);
	}
	nk_menubar_end(ctx);
}

int     check_exists(const char *name, int type)
{
	if (!name[0]) return (-1);
	DIR	*dir;
	if (type == MAP)
		dir = opendir(MAP_SAVE_PATH);
	else if (type == WAD)
		dir = opendir(WAD_EXPORT_PATH);
	struct dirent *sd;
	while (dir && (sd = readdir(dir))) {
		if (type == WAD) strcat(sd->d_name, ".wad");
		if (!strcmp(name, sd->d_name)) {
			closedir(dir);
			return (1);
		}
	}
	if (dir) closedir(dir);
	return (0);
}
