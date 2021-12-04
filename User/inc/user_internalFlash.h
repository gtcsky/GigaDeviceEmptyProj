/*
 * user_internalFlash.h
 *
 *  Created on: 2021Äê11ÔÂ6ÈÕ
 *      Author: Sky
 */

#ifndef PROJECTS_80_EMPTY_PROJ_USER_INC_USER_INTERNALFLASH_H_
#define PROJECTS_80_EMPTY_PROJ_USER_INC_USER_INTERNALFLASH_H_
#include		"public_define.h"

#define  		CHECK_INFO_DATA	0x5A3A

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
FLASH_Status StoreExceptions2Flash(uint8 addCheckInfo);

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
FLASH_Status LoadExceptionsFromFlash(displayParamsStruct * disParams);


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
int exceptionsCheck(void);

#endif /* PROJECTS_80_EMPTY_PROJ_USER_INC_USER_INTERNALFLASH_H_ */
