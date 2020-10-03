/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 27/9/2020                          */
/*****************************************************/
#ifndef BLOCKBUILD_PRIVATE_H
#define BLOCKBUILD_PRIVATE_H

/* Data type to hold the direction of the snake */
typedef enum {
	SHAPE_1  ,    // square
	SHAPE_2  ,    // H Line
	SHAPE_3  ,    // V Line
	/* 2 * 3 shapes */
	SHAPE_4  ,
	SHAPE_5  ,
	SHAPE_6  ,
	SHAPE_7  ,
	SHAPE_8  ,
	SHAPE_9  ,
	SHAPE_10 ,
	SHAPE_11 ,
	/* 3 * 2 shapes  */
	SHAPE_12 ,
	SHAPE_13 ,
	SHAPE_14 ,
	SHAPE_15 ,
	SHAPE_16 ,
	SHAPE_17 ,
	SHAPE_18 ,
	SHAPE_19 ,
	MAX_SHAPE
}BlocksShape_t;

/* colors */
#define    BACKGROUND     TFT_BLACK
#define    SQ_COLOR       0x8eff

u8 shapes2_3[8][2] = {
		{ 3 , 6 },
		{ 6 , 3 },
		{ 7 , 2 },
		{ 2 , 7 },
		{ 1 , 7 },
		{ 7 , 1 },
		{ 7 , 4 },
		{ 4 , 7 }
};

u8 shapes3_2[8][3] = {
		{ 2 , 3 , 1 },
		{ 1 , 3 , 2 },
		{ 2 , 3 , 2 },
		{ 1 , 3 , 1 },
		{ 2 , 2 , 3 },
		{ 3 , 2 , 2 },
		{ 3 , 1 , 1 },
		{ 1, 1 , 3 }

};

/* Dimensions */
#define    TFT_WIDTH     127
#define    TFT_HIGHT     159
#define    BULDING_SQ    10

/* Constants */
#define    MAX_SHAPES     19
#define    START_X        54
#define    START_Y        0
#define    SHAPE2_3_END   11
#define    SHAPE3_2_END   19


/* Private functions prototypes */
static void BlockBuild_voidGenerateRandomShape(void);
static void BlockBuild_voidDrawShape (u16 copy_u16Colour);
static void BlockBuild_voidDrawSpecialShape(u8 copy_u8Index , u8 copy_u8X , u8 copy_u8Y ,u16 copy_u16Colour);
static void BlockBuild_voidShapeDown(void);
static void BlockBuild_voidCheckEndOfDown(void);
static void voidUpdatePosition(void);
static u8 voidCheckCollisionRight (void);
static u8 voidCheckCollisionLeft (void);
static void voidCheckGameOver(void);
static void voidCheckRowComplete (void);
static void voidSpeedDown(void);
static void voidRotateShape(void);





#endif
