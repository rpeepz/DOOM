#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "includes/lump.h"
#include "includes/map.h"

typedef struct	s_data
{
	t_linedef			*lines;
	int						line_count;
	t_sidedef			**sides;
	t_thing				*things;
	int						thing_count;
	t_sector			*sectors;
	int						sector_count;
	t_float_pair	*vertex;
	int						vertex_count;
}								t_data;

char	*lump_names[] = {
			"FLOORS",
			"WALLS",
			"SOUNDS",
			"MUSIC",
			"LINEDEFS",
			"SIDEDEFS",
			"THINGS",
			"SECTORS",
			"VERTEXES"
};

void  					run(char *wad);
t_header  			read_head(int fd);
unsigned char 	*read_wad_data(int fd, t_header *head);
t_lump  				*read_lumps(int fd, t_header *head);
t_data					assign_data(t_header head, unsigned char *wad_data, t_lump *lumps);
void						assign_extra_data(t_data data);
void						print_head_and_lump_info(t_header head, t_lump *lumps);
void						print_data(t_data data);
void						die(t_data data);

int   check_arg(char **av)
{
  int	len = strlen(av[1]);
  if (len <= 4 || strcmp(av[1] + len - 4, ".wad"))
    return (1);
  return (0);
}

int   main(int ac, char **av)
{
  if (ac < 2 || check_arg(av))
  {
    dprintf(2, "pass in wad file as argument\n");
    return (-1);
  }
  run(av[1]);
  return 0;
}

void  run(char *wad)
{
  int           fd = open(wad, O_RDONLY);
	t_header      head = read_head(fd);
  unsigned char *wad_data = read_wad_data(fd, &head);
  t_lump        *lumps = read_lumps(fd, &head);
	close(fd);

	// print_head_and_lump_info(head, lumps);
	t_data data = assign_data(head, wad_data, lumps);
	free(wad_data);
	free(lumps);

	assign_extra_data(data);
	print_data(data);
	die(data);
}

// read header data and save in t_header
t_header  read_head(int fd)
{
  t_header  head;
  read(fd, &head, sizeof(head));
  return (head);
}

// read lump data and save in byte array `wad_data`
unsigned char *read_wad_data(int fd, t_header *head)
{
	unsigned char *wad_data = malloc(sizeof(unsigned char) * head->lump_offset);
	read(fd, wad_data, head->lump_offset);
  return (wad_data);
}

// read lump offsets data and save in t_lumps array
t_lump  *read_lumps(int fd, t_header *head)
{
	t_lump *lumps = malloc(sizeof(t_lump) * head->num_lumps);
	for (int i = 0; i < head->num_lumps; i++)
		read(fd, &lumps[i], sizeof(t_lump));
  return (lumps);
}

/*
** assign data from wad to array of `t_linedef`
** skip assignation of line->sides & line->sectorized
*/
t_linedef	*assign_linedefs(unsigned char *wad_data, t_lump lump)
{
	unsigned char line_data[lump.size];
	memcpy(line_data, wad_data + lump.offset, lump.size);
	int count;
	memcpy(&count, line_data, sizeof(count));
	t_linedef *lines = malloc(sizeof(t_linedef) * count);
	int size = sizeof(t_linedef) - sizeof(lines->sides) - sizeof(lines->sectorized);
	for (int j = 0; j < count; j++) {
		memcpy(&lines[j], line_data + sizeof(count) + (j * size), size);
	}
	return (lines);
}

/*
** assign data from wad to 2d-array of `t_sidedef`
** ex: t_sidedef *side[2]
*/
t_sidedef	**assign_sidedefs(unsigned char *wad_data, t_lump lump, int line_count)
{
	unsigned char side_data[lump.size];
	memcpy(side_data, wad_data + lump.offset, lump.size);
	t_sidedef	**sides = malloc(sizeof(t_sidedef*) * line_count);
	int size = sizeof(t_sidedef) * 2;
	for (int j = 0; j < line_count; j++) {
		sides[j] = malloc(size);
		memcpy(sides[j], side_data + (j * size), size);
	}
	return (sides);
}

