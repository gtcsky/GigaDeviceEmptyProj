/*
 * user_exit.c
 *
 *  Created on: 2021Äê11ÔÂ4ÈÕ
 *      Author: Sky
 */

#include "user_exti.h"
#include	"io_define.h"
#include "user_key.h"

void	 singleExtiConfig(uint32_t gpio_periph,uint32_t pin,uint8_t exti_port, uint8_t exti_pin,exti_line_enum linex,uint8_t nvic_irq, uint8_t nvic_irq_priority){
	    gpio_mode_set(gpio_periph, GPIO_MODE_INPUT, GPIO_PUPD_NONE, pin);
	    gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin);

	    /* enable and set key EXTI interrupt priority */
	    nvic_irq_enable(nvic_irq,nvic_irq_priority);
	    /* connect key EXTI line to key GPIO pin */
	    syscfg_exti_line_config(exti_port, exti_pin);
	    /* configure key EXTI line */
	    exti_init(linex, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	    exti_interrupt_flag_clear(linex);
}



void	extiConfig(void){
	    /* enable the Tamper key GPIO clock */
	    rcu_periph_clock_enable(RCU_GPIOA);
	    rcu_periph_clock_enable(RCU_GPIOC);
	    rcu_periph_clock_enable(RCU_GPIOB);
	    rcu_periph_clock_enable(RCU_CFGCMP);

	    singleExtiConfig(POWER_KEY_PORT,POWER_KEY_PIN,POWER_KEY_SOURCE_PORT,POWER_KEY_SOURCE_PIN,POWER_KEY_EXTI_NO,EXTI4_15_IRQn,1U);
	    singleExtiConfig(SWITCH_KEY_PORT,SWITCH_KEY_PIN,SWITCH_KEY_SOURCE_PORT,SWITCH_KEY_SOURCE_PIN,SWITCH_KEY_EXTI_NO,EXTI0_1_IRQn,1U);

//	    gpio_mode_set(POWER_KEY_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, POWER_KEY_PIN);
//	    gpio_output_options_set(POWER_KEY_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, POWER_KEY_PIN);
//
//	    /* enable and set key EXTI interrupt priority */
//	    nvic_irq_enable(EXTI4_15_IRQn,1U);
//	    /* connect key EXTI line to key GPIO pin */
//	    syscfg_exti_line_config(POWER_KEY_SOURCE_PORT, POWER_KEY_SOURCE_PIN);
//	    /* configure key EXTI line */
//	    exti_init(POWER_KEY_EXTI_NO, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
//	    exti_interrupt_flag_clear(POWER_KEY_EXTI_NO);


}
