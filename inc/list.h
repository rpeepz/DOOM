/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 22:13:43 by smaddox           #+#    #+#             */
/*   Updated: 2020/02/05 20:22:40 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
//#include "nuklear.h"

typedef struct		s_line_node{
	t_linedef		line;
//	struct nk_color color;
	t_line_node		*next;
	t_line_node		*prev;
}					t_line_node;

typedef struct		s_line_bank{
	int				empty;
	int				count;
	t_line_node 	*selected;
	t_line_node		*head;
	t_line_node 	*tail;
}					t_line_bank;
