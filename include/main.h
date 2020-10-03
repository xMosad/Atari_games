/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 3/10/2020                          */
/*****************************************************/
#ifndef MAIN_H
#define MAIN_H

/* Data types */
/* Hold the current state of atari */
typedef enum {
	SNAKE_GAME ,
	TETRIS_GAME,
	MAIN_SCREEN
}Atari_states_t;

/* To hold where the cursor is */
typedef enum {
	AT_SNAKE ,
	AT_TETRIS
}Cursor_state_t;

/* H W configurations */
#define     UP_BUTTON         PORTB,5
#define     DOWN_BUTTON       PORTB,6
#define     RIGHT_BUTTON      PORTB,7
#define     LEFT_BUTTON       PORTB,8
#define     OK_BUTTON         PORTB,9


/* Initialization functions */
static void voidInitH_W(void);
static void voidInitMainScreen(void);

/* Tasks */
void voidCheckButtonsTask(void);

/* Helper functions */
void voidCheckButtonsMainScreen(void);


#endif
