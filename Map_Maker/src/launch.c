#include "demo.h"

#include "list.h"
// #define NK_INCLUDE_FIXED_TYPES
// #define NK_INCLUDE_STANDARD_IO
// #define NK_INCLUDE_STANDARD_VARARGS
// #define NK_INCLUDE_DEFAULT_ALLOCATOR
// #define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
// #define NK_INCLUDE_FONT_BAKING
// #define NK_INCLUDE_DEFAULT_FONT
// #define NK_IMPLEMENTATION
// #define NK_SDL_GL3_IMPLEMENTATION
#include "../../Nuklear/nuklear.h"
#include "nuklear_sdl_gl3.h"
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define LAUNCH_W 250
#define LAUNCH_H 450

void    launch_info(struct nk_context *ctx);
void    get_resolution(struct nk_context *ctx, int *window_width, int *window_height);

int     running = 1;
void    define_window_size(int *window_width, int *window_height)
{
       /* Platform */
    SDL_Window *win;
    SDL_GLContext glContext;
    int win_width, win_height;
    struct nk_colorf bg;
    
    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    win = SDL_CreateWindow("Launch",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LAUNCH_W, LAUNCH_H, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
    glContext = SDL_GL_CreateContext(win);
    SDL_GetWindowSize(win, &win_width, &win_height);
    /* OpenGL setup */
    glViewport(0, 0, LAUNCH_W, LAUNCH_H);
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

    bg.r = 0.18f, bg.g = 0.18f, bg.b = 0.18f, bg.a = 1.0f;

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

        launch_info(ctx);
        get_resolution(ctx, window_width, window_height);

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
    return ;
}

void    launch_info(struct nk_context *ctx)
{
    struct nk_rect size = nk_rect(WINDOW_OFFSET, WINDOW_OFFSET,
    LAUNCH_W - (WINDOW_OFFSET * 2), LAUNCH_H / 4);
    if (nk_begin(ctx, "launch", size, NK_WINDOW_BORDER | NK_WINDOW_NOT_INTERACTIVE))
    {
        nk_layout_row_dynamic(ctx, 40, 1);
        nk_label_wrap(ctx, "Welcome to the Map Maker for our Doom-Nukem project at 42.");
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_label_wrap(ctx, "Choose a resolution to open the program");
    }
    nk_end(ctx);
}

void    get_resolution(struct nk_context *ctx, int *window_width, int *window_height)
{
    static int current_size = 0;
    static const char *sizes[] = {
        "Default", "1920 x 1080", "1600 x 900", "1280 x 720", "1024 x 576"
    };
    struct nk_rect size = nk_rect(WINDOW_OFFSET, (LAUNCH_H / 4) + (WINDOW_OFFSET * 2),
    LAUNCH_W - (WINDOW_OFFSET * 2), LAUNCH_H - (LAUNCH_H * 0.1667) - ((LAUNCH_H / 4) + (WINDOW_OFFSET * 3)));
    if (nk_begin(ctx, "resulution", size, NK_WINDOW_BORDER))
    {
        struct nk_vec2 dropdown = nk_vec2(LAUNCH_W - (WINDOW_OFFSET * 20), LAUNCH_H * 0.45);
        /* resolution combobox */
        nk_layout_row_static(ctx, 25, LAUNCH_W - (WINDOW_OFFSET * 10), 1);
        current_size = nk_combo(ctx, sizes, NK_LEN(sizes), current_size, 30, dropdown);

    }
    nk_end(ctx);
    size.y = LAUNCH_H - (LAUNCH_H * 0.1667);
    size.h = (LAUNCH_H * 0.1667) - WINDOW_OFFSET;
    if (nk_begin(ctx, "start", size, NK_WINDOW_BORDER))
    {
        nk_layout_row_dynamic(ctx, 7, 1);
        nk_label(ctx, " ", 1);
        nk_layout_row_dynamic(ctx, 35, 2);
        if (nk_button_label(ctx, "OK"))
        {
            running = 0;
            switch (current_size)
            {
                case 0:
                    break ;
                case 1:
                    *window_width = 1920;
                    *window_height = 1080;
                    break ;
                case 2:
                    *window_width = 1600;
                    *window_height = 1080;
                    break ;
                case 3:
                    *window_width = 1280;
                    *window_height = 720;
                    break ;
                case 4:
                    *window_width = 1024;
                    *window_height = 576;
                    break ;
                default:
                    dprintf(2, "NON ERROR\n");
                    break ;
            }
        }
        if (nk_button_label(ctx, "Cancel"))
            exit(0);
    }
    nk_end(ctx);
}
