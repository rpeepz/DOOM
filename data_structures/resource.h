/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 16:59:54 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/19 17:23:52 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCE_H
# define RESOURCE_H

#include "generic_structs.h"

typedef struct		s_resource
{
	int				size;
	char			name[8]; // Max Length for resource filename
	byte			*raw_data;
}					t_resource;

#endif
