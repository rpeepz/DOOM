/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 16:59:54 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/20 17:54:48 by rpapagna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCE_H
# define RESOURCE_H

#include "generic_structs.h"

typedef struct		s_resource
{
	int				size;
	char			name[8]; // Max Length for resource filename
	uint8_t			*raw_data;
}					t_resource;

#endif
