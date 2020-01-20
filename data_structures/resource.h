/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 16:59:54 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/19 17:10:04 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 ** Structure for storing resources that will converted into Lumps for the WAD file
 **/

#ifndef RESOURCE_H
# define RESOURCE_H

# define MAX_FILENAME_LEN 8

typedef unsigned char byte

typedef struct		s_resource
{
	int				size;
	char			name[MAX_FILENAME_LEN]; // Max Length for resource filename
	byte			*raw_data;
}					t_resource;

#endif
