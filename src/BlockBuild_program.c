/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 27/9/2020                          */
/*****************************************************/

/* Library includes */
#include <stdio.h>
#include "stdlib.h"
#include "BIT_MATH.h"
#include "STD_TYPES.h"

/* MCAL */
#include "RCC_interface.h"
#include "GPIO_interface.h"

/* HAL */
#include "TFT_interface.h"
#include "BUTTON_interface.h"

/* Services */
#include "OS_interface.h"

/* Module includes */
#include "BLOCKBUILD_interface.h"
#include "BLOCKBUILD_config.h"
#include "BLOCKBUILD_private.h"

/******************** Global variables **********************/
extern volatile BUTTON_state_t global_DownButton  ;
extern volatile BUTTON_state_t global_RightButton ;
extern volatile BUTTON_state_t global_LeftButton  ;
extern volatile BUTTON_state_t global_UpButton  ;
/* Shape variables */
u8 global_u8CurrentShape_X = 0 ;
u8 global_u8CurrentShape_Y = 0 ;
u8 global_u8CurrentShapeIndex = 0 ;



/* hold the max square in every column */
u8 lowerVlauesY [12] = {160 , 160 , 160 , 160 , 160 ,160 , 160 , 160 , 160 , 160 ,160 , 160};

/* Hold the busy squares in the screen */
u16 shapesPosition [16] ={0};


/************************** Initialization ***************************/

void BlockBuild_voidInitGame(void)
{
	/* Set the screen for game to begin */
	BlockBuild_voidSetInitialScreen();

	/* Create tasks */
	OS_u8CreateTask(0 , 0 , 300 , BlockBuild_voidStartGame );

	BlockBuild_voidGenerateRandomShape();
}

void BlockBuild_voidTerminateGame(void)
{
	OS_u8DeleteTask(0);
	/* Zero global variables */
	global_u8CurrentShape_X = 0 ;
	global_u8CurrentShape_Y = 0 ;
	global_u8CurrentShapeIndex = 0 ;
	for (u8 i = 0 ; i < 12 ; i++){
		lowerVlauesY[i]= 160 ;
	}
	for (u8 i = 0 ; i < 16 ; i++){
		shapesPosition[i]= 0 ;
	}
}

/************************* Tasks ******************************/

void BlockBuild_voidStartGame(void) {
	BlockBuild_voidShapeDown();
	BlockBuild_voidCheckEndOfDown();
	voidCheckRowComplete();
	voidCheckGameOver();
}

void BlockBuild_voidCheckButtons(void)
{
	u8 local_u8Flag = 1 ;
	if ((global_LeftButton == PRE_RELEASED) ){
		/* Clear the previous shape */
		BlockBuild_voidDrawShape(TFT_BLACK);
		if(global_u8CurrentShape_X !=  4 ){
			if (voidCheckCollisionLeft()){
				global_u8CurrentShape_X -= BULDING_SQ ;
			}
		}
	}
	else if ((global_DownButton == PRE_RELEASED)){
		voidSpeedDown();
	}
	else if ((global_RightButton == PRE_RELEASED)){
		/* Clear the previous shape */
		BlockBuild_voidDrawShape(TFT_BLACK);
		switch (global_u8CurrentShapeIndex){
			case 0 :
				if (global_u8CurrentShape_X + BULDING_SQ == 114){
					local_u8Flag = 0 ;
				}
				break;
			case 1 :
				if (global_u8CurrentShape_X + 2*BULDING_SQ == 114){
					local_u8Flag = 0 ;
				}
				break;
			case 2 :
				if (global_u8CurrentShape_X  == 114){
					local_u8Flag = 0 ;
				}
				break;
			default :
				if (global_u8CurrentShapeIndex < 11){
					if (global_u8CurrentShape_X + BULDING_SQ == 114){
						local_u8Flag = 0 ;
					}
				}
				else{
					if (global_u8CurrentShape_X + 2*BULDING_SQ == 114){
						local_u8Flag = 0 ;
					}
				}
				break;
		}
		if (local_u8Flag){
			if (voidCheckCollisionRight()){
			global_u8CurrentShape_X += BULDING_SQ ;
			}
		}
	}
	else if ((global_UpButton == PRE_RELEASED) ){
		voidRotateShape();
	}
	else {

	}
}


/************************* Helper functions *********************************/

