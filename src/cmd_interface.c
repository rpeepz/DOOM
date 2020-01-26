/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_interface.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpapagna <rpapagna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 18:03:44 by smaddox           #+#    #+#             */
/*   Updated: 2020/01/24 21:50:35 by rpapagna         ###   ########.fr       */
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
	t_thing *thing = &(map_data.things[map_data.thing_count++]);


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
	char *t = "NULL";
	char *line = "NULL";
	char **temp = NULL;
	t_linedef *linedef = &(map_data.lines[map_data.line_count++]);


	system("clear");
	printf("adding new line\n");

	printf("\nEnter start position Ex. \"45, 3.1415\"\n");
	get_next_line(0, &line);
	linedef->start_vertex.x = atof( (t = strtok(line, ",")) ? t : "NULL" );
	linedef->start_vertex.y = atof( (t = strtok(NULL, ",")) ? t : "NULL" );
	free(line);
	printf("\nEnter end position\n");
	get_next_line(0, &line);
	linedef->end_vertex.x = atof( (t = strtok(line, ",")) ? t : "NULL" );
	linedef->end_vertex.y = atof( (t = strtok(NULL, ",")) ? t : "NULL" );
	free(line);

	printf("\nEnter flags\n");
	get_next_line(0, &line);
	linedef->flags = atoi(line);
	free(line);

	printf("\nEnter the type info\n");
	get_next_line(0, &line);
	linedef->special = atoi(line);
	free(line);

	printf("\nSector tag\n");
	get_next_line(0, &line);
	linedef->tag = atoi(line);
	free(line);

	for (int i = 0; (i < 2 && linedef->flags & 0x4) || i < 1; i++){
		printf("\nSidedef properties");
		printf("\nEnter texture offset Ex. \"45, 3.1415\"\n");
		get_next_line(0, &line);
		linedef->sides[i].offset.x = atof( (t = strtok(line, ",")) ? t : "NULL" );
		linedef->sides[i].offset.y = atof( (t = strtok(NULL, ",")) ? t : "NULL" );
		free(line);

		printf("\nTop Texture: ");

		get_next_line(0, &line);
		ft_memcpy(linedef->sides[i].textures[0], line ? line : "\0\0\0\0\0\0\0\0", line ? 8 : 2);
		free(line);
		printf("\nBottom Texture: ");
		get_next_line(0, &line);
		ft_memcpy(linedef->sides[i].textures[2], line ? line : "\0\0\0\0\0\0\0\0", line ? 8 : 2);
		free(line);
		printf("\nMiddle Texture: ");
		get_next_line(0, &line);
		ft_memcpy(linedef->sides[i].textures[1], line ? line : "\0\0\0\0\0\0\0\0", line ? 8 : 2);
		free(line);

		printf("\nSector tag\n");
		get_next_line(0, &line);
		linedef->sides[i].sector_num = atoi(line);
		free(line);

		printf("\nCeiling height\n");
		get_next_line(0, &line);
		linedef->sides[i].sector_info.room_heights.x = atoi(line);
		free(line);
		printf("\nFloor height\n");
		get_next_line(0, &line);
		linedef->sides[i].sector_info.room_heights.y = atoi(line);
		free(line);

		printf("\nCeiling Texture: ");
		get_next_line(0, &line);
		ft_memcpy(linedef->sides[i].sector_info.flats[0], line ? line : "\0\0\0\0\0\0\0\0", 8);
		free(line);
		printf("\nFloor Texture: ");
		get_next_line(0, &line);
		ft_memcpy(linedef->sides[i].sector_info.flats[1], line ? line : "\0\0\0\0\0\0\0\0", 8);
		free(line);

		printf("\nLight level\n");
		get_next_line(0, &line);
		linedef->sides[i].sector_info.light = atoi(line);
		free(line);

		printf("\nSpecial\n");
		get_next_line(0, &line);
		linedef->sides[i].sector_info.special = atoi(line);
		free(line);

		printf("\ntag\n");
		get_next_line(0, &line);
		linedef->sides[i].sector_info.tag = atoi(line);
		free(line);
	}
	return;
}

void	my_exit(void){
	write(1, "exiting....\n", 12);
	exit(0);
}

void	clear(void){
	system("clear");
}

