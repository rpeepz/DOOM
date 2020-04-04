#include "demo.h"
#include "list.h"

void    open_map_list(t_map_interface *draw_mode, int *open_map)
{
	struct nk_context *ctx = draw_mode->ctx;
	nk_window_set_focus(ctx, "Open");
	struct nk_rect s = nk_rect((draw_mode->win_w * .05),
	(draw_mode->win_h * .15), 200, (draw_mode->win_h / 3));
	if (nk_begin(ctx, "Open", s, NK_WINDOW_BORDER)) {
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Map select", NK_TEXT_CENTERED);
		// spacing between rows
		nk_layout_row_dynamic(ctx, 10, 1);
		nk_label(ctx, " ", 1);
		/* print out map selections */
		nk_layout_row_dynamic(ctx, 25, 2);
		DIR *dir = opendir(MAP_SAVE_PATH);
		struct dirent *sd;
		while (dir && (sd = readdir(dir))) {
			if (sd->d_name[0] == '.') continue ;
			if (nk_button_label(ctx, "Select")) {
				if (!load_map(draw_mode, sd->d_name)) {
					printf("Opened %s\n", sd->d_name);
					strcpy(draw_mode->map_name, sd->d_name);
					*open_map = nk_false;
					break ;
				}
			}
			nk_label(ctx, sd->d_name, NK_TEXT_CENTERED);
		}
		if (dir) closedir(dir);
		nk_layout_row_dynamic(ctx, 10, 1);
		nk_label(ctx, " ", 1);
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_button_label(ctx, "Cancel"))
			*open_map = nk_false;
	}
	nk_end(ctx);

}

int     load_map(t_map_interface *draw_mode, char *name)
{
	int     fd;
	char    path[strlen(MAP_SAVE_PATH) + sizeof(draw_mode->map_name)] = {0};
	strcat(path, MAP_SAVE_PATH);
	strcat(path, name);
	printf("Opening %s ...\n", name);
	fd = open(path, O_RDONLY);
	if (fd < 3)
		return (dprintf(2, "MAP PATH ERROR\n"));
	/* read header */
	char head[4];
	read(fd, &head, sizeof(head));
	if (strncmp("DWD\n", head, 4))
		return (dprintf(2, "Improper header\n"));
	t_bank *bank = draw_mode->bank;
	bzero(bank, sizeof(*bank));
	for (int i = 0; i < 2; i++) {
		int index;
		int count;
		/* read lump index */
		read(fd, &index, sizeof(index));
		/* read item count */
		read(fd, &count, sizeof(count));
		if (count < 0 || count > 4096)
			return (dprintf(2, "Invalid %s bank count %d\n", i ? "thing" : "line", count));
		for (int j = 0; j < count; j++) {
			struct nk_color color;
			read(fd, &color, sizeof(color));
			if (i == 0) {
				struct nk_vec2 start;
				struct nk_vec2 end;
				/* read line data */
				read(fd, &start, sizeof(start));
				read(fd, &end, sizeof(end));
				add_line(draw_mode, start, end);
				t_linedef *line = bank->tail_line->line;
				read(fd, &line->special, sizeof(line->special));
				read(fd, &line->tag, sizeof(line->tag));
				read(fd, &line->flags, sizeof(line->flags));
				for (int k = 0; k < 2; k++) {
					read(fd, &line->sides[k].offset, sizeof(line->sides[k].offset));
					read(fd, &line->sides[k].textures, sizeof(line->sides[k].textures));
					read(fd, &line->sides[k].sector_num, sizeof(line->sides[k].sector_num));
				/*
					read(fd, &line->sides[k].sector_info.flats[0], sizeof(line->sides[k].sector_info.flats[0]));
					read(fd, &line->sides[k].sector_info.flats[1], sizeof(line->sides[k].sector_info.flats[1]));
					read(fd, &line->sides[k].sector_info.light, sizeof(line->sides[k].sector_info.light));
					read(fd, &line->sides[k].sector_info.special, sizeof(line->sides[k].sector_info.special));
					read(fd, &line->sides[k].sector_info.tag, sizeof(line->sides[k].sector_info.tag));
					read(fd, &line->sides[k].sector_num, sizeof(line->sides[k].sector_num));
				*/
				}
			} else {
				/* read thing data */
				struct nk_vec2 location;
				char name[16];
				read(fd, &location, sizeof((location)));
				read(fd, name, sizeof((name)));
				int q = 0;
				for (int p = 0; p < 16; p++) {
					if (name[p]) {
						name[q++] = name[p];
						name[p] = 0;
					}
				}
				add_thing(draw_mode, location, name);
				t_thing *thing = bank->tail_thing->thing;
				read(fd, &thing->angle, sizeof((thing->angle)));
				read(fd, &thing->type, sizeof((thing->type)));
				read(fd, &thing->flags, sizeof((thing->flags)));
				read(fd, &thing->color, sizeof((thing->color)));
			}
		}
	}
	return (0);
}
