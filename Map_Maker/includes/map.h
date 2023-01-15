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
# include "sector.h"

/*	line data */

/*	Info for lines to be drawn on the map


	Flags:
		"Block", "Mblock", "2-Sided",
		"Ftop", "Fbot", "Secret",
		"Snd Block", "No Draw"

	start_vertex = top of line
	end_vertex = bottom of line


	Sides:
		sides[0] = right side
		sides[1] = left side
*/
typedef struct		s_linedef
{
	t_float_pair	start_vertex;
	t_float_pair	end_vertex;
	uint32_t		special;
	uint32_t		tag;
	uint8_t			flags;
	t_sidedef		sides[2];
	int				sectorized[2];
}					t_linedef;

/* Info for things that appear on the map


	Flags:
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

typedef enum	e_exist_file
{
	MAP,
	WAD
}				t_exist_file;

/* check given type path for existing file */
int     check_exists(const char *name, int type);

/* save current state of map maker to reopen later */
void    save_map_name(t_map_interface *draw_mode, int *save_as);
int     save(t_map_interface *draw_mode);

/* open and load existing saved maps */
void    open_map_list(t_map_interface *draw_mode, int *open_map);
int     load_map(t_map_interface *draw_mode, char *name);

/* create and export the map info as bsp with lumps for engine to run */
void	export_wad(t_map_interface *draw_mode);

#endif
