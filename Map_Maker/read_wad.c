#include <unistd.h>
#include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdarg.h>
# include <string.h>
# include <math.h>
# include <assert.h>
# include <math.h>
# include <limits.h>
# include <time.h>
#include <string.h>
#include <stdio.h>
#include "includes/lump.h"
#include "includes/map.h"

int		main(int ac, char **av) {
	if (ac < 2) {
		dprintf(2, "Pass in wad file as argument\n");
		return -1;
	}
	int len = strlen(av[1]);
	if (len <= 4 || strcmp(av[1] + len - 4, ".wad")) {
		dprintf(2, "file must end in .wad\n");
		return -1;
	}

	//open wad
	int fd = open(av[1], O_RDONLY);
	
	//read header info
	t_header head;
	read(fd, &head, sizeof(head));
	printf("num lumps %u\ntotal wad size %u\n",head.num_lumps, head.lump_offset);

	//read lump data
	unsigned char wad_data[head.lump_offset];
	read(fd, wad_data, head.lump_offset);
	
	//read lump directory info 
	t_lump lumps[head.num_lumps];
	for (int i = 0; i < head.num_lumps; i++)
		read(fd, &lumps[i], sizeof(t_lump));

	for (int i = 0; i < head.num_lumps; i++) {
		printf("%s\n", lumps[i].lump_name);
		printf("\tsize %d\n", lumps[i].size);
		printf("\toffset %d\n", lumps[i].offset);
	}
	char *lump_names[] = {"FLOORS", "WALLS", "SOUNDS", "MUSIC", "LINEDEFS", "SIDEDEFS", "THINGS", "SECTORS"};
	int count;
	for (int i = 0; i < head.num_lumps; i++){
		if (!strcmp(lump_names[i], "LINEDEFS")) {
			//read linedefs ( lumps[4] )
			unsigned char line_data[lumps[i].size];
			memcpy(line_data, wad_data + lumps[i].offset, lumps[i].size);
			memcpy(&count, line_data, sizeof(count));
			printf("line count %d\n", count);
			t_linedef line[count];
			int size = sizeof(*line) - sizeof(line->sectorized) - sizeof(line->sides);
			for (int j = 0; j < count; j++) {
				memcpy(&line[j], line_data + sizeof(count) + (j * size), size);
				printf("start x %.0f start y %.0f\n", line[j].start_vertex.x, line[j].start_vertex.y);
				printf("end x %.0f end y %.0f\n", line[j].end_vertex.x, line[j].end_vertex.y);
				printf("flag %hhx, special %d, tag %d\n", line[j].flags, line[j].special, line[j].tag);
			}
		}
		else if (!strcmp(lump_names[i], "SIDEDEFS")) {
			//read sidedefs ( lumps[5] )
			unsigned char side_data[lumps[i].size];
			memcpy(side_data, wad_data + lumps[i].offset, lumps[i].size);
			printf("side count %d\n", count);
			t_sidedef sides[2];
			int size = sizeof(sides);
			for (int j = 0; j < count; j++) {
				memcpy(sides, side_data + sizeof(count) + (j * size), size);
				for (int k = 0; k < 2; k++) {
					printf("side %d offset x %.0f y %.0f\n", k, sides[k].offset.x, sides[k].offset.y);
					printf("side %d sector number %u\n", k, sides[k].sector_num);
					for (int tex = 0; tex < 3; tex++)
						printf("side %d texture %s\n", k, sides[k].textures[tex]);

				}
			}
		}
		else if (!strcmp(lump_names[i], "THINGS")) {
			//read things ( lumps[6] )
			unsigned char thing_data[lumps[i].size];
			memcpy(thing_data, wad_data + lumps[i].offset, lumps[i].size);
			memcpy(&count, thing_data, sizeof(count));
			printf("thing count %d\n", count);
			t_thing thing[count];
			int size = sizeof(thing) - 4;
			for (int j = 0; j < count; j++) {
				memcpy(&thing[j], thing_data + sizeof(count) + (j * size), size);
				printf("name %s\n", thing[j].name);
				printf("angle %d\n", thing[j].angle);
				printf("flags %hhx\n", thing[j].flags);
				printf("x %.0f, y %.0f\n", thing[j].pos.x, thing[j].pos.y);
				printf("type %d\n", thing[j].type);
			}
		}
		else if (i < 4) printf("Skipping assets %s\n", lump_names[i]);
	}
	return 0;
}