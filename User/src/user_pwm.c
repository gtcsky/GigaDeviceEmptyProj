/*
 * user_pwm.c
 *
 *  Created on: 2021Äê11ÔÂ4ÈÕ
 *      Author: Sky
 */

#include "user_pwm.h"


void pwmIoConfig(uint32_t TIMEx) {

	uint32_t alt_func_num = 0;
	if (TIMEx == TIMER0) {
		alt_func_num = GPIO_AF_2;
	} else if (TIMEx == TIMER2) {
		alt_func_num = GPIO_AF_1;
	}
	/*Configure PA8(TIMER0_CH0) as alternate function*/
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8 | GPIO_PIN_11);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_11);
	gpio_af_set(GPIOA, alt_func_num, GPIO_PIN_8 | GPIO_PIN_11);
}


void pwmConfig(uint32_t TIMEx) {
	pwmIoConfig(TIMEx);

	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(RCU_TIMER0);
	timer_deinit(TIMEx);

	/* TIMER0 configuration */
	timer_initpara.prescaler = 0;
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection = TIMER_COUNTER_UP;
	timer_initpara.period = PWM_FRQ_CONST;
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMEx, &timer_initpara);

	/* CH0 configuration in PWM mode */
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	timer_channel_output_config(TIMEx, TIMER_CH_0, &timer_ocintpara);
	timer_channel_output_pulse_value_config(TIMEx, TIMER_CH_0, DEFAULT_TIME0_CH0_PWM);
	timer_channel_output_mode_config(TIMEx, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMEx, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

	timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_LOW;
	timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_HIGH;
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	timer_channel_output_config(TIMEx, TIMER_CH_3, &timer_ocintpara);
	timer_channel_output_pulse_value_config(TIMEx, TIMER_CH_3, DEFAULT_TIME0_CH3_PWM);
	timer_channel_output_mode_config(TIMEx, TIMER_CH_3, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMEx, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

	timer_primary_output_config(TIMEx, ENABLE);
	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMEx);
	timer_enable(TIMEx);
}
