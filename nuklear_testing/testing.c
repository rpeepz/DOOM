#include "list.h"
#include <string.h>
#define THICKNESS 1.0f

int stroke_my_line( struct nk_command_buffer *b, t_line_node *node){
	nk_stroke_line(b, node->line.start_vertex.x, node->line.start_vertex.y,
					node->line.end_vertex.x, node->line.end_vertex.y,
					THICKNESS, node->color);
	return(0);
}

t_float_pair	snap(struct nk_vec2 v)
{
	t_float_pair res;

	int x = (int)v.x % 10;
	int y = (int)v.y % 10;
	if (x >= 5)
		res.x = v.x + (10 - x);
	else
		res.x = v.x - x;
	if (y >= 5)
		res.y = v.y + (10 - y);
	else
		res.y = v.y - y;
	return (res);
}

void	add_line(t_line_bank *linebank, struct nk_vec2 start, struct nk_vec2 end){
	if (start.x == end.x && start.y == end.y)
		return ;
	t_line_node *new = (t_line_node*)malloc(sizeof(t_line_node));
	memset(new, 0, sizeof(t_line_node));
	new -> color = nk_rgb(255, 0, 0);
	new->line.start_vertex = snap(start);
	new->line.end_vertex = snap(end);

	if( linebank->tail == NULL ){
		linebank->tail = linebank->head = new;
	}
	else {
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
	if (node){
		//unlink
		if (linebank->count == 1)
		{
			linebank->empty = 1;
			linebank->head = NULL;
			linebank->tail = NULL;
			linebank->selected = NULL;
		}
		else if (node == linebank->head)
		{
			linebank->head = node->next;
			linebank->selected = node->next;
			linebank->head->prev = NULL;
		}
		else if (node == linebank->tail)
		{
			linebank->tail = node->prev;
			linebank->selected = node->prev;
			linebank->tail->next = NULL;
		}
		else
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			linebank->selected = node->prev;
		}
		//free
		free(node);
		//decrement count
		linebank->count--;
	}
}

/* direction 1 traverses next : 0 traverses prev */
void change_selected(t_line_bank *linebank, int direction){
	if ( linebank->selected != NULL ){
		// unhighlight selected
		linebank->selected->color = nk_rgb(10, 10, 10);
		// one element in list
		if (linebank->count == 1)
			;
		// at head
		if (linebank->selected == linebank->head)
			linebank->selected =
			direction ? linebank->head->next : linebank->tail;
		// at tail
		if (linebank->selected == linebank->tail)
			linebank->selected =
			direction ? linebank->head : linebank->tail->prev;
		else
			linebank->selected =
			direction ? linebank->selected->next : linebank->selected->prev;
		// highlight new selected
		linebank->selected->color = nk_rgb(255, 140, 60);
	}
	// if selection is valid unhighlight the current one
	// should be circular so you can't fall off the list and not be able to get back
	// highlight selected line
	// linebank.selected->color = nk_rgb(255, 0, 0)
}

