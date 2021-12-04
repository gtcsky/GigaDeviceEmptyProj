/*
 * user_adc.c
 *
 *  Created on: 2021年11月5日
 *      Author: Sky
 */

#include	"user_adc.h"

#define	AD_VALUE_ARRAY_SIZE	30

adcConvertStruct adcConvertParams={0};

__IO uint16_t ad_value[AD_VALUE_ARRAY_SIZE];

/*!
 \brief      GPIO configuration function
 \param[in]  none
 \param[out] none
 \retval     none
 */
static void gpio_config(uint8_t adcChannel) {
	switch (adcChannel) {
	case ADC_CHANNEL_0:
		/* configure PA0(ADC channel0) as analog input */
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);
		break;
	case ADC_CHANNEL_1:
		/* configure PA1(ADC channel1) as analog input */
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);
		break;
	case ADC_CHANNEL_2:
		/* configure PA2(ADC channel2) as analog input */
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_2);
		break;
	case ADC_CHANNEL_3:
		/* configure PA3(ADC channel3) as analog input */
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_3);
		break;
	case ADC_CHANNEL_4:
		/* configure PA4(ADC channel4) as analog input */
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);
		break;
	case ADC_CHANNEL_5:
		/* configure PA5(ADC channel5) as analog input */
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_5);
		break;
	case ADC_CHANNEL_6:
		/* configure PA6(ADC channel6) as analog input */
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_6);
		break;
	case ADC_CHANNEL_7:
		/* configure PA7(ADC channel7) as analog input */
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_7);
		break;
	case ADC_CHANNEL_8:
		/* configure PB0(ADC channel8) as analog input */
		gpio_mode_set(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);
		break;
	case ADC_CHANNEL_9:
		/* configure PB1(ADC channel9) as analog input */
		gpio_mode_set(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);
		break;
	case ADC_CHANNEL_16:
	case ADC_CHANNEL_17:
		break;
	}
}

/*!
    \brief      DMA configuration function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(dma_channel_enum DMA_CHx,uint8_t nvic_irq, uint8_t nvic_irq_priority) {
	dma_parameter_struct dma_init_struct;

	/* initialize DMA channel0 */
	dma_deinit(DMA_CHx);
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_addr = (uint32_t) ad_value;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_init_struct.number = AD_VALUE_ARRAY_SIZE;
	dma_init_struct.periph_addr = (uint32_t) & (ADC_RDATA);
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA_CHx, &dma_init_struct);

	/* configure DMA mode */
	dma_circulation_enable(DMA_CHx);
	dma_memory_to_memory_disable(DMA_CHx);

	dma_interrupt_enable(DMA_CHx, DMA_INT_FTF);
	nvic_irq_enable(nvic_irq, nvic_irq_priority);
	/* enable DMA channel0 */
	dma_channel_enable(DMA_CHx);
}

void	 adcInit(void){
	/* ADCCLK = PCLK2/6 */
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);

	/* enable DMA clock */
	rcu_periph_clock_enable(RCU_DMA);

	/* enable ADC clock */
	rcu_periph_clock_enable(RCU_ADC);

	/* ADC resolution configure */
	adc_resolution_config(ADC_RESOLUTION_12B);

	/* ADC contineous function enable */
	adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);
	/* ADC channel length configure */
	adc_channel_length_config(ADC_REGULAR_CHANNEL, 1);

	/* ADC external trigger enable */
	adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);
	/* ADC external trigger source config */
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
	/* ADC data alignment config */
	adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
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
uint8 userAdcConfig(uint8_t adcChannel) {

	uint32 vtADCChl=0x01<<adcChannel;
	if (adcConvertParams.vSingleChlAdcConverting){			//如果当前有通道正在转换，则推迟转换
		adcConvertParams.vPospendConvertInfo|=vtADCChl;
		adcConvertParams.callbacks[adcChannel]=userAdcConfig;
		return 0;
	}else{
		adcConvertParams.callbacks[adcChannel]=0;
	}
	adcConvertParams.vPospendConvertInfo&=~vtADCChl;
	adcConvertParams.vSingleChlAdcConverting=vtADCChl;

	gpio_config(adcChannel);

	dma_config(DMA_CH0, DMA_Channel0_IRQn, 1U);


	if(adcChannel>15){
		adc_tempsensor_vrefint_enable();
	}


	/* ADC regular channel configure */
	adc_regular_channel_config(0, adcChannel, ADC_SAMPLETIME_55POINT5);
	/* enable ADC interface */
	adc_enable();
	uint8 i=100;
	while(--i);
	/* ADC calibration and reset calibration */
	adc_calibration_enable();
	/* ADC DMA function enable */
	adc_dma_mode_enable();
	/* ADC software trigger enable */
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
	return 1;

}

/************************************************************************************************************
  *  @brief  			检测是否有被推迟的ADC转换请求,如果有就处理
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :			调用周期建议小于50ms,以防止过多任务累计而产生遗漏.
  ************************************************************************************************************/
void checkAdcConvertRetry(void) {
	if (adcConvertParams.vPospendConvertInfo) {
		for (uint8 i = 0; i < 18; i++) {
			if (adcConvertParams.vPospendConvertInfo & (0x01 << i)) {
				adcConvertParams.callbacks[i](i);
				return;
			}
		}
	}
}

/***********************************************************************************************************
 *  @brief  			尝试开启电池电量通道DA转换
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
void startBattVoltDect(void) {

	userAdcConfig(ADC_BATT_CHL);

}
/***********************************************************************************************************
 *  @brief  			尝试开启电池电量通道DA转换
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
void tryTemperatureConvert(void) {

	userAdcConfig(ADC_TEMP_CHL);

}

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
void getAdResultVolt(float *volt, uint8_t adcChannel) {
	uint32 temp = 0x01 << adcChannel;
	if (adcConvertParams.vChlResultGot & temp) {
		*volt = (adcConvertParams.aDaValues[adcChannel] * SYSTEM_VDD) / ADC_CONST;
		adcConvertParams.vChlResultGot&=~temp;
	}
}
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
void	 tryBandgapConvert(void){
	userAdcConfig(ADC_CHANNEL_17);
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
//float calcTargetVolt(void){
//	float volt=0;
//	uint16 totalValue=0;
//	for(uint8 i=0;i<AD_VALUE_ARRAY_SIZE;i++){
//		totalValue+=ad_value[i];
//	}
//	totalValue/=AD_VALUE_ARRAY_SIZE;
//	return  (totalValue* SYSTEM_VDD) / ADC_CONST;
//}
uint16 getTargetDaValue(void){
	float volt=0;
	uint32 totalValue=0;
	for(uint8 i=0;i<AD_VALUE_ARRAY_SIZE;i++){
		totalValue+=ad_value[i];
	}
	return (uint16)(totalValue/AD_VALUE_ARRAY_SIZE);
//	return  (totalValue* SYSTEM_VDD) / ADC_CONST;
//	return totalValue;
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
void DMA0_IRQ_CallBack(uint32_t flag) {
	adc_disable();
	dma_channel_enable(DMA_CH0);
//	float volt=calcTargetVolt();
	uint16 vtTemp = getTargetDaValue();
	uint8 i = 0;
	for (; i < 18; i++) {
		if ((adcConvertParams.vSingleChlAdcConverting >> i) & 0x01)
			break;
	}
	adcConvertParams.aDaValues[i] = (uint16) vtTemp;
	adcConvertParams.vChlResultGot |= adcConvertParams.vSingleChlAdcConverting;
	adcConvertParams.vSingleChlAdcConverting = 0;
	if (i > 15)
		adc_tempsensor_vrefint_disable();
	__nop();
}


