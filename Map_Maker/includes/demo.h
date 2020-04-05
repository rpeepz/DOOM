#ifndef DEMO_H
# define DEMO_H

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdarg.h>
# include <string.h>
# include <math.h>
# include <assert.h>
# include <math.h>
# include <limits.h>
# include <time.h>

# include <GL/glew.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_opengl.h>

# define NK_INCLUDE_FIXED_TYPES
# define NK_INCLUDE_STANDARD_IO
# define NK_INCLUDE_STANDARD_VARARGS
# define NK_INCLUDE_DEFAULT_ALLOCATOR
# define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
# define NK_INCLUDE_FONT_BAKING
# define NK_INCLUDE_DEFAULT_FONT
# include "../../Nuklear/nuklear.h"
# include "nuklear_sdl_gl3.h"
# include "resource.h"
# include "lump.h"

# define WINDOW_WIDTH 1600
# define WINDOW_HEIGHT 900
# define WINDOW_OFFSET 5
# define HIGHLIGHT nk_rgb(255, 140, 60)
# define LINE_COLOR nk_rgb(10, 10, 10)
# define THING_COLOR nk_rgb(145, 145, 145)
# define BUTTON_DEFAULT nk_rgb(50, 50, 50)
# define MAP_SAVE_PATH "maps/"
# define WAD_EXPORT_PATH "wads/"

typedef struct	s_vec2f
{
	float		x;
	float		y;
}				t_vec2f;

typedef struct	s_line
{
	t_vec2f		start;
	t_vec2f		end;
}				t_line;

/* current selected tool from tool panel

*/
typedef enum	e_mode
{
	LINE,
	THING,
	SECTOR,
	EDIT,
	MOVE,
	NK_FILE
}				t_mode;

typedef struct			s_map_interface
{
	struct nk_context	*ctx;
	struct s_bank		*bank;
	struct s_sectors	*sectors;
	t_resource_table	*floor;
	t_resource_table	*wall;
	t_resource_table	*sounds;
	t_resource_table	*music;
	int					win_w;
	int					win_h;
	int					tool_op;
	int					list_op;
	char				map_name[20];
}						t_map_interface;

int					define_window_size(int *window_width, int *window_height);
void				map_panel(t_map_interface *draw_mode, int *launch_help);
void				tool_panel(t_map_interface *draw_mode);
t_resource_table    *gather_assets(int mode);

#endif
