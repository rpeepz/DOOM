/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 22:13:43 by smaddox           #+#    #+#             */
/*   Updated: 2020/02/11 22:27:23 by rpapagna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

# include "map.h"
# include "demo.h"
# define ITEM_LINE 2
# define ITEM_THING 1
# define HIGHLIGHT nk_rgb(255, 140, 60)
# define LINE_COLOR nk_rgb(10, 10, 10)
# define THING_COLOR nk_rgb(145, 145, 145)
//# define NK_INCLUDE_FIXED_TYPES
//# define NK_INCLUDE_STANDARD_IO
//# define NK_INCLUDE_STANDARD_VARARGS
//# define NK_INCLUDE_DEFAULT_ALLOCATOR
//# define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
//# define NK_INCLUDE_FONT_BAKING
//# define NK_INCLUDE_DEFAULT_FONT
//# include "../Nuklear/nuklear.h"
//# include "nuklear_sdl_gl3.h"

/* map-item double linked node */
typedef struct			s_item_node
{
	struct nk_color		color;
	struct s_linedef	*line;
	struct s_thing		*thing;
	struct s_item_node	*next;
	struct s_item_node	*prev;
}						t_item_node;

typedef struct			s_bank
{
	t_item_node			*selected;
	int					count_line;
	t_item_node			*head_line;
	t_item_node			*tail_line;
	int					count_thing;
	t_item_node			*head_thing;
	t_item_node			*tail_thing;
}						t_bank;

/* driver call to `nk_stroke_line` given a t_node pointer
	which holds the vectors from which the line begins and ends */
int	stroke_my_line(struct nk_command_buffer *b, t_item_node *node);

/*	Nuklear pannel allowing the user to view and edit information
	on the current selected line withing the linkbank */
void    edit_pannel(struct nk_context *ctx, t_linedef *line);

/*	Nuklear pannel displaying information such as coordinates and flags
	of the lines that have been added to the map */
void    list_pannel(t_map_interface *draw_mode);

/*	Snap coordinates to a grid and convert from nk_vec2 to float pair */
t_float_pair	snap(struct nk_vec2 v);

/* Given the end points of a line, insert new node at the end of the list. */
void	add_line(t_bank *bank, struct nk_vec2 start, struct nk_vec2 end);

/* Given the location of a thing, insert new node at the end of the list. */
void	add_thing(t_bank *bank, struct nk_vec2 location, char *name);

/* Remove and free the selected item */
void	remove_from_bank(t_bank *bank, int item);

/* Show which item is currently selected
	direction 1 traverses next : 0 traverses prev */
void	change_selected(t_bank *bank, int item, int direction);

#endif
