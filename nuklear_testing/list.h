/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 22:13:43 by smaddox           #+#    #+#             */
/*   Updated: 2020/02/07 16:06:11 by rpapagna         ###   ########.fr       */
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

void    list_pannel(struct nk_context *ctx, t_line_bank *linebank);

/* Given a reference to the line bank and the end points of the line,
	insert a new node at the end of the list. */
void	add_line(t_line_bank *linebank, struct nk_vec2 start, struct nk_vec2 end);

void remove_line(t_line_bank *linebank);

int	stroke_my_line( struct nk_command_buffer *b, t_line_node *node);

#endif
