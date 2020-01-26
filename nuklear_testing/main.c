/* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "../Nuklear/nuklear.h"
#include "nuklear_sdl_gl3.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

/* #define INCLUDE_NODE_EDITOR */

#ifdef INCLUDE_NODE_EDITOR
  #include "../node_editor.c"
#endif

typedef struct		s_vec2f
{
	float			x;
	float			y;
}					t_vec2f;
typedef struct s_line {
	t_vec2f start;
	t_vec2f end;
}				t_line;
typedef struct s_sector {
	t_line *sector_lines;
}				t_sector;
typedef struct	s_seg{
	t_line *ptr_to_line_segmenting;
	int x;
	int y;
}				t_seg;
typedef struct	s_subsec{
	// Im not sure what a subsector consists of besides segments
	t_seg *subsec_segments;
}				t_subsec;
typedef enum e_mode{Line_mode, Sector_mode, Selecting_mode, Vertex_mode} t_mode;

struct s_drawing_mode {
	int active;
	t_mode mode;
};
struct s_drawing_mode draw_mode;
// Use lines for now but switch to a doubly linked list to make deletion easier. Im still curious with why ID put lines in an array in DoomEd.
t_line lines[10000];
int main(void)
{
    /* Platform */
    SDL_Window *win;
    SDL_GLContext glContext;
    int win_width, win_height;
    int running = 1;
    /* GUI */
    struct nk_context *ctx;
    struct nk_colorf bg;
    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    win = SDL_CreateWindow("Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
    glContext = SDL_GL_CreateContext(win);
    SDL_GetWindowSize(win, &win_width, &win_height);
    /* OpenGL setup */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }
    ctx = nk_sdl_init(win);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
    /* style.c */
    #ifdef INCLUDE_STYLE
    /*set_style(ctx, THEME_DARK);*/
    #endif
//    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
/* int started_line = 0; */
/* int ended_line = 0; */
int printOnce = 0;
	int drawing_line = 0;

    while (running)
    {
        /* Input */
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&evt);
        } nk_input_end(ctx);
        /* GUI */
		struct nk_rect size;
		struct nk_command_buffer *canvas;
		const struct nk_input *in = &ctx->input;
		if (nk_begin(ctx, "Mode Selection", nk_rect(0, 400, 300, 100 ), NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE))
		{
			enum {LINE, SELECTION};
            static int op = LINE;
            static int property = 20;
			nk_layout_row_static(ctx, 30, 80, 1);
			if (nk_option_label(ctx, "line", op == LINE)) op = LINE;
			if (nk_option_label(ctx, "selection", op == SELECTION)) op = SELECTION;
		}
		nk_end(ctx);
		if (nk_begin(ctx, "Map Maker", nk_rect(0, 0, 800, 600),
				NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE))
		{
			canvas = nk_window_get_canvas(ctx);
			size = nk_window_get_content_region(ctx);
			if (!printOnce){
				printf("size.x: %f, size.y: %f, size.w: %f, size.h: %f\n\n", size.x, size.y, size.w, size.h);
				printOnce = 1;
			}
			//nk_layout_space_begin(ctx, NK_STATIC, total_space.h, nodedit->node_count);
			/* display grid */
			/* struct nk_rect size = nk_layout_space_bounds(ctx); */
			float x, y;
			const float grid_size = 32.0f;
			const struct nk_color grid_color = nk_rgb(50, 50, 50);
			for (x = (float)fmod(size.x, grid_size); x < size.w; x += grid_size)
				nk_stroke_line(canvas, x+size.x, size.y, x+size.x, size.y+size.h, 1.0f, grid_color);
			for (y = (float)fmod(size.y, grid_size); y < size.h; y += grid_size)
				nk_stroke_line(canvas, size.x, y+size.y, size.x+size.w, y+size.y, 1.0f, grid_color);
			// Draw lines //
			struct nk_vec2 line_start;
			struct nk_vec2 line_end;
			struct nk_rect circle1;
			struct nk_rect circle2;
			circle1.w = 8; circle1.h = 8;
			circle2.w = 8; circle2.h = 8;
			if (!draw_mode.active && nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_window_get_bounds(ctx)))
			{
				draw_mode.active = nk_true;
			}
			//
			/*
			if (!started_line && nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_window_get_bounds(ctx)))
			{
				started_line = 1;
				line_start = in->mouse.pos;
				circle1.x = line_start.x;
				circle1.y = line_start.y;
			}
			if (started_line)
				nk_fill_circle(canvas, circle1, nk_rgb(100, 100, 100));
			if (started_line && nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_window_get_bounds(ctx)))
			{
				ended_line = 1;
				line_end = in->mouse.pos;
				circle2.x = line_end.x;
				circle2.y = line_end.y;
			}
			if (ended_line)
			{
				nk_fill_circle(canvas, circle2, nk_rgb(100, 100, 100));
				nk_stroke_line(canvas, line_start.x, line_start.y, line_end.x, line_end.y, 1.0f, nk_rgb(10,10,0));
			}
			*/
		}
		nk_end(ctx);
        /* -------------- EXAMPLES ---------------- */
        #ifdef INCLUDE_CALCULATOR
          calculator(ctx);
        #endif
        #ifdef INCLUDE_OVERVIEW
          overview(ctx);
        #endif
        #ifdef INCLUDE_NODE_EDITOR
          node_editor(ctx);
        #endif
        /* ----------------------------------------- */

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

