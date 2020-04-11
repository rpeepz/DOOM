#include "demo.h"
#include "map.h"


uint8_t		*export_map(uint8_t *buffer, t_lump *lumps, t_header *head, void *map_interface)
{
    t_map_interface *draw_mode = map_interface;
	static char *names[] = {"VERTEXES", "LINEDEFS", "SIDEDEFS", "SECTORS", "THINGS"};
	for (uint32_t i = head->num_lumps; i < (head->num_lumps + NK_LEN(names)); i++) {
		// TODO
		// find here here when indexing past 5
		// index thru lumps[i]
		dprintf(2, "i = %u\n", i);
		snprintf(lumps[i].lump_name, sizeof(lumps[i].lump_name), "%s", names[i]);
	}
	// extract map info
	// lumps[0].size = asset_size(draw_mode->floor);
	// lumps[1].size = asset_size(draw_mode->wall);
	// lumps[2].size = asset_size(draw_mode->sounds);
	// lumps[3].size = asset_size(draw_mode->music);
	// get sizes of each new lump
	size_t size = 0;
	// size += (16 * (draw_mode->floor->size)) + lumps[0].size;
	// size += (16 * (draw_mode->wall->size)) + lumps[1].size;
	// size += (16 * (draw_mode->sounds->size)) + lumps[2].size;
	// size += (16 * (draw_mode->music->size)) + lumps[3].size;
	// second malloc on buffer
	uint32_t offset = head->lump_offset - sizeof(*head);
	uint8_t	*tmp = malloc(size + offset); memset(tmp, 0, size + offset);
	memcpy(tmp, buffer, offset);
	free(buffer);
	buffer = tmp;
	// fill buffer with map info
	// offset += fill_assets(buffer + offset, draw_mode->floor);
	// offset += fill_assets(buffer + offset, draw_mode->wall);
	// offset += fill_assets(buffer + offset, draw_mode->sounds);
	// offset += fill_assets(buffer + offset, draw_mode->music);
	head->num_lumps += NK_LEN(names);
	head->lump_offset += size;
    return buffer;
}
