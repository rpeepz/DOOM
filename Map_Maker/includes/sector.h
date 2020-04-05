/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 20:18:43 by rpapagna          #+#    #+#             */
/*   Updated: 2020/04/03 22:27:23 by rpapagna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SECTOR_H
# define SECTOR_H

# define SECTOR_ADD 0
# define SECTOR_EDIT 1
# define SECTOR_MAX 64

# include "map.h"
# include "demo.h"

# include "../../Nuklear/nuklear.h"
# include "nuklear_sdl_gl3.h"

typedef struct      s_sector_info
{
	t_int_pair	    room_heights;
	char		    flats[2][16];
	uint32_t	    light;
	uint32_t	    special;
	uint32_t	    tag;
}				    t_sector_info;

typedef struct	    s_sector
{
	t_line		    *sector_lines;
    int             line_count;
	int				sector_num;
    t_sector_info   sector_info;
}				    t_sector;

typedef struct      s_seg
{
	t_line		    *ptr_to_line_segmenting;
	int			    x;
	int			    y;
}				    t_seg;

typedef struct	    s_subsec
{
	// Im not sure what a subsector consists of besides segments
	t_seg		    *subsec_segments;
}				    t_subsec;

typedef struct      s_sectors
{
    t_sector        sectors[SECTOR_MAX];
    int             selected;
}                   t_sectors;

/*	Nuklear panel allowing the user to view and edit information
	on the sectors in the current map */
void	sector_panel(t_map_interface *draw_mode);
#endif
