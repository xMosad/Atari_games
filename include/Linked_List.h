#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct type {
	u8 x_position ;
	u8 y_position ;
	struct type *next ;
};

typedef struct type Snake_point_t;

void appendNode(u8 copy_u8X , u8 copy_u8Y  );
Snake_point_t* getHead (void);
u8 updateNodes(u8 copy_u8X , u8 copy_u8Y );
u8 checkForCollison(void);
void restorInitSnake();


#endif