// assign data from wad to array of `t_thing`
t_thing	*assign_things(unsigned char *wad_data, t_lump lump)
{
	unsigned char thing_data[lump.size];
	memcpy(thing_data, wad_data + lump.offset, lump.size);
	int	count;
	memcpy(&count, thing_data, sizeof(count));
	t_thing	*things = malloc(sizeof(t_thing) * count);
	int size = sizeof(t_thing) - sizeof(struct nk_color);
	for (int j = 0; j < count; j++) {
		memcpy(&things[j], thing_data + sizeof(count) + (j * size), size);
	}
	return (things);
}

/*
** assign data from wad to array of `t_sector`
** skip assignation of sector->sector_lines & sector->line_count
*/
t_sector	*assign_sectors(unsigned char *wad_data, t_lump lump)
{
	unsigned char sector_data[lump.size];
	memcpy(sector_data, wad_data + lump.offset, lump.size);
	int count;
	memcpy(&count, sector_data, sizeof(count));
	t_sector	*sectors = malloc(sizeof(t_sector) * count);
	int size = sizeof(sectors->sector_num) + sizeof(sectors->sector_info);
	for (int j = 0; j < count; j++) {
		memcpy(&(sectors[j].sector_num), sector_data + sizeof(count) + (j * size), size);
	}
	return (sectors);
}

// assign data from wad to array of `t_float_pair`
t_float_pair	*assign_vertex(unsigned char *wad_data, t_lump lump)
{
	unsigned char vertex_data[lump.size];
	memcpy(vertex_data, wad_data + lump.offset, lump.size);
	int count;
	memcpy(&count, vertex_data, sizeof(count));
	t_float_pair	*vertex = malloc(sizeof(t_float_pair) * count);
	int size = sizeof(t_float_pair);
	for (int j = 0; j < count; j++) {
		memcpy(&vertex[j], vertex_data + sizeof(count) + (j * size), size);
	}
	return (vertex);
}

t_data	assign_data(t_header head, unsigned char *wad_data, t_lump *lumps)
{
	t_data		data;
	for (int i = 0; i < head.num_lumps; i++) {
		if (!strcmp(lump_names[i], "LINEDEFS")) {
			data.lines = assign_linedefs(wad_data, lumps[i]);
			memcpy(&data.line_count, wad_data + lumps[i].offset, sizeof(data.line_count));
		}
		else if (!strcmp(lump_names[i], "SIDEDEFS"))
			data.sides = assign_sidedefs(wad_data, lumps[i], data.line_count);
		else if (!strcmp(lump_names[i], "THINGS")) {
			data.things = assign_things(wad_data, lumps[i]);
			memcpy(&data.thing_count, wad_data + lumps[i].offset, sizeof(data.thing_count));
		}
		else if (!strcmp(lump_names[i], "SECTORS")) {
			data.sectors = assign_sectors(wad_data, lumps[i]);
			memcpy(&data.sector_count, wad_data + lumps[i].offset, sizeof(data.sector_count));
		}
		else if (!strcmp(lump_names[i], "VERTEXES")) {
			data.vertex = assign_vertex(wad_data, lumps[i]);
			memcpy(&data.vertex_count, wad_data + lumps[i].offset, sizeof(data.vertex_count));
		}
		else if (i < 4) {
			// printf("Skipping assets %s\n", lump_names[i]);
		}
	}
	return (data);
}

void		assign_extra_data(t_data data)
{
	// assign side data to corresponding linedef
	for (int i = 0; i < data.line_count; i++) {
		memcpy(data.lines[i].sides, data.sides[i], sizeof(data.lines->sides));
		for (int j = 0; j < 2; j++) {
			// increment line count of corresponding sector
			if (data.sides[i][j].sector_num > 0) {
				data.sectors[data.sides[i][j].sector_num - 1].line_count++;
			}
		}
	}
	// assign data.lines->sectorized

	// assign line data of sectors to each array of `sector_lines`
	for (int i = 0; i < data.sector_count; i++) {
		data.sectors[i].sector_lines = malloc(sizeof(t_line) * data.sectors[i].line_count);
		for (int j = 0; j < data.sectors[i].line_count; j++) {
			// itterate through all lines, find matching sector number, assign to sector line
			for (int k = 0; k < data.line_count; k++) {
				for (int l = 0; l < 2; l++) {
					// TODO error in assigning vertex data
					if (data.lines[k].sides[l].sector_num == data.sectors[i].sector_num) {
						data.sectors[i].sector_lines[k].start.x = data.lines[k].start_vertex.x;
						data.sectors[i].sector_lines[k].start.y = data.lines[k].start_vertex.y;
						data.sectors[i].sector_lines[k].end.x = data.lines[k].end_vertex.x;
						data.sectors[i].sector_lines[k].end.y = data.lines[k].end_vertex.y;
						break ;
					}
				}
			}
		}
	}
}

