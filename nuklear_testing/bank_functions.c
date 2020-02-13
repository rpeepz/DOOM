#include "list.h"
#include <string.h>
#define THICKNESS 1.0f
#define SNAP 10

int stroke_my_line( struct nk_command_buffer *b, t_item_node *line_node)
{
	nk_stroke_line(b,
		line_node->line->start_vertex.x + 1, line_node->line->start_vertex.y + 2,
		line_node->line->end_vertex.x + 1, line_node->line->end_vertex.y + 2,
		THICKNESS, line_node->color);
	return(0);
}

t_float_pair	snap(struct nk_vec2 v)
{
	t_float_pair res;

	int x = (int)v.x % SNAP;
	int y = (int)v.y % SNAP;
	if (x >= 5) res.x = v.x + (SNAP - x);
	else res.x = v.x - x;
	if (y >= 5) res.y = v.y + (SNAP - y);
	else res.y = v.y - y;
	/* check lower bounds */
	if (res.y < 30) res.y = 30;
	if (res.x < 10) res.x = 10;
	/* check upper bounds */
	if (res.y >= WINDOW_HEIGHT - WINDOW_OFFSET) res.y = 890;
	if (res.x >= (WINDOW_WIDTH - (WINDOW_WIDTH / 4)) - WINDOW_OFFSET) res.x = 1190;
	return (res);
}

void	add_line(t_bank *bank, struct nk_vec2 start, struct nk_vec2 end)
{
	t_item_node *new;
	/* snap input coordinates */
	t_float_pair st = snap(start);
	t_float_pair en = snap(end);
	/* return if both points in same location */
	if (st.x == en.x && st.y == en.y)
		return ;
	/* return if duplicate line found */
	for (new = bank->head_line; new; new = new->next) {
		if ((st.x == new->line->start_vertex.x &&
		st.y == new->line->start_vertex.y &&
		en.x == new->line->end_vertex.x &&
		en.y == new->line->end_vertex.y) ||
		(st.x == new->line->end_vertex.x &&
		st.y == new->line->end_vertex.y &&
		en.x == new->line->start_vertex.x &&
		en.y == new->line->start_vertex.y))
		return ;
	}
	//allocate memory for new node
	new = malloc(sizeof(t_item_node));
	memset(new, 0, sizeof(t_item_node));
	new->color = HIGHLIGHT;
	new->line = malloc(sizeof(t_linedef));
	memset(new->line, 0, sizeof(t_linedef));
	//set values from args
	new->line->start_vertex = st;
	new->line->end_vertex = en;

	//add to list 
	if (bank->count_line == 0) bank->tail_line = bank->head_line = new;
	else
	{
		new->prev = bank->tail_line;
		bank->tail_line->next = new;
		bank->tail_line = new;
	}
	//make the new line as bank's current selected
	if (bank->selected)
		bank->selected->color = LINE_COLOR;
	bank->selected = new;
	bank->count_line++;
}

void	add_thing(t_bank *bank, struct nk_vec2 location, char *name)
{
	t_item_node	*new;
	/* snap input coordinates */
	t_float_pair pos = snap(location);
	/* return if duplicate thing found */
	for (new = bank->head_thing; new; new = new->next)
		if (pos.x == new->thing->pos.x && pos.y == new->thing->pos.y) return ;
	//allocate memory for new node
	new = malloc(sizeof(t_item_node));
	memset(new, 0, sizeof(t_item_node));
	new->color = HIGHLIGHT;
	new->thing = malloc(sizeof(t_thing));
	memset(new->thing, 0, sizeof(t_thing));
	memcpy(new->thing->name, name, strlen(name));
	//set values from args
	new->thing->pos = pos;

	//add to list 
	if (bank->count_thing == 0) bank->tail_thing = bank->head_thing = new;
	else
	{
		new->prev = bank->tail_thing;
		bank->tail_thing->next = new;
		bank->tail_thing = new;
	}
	//make the new thing as bank's current selected
	if (bank->selected)
		bank->selected->color = THING_COLOR;
	bank->selected = new;
	bank->count_thing++;
}

void	remove_from_bank(t_bank *bank, int item)
{
	t_item_node *node = bank->selected;
	if (node)
	{
		change_selected(bank, item, 0);
		//decrement count
		(item == ITEM_LINE) ?
			bank->count_line-- : bank->count_thing--;
		//unlink
		if (item == ITEM_LINE && bank->count_line == 0) {
			bank->head_line = NULL;
			bank->tail_line = NULL;
			bank->selected = bank->tail_thing;
		} else if (item == ITEM_THING && bank->count_thing == 0) {
			bank->head_thing = NULL;
			bank->tail_thing = NULL;
			bank->selected = bank->head_thing;
		} else if ((item == ITEM_LINE ?
			bank->head_line : bank->head_thing) == node) {
			if (item == ITEM_LINE) {
				bank->head_line = node->next;
				bank->head_line->prev = NULL;
			} else {
				bank->head_thing = node->next;
				bank->head_thing->prev = NULL;
			}
		} else if ((item == ITEM_LINE ?
			bank->tail_line : bank->tail_thing) == node) {
			if (item == ITEM_LINE) {
				bank->tail_line = node->prev;
				bank->tail_line->next = NULL;
			} else {
				bank->tail_thing = node->prev;
				bank->tail_thing->next = NULL;
			}
		} else {
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
		//free
		if (node->line) free(node->line);
		if (node->thing) free(node->thing);
		free(node);
	}
}

void change_selected(t_bank *bank, int item, int direction)
{
	t_item_node *node = bank->selected;
	if (node)
	{
		// one element in list
		if ((item == ITEM_LINE ? bank->count_line : bank->count_thing) == 1)
			return ;
		// unhighlight selected
		node->color = (item == ITEM_LINE) ? LINE_COLOR : THING_COLOR;
		// at head
		if ((item == ITEM_LINE ? bank->head_line : bank->head_thing) == node)
			bank->selected = (direction == 1) ?
			node->next : (item == ITEM_LINE) ?
			bank->tail_line : bank->tail_thing;
		// at tail
		else if ((item == ITEM_LINE ? bank->tail_line : bank->tail_thing) == node)
			bank->selected = (direction == 1) ?
			(item == ITEM_LINE) ?
			bank->head_line : bank->head_thing :
			node->prev;
		else
			bank->selected = (direction == 1) ?
			bank->selected->next : bank->selected->prev;
		// highlight new selected
		bank->selected->color = HIGHLIGHT;
	}
	// if selection is valid unhighlight the current one
	// should be circular so you can't fall off the list and not be able to get back
	// highlight selected line
	// linebank.selected->color = nk_rgb(255, 0, 0)
}
