#include "demo.h"
#include "map.h"

uint32_t	asset_size(t_resource_table *assets)
{
	if (!assets) return 0;
	uint32_t size = 0;
	for (int i = 0; i < assets->size; i++) {
		size += assets->table[i].size;
		size += sizeof(assets->table->name);
	}
	return size;
}

uint32_t	fill_assets(uint8_t *buffer, t_resource_table *assets)
{
	if (!assets) return 0;
	uint32_t	offset = 0;
	uint32_t	size_last = 0;
	for (int i = 0; i < assets->size; i++) {
		t_resource *resource = &assets->table[i];
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
	t_resource_table *asset;
	size_t size = 0;
	for (int i = 0; i < NK_LEN(names); i++) {
		if (i == 0) asset = draw_mode->floor;
		else if (i == 1) asset = draw_mode->wall;
		else if (i == 2) asset = draw_mode->sounds;
		else if (i == 3) asset = draw_mode->music;
		else asset = NULL;
		lumps[i].size = asset_size(asset);
		size += lumps[i].size;
		if (!i) lumps[i].offset = 0;
		else lumps[i].offset = lumps[i - 1].offset + lumps[i - 1].size;
	}
	// first malloc on buffer
	buffer = malloc(size); memset(buffer, 0, size);
	// fill buffer with names and resource info
	uint32_t offset = 0;
	for (int i = 0; i < NK_LEN(names); i++) {
		if (i == 0) asset = draw_mode->floor;
		else if (i == 1) asset = draw_mode->wall;
		else if (i == 2) asset = draw_mode->sounds;
		else if (i == 3) asset = draw_mode->music;
		else asset = NULL;
		offset += fill_assets(buffer + offset, asset);
	}
	head->num_lumps += NK_LEN(names);
	head->lump_offset += size;
    return buffer;
}
