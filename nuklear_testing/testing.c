
#include "generic_structs.c"
//#include nuklear stuff

#define THICKNESS 1.0f

int stroke_my_line( struct nk_command_buffer *b, t_line_node *node){
	nk_stroke_line(b, node->start_vertex.x, node->start_vertex.y, node->end_vertex.x, node->end_vertex.y, THICKNESS, node->color);
}


int add_line(t_line_bank *linebank, nk_vec2 start, nk_vec2 end){
	//allocate memory for new node
	//set values from args
	//add to list 
	//make that line the current selected
	return(0);
}

int remove_line(t_line_bank *linebank){
	//unlink
	//free
	return(0);
}


int	change_selected(t_line_bank *linebank, int direction){
	// if selection is valid unhighlight the current one
	// linebank.selected->color = nk_rgb(10, 10, 0)
	// linebank.selected = linebank.selected->next or linebank.selected->prev
	// should be circular so you can't fall off the list and not be able to get back
	// highlight selected line
	// linebank.selected->color = nk_rgb(255, 0, 0)
	return(0);
}
