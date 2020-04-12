/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lump.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 17:11:31 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/20 17:54:06 by rpapagna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMP_H
# define LUMP_H

#include "generic_structs.h"
typedef struct			s_header
{
	char				name[4];
	uint32_t			num_lumps;
	uint32_t			lump_offset;
}						t_header;

typedef struct			s_lump
{
	char				lump_name[16];
	uint32_t			offset;
	uint32_t			size;
}						t_lump;
uint8_t					*export_assets(uint8_t *buffer, t_lump *lumps, \
t_header *head, void *map_interface);
uint8_t					*export_map(uint8_t *buffer, t_lump *lumps, \
t_header *head, void *map_interface);

#endif





