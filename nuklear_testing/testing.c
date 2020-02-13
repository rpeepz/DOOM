#include "list.h"
#include <string.h>
#define THICKNESS 1.0f
#define SNAP 10

int stroke_my_line( struct nk_command_buffer *b, t_line_node *node){
	nk_stroke_line(b, node->line.start_vertex.x + 1, node->line.start_vertex.y + 2,
					node->line.end_vertex.x + 1, node->line.end_vertex.y + 2,
					THICKNESS, node->color);
	return(0);
}

t_float_pair	snap(struct nk_vec2 v)
{
	t_float_pair res;

	int x = (int)v.x % SNAP;
	int y = (int)v.y % SNAP;
	if (x >= 5)
		res.x = v.x + (SNAP - x);
	else
		res.x = v.x - x;
	if (y >= 5)
		res.y = v.y + (SNAP - y);
	else
		res.y = v.y - y;
	if (res.y < 30) res.y = 30;
	if (res.x < 10) res.x = 10;

	if (res.y >= WINDOW_HEIGHT - WINDOW_OFFSET) res.y = 890;
	if (res.x >= (WINDOW_WIDTH - (WINDOW_WIDTH / 4)) - WINDOW_OFFSET) res.x = 1190;
	return (res);
}

void	add_line(t_line_bank *linebank, struct nk_vec2 start, struct nk_vec2 end){
	if (start.x == end.x && start.y == end.y)
		return ;
	t_line_node *new;

	/* return if duplicate line found */
	t_float_pair st = snap(start);
	t_float_pair en = snap(end);
	for (new = linebank->head; new; new = new->next) {
		if ((st.x == new->line.start_vertex.x &&
		st.y == new->line.start_vertex.y &&
		en.x == new->line.end_vertex.x &&
		en.y == new->line.end_vertex.y) ||
		(st.x == new->line.end_vertex.x &&
		st.y == new->line.end_vertex.y &&
		en.x == new->line.start_vertex.x &&
		en.y == new->line.start_vertex.y))
		return ;
	}
	new = (t_line_node*)malloc(sizeof(t_line_node));
	memset(new, 0, sizeof(t_line_node));
	new -> color = nk_rgb(255, 140, 60);
	new->line.start_vertex = snap(start);
	new->line.end_vertex = snap(end);

	if(linebank->count == 0) {
		linebank->tail = linebank->head = new;
	} else {
		new->prev = linebank->tail;
		linebank->tail->next = new;
		linebank->tail = new;
	}
	if (linebank->selected)
		linebank->selected->color = nk_rgb(10, 10, 10);
	linebank->selected = new;
	linebank->count++;

	//allocate memory for new node
	//set values from args
	//add to list 
	//make that line the current selected
}

void remove_line(t_line_bank *linebank)
{
	t_line_node *node;

	node = linebank->selected;
	change_line_selected(linebank, 0);
	if (node){
		//decrement count
		linebank->count--;
		//unlink
		if (linebank->count == 0)
		{
			memset(linebank, 0, sizeof(*linebank));
		}
		else if (node == linebank->head)
		{
			linebank->head = node->next;
			linebank->head->prev = NULL;
		}
		else if (node == linebank->tail)
		{
			linebank->tail = node->prev;
			linebank->tail->next = NULL;
		}
		else
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
		//free
		free(node);
	}
}

void change_line_selected(t_line_bank *linebank, int direction){
	if ( linebank->selected != NULL ){
		// one element in list
		if (linebank->count == 1)
			return ;
		// unhighlight selected
		linebank->selected->color = nk_rgb(10, 10, 10);
		// at head
		if (linebank->selected == linebank->head)
			linebank->selected = direction == 1 ?
			linebank->head->next : linebank->tail;
		// at tail
		else if (linebank->selected == linebank->tail)
			linebank->selected = direction == 1 ?
			linebank->head : linebank->tail->prev;
		else
			linebank->selected = direction == 1?
			linebank->selected->next : linebank->selected->prev;
		// highlight new selected
		linebank->selected->color = nk_rgb(255, 140, 60);
	}
	// if selection is valid unhighlight the current one
	// should be circular so you can't fall off the list and not be able to get back
	// highlight selected line
	// linebank.selected->color = nk_rgb(255, 0, 0)
}

