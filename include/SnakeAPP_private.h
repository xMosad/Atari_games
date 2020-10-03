/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 12/9/2020                          */
/*****************************************************/
#ifndef SNAKEAPP_PRIVATE_H
#define SNAKEAPP_PRIVATE_H

/* Data type to hold the direction of the snake */
typedef enum {
	TO_RIGHT ,
	TO_LEFT  ,
	TO_UP    ,
	TO_DOWN
}directionState_t;

/* colors */
#define    BACKGROUND_1   0x07f3
#define    SCORE_BAR      0X60a1
#define    BORDERS        0xa861

/* Dimensions */
#define    TFT_WIDTH     127
#define    TFT_HIGHT     159
#define    BORDER_THICK  4
#define    FOOD_SIZE     12
#define    SNAKE_SIZE    12

/* Private functions prototypes */
static void SnakeAPP_voidUpdateScreen(void);
static void SnakeAPP_voidUpdatePosition(void);
static void SnakeApp_voidCheckEatFood(void);
static void SnakeApp_voidCheckGameOver(void);
static void SnakeAPP_voidSetInitialScreen(void);



#endif
