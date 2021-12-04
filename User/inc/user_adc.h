/*
 * user_adc.h
 *
 *  Created on: 2021年11月5日
 *      Author: Sky
 */

#ifndef PROJECTS_80_EMPTY_PROJ_USER_INC_USER_ADC_H_
#define PROJECTS_80_EMPTY_PROJ_USER_INC_USER_ADC_H_
#include	"public_define.h"


#define	ADC_BATT_CHL	ADC_CHANNEL_2
#define	ADC_TEMP_CHL	ADC_CHANNEL_1

extern adcConvertStruct adcConvertParams;

uint8 userAdcConfig(uint8 adcChannel);
void checkAdcConvertRetry(void);
extern	void startBattVoltDect(void);
extern	void tryTemperatureConvert(void);
extern	void	 adcInit(void);
/***********************************************************************************************************
  *  @brief  		尝试转换Bandgap
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
void	 tryBandgapConvert(void);

/***********************************************************************************************************
  *  @brief			read result of ADC convert
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void getAdResultVolt(float *volt, uint8_t adcChannel) ;
#endif /* PROJECTS_80_EMPTY_PROJ_USER_INC_USER_ADC_H_ */
