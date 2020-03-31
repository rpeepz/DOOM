#include "demo.h"
#include "map.h"

void    export_map(t_map_interface *draw_mode)
{

	// int fd = open("output.wad", O_RDWR | O_CREAT, 0666);
	// if (fd < 3)
	// 	return (dprintf(2, "EXPORT PATH ERROR\n"));
	t_header			head;
	strcat(head.name, "IWAD");
	// size += write(fd, head.name, 4);
	t_lump				lump_floor, lump_wall;
	for (int i = 0; i < 2; i++) {
		uint32_t size = 0;
		t_resource_table *assets = i ? draw_mode->wall : draw_mode->floor;
		t_resource asset;
		for (int i = 0; i < assets->size; i++) {
			asset = assets->table[i];
			size += asset.size;
		}
		if (i) {
			strcat(lump_wall.lump_name, "Walls\n");
			lump_wall.size = size;
		} else {
			strcat(lump_wall.lump_name, "Floors\n");
			lump_floor.size = size;
		}
	}
}
