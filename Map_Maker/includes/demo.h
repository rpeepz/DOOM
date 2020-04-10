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

// WAVE file header format
typedef struct HEADER_WAV {
	unsigned char riff[4];						// RIFF string
	unsigned int overall_size	;				// overall size of file in bytes
	unsigned char wave[4];						// WAVE string
	unsigned char fmt_chunk_marker[4];			// fmt string with trailing null char
	unsigned int length_of_fmt;					// length of the format data
	unsigned int format_type;					// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	unsigned int channels;						// no.of channels
	unsigned int sample_rate;					// sampling rate (blocks per second)
	unsigned int byterate;						// SampleRate * NumChannels * BitsPerSample/8
	unsigned int block_align;					// NumChannels * BitsPerSample/8
	unsigned int bits_per_sample;				// bits per sample, 8- 8bits, 16- 16 bits etc
	unsigned char data_chunk_header [4];		// DATA string or FLLR string
	unsigned int data_size;						// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
} WAV_HEADER;

int	load_wav(t_resource *sound);

#endif
