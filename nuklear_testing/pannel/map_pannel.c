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

void    draw_lines(t_map_interface *draw_mode);
void    draw_things(t_map_interface *draw_mode);

void    map_pannel(t_map_interface *draw_mode)
{
    struct nk_rect size;

    /* pannel size nk_rect(5, 5, 1200, 800) */
    size = nk_rect(WINDOW_OFFSET, WINDOW_OFFSET,
    WINDOW_WIDTH - (WINDOW_WIDTH / 4),
    WINDOW_HEIGHT - (WINDOW_OFFSET * 2));

    if (nk_begin(draw_mode->ctx, "Map Maker", size,
		NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MINIMIZABLE))
		{
			canvas = nk_window_get_canvas(draw_mode->ctx);
			size = nk_window_get_content_region(draw_mode->ctx);

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
            for (t_item_node *temp = draw_mode->bank->head_line; temp; temp = temp->next)
                stroke_my_line(canvas, temp);
            // for (t_item_node *temp = draw_mode->bank->head_thing; temp; temp = temp->next)
            //     stroke_box(canvas, temp);
		}
		nk_end(draw_mode->ctx);
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
            nk_fill_circle(canvas, end_point, nk_rgb(255, 0, 225));
        }
        if (count == 2) {
            add_line(draw_mode->bank, line_start, line_end);
            count = 0;
        }
    }
}

void    draw_things(t_map_interface *draw_mode)
{
    struct nk_vec2 thing_pos;
    struct nk_rect s = nk_rect(
    (WINDOW_WIDTH / 2) - 150, (WINDOW_HEIGHT / 2) - 100, 300, 120);

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
                static char buffer2[16];
                char buffer[16];
                int len = snprintf(buffer, 16, "%s", buffer2);
                nk_layout_row_dynamic(draw_mode->ctx, 25, 1);
                nk_label(draw_mode->ctx, "Name Thing", NK_TEXT_CENTERED);
                nk_edit_string(draw_mode->ctx, NK_EDIT_SIMPLE, buffer, &len, 16, nk_filter_ascii);
                buffer[len] = 0;
                memcpy(buffer2, buffer, strlen(buffer));
                nk_layout_row_dynamic(draw_mode->ctx, 10, 1);
                nk_label(draw_mode->ctx, " ", 1);
                nk_layout_row_dynamic(draw_mode->ctx, 25, 2);
                if (nk_button_label(draw_mode->ctx, "OK")) {
                    add_thing(draw_mode->bank, thing_pos, buffer2);
                    memset(buffer2, 0, 16);
                    count2 = 0;
                }
                if (nk_button_label(draw_mode->ctx, "No Name")) {
                    add_thing(draw_mode->bank, thing_pos, "");
                    memset(buffer2, 0, 16);
                    count2 = 0;
                }
                nk_popup_end(draw_mode->ctx);
            }
        }
    }
}
