/*
 * user_menu.c
 *
 *  Created on: 2021年11月4日
 *      Author: Sky
 */

#include	"user_menu.h"
#include	"user_main.h"
#include	"user_key.h"
#include	"user_adc.h"
#include	"io_define.h"


uint8	fIsSystemOff=0;
displayParamsStruct 	displayParams;
displayParamsStruct  	 *pDisplayParams=&displayParams;

uint16	time0Ch0Value=0;
uint8	testFlag=0;
float 	vBattVolt=0;
float	vTemperatureVolt=0;
float 	vBandgapVolt=0;

/***********************************************************************************************************
  *  @brief		重置系统进入休眠的时间
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void resetEntrySleepTime(void) {
	if (fIsSystemOff)
		sysTimes.vTimeToSleep = POWER_DOWN_MODE_TIMEOUT;							//(5+1)*50ms=300ms
	else
		sysTimes.vTimeToSleep = NORMAL_MODE_TIMEOUT;								//60s
//			//vTimeToSleep=60;								//60s
}


void	 wakeupPreProcess(void){
	resetEntrySleepTime();
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	if(!(SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)){

		SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_TICKINT_Msk);
	}

}
/***********************************************************************************************************
  *  @brief
  *
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer500ms(void) {

	gpio_bit_toggle(GPIOA,GPIO_PIN_15);

}

void	 entrySleep(void){
	fIsSystemOff=1;
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

	/* enable PMU clock */
	rcu_periph_clock_enable(RCU_PMU);
	/* enter sleep mode */
	pmu_to_sleepmode(WFI_CMD);
}

/***********************************************************************************************************
  *  @brief
  *
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer1s(void) {
	if (sysTimes.vSystem1s % 2==0) {
//		entrySleep();
//		userAdcConfig(ADC_CHANNEL_2);
		tryBandgapConvert();
		tryTemperatureConvert();
		startBattVoltDect();
	}
}
/***********************************************************************************************************
  *  @brief
  *
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer100ms(void) {

}
/***********************************************************************************************************
  *  @brief
  *
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fucnPer50ms(void) {
	if(!GET_POWER_KEY_STTS)
		resetEntrySleepTime();
	if (sysTimes.vTimeToSleep && !--sysTimes.vTimeToSleep) {

		entrySleep();
	}
	if(!fIsSystemOff){
		checkAdcConvertRetry();
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
void fucnPer5ms(void) {
	if (testFlag) {
		time0Ch0Value = (time0Ch0Value <= 10) ? 0 : (time0Ch0Value - 10);
		if (!time0Ch0Value)
			testFlag = 0;
	} else {
		time0Ch0Value += 10;
		time0Ch0Value = (time0Ch0Value >= PWM_FRQ_CONST) ? PWM_FRQ_CONST : time0Ch0Value;
		if (time0Ch0Value == PWM_FRQ_CONST)
			testFlag = 1;
	}

	getAdResultVolt(&vBattVolt,ADC_BATT_CHL);
	getAdResultVolt(&vTemperatureVolt,ADC_TEMP_CHL);
	getAdResultVolt(&vBandgapVolt,ADC_CHANNEL_17);

	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, time0Ch0Value);
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, time0Ch0Value);

	if (sysTimes.fTime50ms == 1) {
		sysTimes.fTime50ms = 0;
		fucnPer50ms();
		if (sysTimes.fTime100ms == 1) {
			sysTimes.fTime100ms = 0;
			fucnPer100ms();
			if (sysTimes.fTime500ms == 1) {
				sysTimes.fTime500ms = 0;
				fucnPer500ms();
				if (sysTimes.fTime1s == 1) {
					sysTimes.fTime1s = 0;
					sysTimes.vSystem1s += 1;
					fucnPer1s();
				}
			}
		}
	}
}

void interaction(void){
	gpio_bit_toggle(GPIOA, GPIO_PIN_12);
	if (POWER_KEY_LONG_PRESSED == vKeyValue) {
//		NVIC_DisableIRQ(SysTick_IRQn);
		fIsSystemOff=(fIsSystemOff)?0:1;
	}
	vKeyValue=0;


}
