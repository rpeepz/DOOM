/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_structs.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 17:20:23 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/20 17:54:57 by rpapagna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERIC_STRUCT_H
# define GENERIC_STRUCT_H

# define PI 3.141592657
#include <unistd.h>
#include <stdint.h>

typedef struct				s_int_pair
{
	int						x;
	int						y;
}							t_int_pair;

typedef struct				s_short_pair
{
	short					x;
	short					y;
}							t_short_pair;

typedef struct				s_float_pair
{
	double					x;
	double					y;
}							t_float_pair;

typedef enum {FALSE, TRUE}	e_boolean;
typedef unsigned char		t_byte;


#endif
