#include "list.h"
#include "demo.h"

#define THICKNESS 1.0f

int stroke_my_line( struct nk_command_buffer *b, t_line_node *node){
	nk_stroke_line(b, node->start_vertex.x, node->start_vertex.y, node->end_vertex.x, node->end_vertex.y, THICKNESS, node->color);
}


int add_line(t_line_bank *linebank, nk_vec2 start, nk_vec2 end){

	t_line_node *new = (t_line_node*)malloc(sizeof(t_line_node));
	new->line.start_vertex.x = start.x;
	new->line.start_vertex.y = start.y;
	new->line.end_vertex.x = end.x;
	new->line.end_vertex.y = end.y;

	if( linebank->head == NULL ){
		linebank->head = new;
		new->next = new;
		new->prev = new;
	}
	else {
		linebank->tail->next = new;
		new.prev = linebank->tail;
		new.next = linebank->head;
		linebank->head->prev = new;
	}
		linebank->tail = new;
		linebank->selected = new;
		linebank->count++;

	//allocate memory for new node
	//set values from args
	//add to list 
	//make that line the current selected
	return(0);
}

int remove_line(t_line_bank *linebank){
	if ( linebank->selected ){
		linebank -> selected -> prev -> next = linebank -> next;
		linebank -> selected -> next -> prev = linebank -> prev;
	}
	linebank->count--;
	//unlink
	//free
	return(0);
}


int	change_selected(t_line_bank *linebank, int direction){
	if ( linebank->selected != NULL ){
		linebank.selected->color = nk_rgb(10,10,0);
		if (direction == 0)
			linebank->selected = linebank->selected.prev;
		if (direction == 1)
			linebank->selected = linebank->selected.next;
		linebank.selected->color = nk_rgb(255, 0, 0);
		return(1);
	}
	// if selection is valid unhighlight the current one
	// linebank.selected->color = nk_rgb(10, 10, 0)
	// linebank.selected = linebank.selected->next or linebank.selected->prev
	// should be circular so you can't fall off the list and not be able to get back
	// highlight selected line
	// linebank.selected->color = nk_rgb(255, 0, 0)
	return(0);
}
