/*
 * main.c
 *
 *  Created on: Aug 13, 2020
 *      Author: mosad
 */

/************* Includes ***************/
/* Library */
#include <stdio.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/* MCAL */
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "STK_interface.h"
#include "SPI_interface.h"

/* HAL */
#include "TFT_interface.h"
#include "BUTTON_interface.h"

/* APP */
#include "BlockBuild_interface.h"
#include "SnakeAPP_interface.h"

/* Services */
#include "OS_interface.h"

#include "main.h"

/*********** Global variables ****************/
/** Buttons variables **/
volatile BUTTON_state_t global_DownButton = RELEASED ;
volatile BUTTON_state_t global_RightButton = RELEASED ;
volatile BUTTON_state_t global_LeftButton = RELEASED ;
volatile BUTTON_state_t global_UpButton = RELEASED ;
volatile BUTTON_state_t global_OkButton = RELEASED ;
static u8 global_u8DownButtonID = 0 ;
static u8 global_u8RightButtonID = 0 ;
static u8 global_u8LeftButtonID = 0 ;
static u8 global_u8UpButtonID = 0 ;
static u8 global_u8OkButtonID = 0 ;

/* Atari states */
Atari_states_t global_mainState = MAIN_SCREEN ;
Cursor_state_t global_cursorState = AT_SNAKE ;

/************** APP functions ************/
void main(void)
{
	/* Initialize H.W */
	voidInitH_W();

	/* Initialize the main screen */
	voidInitMainScreen();

	/* Create the button task */
	OS_u8CreateTask(1 , 0 , 10 , voidCheckButtonsTask );

	/* Start OS */
	OS_voidStart();

	while(1){
		/* Call dispatcher */
		OS_voidDispatcher();
	}

}

static void voidInitH_W(void)
{
	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();
	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , SPI1_PERIPHERAL);

	/* Configure pins for SPI */
	GPIO_voidSetPinMode(PORTA , PIN7 , GPIO_OUTPUT_10MHZ_AFPP); //MOSI
	GPIO_voidSetPinMode(PORTA , PIN6 , GPIO_INPUT_FLOATING); //MISO
	GPIO_voidSetPinMode(PORTA , PIN5 , GPIO_OUTPUT_10MHZ_AFPP); //SCK

	/* Configure pins for TFT */
	GPIO_voidSetPinMode(PORTA , PIN1 , GPIO_OUTPUT_10MHZ_PP);
	GPIO_voidSetPinMode(PORTA , PIN2 , GPIO_OUTPUT_10MHZ_PP);

	/* Initialize STK */
	STK_voidInit();

	// master - idle high - write first - msb - 8bit format - sw m slave ,full duplex , f/2 / disable ISR
	SPI_config_t SPI1_config =
		{ 1 , 1 , 1 , 0 , 0 , 1, 0 ,0 ,0 };
	SPI_u8ConfigureCh(SPI1 , &SPI1_config );

	/* Initialize TFT */
	TFT_voidInit();

	/* Initialize H.W */
	global_u8DownButtonID  = BUTTON_voidInit(DOWN_BUTTON);
	global_u8RightButtonID = BUTTON_voidInit(RIGHT_BUTTON);
	global_u8LeftButtonID  = BUTTON_voidInit(LEFT_BUTTON);
	global_u8UpButtonID    = BUTTON_voidInit(UP_BUTTON);
	global_u8OkButtonID    = BUTTON_voidInit(OK_BUTTON);

}

static void voidInitMainScreen(void)
{
	TFT_voidFillDisplay(TFT_BLACK);
	TFT_voidPrintText("GAMES" , 20 , 20 , 3 , TFT_RED , TFT_BLACK);
	TFT_voidPrintText("SNAKE" , 30 , 60 , 2 , TFT_BLUE , TFT_BLACK);
	TFT_voidPrintText("TETRIS" , 30 , 80 , 2 , TFT_BLUE , TFT_BLACK);
	TFT_voidPrintText(">" , 10 , 60 , 2 , TFT_GREEN , TFT_BLACK);
}

/**************** Tasks *************************/
void voidCheckButtonsTask(void)
{
	BUTTON_voidScane();
	global_DownButton = BUTTON_voidGetState(global_u8DownButtonID);
	global_RightButton = BUTTON_voidGetState(global_u8RightButtonID);
	global_LeftButton = BUTTON_voidGetState(global_u8LeftButtonID);
	global_UpButton = BUTTON_voidGetState(global_u8UpButtonID);
	global_OkButton = BUTTON_voidGetState(global_u8OkButtonID);

	switch (global_mainState){
		case SNAKE_GAME  :
			SnakeAPP_voidCheckInput();
			if (global_OkButton == PRE_RELEASED ){
				SnakeAPP_voidTerminateGame();
				voidInitMainScreen();
				global_mainState = MAIN_SCREEN;
			}
			break;
		case TETRIS_GAME :
			BlockBuild_voidCheckButtons();
			if (global_OkButton == PRE_RELEASED ){
				BlockBuild_voidTerminateGame();
				voidInitMainScreen();
				global_mainState = MAIN_SCREEN;
			}
			break;
		case MAIN_SCREEN :
			voidCheckButtonsMainScreen();
			break;
	}
}

void voidCheckButtonsMainScreen(void)
{
	if ((global_UpButton == PRE_RELEASED) ||(global_DownButton == PRE_RELEASED) ){
		switch (global_cursorState){
			case AT_SNAKE  :
				TFT_voidPrintText(" " , 10 , 60 , 2 , TFT_GREEN , TFT_BLACK);
				TFT_voidPrintText(">" , 10 , 80 , 2 , TFT_GREEN , TFT_BLACK);
				global_cursorState = AT_TETRIS ;
				break;
			case AT_TETRIS :
				TFT_voidPrintText(" " , 10 , 80 , 2 , TFT_GREEN , TFT_BLACK);
				TFT_voidPrintText(">" , 10 , 60 , 2 , TFT_GREEN , TFT_BLACK);
				global_cursorState = AT_SNAKE ;
				break;
		}
	}
	if (global_OkButton == PRE_RELEASED ){
		switch (global_cursorState){
			case AT_SNAKE  :
				SnakeAPP_voidInitScreen();
				global_mainState = SNAKE_GAME ;
				break;
			case AT_TETRIS :
				BlockBuild_voidInitGame();
				global_mainState = TETRIS_GAME ;
				break;
		}
	}
}









