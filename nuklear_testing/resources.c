#include "demo.h"

int get_file_size(char *name)
{
    FILE *fp = fopen(name, "r");
    if (!fp) return (-1);
    fseek(fp, 0L, SEEK_END);
    long int res = ftell(fp);
    fclose(fp);
    return (res);
}

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
    if (mode == 0) asset_path = "assets/floor/";
    else asset_path = "assets/wall/";
    dir = opendir(asset_path);
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
        /* assign file size in bytes */
        table->table[table->size].size = get_file_size(table->table->full_path);
        //table->table->raw_data = get_bytes_from_file
        table->size++;
    }
    if (dir) closedir(dir);
    return (table);
}
