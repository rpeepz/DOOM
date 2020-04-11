#include "demo.h"
#include "list.h"
#include "map.h"

uint32_t	item_size(t_bank *bank, int mode)
{
	uint32_t size;
	t_item_node *item;
	item = mode == 2 ? bank->head_thing : bank->head_thing;
	if (mode == 2) {
		size = (sizeof(*item->thing) - sizeof(struct nk_color)) * bank->count_thing;
		size += sizeof(bank->count_thing);
	}
	else {
		uint32_t sector_size = sizeof(item->line->sides) + sizeof(item->line->sectorized);
		if (!mode) {
			size = (sizeof(*item->line) - sector_size) * bank->count_line;
			size += sizeof(bank->count_line);
		} else
			size = sizeof(item->line->sides) * bank->count_line;
	}
	return size;
}

uint32_t	fill_map(uint8_t *buffer, t_bank *bank)
{
	t_item_node *item;
	uint32_t offset = sizeof(bank->count_line);
	uint32_t sector_size = sizeof(item->line->sides) + sizeof(item->line->sectorized);
	memcpy(buffer, &bank->count_line, sizeof(bank->count_line));
	for (item = bank->head_line; item; item = item->next) {
		memcpy(buffer + offset, item->line, sizeof(*item->line) - sector_size);
		offset += sizeof(*item->line) - sector_size;
	}
	memcpy(buffer + offset, &bank->count_thing, sizeof(bank->count_thing));
	offset += sizeof(bank->count_thing);
	for (item = bank->head_thing; item; item = item->next) {
		memcpy(buffer + offset, item->thing, sizeof(*item->thing) - sizeof(struct nk_color));
		offset += sizeof(*item->thing) - sizeof(struct nk_color);
	}
	return offset;
}

uint32_t	fill_sectors(uint8_t *buffer, t_sector *sector)
{
	uint32_t offset = 0;
	for (int i = 0; sector[i].line_count; i++) {
		memcpy(buffer + offset, &sector[i].sector_num, sizeof(sector[i].sector_num));
		offset += sizeof(sector[i].sector_num);
		memcpy(buffer + offset, &sector[i].sector_info, sizeof(sector[i].sector_info));
		offset += sizeof(sector[i].sector_info);
	}
	return offset;
}

uint8_t		*export_map(uint8_t *buffer, t_lump *lumps, t_header *head, void *map_interface)
{
    t_map_interface *draw_mode = map_interface;
	static char *names[] = {"LINEDEFS", "SIDEDEFS", "THINGS", "SECTORS"};
	for (uint32_t i = head->num_lumps; i < (head->num_lumps + NK_LEN(names)); i++)
		snprintf(lumps[i].lump_name, sizeof(lumps[i].lump_name), "%s", names[i - head->num_lumps]);
	// extract map info
	// get sizes of each new lump
	size_t size = 0;
	for (int i = 0; i < 3; i++) {
		lumps[head->num_lumps + i].size = item_size(draw_mode->bank, i);
		size += lumps[head->num_lumps + i].size;
	}
	uint32_t sec_size = 0;
	for (int i = 0; draw_mode->sectors->sectors[i].line_count; i++)
		sec_size += sizeof(draw_mode->sectors->sectors->sector_info) + sizeof (draw_mode->sectors->sectors->sector_num);
	lumps[head->num_lumps + 3].size = sec_size;
	size += sec_size;

	// space to assign VERTEX lump

	// second malloc on buffer
	uint32_t offset = head->lump_offset - sizeof(*head);
	uint8_t	*tmp = malloc(size + offset); memset(tmp, 0, size + offset);
	memcpy(tmp, buffer, offset);
	free(buffer);
	buffer = tmp;
	// fill buffer with map info
	offset += fill_map(buffer + offset, draw_mode->bank);
	offset += fill_sectors(buffer + offset, draw_mode->sectors->sectors);
	head->num_lumps += NK_LEN(names);
	head->lump_offset += size;
    return buffer;
}