void BlockBuild_voidSetInitialScreen(void)
{
	/* Set background */
	TFT_voidFillDisplay(BACKGROUND);
	TFT_voidDrawRectangle(0 , 0 , 4 , 160 , TFT_YELLOW);
	TFT_voidDrawRectangle(124 , 0 , 4 , 160 , TFT_YELLOW);
}
/* Private Helper functions for BlockBuild_voidStartGame task  */
/**************************************************************/
/*
 * Move shape down with rate of BlockBuild_voidStartGame task period
 * */
static void BlockBuild_voidShapeDown(void)
{
	/* Clear the previous shape */
	BlockBuild_voidDrawShape(TFT_BLACK);

	global_u8CurrentShape_Y += BULDING_SQ ;

	/* Draw the new shape */
	BlockBuild_voidDrawShape(SQ_COLOR);

}
/*
 * Check if shape hit down or not
 * if shape reach the end will call voidUpdatePosition to update with the new busy squars
 * and will call BlockBuild_voidGenerateRandomShape to generate a new shape
 * */
static void BlockBuild_voidCheckEndOfDown(void)
{
	u8 flag = 0;
	switch (global_u8CurrentShapeIndex){
		case 0 :
			if (((global_u8CurrentShape_Y+(BULDING_SQ*2)) == lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)]) ||
				((global_u8CurrentShape_Y+(BULDING_SQ*2)) == lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+1])){
				lowerVlauesY[global_u8CurrentShape_X/BULDING_SQ]      = global_u8CurrentShape_Y ;
				lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+1]  = global_u8CurrentShape_Y ;
				voidUpdatePosition();
				BlockBuild_voidGenerateRandomShape();
			}
			break;
		case 1 :
			if (((global_u8CurrentShape_Y+BULDING_SQ) == lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)]) ||
				((global_u8CurrentShape_Y+BULDING_SQ) == lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+1]) ||
				((global_u8CurrentShape_Y+BULDING_SQ) == lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+2])  ){
				lowerVlauesY[global_u8CurrentShape_X/BULDING_SQ]      = global_u8CurrentShape_Y ;
				lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+1]  = global_u8CurrentShape_Y ;
				lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+2]  = global_u8CurrentShape_Y ;
				voidUpdatePosition();
				BlockBuild_voidGenerateRandomShape();
			}
			break;
		case 2 :
			if ((global_u8CurrentShape_Y+(BULDING_SQ*3)) == lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)]){
				lowerVlauesY[global_u8CurrentShape_X/BULDING_SQ]    = global_u8CurrentShape_Y ;
				voidUpdatePosition();
				BlockBuild_voidGenerateRandomShape();
			}
			break;
		default :
			if (global_u8CurrentShapeIndex < SHAPE2_3_END){
				u8 index1 = global_u8CurrentShapeIndex-3 ;
				for (u8 j = 0 ; j < 2 ; j++){
					for (u8 i = 0 ; i < 3 ; i++){
						if (shapes2_3[index1][j] & (1 << i)){
							if ((global_u8CurrentShape_Y+(BULDING_SQ*(i+1))) == lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+j]){
								for (u8 k = 0 ; k < 2 ; k++){
									if (shapes2_3[index1][k] & (1 << 0)){
										lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+k]   = global_u8CurrentShape_Y ;
									}
									else if (shapes2_3[index1][k] & (1 << 1)){
										lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+k]   = (global_u8CurrentShape_Y+BULDING_SQ) ;
									}
									else if (shapes2_3[index1][k] & (1 << 2)){
										lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+k]   = (global_u8CurrentShape_Y+(BULDING_SQ*2));
									}
								}
								flag = 1 ;
							}
						}
					}
				}
				if (flag){
					voidUpdatePosition();
					BlockBuild_voidGenerateRandomShape();}
		    }
			else if (global_u8CurrentShapeIndex < SHAPE3_2_END) {
				u8 index2 = global_u8CurrentShapeIndex-11 ;
				for (u8 j = 0 ; j < 3 ; j++){
					for (u8 i = 0 ; i < 2 ; i++){
						if (shapes3_2[index2][j] & (1 << i)){
							if (((global_u8CurrentShape_Y+(BULDING_SQ*(i+1))) == lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+j])){
								for (u8 k = 0 ; k < 3 ; k++){
									if (shapes3_2[index2][k] & (1 << 0)){
										lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+k]   = global_u8CurrentShape_Y ;
									}
									else if (shapes3_2[index2][k] & (1 << 1)){
										lowerVlauesY[(global_u8CurrentShape_X/BULDING_SQ)+k]   = (global_u8CurrentShape_Y +BULDING_SQ) ;
									}
								}
								flag = 1 ;
							}
						}
					}
				}
				if (flag){
					voidUpdatePosition();
					BlockBuild_voidGenerateRandomShape();}
			}
			else{

			}

			break;
	}

}
/*
 * Check periodically if a row is completed
 * if row is completed the row will be removed and shift the rows down
 * */
