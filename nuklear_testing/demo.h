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
# include "../Nuklear/nuklear.h"
# include "nuklear_sdl_gl3.h"

typedef struct		s_vec2f
{
	float			x;
	float			y;
}					t_vec2f;

typedef struct	s_line
{
	t_vec2f 	start;
	t_vec2f		end;
}				t_line;

typedef struct 	s_sector
{
	t_line 		*sector_lines;
}				t_sector;

typedef struct	s_seg
{
	t_line 		*ptr_to_line_segmenting;
	int 		x;
	int 		y;
}				t_seg;

typedef struct	s_subsec
{
	// Im not sure what a subsector consists of besides segments
	t_seg 		*subsec_segments;
}				t_subsec;

typedef enum    e_mode
{
	LINE,
	THING,
	SECTOR,
    EDIT,
	MOVE
}               t_mode;

typedef struct  s_map_interface
{
    struct nk_context *ctx;
	struct s_line_bank *linebank;
    int 	tool_op;
}               t_map_interface;

void    map_pannel(struct nk_context *ctx, t_map_interface *draw_mode);
void    tool_pannel(struct nk_context *ctx, int *tool_op);
void    calculator(struct nk_context *ctx);

#endif
