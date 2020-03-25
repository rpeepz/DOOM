/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 17:44:46 by smaddox           #+#    #+#             */
/*   Updated: 2020/02/13 17:25:34 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

/*	Line flag masks */
# define L_BLOCK 0x1
# define L_MBLOCK 0x2
# define L_TWO_SIDED 0x4
# define L_FTOP 0x8
# define L_FBOT 0x10
# define L_SECRET 0x20
# define L_SNDBLOCK 0x40
# define L_NODRAW 0x80

/*	Thing flag masks */
# define T_EASY 0x1
# define T_NORMAL 0x2
# define T_HARD 0x4
# define T_DEAF 0x8
# define T_NETWORK 0x10
# define T_SINGLE 0x20
# define T_COOP 0x40
# define T_FRIEND 0x80

# include "generic_structs.h"
# include "demo.h"

/*	line data */

typedef struct	s_sector_info
{
	//value pairs: ceiling, floor t_int_pair	room_heights;
	char		flats[2][16];
	uint32_t	light;
	uint32_t	special;
	uint32_t	tag;
}				t_sector_info;

typedef struct		s_sidedef
{
	t_float_pair	offset; //row, column
	char			textures[3][16]; //top mid and bottom 0, 1, 2 respectively
	t_sector_info	sector_info;
	uint32_t		sector_num;
}					t_sidedef;

/*	Flags in order for Linedefs

	"Block", "Mblock", "2-Sided",
	"Ftop", "Fbot", "Secret",
	"Snd Block", "No Draw"
*/
typedef struct		s_linedef
{
	t_float_pair	start_vertex;
	t_float_pair	end_vertex;
	uint32_t		special;
	uint32_t		tag;
	uint8_t			flags;
	t_sidedef		sides[2];
}					t_linedef;

/*	Flags in order for Things

	"Easy", "Normal", "Hard",
	"Ambush", "Network", "Single Player",
	"Co-op", "Friendly"
*/
typedef struct		s_thing {
	t_float_pair	pos;
	int				angle;
	int				type;
	char			name[16];
	uint8_t			flags;
	struct nk_color	color;
}					t_thing;

typedef struct  s_lumped
{
	t_header    head;
	t_lump      lumps[2];
}               t_lumped;

int     check_exists(const char *name);

void    save_map_name(t_map_interface *draw_mode, int *save_as);
int     save(t_map_interface *draw_mode);

void    open_map_list(t_map_interface *draw_mode, int *open_map);
int     load_map(t_map_interface *draw_mode, char *name);

void	export_map(t_map_interface *draw_mode);

#endif
