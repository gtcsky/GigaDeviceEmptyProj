/*
 * user_menu.h
 *
 *  Created on: 2021年11月4日
 *      Author: Sky
 */

#ifndef PROJECTS_80_EMPTY_PROJ_USER_INC_USER_MENU_H_
#define PROJECTS_80_EMPTY_PROJ_USER_INC_USER_MENU_H_
#include "public_define.h"

extern	uint8	fIsSystemOff;
extern	displayParamsStruct 	displayParams;

void fucnPer500ms(void);
void fucnPer1s(void);
void fucnPer100ms(void) ;
void fucnPer50ms(void);
void fucnPer5ms(void);
void interaction(void);
void	 wakeupPreProcess(void);
void	 entrySleep(void);
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
void resetEntrySleepTime(void) ;

#endif /* PROJECTS_80_EMPTY_PROJ_USER_INC_USER_MENU_H_ */
