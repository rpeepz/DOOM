/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 22:13:43 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/25 19:22:24 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

typedef struct		s_line_node{
	t_linedef		line;
	t_line_node		*next;
	t_line_node		*prev;
}					t_line_node;

typedef struct		LBANK_VTABLE;

typedef struct		s_line_bank{

	t_line_node 	*selected;
	t_line_node		*head;
	t_line_node 	*tail;
	const LBANK_VTABLE	_vtable;		
}					t_line_bank;
