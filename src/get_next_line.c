/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 13:45:28 by smaddox           #+#    #+#             */
/*   Updated: 2019/05/14 14:40:27 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_list	*fd_handler(const int fd, t_list **lst)
{
	t_list *temp;

	temp = *lst;
	while (temp)
	{
		if ((int)temp->content_size == fd)
		{
			return (temp);
		}
		temp = temp->next;
	}
	temp = ft_lstnew(NULL, fd);
	temp->content_size = fd;
	ft_lstadd(lst, temp);
	return (temp);
}

int		string_constructor(t_list *node)
{
	char	*str;
	char	*content;
	int		r;
	char	buffer[BUFF_SIZE + 1];

	ft_bzero(buffer, BUFF_SIZE + 1);
	while ((r = read((int)node->content_size, buffer, BUFF_SIZE)) > 0)
	{
		str = ft_strdup(buffer);
		content = node->content;
		if (node->content)
		{
			node->content = ft_strjoin(content, str);
			if (content)
				free(content);
			if (str)
				free(str);
		}
		else
			node->content = str;
		if (ft_strclen(buffer, '\n') < BUFF_SIZE)
			break ;
		ft_bzero(buffer, BUFF_SIZE + 1);
	}
	return (r);
}

void	clean_up(t_list *node, t_list *lst)
{
	t_list *temp;

	temp = lst;
	if (!(node == lst))
	{
		while (temp->next != node)
		{
			temp = temp->next;
		}
		temp->next = temp->next->next;
		free(node->content);
		free(node);
	}
}

int		content_handler(t_list *node, char **line)
{
	int		ndist;
	int		dist;
	char	*temp;

	ndist = ft_strclen(node->content, '\n');
	dist = ft_strlen(node->content);
	if (!dist)
		return (0);
	*line = ft_strsub(node->content, 0, ndist);
	temp = node->content;
	node->content = ft_strsub(node->content, ndist + 1, dist - ndist);
	if (temp)
		free(temp);
	return (1);
}

int		get_next_line(const int fd, char **line)
{
	static t_list	*lst;
	t_list			*node;
	int				n;
	int				r;

	if (fd < 0 || line == NULL)
		return (-1);
	n = 0;
	node = fd_handler(fd, &lst);
	if ((r = string_constructor(node)) == -1)
		return (-1);
	if ((n = content_handler(node, line)) == 0)
	{
		*line = NULL;
		clean_up(node, lst);
	}
	return (n);
}
