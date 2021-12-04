/*!
 \file    gd32e23x_it.c
 \brief   interrupt service routines

 \version 2019-02-19, V1.0.0, firmware for GD32E23x
 */

/*
 Copyright (c) 2019, GigaDevice Semiconductor Inc.

 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software without
 specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 OF SUCH DAMAGE.
 */

#include "gd32e23x_it.h"
#include "systick.h"
#include	"user_key.h"
#include	"user_menu.h"
#include "gd32e23x_dma.h"
#include	"user_internalFlash.h"

/*!
 \brief      this function handles NMI exception
 \param[in]  none
 \param[out] none
 \retval     none
 */
void NMI_Handler(void) {

}

/*!
 \brief      this function handles HardFault exception
 \param[in]  none
 \param[out] none
 \retval     none
 */
void HardFault_Handler(void) {
	/* if Hard Fault exception occurs, go to infinite loop */
	while (1) {
		StoreExceptions2Flash(1);
		NVIC_SystemReset();
	}
}

/*!
 \brief      this function handles MemManage exception
 \param[in]  none
 \param[out] none
 \retval     none
 */
void MemManage_Handler(void) {
	/* if Memory Manage exception occurs, go to infinite loop */
	while (1) {
		StoreExceptions2Flash(1);
		NVIC_SystemReset();
	}
}

/*!
 \brief      this function handles BusFault exception
 \param[in]  none
 \param[out] none
 \retval     none
 */
void BusFault_Handler(void) {
	/* if Bus Fault exception occurs, go to infinite loop */
	while (1) {
		StoreExceptions2Flash(1);
		NVIC_SystemReset();
	}
}

/*!
 \brief      this function handles UsageFault exception
 \param[in]  none
 \param[out] none
 \retval     none
 */
void UsageFault_Handler(void) {
	/* if Usage Fault exception occurs, go to infinite loop */
	while (1) {
		NVIC_SystemReset();
	}
}

/*!
 \brief      this function handles SVC exception
 \param[in]  none
 \param[out] none
 \retval     none
 */
void SVC_Handler(void) {
}

/*!
 \brief      this function handles DebugMon exception
 \param[in]  none
 \param[out] none
 \retval     none
 */
void DebugMon_Handler(void) {
}

/*!
 \brief      this function handles PendSV exception
 \param[in]  none
 \param[out] none
 \retval     none
 */
void PendSV_Handler(void) {
}

/*!
 \brief      this function handles SysTick exception
 \param[in]  none
 \param[out] none
 \retval     none
 */
void SysTick_Handler(void) {
//	delay_decrement();
	sysTickCallback();
}

/*!
 \brief      this function handles external lines 0 to 1 interrupt request
 \param[in]  none
 \param[out] none
 \retval     none
 */
void EXTI0_1_IRQHandler(void) {
	wakeupPreProcess();
	if (RESET != exti_interrupt_flag_get(EXTI_0)) {
		exti_interrupt_flag_clear(EXTI_0);
		setSwitchKeyPressedFlag();
	}
}
/*!
 \brief      this function handles external lines 2 to 3 interrupt request
 \param[in]  none
 \param[out] none
 \retval     none
 */
void EXTI2_3_IRQHandler(void) {
	wakeupPreProcess();

}
/*!
 \brief      this function handles external lines 4 to 15 interrupt request
 \param[in]  none
 \param[out] none
 \retval     none
 */
void EXTI4_15_IRQHandler(void) {
	wakeupPreProcess();
	if (RESET != exti_interrupt_flag_get(EXTI_13)) {
		exti_interrupt_flag_clear(EXTI_13);
		setPowerKeyPressedFlag();
	}
}

void __WEAK DMA0_IRQ_CallBack(uint32_t flag) {

}

/*!
    \brief      this function handles DMA channel 0 request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA_Channel0_IRQHandler(void){
  	if (RESET != dma_interrupt_flag_get(DMA_CH0,DMA_INT_FLAG_FTF)) {
      dma_interrupt_flag_clear(DMA_CH0,DMA_INT_FLAG_FTF);
      DMA0_IRQ_CallBack(DMA_INT_FLAG_FTF);
    }
}
