/*
 * user_display.c
 *
 *  Created on: 2021��11��8��
 *      Author: Sky
 */

#include	"user_display.h"
#include "gd32e230c_lcd_eval.h"

void displaySystemMenu(displayParamsStruct * disParams) {


        lcd_draw_number(x_offset+8,150,WHITE,BLUE,1234567890,GBK16);


}
