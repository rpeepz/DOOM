/* nuklear - 1.32.0 - public domain */
#include "demo.h"

#include "list.h"
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

t_map_interface draw_mode;
t_line_bank linebank;
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

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    int tool_op = MOVE;     //Tools pannel selected tool
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

		// map_pannel(ctx, (void*)&draw_mode);

//Map pannel
		if (nk_begin(ctx, "Map Maker", nk_rect(400, 10, 800, 600),
				NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MINIMIZABLE|NK_WINDOW_MOVABLE))
		{
			canvas = nk_window_get_canvas(ctx);
			size = nk_window_get_content_region(ctx);
			
			float x, y;
			const float grid_size = 32.0f;
			const struct nk_color grid_color = nk_rgb(50, 50, 50);
			for (x = (float)fmod(size.x, grid_size); x < size.w; x += grid_size)
				nk_stroke_line(canvas, x+size.x, size.y, x+size.x, size.y+size.h, 1.0f, grid_color);
			for (y = (float)fmod(size.y, grid_size); y < size.h; y += grid_size)
				nk_stroke_line(canvas, size.x, y+size.y, size.x+size.w, y+size.y, 1.0f, grid_color);

			// Draw lines //
			// if lines don't draw try removing the initialization
			struct nk_vec2 line_start ;
			struct nk_vec2 line_end ;
			static struct nk_rect circle1 = { .w = 4, .h = 4};
			static struct nk_rect circle2 = { .w = 4, .h = 4};
			struct nk_rect circle3 = { .w = 4, .h = 4};
			static int count = 0;

            if (*(draw_mode.tool_op) == LINE) {

				//SDL_ShowCursor(SDL_DISABLE);
			
				if (nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_window_get_bounds(ctx))){
						count++;
				}
                if (count == 0 )
                {
                    line_start = in->mouse.pos; //set coordinates for beginning of line
                    circle1.x = line_start.x;
                    circle1.y = line_start.y;
                }
				if (count >= 0)
                    nk_fill_circle(canvas, circle1, nk_rgb(255, 0, 0)); //place cirlce on line start

				if ( count == 1 )
				{
					line_end = in->mouse.pos;
					circle2.x = line_end.x;
                    circle2.y = line_end.y;
                    nk_fill_circle(canvas, circle2, nk_rgb(255, 0, 0)); //place cirlce on line start
				}
				if (count == 2){
					add_line( &linebank, line_start, line_end);
					count = 0;
				}
				// Draw circles on top on the vertexs 
				}
				t_line_node *temp;
				temp = linebank.head;

				for( int i = 0; i < linebank.count; ++i){
					stroke_my_line(canvas, temp);
					circle3.x = temp->line.start_vertex.x;	
					circle3.y = temp->line.start_vertex.y;	
					nk_fill_circle(canvas, circle3 , nk_rgb(100, 100, 100)); //place cirlce on line start
					circle3.x = temp->line.end_vertex.x;	
					circle3.y = temp->line.end_vertex.y - 5;	
					nk_fill_circle(canvas, circle3, nk_rgb(100, 100, 100)); //place cirlce on line start
					temp = temp->next;
				}
		}
		nk_end(ctx);

		list_pannel(ctx, &linebank);
        
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
