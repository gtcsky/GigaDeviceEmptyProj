/*
 * user_internalFlash.c
 *
 *  Created on: 2021年11月6日
 *      Author: Sky
 */

#include	"user_internalFlash.h"
#include	"user_menu.h"
#include	"user_i2cm.h"
#include	"stdlib.h"
#include	<string.h>

#define 	FMC_PAGE_SIZE           		((uint16)0x400U)
#define 	FMC_WRITE_START_ADDR    	((uint32)0x0800FC00U)
#define 	WRITE_START_ADDR    		((uint32)0x0800FC00U)
#define 	FMC_WRITE_END_ADDR      	((uint32)0x08010000U)
#define	WRITE_END_ADDR      			((uint32)0x08010000U)

uint32 *ptrd;
uint32 address = 0x00U;
uint32 data0 = 0x01234567U;
uint64_t data1 = 0x0123456789abcdefU;
led_typedef_enum lednum = LED2;

/* calculate the number of page to be programmed/erased */
uint32 page_num = (FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR) / FMC_PAGE_SIZE;
/* calculate the number of page to be programmed/erased */
uint32 word_num0 = ((FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR) >> 2);
/* calculate the number of page to be programmed/erased by double word */
uint32 word_num1 = ((FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR) >> 3);

/*!
 \brief      erase fmc pages from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR
 \param[in]  none
 \param[out] none
 \retval     none
 */
void fmc_erase_pages(void) {
	uint32 erase_counter;

	/* unlock the flash program/erase controller */
	fmc_unlock();

	/* clear all pending flags */
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

	/* erase the flash pages */
	for (erase_counter = 0U; erase_counter < page_num; erase_counter++) {
		fmc_page_erase(FMC_WRITE_START_ADDR + (FMC_PAGE_SIZE * erase_counter));
		fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
	}

	/* lock the main FMC after the erase operation */
	fmc_lock();
}

/*!
 \brief      program fmc word by word from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR
 \param[in]  none
 \param[out] none
 \retval     none
 */
void fmc_program(void) {
	/* unlock the flash program/erase controller */
	fmc_unlock();

	address = FMC_WRITE_START_ADDR;
	/* program flash */
	while (address < FMC_WRITE_END_ADDR) {
		fmc_word_program(address, data0);
		address += 4U;
		fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
	}

	/* lock the main FMC after the program operation */
	fmc_lock();
}

/*!
 \brief      program fmc double word by double word from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR
 \param[in]  none
 \param[out] none
 \retval     none
 */
void fmc_program_doubleword(void) {
	/* unlock the flash program/erase controller */
	fmc_unlock();

	address = FMC_WRITE_START_ADDR;

	/* program flash */
	while (address < FMC_WRITE_END_ADDR) {
		fmc_doubleword_program(address, data1);
		address += 8U;
		fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
	}

	/* lock the main FMC after the program operation */
	fmc_lock();
}

/*!
 \brief      check fmc erase result
 \param[in]  none
 \param[out] none
 \retval     none
 */
void fmc_erase_pages_check(void) {
	uint32 i;

	ptrd = (uint32 *) FMC_WRITE_START_ADDR;

	/* check flash whether has been erased */
	for (i = 0U; i < word_num0; i++) {
		if (0xFFFFFFFFU != (*ptrd)) {
			lednum = LED1;
			gd_eval_led_on(lednum);
			break;
		} else {
			ptrd++;
		}
	}
}

/*!
 \brief      check fmc program result
 \param[in]  none
 \param[out] none
 \retval     none
 */
void fmc_program_check(void) {
	uint32 i;

	ptrd = (uint32 *) FMC_WRITE_START_ADDR;

	/* check flash whether has been programmed */
	for (i = 0U; i < word_num0; i++) {
		if ((*ptrd) != data0) {
			lednum = LED1;
			gd_eval_led_on(lednum);
			break;
		} else {
			ptrd++;
		}
	}
}

/*!
 \brief      check fmc program result
 \param[in]  none
 \param[out] none
 \retval     none
 */
void fmc_program_doubleword_check(void) {
	uint32 i;
	uint32 data_0, data_1;

	ptrd = (uint32 *) FMC_WRITE_START_ADDR;

	/* check flash whether has been programmed */
	for (i = 0U; i < word_num1; i++) {
		data_0 = *ptrd;
		ptrd++;
		data_1 = *ptrd;
		if ((data_0 | ((uint64_t) data_1 << 32U)) != data1) {
			lednum = LED1;
			gd_eval_led_on(lednum);
			break;
		} else {
			ptrd++;
		}
	}
}

/*!
 \brief      main function
 \param[in]  none
 \param[out] none
 \retval     none
 */
