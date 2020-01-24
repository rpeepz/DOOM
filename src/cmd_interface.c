/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_interface.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaddox <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 18:03:44 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/23 20:49:16 by smaddox          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "get_next_line.h"
#include "map.h"

t_map_data map_data = { 0 };

void	add_thing(void){

	char *t = "NULL";
	char *line = "NULL";
	char **temp = NULL;
	t_thing *thing = &(map_data.things[map_data.thing_count]);


	system("clear");
	printf("adding new thing\n");

	printf("\nEnter position Ex. \"45, 3.1415\"\n");
	get_next_line(0, &line);
	thing->pos.x = atof( (t = strtok(line, ",")) ? t : "NULL" );
	thing->pos.y = atof( (t = strtok(NULL, ",")) ? t : "NULL" );
	free(line);

	printf("\nEnter the starting angle i.e 1 - 360 \n");
	get_next_line(0, &line);
	thing->angle = atoi(line);
	free(line);

	printf("\nEnter the type info\n");
	get_next_line(0, &line);
	thing->type = atoi(line);
	free(line);

	printf("\nEnter flags\n");
	get_next_line(0, &line);
	thing->flags = atoi(line);
	free(line);

/*	testing
	printf("pos: (%f, %f)\n", thing->pos.x, thing->pos.y);
	printf("angle: %d\n", thing->angle);
	printf("type: %d\n", thing->type);
	printf("flags: %d\n", thing->flags);
*/

	return;
}

void	add_line(void){
	printf("adding new line\n");
	return;
}

void	my_exit(void){
	write(1, "exiting....\n", 12);
	exit(0);
}

void	clear(void){
	system("clear");
}

int cmd_interface(void)
{
	char *line = "null";
	char *options[] = { "new line", "new thing", "exit", "clear"};
	void (*funcs[])(void) = { &add_line, &add_thing, &my_exit, &clear };
	int y = sizeof(options) / sizeof(options[0]);
	
	while (1){
		write(1, "<$MAPMAKER> ", 12);
		get_next_line(0, &line);
		for (int j = 0; j < y ; ++j){
			if (!strcmp(line, options[j])){
					funcs[j]( );
					free(line);
					break;
			}
			if( j == y - 1)
				write(1, "invalid option\n", 16);
		}
	}	
	return(0);
}

int main(void){
	return( cmd_interface() );
}
