/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   import_resources.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 20:14:57 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/23 17:49:24 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "resource.h"

t_resource_table	get_resources(char *directory_path)
{
	int					fd;
	t_resource			*resource_current;
	char 				path[4096];
	DIR					*dir;
	struct stat			st;
	struct dirent		*entry;
	t_resource_table	rtable;

	entry = NULL;
	bzero(&rtable, sizeof(t_resource_table));
	if ((dir = opendir(directory_path)))
	{
		while (((entry = readdir(dir)) != NULL) && (rtable.size < MAX_RESOURCE_COUNT))
		{
			bzero(path, sizeof(path) / sizeof(path[0]));
			if (*(entry->d_name) == '.')
				continue;
			else
			{

				resource_current = &(rtable.table[rtable.size]);

				//stick the the directory path string and filename string together

				 strcat(path, directory_path);
				 if(*(path + strlen(path) - 1) != '/')
					 strcat(path, "/");
				 strcat(path, entry->d_name);

				 //get the size of the file

				 if(stat(path, &st))
				 {
					 perror("Resource Import Error: ");
					 continue;
				 }

				 //set resource values

				 resource_current->size = st.st_size;
				 strlcat( resource_current->name, entry->d_name,  sizeof(resource_current->name) / sizeof(resource_current->name[0]) );
				 resource_current -> raw_data = (void *)malloc(st.st_size);
				 fd = open(path, O_RDONLY);
				 if((read(fd, resource_current -> raw_data, st.st_size)) != st.st_size)
				 {
					 perror("Resource Import Error: Data Read error ");
					 continue;
				 }

				 rtable.size++;


				 close(fd);
			}
		}
		closedir(dir);
	}
	else
		perror("Resource Import Error");
	return (rtable);
}