//int main(void) {
//	/* initialize led on the board */
//	gd_eval_led_init(LED1);
//	gd_eval_led_init(LED2);
//
//	/* step1: erase pages and check if it is successful. If not, light the LED1. */
//	fmc_erase_pages();
//	fmc_erase_pages_check();
//
//	/* step2: word program and check if it is successful. If not, light the LED2. */
//	fmc_program();
//	fmc_program_check();
//
//	/* step3: erase pages and check if it is successful. If not, light the LED1. */
//	fmc_erase_pages();
//	fmc_erase_pages_check();
//
//	/* step4: double word program and check if it is successful. If not, light the LED3. */
//	fmc_program_doubleword();
//	fmc_program_doubleword_check();
//
//	/* if all the operations are successful, light the LED2. */
//	if (LED2 == lednum) {
//		gd_eval_led_on(LED2);
//	}
//
//	while (1) {
//	}
//}


/***********************************************************************************************************
  *  @brief	store the system parameters when some exceptions occurred ,before  the MCU reset
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
FLASH_Status StoreExceptions2Flash(uint8 addCheckInfo) {
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
//	uint32 arr[10]={0};
	uint8 len = sizeof(displayParams); // add checkInfo
	uint32 * p = (uint32 *) malloc(len);
	if (p == NULL)
		return FLASH_BUSY;
	if(addCheckInfo)
		displayParams.checkCode = CHECK_INFO_DATA;
	else
		displayParams.checkCode = 0x0000;
#if(EEPROM_INSIDE==1)
	writeEepromData((uint8 *)&displayParams,SYSTEM_PARAM_START_ADDR,len);
#else
	fmc_erase_pages();
	memcpy(p , &displayParams, len);
	uint32 Address = 0;
	fmc_unlock();

	address = WRITE_START_ADDR;
	/* program flash */
	uint8 i=0;
	while (i++ < (len+1)/4) {
		fmc_word_program(address, *(p ++));
		address += 4U;
		fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
	}
	/* lock the main FMC after the program operation */
	fmc_lock();

#endif
	free(p);
	return FLASHStatus;
}
/***********************************************************************************************************
  *  @brief			load exceptions from the flash which stored data by last Error if exist.
  *
  *
  *  @param [in] :
  *
  *  @param [out] :		disParams: if exceptions exist ,load the last system parameters  and store to
  *  							disParams.
  *  							if  not exceptions,   disParams will not be changed.
  *
  *  @return :			read flash result
  *
  *  @note :
  ************************************************************************************************************/
FLASH_Status LoadExceptionsFromFlash(displayParamsStruct * disParams){
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	uint8 len=sizeof(displayParams);// add checkInfo
#if(EEPROM_INSIDE==1)
	uint8 * p=(uint8 *)malloc(len);
	if(p==NULL)
		return FLASH_BUSY;
//	memset(p,0x11,5);
	readEepromData(p,SYSTEM_PARAM_START_ADDR,len);
	if(*((uint16 *)p)!=CHECK_INFO_DATA){
		free(p);
		return FLASH_BUSY;
	}
#else
	uint32 Address = WRITE_START_ADDR;
	uint8 i=0;
	uint32 * p=(uint32 *)malloc(len);
	if(p==NULL)
		return FLASH_BUSY;
//	i=0;
//	while (i < (len+1)/4) {
////		*(p+(i++))=*(__IO uint32*) Address;
//		p=(uint32 *) Address;
//		Address += 4;
//		i++;
//	}
		p=(uint32 *) Address;
	if((*p&0x0000FFFF)!=CHECK_INFO_DATA){
		free(p);
		return FLASH_BUSY;
	}
#endif
	memcpy(disParams,p,len);
		free(p);
	return	FLASHStatus;
}

void clearExceptions(void) {

#if(EEPROM_INSIDE==1)
	displayParams.checkCode = 0x0000;
	writeEepromData((uint8 *) &displayParams, SYSTEM_PARAM_START_ADDR, 2);
#else
	StoreExceptions2Flash(0);
#endif

}
/***********************************************************************************************************
  *  @brief  						check if there are some exceptions exist
  *
  *
  *  @param [in] :					None
  *
  *  @param [out] :					if some exceptions exist ,displayParams will be load as the last
  *  									data before exceptions  occurred
  *
  *  @return :						0/1   no/have some exceptions
  *
  *  @note :
  ************************************************************************************************************/
int exceptionsCheck(void) {
	if (FLASH_COMPLETE == LoadExceptionsFromFlash(&displayParams)) {
		if (CHECK_INFO_DATA != displayParams.checkCode) {					//新Flash或未记录到异常关机信息
			return 1;
		}else{
			clearExceptions();
			return 0;
		}
	}else {
		return 1;
	}
}

