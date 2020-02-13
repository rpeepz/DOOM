/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 17:44:46 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/24 11:45:52 by rpapagna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# define MAX_LINE_COUNT 4096	//should be dynamic
# define MAX_THING_COUNT 1000	//should be dynamic -rpapagna

#include "generic_structs.h"

/*	line data */

/*	typedef enum { LINE_BLOCKPLAYER, LINE_BLOCKMONSTER, LINE_TWOSIDES, \
 	LINE_TOPNOPEG, LINE_BOTNOPEG, LINE_SECRET, LINE_BLOCKSOUND, LINE_NODRAW } e_line_flags;	

	depricated -rpapagna */
typedef struct	s_sector_info
{
	//value pairs: ceiling, floor
	t_int_pair	room_heights;
	char		flats[2][9];
	uint32_t	light;
	uint32_t	special;
	uint32_t	tag;
}				t_sector_info;

typedef struct		s_sidedef
{
	t_float_pair	offset; //row, column
	char			textures[3][9]; //top mid and bottom 0, 1, 2 respectively
	t_sector_info	sector_info;
	uint32_t		sector_num;
}					t_sidedef;

typedef struct		s_line_flags
{
	int				block;	// block player & monsters
	int				mblock;	// block moster
	int				two_side;
	int				ftop;	// unpegged upper texture
	int				fbot;	// unpegged lower texture
	int				secret;
	int				snd_block;	// blocks sound
	int				no_draw;	// never shows up on automap
}					t_line_flags;

typedef struct		s_linedef
{
	t_float_pair	start_vertex;
	t_float_pair	end_vertex;
	uint32_t		special;
	uint32_t		tag;
	t_line_flags	flags;
	t_sidedef		sides[2];
}					t_linedef;

/* skill levels
Easy - 1, 2 / Normal - 3 / Hard - 4, 5 */
typedef struct		s_thing_flags
{
	int				skill_one;	// skill level 1 & 2
	int				skill_three;// skill level 3
	int				skill_five;	// skill level 4 & 5
	int				ambush;		// deaf
	int				network;	// not in single player
	int				single_p;	// not in deathmatch
	int				coop;		// not in coop
	int				friendly;	// friendly monster
}					t_thing_flags;

typedef struct		s_thing {
	t_float_pair	pos;
	int				angle;
	int				type;
	char			name[16];
	t_thing_flags	flags;
}					t_thing;

#endif
