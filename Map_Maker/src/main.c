/* nuklear - 1.32.0 - public domain */
#include "demo.h"

#include "list.h"
#include "sector.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "../../Nuklear/nuklear.h"
#include "nuklear_sdl_gl3.h"
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define INCLUDE_STYLE
#include "../../Nuklear/demo/style.c"

t_map_interface draw_mode;
t_bank bank = {0};
t_sectors sectors = {0};
int launch_help;

void    hooks(void)
{
	// if (nk_input_is_key_pressed(&draw_mode.ctx->input, NK_KEY_DEL))
	//     remove_from_bank(&bank, draw_mode.list_op);
	if (nk_input_is_key_pressed(&draw_mode.ctx->input, NK_KEY_UP))
		change_selected(&bank, draw_mode.list_op, 1);
	if (nk_input_is_key_pressed(&draw_mode.ctx->input, NK_KEY_DOWN))
		change_selected(&bank, draw_mode.list_op, 0);

}
// #include "../../Nuklear/demo/overview.c"
void    panels(void)
{
	// overview(draw_mode.ctx);
	tool_panel(&draw_mode);
	map_panel(&draw_mode, &launch_help);
	list_panel(&draw_mode);

	if (draw_mode.tool_op == EDIT)
		edit_panel(&draw_mode);
	if (draw_mode.tool_op == SECTOR)
		sector_panel(&draw_mode);
	else
	{
		for (t_item_node *list = draw_mode.bank->head_line; list; list = list->next)
			if (list != draw_mode.bank->selected)
				list->color = LINE_COLOR;
	}

}

int     main(void)
{
	//default window size
	int window_width = WINDOW_WIDTH, window_height = WINDOW_HEIGHT;
	/* open launcher to chose window size */
	launch_help = define_window_size(&window_width, &window_height);
	/* Platform */
	SDL_Window *win;
	SDL_GLContext glContext;
	int win_width, win_height;
	int running = 1;
	struct nk_colorf bg;
	
	/* SDL setup */
	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS|SDL_INIT_AUDIO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	win = SDL_CreateWindow("Map Maker",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
	glContext = SDL_GL_CreateContext(win);
	SDL_GetWindowSize(win, &win_width, &win_height);
	/* OpenGL setup */
	glViewport(0, 0, window_width, window_height);
	glewExperimental = 1;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to setup GLEW\n");
		exit(1);
	}
	struct nk_context *ctx = nk_sdl_init(win);
	/* Load Fonts: if none of these are loaded a default font will be used  */
	/* Load Cursor: if you uncomment cursor loading please hide the cursor */
	struct nk_font_atlas *atlas;
	nk_sdl_font_stash_begin(&atlas);
	nk_sdl_font_stash_end();

	#ifdef INCLUDE_STYLE
		set_style(ctx, 0);
	#endif

	bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

	draw_mode.ctx = ctx;
	draw_mode.bank = &bank;
	draw_mode.sectors = &sectors;
	draw_mode.floor = gather_assets(0);
	draw_mode.wall = gather_assets(1);
	draw_mode.sounds = gather_assets(2);
	draw_mode.music = gather_assets(3);
	if (!draw_mode.floor || !draw_mode.wall || !draw_mode.sounds || !draw_mode.music)
		exit(-1);
	draw_mode.win_w = window_width;
	draw_mode.win_h = window_height;
	draw_mode.tool_op = launch_help ? NK_FILE : LINE;// Tools panel selected tool
	draw_mode.list_op = ITEM_LINE;

	struct nk_style_toggle *box = &ctx->style.checkbox;
	box->border = 1.0f;
	box->border_color = nk_rgb(80, 80, 80);
    // full box when 0
	/* no hover */
	box->normal = nk_style_item_color(nk_rgb(80, 80, 80));
	/* hover */
	box->hover = nk_style_item_color(nk_rgb(150, 80, 80));

    // inside of box when 1
	/* no hover */
	box->cursor_normal = nk_style_item_color(nk_rgb(140, 140, 140));
	/* hover */
	box->cursor_hover = nk_style_item_color(nk_rgb(140, 140, 140));

	const struct nk_input *in = &ctx->input;

	while (running)
	{
		/* Input */
		SDL_Event evt;
		nk_input_begin(ctx);
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) goto cleanup;
			nk_sdl_handle_event(&evt);
		} nk_input_end(ctx);

		hooks();
		panels();

		/* Draw */
		SDL_GetWindowSize(win, &win_width, &win_height);
		glViewport(0, 0, win_width, win_height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(bg.r, bg.g, bg.b, bg.a);
		/* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
		 * with blending, scissor, face culling, depth test and viewport and
		 * defaults everything back into a default state.
		 * Make sure to either a.) save and restore or b.) reset your own state after
		 * rendering the UI. */
		nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
		SDL_GL_SwapWindow(win);
	}

cleanup:
	nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