static void voidCheckRowComplete (void)
{
	for (u8 i =15 ; i > 0 ; i--){
		if(shapesPosition[i] == 4095){
			/* To clear the completed row and the above rows of it*/
			TFT_voidDrawRectangle( 4 , 0 ,120 , (i*10)+10 ,BACKGROUND );
			/* Update both arrays shapesPosition , lowerVlauesY */
			for (u8 j =i ; j > 0 ; j--){
				shapesPosition[j] = shapesPosition[j-1] ;
				if (shapesPosition[j]){
					for (u8 k = 0 ; k < 12 ; k++){
						if (GIT_BIT (shapesPosition[j] ,k)){
							/* Draw after the shift */
							TFT_voidDrawRectangle( (k*10) +5 , (j*10) +1 ,BULDING_SQ-2 , BULDING_SQ-2,SQ_COLOR );
						}
					}
			    }
				else {
					break ;
				}
			}
			for (u8 j =0 ; j < 12 ; j++){
				if (lowerVlauesY[j] != 160){
					lowerVlauesY[j] += 10;
				}
			}
			break;
		}
	}
}

static void voidCheckGameOver(void)
{
	for (u8 i = 0 ; i < 12 ; i++){
		if (GIT_BIT (shapesPosition[2] ,i)){
			BlockBuild_voidSetInitialScreen();
			TFT_voidPrintText("game" , 30 , 50 , 3 , TFT_RED , TFT_BLACK);
			TFT_voidPrintText("over" , 30 , 80 , 3 , TFT_RED , TFT_BLACK);

			BlockBuild_voidTerminateGame();
			break;
		}

	}
}



/*********************************************************************************************/
/********************************************************************************************/
/* Helper functions for the above helper functions */

static void BlockBuild_voidGenerateRandomShape(void)
{
    global_u8CurrentShapeIndex = MAX_SHAPES + 1 ;
	while ((global_u8CurrentShapeIndex > (MAX_SHAPES -1))){
		global_u8CurrentShapeIndex = rand() ;
		global_u8CurrentShapeIndex+=rand();
	}
	global_u8CurrentShape_X = START_X ;
	global_u8CurrentShape_Y = START_Y ;
	BlockBuild_voidDrawShape(SQ_COLOR);

}

/*
 * Draw the current shape using current shape variable with certain colors
 * */
static void BlockBuild_voidDrawShape (u16 copy_u16Colour)
{
	switch (global_u8CurrentShapeIndex){
		case 0 :
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1 , global_u8CurrentShape_Y+1 ,BULDING_SQ-2 , BULDING_SQ-2 , copy_u16Colour);
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1 , global_u8CurrentShape_Y+1+BULDING_SQ ,BULDING_SQ-2 , BULDING_SQ-2 , copy_u16Colour);
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1+BULDING_SQ , global_u8CurrentShape_Y+1 ,BULDING_SQ-2 , BULDING_SQ-2 , copy_u16Colour);
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1+BULDING_SQ , global_u8CurrentShape_Y+1+BULDING_SQ ,BULDING_SQ-2 , BULDING_SQ-2 , copy_u16Colour);
			break;
		case 1 :
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1 , global_u8CurrentShape_Y+1 ,BULDING_SQ-2 , BULDING_SQ-2 , copy_u16Colour);
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1+BULDING_SQ , global_u8CurrentShape_Y+1 ,BULDING_SQ-2 , BULDING_SQ-2, copy_u16Colour);
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1+2*BULDING_SQ , global_u8CurrentShape_Y+1 ,BULDING_SQ-2 , BULDING_SQ-2 , copy_u16Colour);
			break;
		case 2 :
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1 , global_u8CurrentShape_Y+1 ,BULDING_SQ-2 , BULDING_SQ-2 , copy_u16Colour);
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1 , global_u8CurrentShape_Y+1+BULDING_SQ ,BULDING_SQ-2 , BULDING_SQ-2, copy_u16Colour);
			TFT_voidDrawRectangle( global_u8CurrentShape_X+1 , global_u8CurrentShape_Y+1+2*BULDING_SQ ,BULDING_SQ-2 , BULDING_SQ-2 , copy_u16Colour);
			break;
		default :
			BlockBuild_voidDrawSpecialShape( global_u8CurrentShapeIndex , global_u8CurrentShape_X , global_u8CurrentShape_Y , copy_u16Colour);
			break;
	}
}