void	print_head_and_lump_info(t_header head, t_lump *lumps)
{
	printf("number of lumps %u\n", head.num_lumps);
  printf("total wad size %u\n\n", head.lump_offset);

	for (int i = 0; i < head.num_lumps; i++) {
		printf("%s\n", lumps[i].lump_name);
		printf("\tsize %d\n", lumps[i].size);
		printf("\toffset %d\n", lumps[i].offset);
	}
	printf("\n");
}

void	print_data(t_data data)
{
	t_linedef			*lines = data.lines;
	printf("line count %d\n\n", data.line_count);
	for (int j = 0; j < data.line_count; j++) {
		printf("start x %.0f start y %.0f\n", lines[j].start_vertex.x, lines[j].start_vertex.y);
		printf("end x %.0f end y %.0f\n", lines[j].end_vertex.x, lines[j].end_vertex.y);
		printf("flag %hhx, special %d, tag %d\n", lines[j].flags, lines[j].special, lines[j].tag);
		for (int i = 0; i < 2; i++) {
			printf("line %d, side %d, sector number %d\n", j, i, lines[j].sides[i].sector_num);
		}
		printf("\n");
	}
	t_sidedef			**sides = data.sides;
	printf("side count %d\n\n", data.line_count * 2);
	for (int j = 0; j < data.line_count; j++) {
		for (int k = 0; k < 2; k++) {
			printf("side %d offset x %.0f y %.0f\n", k, sides[j][k].offset.x, sides[j][k].offset.y);
			printf("side %d sector number %u\n", k, sides[j][k].sector_num);
			for (int tex = 0; tex < 3; tex++)
				printf("side %d texture %s\n", k, sides[j][k].textures[tex]);
			printf("\n");
		}
	}
	t_thing				*things = data.things;
	printf("thing count %d\n\n", data.thing_count);
	for (int j = 0; j < data.thing_count; j++) {
		printf("name %s\n", things[j].name);
		printf("angle %d\n", things[j].angle);
		printf("flags %hhx\n", things[j].flags);
		printf("x %.0f, y %.0f\n", things[j].pos.x, things[j].pos.y);
		printf("type %d\n\n", things[j].type);
	}
	t_sector			*sectors = data.sectors;
	printf("sector count %d\n\n", data.sector_count);
	for (int j = 0; j < data.sector_count; j++) {
		printf("sector num %d\n", sectors[j].sector_num);
		printf("room height ceiling %d floor %d\n", sectors[j].sector_info.room_heights.x, sectors[j].sector_info.room_heights.y);
		printf("ceiling texture %s\n", sectors[j].sector_info.flats[0]);
		printf("floor texture %s\n", sectors[j].sector_info.flats[1]);
		printf("light %d\n", sectors[j].sector_info.light);
		printf("special %d\n", sectors[j].sector_info.special);
		printf("tag %d\n\n", sectors[j].sector_info.tag);
		printf("Total lines in sector %d\n", sectors[j].line_count);
		for (int i = 0; i < sectors[j].line_count; i++) {
			printf("start - x %.0f, y %.0f\n", sectors[j].sector_lines[i].start.x, sectors[j].sector_lines[i].start.y);
			printf("end - x %.0f, y %.0f\n", sectors[j].sector_lines[i].end.x, sectors[j].sector_lines[i].end.y);
		}
		printf("\n");
	}
	t_float_pair	*vertex = data.vertex;
	printf("vertex count %d\n\n", data.vertex_count);
	for (int j = 0; j < data.vertex_count; j++) {
		printf("vertex x %0.f, y %0.f\n\n", vertex[j].x, vertex[j].y);
	}
}

void	die(t_data data)
{
	free(data.lines);
	for (int i = 0; i < data.line_count; i++) {
		free(data.sides[i]);
	}
	free(data.sides);
	free(data.things);
	free(data.sectors);
	free(data.vertex);
}
