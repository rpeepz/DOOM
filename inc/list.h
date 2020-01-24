/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 22:13:43 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/24 00:42:56 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

typedef struct		s_line_node{
	t_linedef		line;
	t_line_node		*next;
	t_line_node		*prev;
}					t_line_node;

typedef struct		s_line_bank{
	t_line_node 	*selected;
	t_line_node		*head;
	t_line_node 	*tail;

	void			(*add_line)(t_line_bank *self, t_line_node new_node);
	void			(*remove_line)(t_line_bank *self);
	void			(*search)(t_line_bank *self, int id);
}					t_line_bank;
