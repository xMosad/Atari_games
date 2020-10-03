
/* Library */
#include "stdlib.h"
#include "STD_TYPES.h"

/* Module includes */
#include "Linked_List.h"


static Snake_point_t* head = NULL ;
static Snake_point_t* tail = NULL ;

void appendNode(u8 copy_u8X , u8 copy_u8Y )
{
    Snake_point_t *node = (Snake_point_t*) malloc(sizeof(Snake_point_t));
    node->next = NULL ;
    node->x_position = (copy_u8X*12) + 6 ;
    node->y_position = (copy_u8Y*12) + 20;

    if (head == NULL){
        head = node ;
        tail = node ;
    }
    else {
        tail->next=node;
        tail=node;
    }

}

Snake_point_t* getHead (void)
{
	return head ;
}

u8 updateNodes(u8 copy_u8X , u8 copy_u8Y )
{
	Snake_point_t *node = '\0' ;
	u8 local_u8Flag = 0;
	for(node= (head->next)  ; node!= NULL; node=node->next){
		if (node->next == NULL){
			node->x_position = (copy_u8X*12) + 6  ;
			node->y_position = (copy_u8Y*12) + 20 ;
		}
		else {
			Snake_point_t *next_node = node->next ;
			node->x_position = next_node->x_position ;
			node->y_position = next_node->y_position ;
		}
	}
	if ((tail->x_position == head->x_position ) && (tail->y_position == head->y_position ) ){
		local_u8Flag = 1;
		u8 x = 12;
		u8 y = 12 ;
		u8 flag = 0;
		/* Generate new random position for the food */
		do{
			while (x > 8 ){
			x = rand()/10 ;
			}
			while (y > 10  ){
			y = rand()/10 ;
			}
			/* To make sure the random position isn't a reserved place */
			for(node= (head->next)  ; node!= NULL; node=node->next){
				if((((x*12) + 6) == node->x_position ) && (((y*12) + 20) == node->y_position )){
					flag = 1 ;
					x = 12 ;
					y = 12 ;
					break ;
				}
				else {
					flag = 0 ;
				}
			}
		}
		while(flag);
		head->x_position = (x*12) + 6;
		head->y_position = (y*12) + 20;
	}
	return local_u8Flag ;
}

u8 checkForCollison(void){
	Snake_point_t *node = '\0' ;
	u8 local_u8Flag = 0 ;
	for(node= (head->next); node!= tail ; node=node->next){
		if ((tail->x_position == node->x_position ) && (tail->y_position == node->y_position )){
			local_u8Flag = 1 ;
		}
	}
	return local_u8Flag ;
}

void restorInitSnake(){
	Snake_point_t *node = '\0' ;
	Snake_point_t *temp = '\0' ;
	for(node=head; node!= NULL ; node=node->next){
		temp = node ;
		free(temp);
	}
	head = NULL;
}