/*
 * Draw the 2*3 and 3*2 shapes with certain colors
 * called by BlockBuild_voidDrawShape
 * */
static void BlockBuild_voidDrawSpecialShape(u8 copy_u8Index , u8 copy_u8X , u8 copy_u8Y ,u16 copy_u16Colour)
{
	if (copy_u8Index < SHAPE2_3_END){
		u8 index1 = copy_u8Index-3 ;
		for (u8 j = 0 ; j < 2 ; j++){
			for (u8 i = 0 ; i < 3 ; i++){
				if (shapes2_3[index1][j] & (1 << i)){
					TFT_voidDrawRectangle( copy_u8X+(j*BULDING_SQ)+1 , copy_u8Y+(i*BULDING_SQ)+1 ,BULDING_SQ-2 , BULDING_SQ-2, copy_u16Colour);
				}
			}
		}
	}
	else if (copy_u8Index < SHAPE3_2_END){
		u8 index2 = copy_u8Index-11 ;
		for (u8 j = 0 ; j < 3 ; j++){
			for (u8 i = 0 ; i < 2 ; i++){
				if (shapes3_2[index2][j] & (1 << i)){
					TFT_voidDrawRectangle( copy_u8X+(j*BULDING_SQ)+1 , copy_u8Y+(i*BULDING_SQ)+1 ,BULDING_SQ-2 , BULDING_SQ-2, copy_u16Colour);
				}
			}
		}
	}
	else {

	}
}

/*
 * Called when the shape stop at down
 * update the shapesPosition[] array with the new busy squares
 * */
static void voidUpdatePosition(void){
	switch (global_u8CurrentShapeIndex){
		case 0 :
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,(global_u8CurrentShape_X /10) );
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,((global_u8CurrentShape_X /10)+1) );
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+1] ,(global_u8CurrentShape_X /10) );
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+1] ,((global_u8CurrentShape_X /10)+1) );
			break;
		case 1 :
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,(global_u8CurrentShape_X /10) );
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,((global_u8CurrentShape_X /10)+1) );
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,((global_u8CurrentShape_X /10)+2));
			break;
		case 2 :
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,(global_u8CurrentShape_X /10) );
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+1] ,(global_u8CurrentShape_X /10));
			SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+2] ,(global_u8CurrentShape_X /10));
			break;
		default :
			if (global_u8CurrentShapeIndex < 11){
				u8 index1 = global_u8CurrentShapeIndex-3 ;
				for (u8 j = 0 ; j < 2 ; j++){
					for (u8 i = 0 ; i < 3 ; i++){
						if (shapes2_3[index1][j] & (1 << i)){
							SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+i] , ((global_u8CurrentShape_X /10)+j) );
						}
					}
				}
		    }
			else if (global_u8CurrentShapeIndex < 19) {
				u8 index2 = global_u8CurrentShapeIndex-11 ;
				for (u8 j = 0 ; j < 3 ; j++){
					for (u8 i = 0 ; i < 2 ; i++){
						if (shapes3_2[index2][j] & (1 << i)){
							SET_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+i] ,((global_u8CurrentShape_X /10)+j) );
						}
					}
				}
			}
			else{

			}

			break;
	}
}


/********************************************************************************/
/********************************************************************************/
/************* helper functions for button **************************************/
/*
 * used when shifting right or left to stop the shift if there is a square to the
 *  right of the shape
 * */
static u8 voidCheckCollisionRight (void)
{
	u8 local_check = 1 ;
	/* To check based on the shape type */
	switch (global_u8CurrentShapeIndex){
		case 0 :
			if ( (GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] , ((global_u8CurrentShape_X /10)+2 ))) ||
				(GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+1] ,((global_u8CurrentShape_X /10)+2) )))
			{
				local_check = 0 ;
			}
			break;
		case 1 :
			if (GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,((global_u8CurrentShape_X /10)+3))){
				local_check = 0 ;
			}
			break;
		case 2 :
			if ((GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,((global_u8CurrentShape_X /10)+1) ))||
			(GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+1] ,((global_u8CurrentShape_X /10)+1)))||
			(GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+2] ,((global_u8CurrentShape_X /10)+1))))
			{
				local_check = 0 ;
			}
			break;
		default :
			if (global_u8CurrentShapeIndex < SHAPE2_3_END){
				u8 index1 = global_u8CurrentShapeIndex-3 ;
				for (u8 i = 0 ; i < 3 ; i++){
					if (shapes2_3[index1][1] & (1 << i)){
						if (GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+i] , ((global_u8CurrentShape_X /10)+2) )){
							local_check = 0 ;
						}
					}
				}
		    }
			else if (global_u8CurrentShapeIndex < SHAPE3_2_END) {
				u8 index2 = global_u8CurrentShapeIndex-11 ;
				for (u8 i = 0 ; i < 2 ; i++){
					if (shapes3_2[index2][2] & (1 << i)){
						if (GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+i] ,((global_u8CurrentShape_X /10)+3) )){
							local_check = 0 ;
						}
					}
				}
			}
			else{

			}

			break;
	}
	return local_check ;
}

