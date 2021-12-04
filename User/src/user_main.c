/*
 * user_main.c
 *
 *  Created on: 2021年11月04日
 *      Author: Sky
 */
#include "string.h"
#include "user_main.h"
#include	"user_menu.h"
#include	"user_exti.h"
#include	"user_key.h"
#include	"user_adc.h"
#include	"user_i2cm.h"
#include	"user_pwm.h"
#include	"systick.h"
#include "gd32e230c_lcd_eval.h"
#include	"user_internalFlash.h"
//#include "gd32e230c_eval.h"

userSysTimesDef	sysTimes={0};
GPIO_TypeDef GPIOA_Config={0};
GPIO_TypeDef GPIOB_Config={0};
GPIO_TypeDef GPIOC_Config={0};
GPIO_TypeDef GPIOF_Config={0};

TIME_TypeDef	TIME0_Config={0};
TIME_TypeDef	TIME2_Config={0};
TIME_TypeDef	TIME5_Config={0};
TIME_TypeDef	TIME13_Config={0};
TIME_TypeDef	TIME14_Config={0};
TIME_TypeDef	TIME15_Config={0};
TIME_TypeDef	TIME16_Config={0};

void getPortConfigRegisters(void) {
	memcpy(&GPIOA_Config, pGPIOA, sizeof(GPIO_TypeDef));
	memcpy(&GPIOB_Config, pGPIOB, sizeof(GPIO_TypeDef));
	memcpy(&GPIOC_Config, pGPIOC, sizeof(GPIO_TypeDef));
	memcpy(&GPIOF_Config, pGPIOF, sizeof(GPIO_TypeDef));

	memcpy(&TIME0_Config, pTIMER0, sizeof(TIME_TypeDef));
	memcpy(&TIME2_Config, pTIMER2, sizeof(TIME_TypeDef));
	memcpy(&TIME5_Config, pTIMER5, sizeof(TIME_TypeDef));
	memcpy(&TIME13_Config, pTIMER13, sizeof(TIME_TypeDef));
	memcpy(&TIME14_Config, pTIMER14, sizeof(TIME_TypeDef));
	memcpy(&TIME15_Config, pTIMER15, sizeof(TIME_TypeDef));
	memcpy(&TIME16_Config, pTIMER16, sizeof(TIME_TypeDef));
}

/*!
    \brief      LCD content initialize
    \param[in]  none
    \param[out] none
    \retval     none
*/
void test_display_init( void ){
    int i;

    #define x_offset 2
    {
        lcd_draw_font_gbk16(x_offset, 10, WHITE, BLUE," Bough Technology Inc.");
//        lcd_draw_font_gbk16(x_offset, 30, WHITE, BLUE,"  -- GD32E230 Series MCU --   ");
//
//        lcd_draw_font_gbk16(x_offset, 50,WHITE, BLUE, "     GD32E230C_EAVL  ");

        lcd_draw_font_gbk24(x_offset,30,YELLOW,BLUE,"  博昊科技  ");
        lcd_draw_font_gbk16(x_offset, 70, WHITE,BLUE, " Demo Project :");
//        lcd_draw_font_gbk16(x_offset, 90, WHITE,BLUE, "    Please adjust the VR1 !");

        lcd_line_draw(x_offset+6, 110, x_offset+6, 310, YELLOW);
        lcd_draw_number(x_offset+30,150,WHITE,BLUE,1234567890,GBK16);
        lcd_draw_number(x_offset+30,170,WHITE,BLUE,1234567890,GBK24);
        lcd_draw_number(x_offset+30,200,WHITE,BLUE,1234567890,GBK32);
//        lcd_line_draw(x_offset+6, 310, x_offset+6+220, 310, YELLOW);
//
//        for(i = 0;i < 4;i ++){
//            lcd_line_draw(x_offset+6-4, 110+i*50, x_offset+6, 110+i*50, YELLOW);
//        }
//
//        for(i = 0;i < 4;i ++){
//            lcd_line_draw(x_offset+6-2, 110+25+i*50, x_offset+6, 110+25+i*50, YELLOW);
//        }

//        lcd_circle_draw(x_offset+100,150,60,RED);
    }
}

/***********************************************************************************************************
  *  @brief
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void	powerDataInit(void){

	displayParams.DisplayModeIndex=IdleIamgeDisplay;
	displayParams.brightness=DEFAULT_BRIGHTNESS;
	displayParams.arrowIndex=DEFAULT_ARROR_INDEX;
	displayParams.colorTemperature=DEFAULT_COLOR_TEMP;
#if(RGB_INSIDE==1)
	displayParams.hues=DEFAULT_HUES;
	displayParams.saturation=DEFAULT_SATURATION;
	displayParams.style1Value=DEFAULT_STYLE1_VALUE;
	displayParams.style2Value=DEFAULT_STYLE2_VALUE;
	displayParams.style3Value=DEFAULT_STYLE3_VALUE;
	displayParams.battIndex=6;
#endif
//	vCurrentBattLv=6;
//	fIsCharging=0;
//	vChargingCurrent=1.0;

}
/***********************************************************************************************************
  *  @brief
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void func_user_main(void) {
	powerDataInit();
	extiConfig();
	fIsSystemOff = 0;
	resetEntrySleepTime();
	pwmConfig(TIMER0);
	adcInit();

	/* TFT LCD initialize */
	lcd_init();
	lcd_clear(BLUE);
	test_display_init();
//	eeprom_test();
	i2c_init();
//	StoreExceptions2Flash(1);
//	memset(&displayParams,0,sizeof(displayParams));
	if (!exceptionsCheck()) {
		__nop();			//some exceptions been thrown;
	}
	while (1) {
		getPortConfigRegisters();
//		if (!fIsSystemOff) {
		funcKeyTask();
//			encoderCheck();
		if (vKeyValue) {
//				if (FactoryMode != displayParams.DisplayModeIndex) {
			interaction();
//				}
			__nop();
		}
//		}
		if (sysTimes.vSystem5ms != 0) {
			sysTimes.vSystem5ms--;
			fucnPer5ms();
			//iCNTVAL=IWDG_GetReload();
//			IWDG_GetReload();
//			IWDG_Feed();
//			IWDG_RELOAD();
		}

		if (fIsSystemOff && !sysTimes.vTimeToSleep) {
			entrySleep();
		}
	}
}
