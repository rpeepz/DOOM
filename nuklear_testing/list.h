/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 22:13:43 by smaddox           #+#    #+#             */
/*   Updated: 2020/02/07 14:07:00 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

#include "map.h"
#include "demo.h"
//# define NK_INCLUDE_FIXED_TYPES
//# define NK_INCLUDE_STANDARD_IO
//# define NK_INCLUDE_STANDARD_VARARGS
//# define NK_INCLUDE_DEFAULT_ALLOCATOR
//# define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
//# define NK_INCLUDE_FONT_BAKING
//# define NK_INCLUDE_DEFAULT_FONT
//# include "../Nuklear/nuklear.h"
//# include "nuklear_sdl_gl3.h"

typedef struct		s_line_node
{
	t_linedef		line;
	struct nk_color color;
	struct s_line_node		*next;
	struct s_line_node		*prev;
}					t_line_node;

typedef struct		s_line_bank
{
	int				empty;
	int				count;
	t_line_node 	*selected;
	t_line_node		*head;
	t_line_node 	*tail;
}					t_line_bank;

/* Given a reference (pointer to pointer) to the head of a list
	and the linedef, inserts a new node on the front of the list. */
//void	line_push(t_line_node **head_ref, t_linedef new_line);

/* Given a node as prev, insert a new node after the given node */
//void	line_insert_after(t_line_node *prev_node, t_linedef new_line);

int	add_line(t_line_bank *linebank, struct nk_vec2 start, struct nk_vec2 end);
int	stroke_my_line( struct nk_command_buffer *b, t_line_node *node);
#endif
