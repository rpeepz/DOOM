#include "demo.h"
#include "map.h"

uint32_t	asset_size(t_resource_table *assets)
{
	uint32_t size = 0;
	for (int i = 0; i < assets->size; i++) {
		size += assets->table[i].size;
	}
	return size;
}

uint32_t	fill_assets(uint8_t *buffer, t_resource_table *table)
{
	uint32_t	offset = 0;
	uint32_t	size_last = 0;
	for (int i = 0; i < table->size; i++) {
		t_resource *resource = &table->table[i];
		memcpy(buffer + (i * 16) + size_last, resource->name, 16);
		memcpy(buffer + ((i + 1) * 16) + size_last, resource->raw_data, resource->size);
		size_last = resource->size;
		offset += size_last + 16;
	}
	return offset;
}

uint8_t	    *export_assets(uint8_t *buffer, t_lump *lumps, t_header *head, void *map_interface)
{
    t_map_interface *draw_mode = map_interface;
	static char *names[] = {"FLOORS", "WALLS", "SOUNDS", "MUSIC"};
	for (uint32_t i = head->num_lumps; i < (head->num_lumps + NK_LEN(names)); i++)
		snprintf(lumps[i].lump_name, sizeof(lumps[i].lump_name), "%s", names[i]);
	// extract asset info
	lumps[0].size = asset_size(draw_mode->floor);
	lumps[1].size = asset_size(draw_mode->wall);
	lumps[2].size = asset_size(draw_mode->sounds);
	lumps[3].size = asset_size(draw_mode->music);
	// get sizes of each asset table + 16 for each of the names
	size_t size = 0;
	size += (16 * (draw_mode->floor->size)) + lumps[0].size;
	size += (16 * (draw_mode->wall->size)) + lumps[1].size;
	size += (16 * (draw_mode->sounds->size)) + lumps[2].size;
	size += (16 * (draw_mode->music->size)) + lumps[3].size;
	// first malloc on buffer
	buffer = malloc(size); memset(buffer, 0, size);
	// fill buffer with names and resource info
	uint32_t offset = 0;
	offset += fill_assets(buffer + offset, draw_mode->floor);
	offset += fill_assets(buffer + offset, draw_mode->wall);
	offset += fill_assets(buffer + offset, draw_mode->sounds);
	offset += fill_assets(buffer + offset, draw_mode->music);
	head->num_lumps += NK_LEN(names);
	head->lump_offset += size;
    return buffer;
}
