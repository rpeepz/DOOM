/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 16:59:54 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/22 22:08:30 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCE_H
# define RESOURCE_H

# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <unistd.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>
# include <resource.h>
# include <generic_structs.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>

# define MAX_RESOURCE_COUNT 100

typedef struct		s_resource
{
	int				size;
	char			name[9]; // Max Length for resource filename
	uint8_t			*raw_data;
}					t_resource;

typedef struct		s_resource_table
{
	size_t			size;
	t_resource		table[MAX_RESOURCE_COUNT];
}					t_resource_table;

t_resource_table	get_resources(char *path);

#endif
