/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 27/9/2020                          */
/*****************************************************/
#ifndef BLOCKBUILD_INTERFACE_H
#define BLOCKBUILD_INTERFACE_H

/*
 * Initialize the H.W and tasks for the snake game
 * */
void BlockBuild_voidInitGame(void);

/*
 * Initialize the screen for game
 * */
void BlockBuild_voidSetInitialScreen(void);

/*
 * The update task for the game algorithm */
void BlockBuild_voidStartGame(void);

/*
 * Terminate game by deleting BlockBuild_voidStartGame task */
void BlockBuild_voidTerminateGame(void);

/*
 * The Check task for the game buttons */
void BlockBuild_voidCheckButtons(void);


#endif
