/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 12:41:27 by rpapagna          #+#    #+#             */
/*   Updated: 2020/02/07 12:51:08 by rpapagna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	line_push(t_line_node **head_ref, t_linedef new_line)
{
    t_line_node *node;

    node = malloc(sizeof(t_line_node));
    node->line = new_line;
    node->next = *head_ref;
    node->prev = NULL;

    if (*head_ref)
        (*head_ref)->prev = node;
    *head_ref = node;
}

void	line_insert_after(t_line_node *prev_node, t_linedef new_line)
{
    t_line_node *node;

    if (!prev_node)
        return ;
    node = malloc(sizeof(t_line_node));
    node->line = new_line;
    
    node->next = prev_node->next;
    prev_node->next = node;

    node->prev = prev_node;
    if (node->next)
        node->next->prev = node;
}