/*
 * used when shifting right or left to stop the shift if there is a square to the
 *  left of the shape
 * */
static u8 voidCheckCollisionLeft (void)
{
	u8 local_check = 1 ;
	/* To check based on the shape type */
	switch (global_u8CurrentShapeIndex){
		case 0 :
			if ( (GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] , ((global_u8CurrentShape_X /10)-1) )) ||
				(GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+1] ,((global_u8CurrentShape_X /10)-1) )))
			{
				local_check = 0 ;
			}
			break;
		case 1 :
			if (GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,((global_u8CurrentShape_X /10)-1))){
				local_check = 0 ;
			}
			break;
		case 2 :
			if ((GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)] ,((global_u8CurrentShape_X /10)-1) ))||
			(GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+1] ,((global_u8CurrentShape_X /10)-1)))||
			(GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+2] ,((global_u8CurrentShape_X /10)-1))))
			{
				local_check = 0 ;
			}
			break;
		default :
			if (global_u8CurrentShapeIndex < SHAPE2_3_END){
				u8 index1 = global_u8CurrentShapeIndex-3 ;
				for (u8 i = 0 ; i < 3 ; i++){
					if (shapes2_3[index1][1] & (1 << i)){
						if (GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+i] , ((global_u8CurrentShape_X /10)-1) )){
							local_check = 0 ;
						}
					}
				}
		    }
			else if (global_u8CurrentShapeIndex < SHAPE3_2_END) {
				u8 index2 = global_u8CurrentShapeIndex-11 ;
				for (u8 i = 0 ; i < 2 ; i++){
					if (shapes3_2[index2][2] & (1 << i)){
						if (GIT_BIT (shapesPosition[(global_u8CurrentShape_Y / 10)+i] ,((global_u8CurrentShape_X /10)-1) )){
							local_check = 0 ;
						}
					}
				}
			}
			else{

			}

			break;
	}
	return local_check ;
}

/*
 * Move the shape down when DOWN button is pressed called by BlockBuild_voidCheckButtons
 * */
static void voidSpeedDown(void)
{
	/* Clear the previous shape */
	BlockBuild_voidDrawShape(TFT_BLACK);

	global_u8CurrentShape_Y += BULDING_SQ ;

	/* Draw the new shape */
	BlockBuild_voidDrawShape(SQ_COLOR);

	/* Check end or not */
	BlockBuild_voidCheckEndOfDown();
}

/*
 * Rotate the shape when UP button is pressed called by BlockBuild_voidCheckButtons
 * */
static void voidRotateShape(void)
{
	/* Clear the previous shape */
	BlockBuild_voidDrawShape(TFT_BLACK);

	/* Rotate */
	if (global_u8CurrentShapeIndex < 3){
		if (global_u8CurrentShapeIndex == 1){
			global_u8CurrentShapeIndex = 2 ;
		}
		else if (global_u8CurrentShapeIndex == 2){
			global_u8CurrentShapeIndex = 1 ;
		}
	}
	else if ( global_u8CurrentShapeIndex < SHAPE2_3_END ){
		if (global_u8CurrentShapeIndex % 2){
			global_u8CurrentShapeIndex += 8 ;
		}
		else {
			global_u8CurrentShapeIndex -= 1 ;
		}

	}
	else if (global_u8CurrentShapeIndex < SHAPE3_2_END){
		if (global_u8CurrentShapeIndex % 2){
			global_u8CurrentShapeIndex += 1 ;
		}
		else {
			global_u8CurrentShapeIndex -= 8 ;
		}
	}

	/* Draw the new shape */
	BlockBuild_voidDrawShape(SQ_COLOR);
}










