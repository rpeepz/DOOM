#include "demo.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
// # define NK_IMPLEMENTATION
// # define NK_SDL_GL3_IMPLEMENTATION
#include "../Nuklear/nuklear.h"
#include "nuklear_sdl_gl3.h"

typedef struct  s_map_interface
{
	int active;
    int started_line;
    int ended_line;
    int *tool_op;
    struct nk_rect  *size;
    struct nk_command_buffer *canvas;
    const struct nk_input   *in;
}               t_map_interface;

void    map_pannel(struct nk_context *ctx, void *p)
{
    t_map_interface *m = p;
    int *active = &m->active;
    int *tool_op = m->tool_op;
    int *started_line = &m->started_line;
    int *ended_line = &m->ended_line;
    struct nk_command_buffer *canvas = m->canvas;
    struct nk_rect *size = m->size;
    const struct nk_input *in = m->in;

    if (nk_begin(ctx, "Map Maker", nk_rect(400, 10, 800, 600),
        NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE))
    {
        canvas = nk_window_get_canvas(ctx);
        *size = nk_window_get_content_region(ctx);
        
        //nk_layout_space_begin(ctx, NK_STATIC, total_space.h, nodedit->node_count);
        /* display grid */
        /* struct nk_rect size = nk_layout_space_bounds(ctx); */
        float x, y;
        const float grid_size = 32.0f;
        const struct nk_color grid_color = nk_rgb(50, 50, 50);
        for (x = (float)fmod(size->x, grid_size); x < size->w; x += grid_size)
            nk_stroke_line(canvas, x+size->x, size->y, x+size->x, size->y+size->h, 1.0f, grid_color);
        for (y = (float)fmod(size->y, grid_size); y < size->h; y += grid_size)
            nk_stroke_line(canvas, size->x, y+size->y, size->x+size->w, y+size->y, 1.0f, grid_color);
        // Draw lines //
        struct nk_vec2 line_start;
        struct nk_vec2 line_end;
        struct nk_rect circle1;
        struct nk_rect circle2;
        circle1.w = 8; circle1.h = 8;
        circle2.w = 8; circle2.h = 8;
        
        if (!*active && nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_window_get_bounds(ctx)))
            *active = nk_true;
        else
            *active = nk_false;
        
        if (*tool_op == LINE) {
            int fill_point1 = 0;
            int fill_point2 = 0;
            if (!*started_line && nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_window_get_bounds(ctx)))
            {
                *started_line = 1;
                line_start = in->mouse.pos; //set coordinates for beginning of line
                printf("point 1\n\tx = %f\n\ty = %f\n",in->mouse.pos.x, in->mouse.pos.y);
                circle1.x = line_start.x;
                circle1.y = line_start.y;
            }
            if (*started_line)
            {
                nk_fill_circle(canvas, circle1, nk_rgb(100, 100, 100)); //place cirlce on line start
            }
            if (*started_line && nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_window_get_bounds(ctx)))
            {
                *ended_line = 1;
                line_end = in->mouse.pos; //set coordinates for end of line
                printf("point 2\n\tx = %f\n\ty = %f\n",in->mouse.pos.x, in->mouse.pos.y);
                circle2.x = line_end.x;
                circle2.y = line_end.y;
            }
            if (*ended_line)
            {
                nk_fill_circle(canvas, circle2, nk_rgb(100, 100, 100)); //place cirlce on line end
                nk_stroke_line(canvas, line_start.x, line_start.y, line_end.x, line_end.y, 1.0f, nk_rgb(10,10,0));
            }
        }
    }
    //         //     //reset line points
    //         //     line_start = (struct nk_vec2){0};
    //         //     line_end = (struct nk_vec2){0};
    //         //     ended_line = 0;
    nk_end(ctx);
}
