#include "demo.h"
#include "list.h"

void    save_map_name(t_map_interface *draw_mode, int *save_as)
{
	struct nk_context *ctx = draw_mode->ctx;
	nk_window_set_focus(ctx, "Save as");
	struct nk_rect s = nk_rect((draw_mode->win_w * .05),
	(draw_mode->win_h * .15), 200, 125);
	if (nk_begin(ctx, "Save as", s, NK_WINDOW_BORDER)) {
		//submit buffer
		static char buffer2[20];
		//edit buffer
		char buffer[20];
		int len = snprintf(buffer, 20, "%s", buffer2);
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Name Map", NK_TEXT_CENTERED);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 20, nk_filter_ascii);

		buffer[len] = 0;
		memcpy(buffer2, buffer, 20);
		// spacing between rows
		nk_layout_row_dynamic(ctx, 10, 1);
		nk_label(ctx, " ", 1);
		/* buttons to cancel, or add thing with or without name */
		nk_layout_row_dynamic(ctx, 25, 2);
		if (nk_button_label(ctx, "OK") || (nk_input_is_key_pressed(&ctx->input, NK_KEY_ENTER))) {
			if (buffer2[0]) {
				strcpy(draw_mode->map_name, buffer2);
				memset(buffer2, 0, 16);
				*save_as = nk_false;
				printf("\nSaving ...\n");
				if (!save(draw_mode))
					printf("Saved as %s\n", draw_mode->map_name);
				else printf("save error\n");
			}
		}
		if (nk_button_label(ctx, "Cancel")) {
			memset(buffer2, 0, 16);
			*save_as = nk_false;
		}
	}
	nk_end(ctx);
}

int     save(t_map_interface *draw_mode)
{
	t_item_node *item;
	t_bank      *bank = draw_mode->bank;
	int         fd;
	int         size = 0;
	char path[strlen(MAP_SAVE_PATH) + sizeof(draw_mode->map_name)] = {0};
	strcat(path, MAP_SAVE_PATH);
	strcat(path, draw_mode->map_name);
	fd = open(path, O_RDWR | O_CREAT, 0666);
	if (fd < 3)
		return (dprintf(2, "MAP PATH ERROR\n"));
	/* write header */
	size += write(fd, "DWD\n", 4);
	/* write item info */
	for (int i = 0; i < 2; i++) {
		int count = i ? bank->count_thing : bank->count_line;
		/* write lump index */
		size += write(fd, &i, sizeof(i));
		/* write item count */
		size += write(fd, &count, sizeof(count));
		item = i ? bank->head_thing : bank->head_line;
		for (int j = 0 ; j < count; j++) {
			size += write(fd, &item->color, sizeof(item->color));
			if (i == 0) {
				/* write line data */
				size += write(fd, &item->line->start_vertex, sizeof(item->line->start_vertex));
				size += write(fd, &item->line->end_vertex, sizeof(item->line->end_vertex));
				size += write(fd, &item->line->special, sizeof(item->line->special));
				size += write(fd, &item->line->tag, sizeof(item->line->tag));
				size += write(fd, &item->line->flags, sizeof(item->line->flags));
				for (int k = 0; k < 2; k++) {
					size += write(fd, &item->line->sides[k].offset, sizeof(item->line->sides[k].offset));
					size += write(fd, &item->line->sides[k].textures, sizeof(item->line->sides[k].textures));
					size += write(fd, &item->line->sides[k].sector_num, sizeof(item->line->sides[k].sector_num));
				/*
					size += write(fd, &item->line->sides[k].sector_info.flats[0], sizeof(item->line->sides[k].sector_info.flats[0]));
					size += write(fd, &item->line->sides[k].sector_info.flats[1], sizeof(item->line->sides[k].sector_info.flats[1]));
					size += write(fd, &item->line->sides[k].sector_info.light, sizeof(item->line->sides[k].sector_info.light));
					size += write(fd, &item->line->sides[k].sector_info.special, sizeof(item->line->sides[k].sector_info.special));
					size += write(fd, &item->line->sides[k].sector_info.tag, sizeof(item->line->sides[k].sector_info.tag));
					size += write(fd, &item->line->sides[k].sector_num, sizeof(item->line->sides[k].sector_num));
				*/
				}
			} else {
				// write thing data
				size += write(fd, &item->thing->pos, sizeof((item->thing->pos)));
				size += write(fd, &item->thing->name, sizeof((item->thing->name)));
				size += write(fd, &item->thing->angle, sizeof((item->thing->angle)));
				size += write(fd, &item->thing->type, sizeof((item->thing->type)));
				size += write(fd, &item->thing->flags, sizeof((item->thing->flags)));
				size += write(fd, &item->thing->color, sizeof((item->thing->color)));

			}
			item = item->next;
		}
	}
	close(fd);
	return (0);
}
