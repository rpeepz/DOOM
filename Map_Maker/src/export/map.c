#include "demo.h"
#include "list.h"
#include "map.h"

uint32_t	item_size(t_bank *bank, int mode)
{
	uint32_t size;
	t_item_node *item;
	item = mode == 2 ? bank->head_thing : bank->head_line;
	if (mode != 2) {
		uint32_t sector_size = sizeof(item->line->sides) + sizeof(item->line->sectorized);
		if (!mode) {
			size = sizeof(bank->count_line);
			size += (sizeof(*item->line) - sector_size) * bank->count_line;
		} else
			size = sizeof(item->line->sides) * bank->count_line;
	}
	else {
		size = sizeof(bank->count_thing);
		size += (sizeof(*item->thing) - sizeof(struct nk_color)) * bank->count_thing;
	}
	return size;
}

int			check_vertex_exists(t_float_pair *v, t_float_pair check, int max_v)
{
	for (int i = 0; i < max_v; i++)
		if (v[i].x == check.x && v[i].y == check.y)
			return 1;
	return 0; //does not exist
}

uint32_t	count_vertexes(t_bank *bank, t_float_pair *vertexes)
{
	uint32_t n = 0;
	int max_v = bank->count_line * 2;
	for (t_item_node *line = bank->head_line; line; line = line->next) {
		if (!check_vertex_exists(vertexes, line->line->start_vertex, max_v))
			vertexes[n++] = line->line->start_vertex;
		if (!check_vertex_exists(vertexes, line->line->end_vertex, max_v))
			vertexes[n++] = line->line->end_vertex;
	}
	return n;
}

uint32_t	fill_vertex(uint8_t *buffer, t_float_pair *v, uint32_t n)
{
	uint32_t offset;
	memcpy(buffer, &n, sizeof(n));
	offset = sizeof(n);
	for (int i = 0; i < n; i++) {
		memcpy(buffer + offset, &v[i], sizeof(v[i]));
		offset += sizeof(v[i]);
	}
	return offset;
}

uint32_t	fill_map(uint8_t *buffer, t_bank *bank)
{
	t_item_node *item;
	uint32_t offset;
	uint32_t sector_size = sizeof(item->line->sides) + sizeof(item->line->sectorized);
	//add lines
	memcpy(buffer, &bank->count_line, sizeof(bank->count_line));
	offset = sizeof(bank->count_line);
	for (item = bank->head_line; item; item = item->next) {
		memcpy(buffer + offset, item->line, sizeof(*item->line) - sector_size);
		offset += sizeof(*item->line) - sector_size;
	}
	for (item = bank->head_line; item; item = item->next) {
		memcpy(buffer + offset, item->line->sides, sizeof(item->line->sides));
		offset += sizeof(item->line->sides);
	}
	//add things
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
	int count = 0;
	while (sector[count].line_count) ++count;
	memcpy(buffer, &count, sizeof(count));
	offset += sizeof(count);
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
	t_float_pair vertexes[draw_mode->bank->count_line * 2];
	uint32_t v_count;
	static char *names[] = {"LINEDEFS", "SIDEDEFS", "THINGS", "SECTORS", "VERTEXES"};
	for (uint32_t i = head->num_lumps; i < (head->num_lumps + NK_LEN(names)); i++)
		snprintf(lumps[i].lump_name, sizeof(lumps[i].lump_name), "%s", names[i - head->num_lumps]);
	// extract map info
	size_t size = 0;
	for (int i = 0; i < NK_LEN(names); i++) {
		if (i < 3) {
			lumps[head->num_lumps + i].size = item_size(draw_mode->bank, i);
			size += lumps[head->num_lumps + i].size;
		}
		else if (!strcmp(names[i], "SECTORS")) {
			uint32_t sec_size = sizeof(int);
			for (int i = 0; draw_mode->sectors->sectors[i].line_count; i++)
				sec_size += sizeof(draw_mode->sectors->sectors->sector_info) + sizeof(draw_mode->sectors->sectors->sector_num);
			lumps[head->num_lumps + i].size = sec_size;
			size += sec_size;
		}
		else if (!strcmp(names[i], "VERTEXES")) {
			//get count of vertexes
			bzero(vertexes, sizeof(vertexes));
			v_count = count_vertexes(draw_mode->bank, vertexes);
			uint32_t v_size = (v_count * sizeof(t_float_pair)) + sizeof(v_count);
			lumps[head->num_lumps + i].size = v_size;
			size += v_size;
		}
		lumps[head->num_lumps + i].offset = lumps[head->num_lumps + i - 1].offset + lumps[head->num_lumps + i - 1].size;
	}
	// second malloc on buffer
	uint32_t offset = head->lump_offset;
	uint8_t	*tmp = malloc(size + offset); memset(tmp, 0, size + offset);
	memcpy(tmp, buffer, offset);
	free(buffer);
	buffer = tmp;
	// fill buffer with map info
	offset += fill_map(buffer + offset, draw_mode->bank);
	offset += fill_sectors(buffer + offset, draw_mode->sectors->sectors);
	offset += fill_vertex(buffer + offset, vertexes, v_count);
	head->num_lumps += NK_LEN(names);
	head->lump_offset += size;
    return buffer;
}
