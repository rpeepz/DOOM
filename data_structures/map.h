/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 17:44:46 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/19 18:41:25 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# define MAX_LINE_COUNT 4096
# define MAX_THING_COUNT 1000

/*	line data */

typedef enum { LINE_BLOCKPLAYER, LINE_BLOCKMONSTER, LINE_TWOSIDES, \
	LINE_TOPNOPEG, LINE_BOTNOPEG, LINE_SECRET, LINE_BLOCKSOUND, LINE_NODRAW } e_line_flags;	

typedef struct	s_sector_info
{
	//value pairs: ceiling, floor
	t_int_pair	room_heights; 
	char		flats[2][9]; 
	uint32_t	light;
	uint32_t	special;
	uint32_t	tag;
}				t_sector_info

typedef struct		s_linedef_side
{
	t_int_pair		first_row_column; //row, column
	char			textures[3][9]; //top mid and bottom 0, 1, 2 respectively
	t_sector_info	sector_info;
	uint32_t		sector_num;
}					t_linedef_side;

typedef struct		s_linedef
{
	t_float_pair	start_vertex;
	t_float_pair	end_vertex;
	uint32_t		special;
	uint32_t		tag;
	uint32_t		flags;
	t_linedef_side	sides[2];
}					t_linedef;

/* Things */

/* things don't have options so ignore that part */

typedef struct		s_thing {
	t_float_pair	pos;
	int				angle;
	int				type;
	int				area;
}					t_thing;

/* Map data */

typedef struct		s_map_data
{
	size_t			line_count;
	size_t			thing_count;
	t_linedef		lines[MAX_LINE_COUNT];
	t_thing			things[MAX_THING_COUNT];
}					t_map_data;


#endif
