#include "demo.h"
// #include "map.h"

void	get_lumps(uint8_t *buffer, t_map_interface *draw_mode, t_lump *lumps, t_header *head)
{
	// asset lumps
	buffer = export_assets(buffer, lumps, head, (void*)draw_mode);
	buffer = export_map(buffer, lumps, head, (void*)draw_mode);
//	LUMP OUTPUT TEST
	dprintf(2, "writing current lump buffer to stdout\n");
	for (uint32_t i = 0; i < head->lump_offset - sizeof(*head); i++) {
		printf("%c", buffer[i]);
	}

	dprintf(2, "num lumps %u\n", head->num_lumps);
	for (int i = 0; i < head->num_lumps; i++)
		dprintf(2, "\t%s\n", lumps[i].lump_name);
	dprintf(2, "current size %u\n", head->lump_offset);
}

void	fill_header(uint8_t *buffer, t_header *head)
{
	memcpy(buffer, head->name, 4);
	memcpy(buffer + 4, &head->num_lumps, 4);
	memcpy(buffer + 8, &head->lump_offset, 4);
}


void    export_wad(t_map_interface *draw_mode)
{
	t_header	head;
	t_lump		lumps[16];
/*
	asset lumps	[0 - 3]
	lump_wall	lump_floor	lump_sound	lump_music
	map lumps	[4 - 8]
	linedefs	sidedefs	sectors	things
	bsp lumps	[9 - 11]
	nodes		segs		ssectors
*/
	uint8_t		*lumps_buf;
	// int			fd;

	// fd = open("output.wad", O_RDWR | O_CREAT, 0666);
	// if (fd < 3) return (dprintf(2, "EXPORT PATH ERROR\n"));
	memcpy(head.name, "IWAD", 4);
	head.num_lumps = 0;
	head.lump_offset = sizeof(head);
	get_lumps(lumps_buf, draw_mode, lumps, &head);

	uint8_t		header_buf[sizeof(head)];
	fill_header(header_buf, &head);
}
