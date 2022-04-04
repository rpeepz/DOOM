#include "demo.h"

void    get_file_info(t_resource *asset)
{
	char *name = asset->full_path;
	FILE *fp = fopen(name, "r");
	if (!fp) {
		asset->size = -1;
		return ;
	}
	/* find byte length by indexing to the end of the file */
	fseek(fp, 0L, SEEK_END);
	asset->size = ftell(fp);
	/* allocate space for raw data */
	asset->raw_data = malloc(sizeof(uint8_t) * asset->size);
	/* set seek to beginning of file and read into raw data pointer */
	fseek(fp, 0L, SEEK_SET);
	fread(asset->raw_data, sizeof(uint8_t), asset->size, fp);
	fclose(fp);
}

char *asset[4] = {
	"assets/floor/",
	"assets/wall/",
	"assets/allsounds/",
	"assets/allmusic/"
	};

t_resource_table    *gather_assets(int mode)
{
	t_resource_table *table;
	struct  dirent *sd;
	char    *asset_path;
	DIR     *dir;

	/* allocate resource table */
	table = malloc(sizeof(t_resource_table));
	memset(table, 0, sizeof(t_resource_table));
	/* assign asset path */
	asset_path = asset[mode];
	dir = opendir(asset_path);
	if (!dir) {
		dprintf(2, "Path '%s' not found\n", asset_path);
		return NULL;
	}
	/* itterate entries */
	while ((sd = readdir(dir)) != NULL)
	{
		/* ignore '.' files */
		if (sd->d_name[0] == '.')
			continue ;
		/* assign file name to resource */
		strcpy(table->table[table->size].name, sd->d_name);
		/* assign full path */
		strcat(table->table[table->size].full_path, asset_path);
		strcat(table->table[table->size].full_path, sd->d_name);
		/* read and store asset data */
		get_file_info(&table->table[table->size]);
		table->size++;
	}closedir(dir);
	return (table);
}
