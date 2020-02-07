/* nuklear - 1.32.0 - public domain */
#include "demo.h"

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

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

<<<<<<< HEAD
//

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
=======
t_map_interface draw_mode;
>>>>>>> dev-rpapagna

// Use lines for now but switch to a doubly linked list to make deletion easier. Im still curious with why ID put lines in an array in DoomEd.
t_line lines[10000];
int    lines_filled = 0;
float  value = 1.0f;


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

    int tool_op = 0;     //Tools pannel selected tool

	// NUM_TOOLS should remain at the end of the enum to represent
	// the number of tools present -smaddox
    enum tools {SELECT, LINE, THING, MOVE, SECTOR, SOME_SHIT, NUM_TOOLS};

    draw_mode.started_line = 0;
    draw_mode.ended_line = 0;
    draw_mode.tool_op = &tool_op;

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
        draw_mode.size = &size;
        draw_mode.canvas = canvas;
        draw_mode.in = in;

        tool_pannel(ctx, (void*)&tool_op);

		map_pannel(ctx, (void*)&draw_mode);

        // testing a display box
        lines_filled = 2;
        if (nk_begin(ctx, "line list", nk_rect(50, 50, 200, 400), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE))
        {
            for (int lf = 0; lf < lines_filled; lf++){
                lines[lf].start.x = 40.1 + ((float)lf * .1f);
                lines[lf].start.y = 40.2 + ((float)lf * .1f);
            }
            for (int zx = 0; zx < lines_filled; zx++) {
                char buffer[16];
                nk_layout_row_dynamic(ctx, 30, 2);
                snprintf(buffer, 16, "%d", zx+1);
                nk_label(ctx, buffer, NK_TEXT_LEFT);
                snprintf(buffer, 16, "%.2f, %.2f", lines[zx].start.x, lines[zx].start.y);
                nk_label(ctx, buffer, NK_TEXT_RIGHT);
            }

            //  testing slider display value
            nk_layout_row_dynamic(ctx, 30, 1);
            {int len; char buffer[64];
            len = snprintf(buffer, 64, "%.2f", value);
            nk_edit_string(ctx, NK_EDIT_READ_ONLY, buffer, &len, 63, nk_filter_float);
            buffer[len] = 0;
            value = atof(buffer);}

            nk_slider_float(ctx, 100, &value, 0.1f, 0.1f);
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