void	list(void){
	printf("\n");
	printf("total %zu\n", map_data.thing_count);
	for (int i = 0; i < map_data.thing_count; i++){
		printf("Position: ");
		printf("%.2f, %.2f\n", map_data.things[i].pos.x, map_data.things[i].pos.y);
		printf("Flags:\t%d\n", map_data.things[i].flags);
		printf("Angle:\t%d\n", map_data.things[i].angle);
		printf("Type:\t%d\n\n", map_data.things[i].type);
	}
	printf("\n");
	printf("total %zu\n", map_data.line_count);
	for (int i = 0; i < map_data.line_count; i++){
		printf("Start Position: ");
		printf("%.2f,\t%.2f\n", map_data.lines[i].start_vertex.x, map_data.lines[i].start_vertex.y);
		printf("End Position:   ");
		printf("%.2f,\t%.2f\n", map_data.lines[i].end_vertex.x, map_data.lines[i].end_vertex.y);
		printf("special:\t%d\n", map_data.lines[i].special);
		printf("tag:\t\t%d\n", map_data.lines[i].tag);
		printf("flags:\t\t%d\n", map_data.lines[i].flags);
		for (int j = 0; (j < 2 && map_data.lines[i].flags & 0x4) || j < 1; j++){
			printf("Texture offset: ");
			printf("%.2f,\t%.2f\n", map_data.lines[i].sides[j].offset.x, map_data.lines[i].sides[j].offset.x);
			printf("Top texture:\t%s\n", map_data.lines[i].sides[j].textures[0]);
			printf("Bottom texture: %s\n", map_data.lines[i].sides[j].textures[2]);
			printf("Middle texture: %s\n", map_data.lines[i].sides[j].textures[1]);
			printf("Sector number: %d\n", map_data.lines[i].sides[j].sector_num);
			printf("\t-- Sector info\n");
			printf("Ceiling height: %d\n", map_data.lines[i].sides[j].sector_info.room_heights.x);
			printf("Ceiling texture: %s\n", map_data.lines[i].sides[j].sector_info.flats[0]);
			printf("Floor height: %d\n", map_data.lines[i].sides[j].sector_info.room_heights.y);
			printf("Floor texture: %s\n", map_data.lines[i].sides[j].sector_info.flats[1]);
			printf("Light:\t%d\n", map_data.lines[i].sides[j].sector_info.light);
			printf("Special:\t%d\n", map_data.lines[i].sides[j].sector_info.special);
			printf("Tag:\t\t%d\n\n", map_data.lines[i].sides[j].sector_info.tag);
		}
	}
}

void	calculate_bsp(t_map_data map_data);

int cmd_interface(void)
{
	setbuf(stdout, NULL);
	char *line = "null";
	char *options[] = { "new line", "new thing", "exit", "clear", "list"};
	void (*funcs[])(void) = { &add_line, &add_thing, &my_exit, &clear, &list };
	int y = sizeof(options) / sizeof(options[0]);
	
	while (1){
		write(1, "<$MAPMAKER> ", 12);
		get_next_line(0, &line);
		if (!strcmp(line, "BSP")) {
			calculate_bsp(map_data);
			continue ;
		}
		for (int j = 0; j < y ; ++j){
			if (!strcmp(line, options[j])){
					funcs[j]( );
					free(line);
					break;
			}
			if( j == y - 1)
				write(1, "invalid option\n", 15);
		}
	}	
	return(0);
}

int main(void){
	return( cmd_interface() );
}



int		intersect_points(double x, double y, t_float_pair vector)
{
	if (vector.x == x && vector.y == y)
		return (1);
	return (0);
}
typedef struct	s_sector {
	int			count; //to count lines in each sector
	t_linedef	lines[10];
	/*	It is more prefferable to make a basic linked list of composing linedefs
		instead of a static array of total lines in a sector
	*/
}				t_sector;
void 	calculate_bsp(t_map_data map_data) {

	printf("HERE IS BSP :)\n");

	int			sector_count = 0;
	t_sector	sectors[16]; // max test sectors

	for (int i = 0; i < map_data.line_count; i++) {

		t_float_pair vectorS = map_data.lines[i].start_vertex;
		t_float_pair vectorE = map_data.lines[i].end_vertex;

		for (int j = i + 1; j < map_data.line_count; j++) {

			t_float_pair start = map_data.lines[j].start_vertex;
			t_float_pair end = map_data.lines[j].end_vertex;

			/* find if the start vertex of inner loop intersects with out line */
			if (intersect_points(start.x, start.y, vectorS) ||
				intersect_points(start.x, start.y, vectorE)){
				++sector_count;
				//set line as a part of a sector

				//find a way to single out each sector and find when its completely closed.
				;
			}
			/* find if the end vertex of inner loop intersects with out line */
			else if (intersect_points(end.x, end.y, vectorS) ||
					intersect_points(end.x, end.y, vectorE)){
				++sector_count;
				//set line as a part of a sector

				//find a way to single out each sector and find when its completely closed.
				;
			}

		}
	}
	return ;
}
