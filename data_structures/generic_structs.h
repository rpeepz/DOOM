/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_structs.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 17:20:23 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/19 18:41:27 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERIC_STRUCT_H
# define GENERIC_STRUCT_H

# define PI 3.141592657

typedef struct				s_int_pair
{
	int						x;
	int						y;
}							s_int_pair;

typedef struct				s_short_pair
{
	short					x;
	short					y;
}							t_short_pair;

typedef struct				s_float_pair
{
							x;
							y;
}							f_float_pair;

typedef enum {false, true}	e_boolean;
typedef unsigned char		t_byte;


#endif
