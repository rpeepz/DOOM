#include "demo.h"
#include "map.h"

uint8_t	*get_lumps(uint8_t *buffer, t_map_interface *draw_mode, t_lump *lumps, t_header *head)
{
	// asset lumps
	buffer = export_assets(buffer, lumps, head, (void*)draw_mode);
	buffer = export_map(buffer, lumps, head, (void*)draw_mode);
//	LUMP OUTPUT TEST
	// dprintf(2, "writing current lump buffer to stdout\n");
	// for (uint32_t i = 0; i < head->lump_offset - sizeof(*head); i++) {
	// 	printf("%c", buffer[i]);
	// }

	dprintf(2, "num lumps %u\n", head->num_lumps);
	for (int i = 0; i < head->num_lumps; i++) {
		dprintf(2, "\t%s\n", lumps[i].lump_name);
		// dprintf(2, "offset [%d]\n", lumps[i].offset);
		// dprintf(2, "size   [%d]\n", lumps[i].size);
	}
	dprintf(2, "current size %u\n", head->lump_offset);
	return buffer;
}

void    export_wad(t_map_interface *draw_mode)
{
	t_header	head;
/*
	asset lumps	[0 - 3]
	lump_wall	lump_floor	lump_sound	lump_music
	map lumps	[4 - 8]
	linedefs	sidedefs	sectors		things
	bsp lumps	[9 - 12]
	vertexes	nodes		segs		ssectors
*/
	t_lump		lumps[16];
	uint8_t		*lumps_buf = 0;
	int			fd;

	char map_name[sizeof(draw_mode->map_name) + 4];
	bzero(map_name, sizeof(map_name));
	strcat(map_name, draw_mode->map_name[0] ? draw_mode->map_name : "Unnamed");
	strcat(map_name, ".wad");
	if (check_exists(draw_mode->map_name, WAD))
		remove(map_name);
	fd = open(map_name, O_RDWR | O_CREAT, 0666);
	if (fd < 3) {
		dprintf(2, "EXPORT PATH ERROR\n");
		return ;
	}
	memcpy(head.name, "IWAD", 4);
	head.num_lumps = 0;
	head.lump_offset = 0;
	// TODO read sectors
	// create nodes
	// create bsp creator
	// create segments
	// create sub sectors
	lumps_buf = get_lumps(lumps_buf, draw_mode, lumps, &head);

	write(fd, &head, sizeof(head));
	write(fd, lumps_buf, head.lump_offset);
	// free(lumps_buf);
	for (int i = 0; i < head.num_lumps; i++)
		write(fd, &lumps[i], sizeof(lumps[i]));
}
