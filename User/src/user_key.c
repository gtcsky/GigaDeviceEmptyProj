
/*
 * user_key.c
 *
 *  	Created on: 2021-05-17
 *      Author: Sky Zhang
 */
#include "user_key.h"
#include "user_menu.h"
#include "user_main.h"
#include "io_define.h"

volatile	uint8 	vKeyValue;



//UINT8 vCodingChkIndex;
uint8	vIncTestCnt=0;
uint8	vDecTestCnt=0;
static 	uint8 	vIsSetKeyGod;
static	uint16	vSetKeyTimer;

//static 	uint8 	vIsIncKeyGod;
//static	uint16	vIncKeyTimer;
//
//static 	uint8 	vIsDecKeyGod;
//static	uint16	vDecKeyTimer;
//
static 	uint8 	vIsSwitchKeyGod;
static	uint16	vSwitchKeyTimer;

volatile	uint8 	vIsKeyPressed=0;
uint8	vDoubleClickCountDown;

//uint8	fIsEncoderPending=FALSE;

//static uint8	fIsSwitchKeyGroupA=FALSE;
//typedef void(* callbacksType)(void);
#define	SHOT_KEY_GOD 	0X01
#define	LONG_KEY_GOD 	0X02
typedef struct{
	uint8 	currentKeyPressed;			//START_KEY_PRESSED
	uint8 	* currentKeyGod;				//vIsStartKeyGod
	uint8	shortKeyValue;
	uint8 	longKeyValue;
	uint16   	* keyTimer;					//vStartKeyTimer
//	GPIO_TypeDef       *GPIOx;
	uint32_t	 GPIOx;
	uint16 	GPIO_Pin;
	uint8 	GPIO_No;
	uint16	shortTimer;
	uint16	longTimer;
	uint8	fastAdjutMode;

}keyDectDef;

uint8 fIsFuncTurnLeft;
uint8 fIsFuncLeftFallEdge;
uint8 fIsFuncLeftRiseEdge;
uint8 fIsFuncTurnRight;
uint8 fIsFuncRightFallEdge;
uint8 fIsFuncRightRiseEdge;
uint8 fIsFuncCodingTurnStart;
UINT16 vFuncLeftLowTimeCnt;
UINT16 vFuncLeftHiTimeCnt;
UINT16 vFuncRightHiTimeCnt;
UINT16 vFuncRightLowTimeCnt;
uint8 vLastFuncKeyValue;
uint16 vLastFuncKeyTimer;

uint8 fIsBrightTurnLeft;
uint8 fIsBrightLeftFallEdge;
uint8 fIsBrightLeftRiseEdge;
uint8 fIsBrightTurnRight;
uint8 fIsBrightRightFallEdge;
uint8 fIsBrightRightRiseEdge;
uint8 fIsBrightCodingTurnStart;
UINT16 vBrightLeftLowTimeCnt;
UINT16 vBrightLeftHiTimeCnt;
UINT16 vBrightRightHiTimeCnt;
UINT16 vBrightRightLowTimeCnt;
uint8 vLastBrightKeyValue;
uint16 vLastBrightKeyTimer;
//
//typedef struct{
//		uint8 vLeftKeyValue;
//		uint8 vRightKeyValue;
//		uint8 *fIsTurnLeft;
//		uint8 *fIsLeftFallEdge;
//		uint8 *fIsLeftRiseEdge;
//		uint8 *fIsTurnRight;
//		uint8 *fIsRightFallEdge;
//		uint8 *fIsRightRiseEdge;
//		uint8 *fIsCodingTurnStart;
//		UINT16 *vLeftLowTimeCnt;
//		UINT16 *vLeftHiTimeCnt;
//		UINT16 *vRightHiTimeCnt;
//		UINT16 *vRightLowTimeCnt;
//		GPIO_TypeDef       *LeftGPIOx;
//		uint16 	Left_GPIO_Pin;
//		uint8 	Left_GPIO_No;
//		GPIO_TypeDef       *RightGPIOx;
//		uint16 	Right_GPIO_Pin;
//		uint8 	Right_GPIO_No;
//		uint8	*vLastKeyValue;
//		uint16	*vLastKeyTimer;
//}encoderDectDef;
//
//encoderDectDef fucnEncoderDef= {
//		.vLeftKeyValue=DEC_KEY_SHORT_PRESSED,
//		.vRightKeyValue=INC_KEY_SHORT_PRESSED,
//		.fIsTurnLeft=&fIsFuncTurnLeft,
//		.fIsLeftFallEdge=&fIsFuncLeftFallEdge,
//		.fIsLeftRiseEdge=&fIsFuncLeftRiseEdge,
//		.fIsTurnRight=&fIsFuncTurnRight,
//		.fIsRightFallEdge=&fIsFuncRightFallEdge,
//		.fIsRightRiseEdge=&fIsFuncRightRiseEdge,
//		.fIsCodingTurnStart=&fIsFuncCodingTurnStart,
//		.vLeftLowTimeCnt=&vFuncLeftLowTimeCnt,
//		.vLeftHiTimeCnt=&vFuncLeftHiTimeCnt,
//		.vRightHiTimeCnt=&vFuncRightHiTimeCnt,
//		.vRightLowTimeCnt=&vFuncRightLowTimeCnt,
//		.LeftGPIOx=KeyDec_Port,
//		.Left_GPIO_Pin=KeyDec_Pin,
//		.Left_GPIO_No=KeyDec_PinNo,
//		.RightGPIOx=KeyInc_Port,
//		.Right_GPIO_Pin=KeyInc_Pin,
//		.Right_GPIO_No=KeyInc_PinNo,
//		.vLastKeyValue=&vLastFuncKeyValue,
//		.vLastKeyTimer=&vLastFuncKeyTimer,
//
//};
//
//encoderDectDef brightEncoderDef= {
//		.vLeftKeyValue=BRIGHTNESS_DOWN_KEY_SHORT_PRESSED,
//		.vRightKeyValue=BRIGHTNESS_UP_KEY_SHORT_PRESSED,
//		.fIsTurnLeft=&fIsBrightTurnLeft,
//		.fIsLeftFallEdge=&fIsBrightLeftFallEdge,
//		.fIsLeftRiseEdge=&fIsBrightLeftRiseEdge,
//		.fIsTurnRight=&fIsBrightTurnRight,
//		.fIsRightFallEdge=&fIsBrightRightFallEdge,
//		.fIsRightRiseEdge=&fIsBrightRightRiseEdge,
//		.fIsCodingTurnStart=&fIsBrightCodingTurnStart,
//		.vLeftLowTimeCnt=&vBrightLeftLowTimeCnt,
//		.vLeftHiTimeCnt=&vBrightLeftHiTimeCnt,
//		.vRightHiTimeCnt=&vBrightRightHiTimeCnt,
//		.vRightLowTimeCnt=&vBrightRightLowTimeCnt,
//		.LeftGPIOx=KeyBrightDec_Port,
//		.Left_GPIO_Pin=KeyBrightDec_Pin,
//		.Left_GPIO_No=KeyBrightDec_PinNo,
//		.RightGPIOx=KeyBrightInc_Port,
//		.Right_GPIO_Pin=KeyBrightInc_Pin,
//		.Right_GPIO_No=KeyBrightInc_PinNo,
//		.vLastKeyValue=&vLastBrightKeyValue,
//		.vLastKeyTimer=&vLastBrightKeyTimer,
//};

//
keyDectDef  powerKeyInfo={
		.currentKeyPressed=POWER_KEY_PRESSED,
		.currentKeyGod=&vIsSetKeyGod,
		.shortKeyValue=FUNC_KEY_SHORT_PRESSED,
		.longKeyValue=POWER_KEY_LONG_PRESSED,
		.keyTimer=&vSetKeyTimer,
		.GPIOx=POWER_KEY_PORT,
		.GPIO_Pin=POWER_KEY_PIN,
		.shortTimer=SHORT_PRESSED_TIME,						//step 5ms
		.longTimer=LONG_PRESSED_1S,							//step 5ms
};
//
keyDectDef  switchKeyInfo={
		.currentKeyPressed=SWITCH_KEY_PRESSED,
		.currentKeyGod=&vIsSwitchKeyGod,
		.shortKeyValue=SWITCH_KEY_SHORT_PRESSED,
		.longKeyValue=SWITCH_KEY_LONG_PRESSED,
		.keyTimer=&vSwitchKeyTimer,
		.GPIOx=SWITCH_KEY_PORT,
		.GPIO_Pin=SWITCH_KEY_PIN,
		.shortTimer=SHORT_PRESSED_TIME,						//step 5ms
		.longTimer=LONG_PRESSED_1S,							//step 5ms
};

/***********************************************************************************************************
  *  @brief		?????????????????????????????????
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void	 keyReleased(keyDectDef * keydef){
		vIsKeyPressed&=~(keydef->currentKeyPressed);//
		*(keydef->keyTimer)=0;
		*(keydef->currentKeyGod)&=~(SHOT_KEY_GOD|LONG_KEY_GOD);
}
/***********************************************************************************************************
  *  @brief		?????????????????????????????????
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void	setKeyPressedFlag(keyDectDef * keydef){
		*(keydef->keyTimer)= 0x10000 - sysTimes.vSystemLIRC5ms;
		vIsKeyPressed |= keydef->currentKeyPressed;
}
/***********************************************************************************************************
  *  @brief  			??????????????????
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
void	 keyReleaseCheck(keyDectDef * keydef){

	if((vIsKeyPressed&keydef->currentKeyPressed)&&(GPIO_ISTAT(keydef->GPIOx)&keydef->GPIO_Pin)){
		if((*(keydef->currentKeyGod)&SHOT_KEY_GOD)&&!(*(keydef->currentKeyGod)&LONG_KEY_GOD)){
			if(!keydef->fastAdjutMode)
				vKeyValue = keydef->shortKeyValue;
		}
		keyReleased(keydef);
	}

}
/***********************************************************************************************************
  *  @brief  			??????????????????
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
//void keyPressCheck(keyDectDef * keydef){
//	if(!(keydef->GPIOx->DI&keydef->GPIO_Pin)){									//pin keep low status
//		if ((vIsKeyPressed&keydef->currentKeyPressed)&&*(keydef->keyTimer) ) {
//			if (!(*(keydef->currentKeyGod)&SHOT_KEY_GOD)) {
//				if (((*(keydef->keyTimer)+ sysTimes.vSystemLIRC5ms)&0xffff) > keydef->shortTimer) {
//					*(keydef->currentKeyGod)|=SHOT_KEY_GOD;
//					if(keydef->fastAdjutMode)
//						vKeyValue =  keydef->shortKeyValue;
//				}
//			} else {
//				if (!(*(keydef->currentKeyGod)&LONG_KEY_GOD)) {
//					if (((*(keydef->keyTimer)+ sysTimes.vSystemLIRC5ms)&0xffff) > keydef->longTimer) {
//						vKeyValue =  keydef->longKeyValue;
//						*(keydef->currentKeyGod)|=LONG_KEY_GOD;
//						*(keydef->keyTimer) = 0;	//stop cnt
//					}
//				}
//			}
//		}else{
//			setKeyPressedFlag(keydef);
//		}
//	}
//}
void keyPressCheck(keyDectDef * keydef) {
	if (!(GPIO_ISTAT(keydef->GPIOx) & keydef->GPIO_Pin)) {									//pin keep low status
		if ((vIsKeyPressed & keydef->currentKeyPressed) && *(keydef->keyTimer)) {
			if (!(*(keydef->currentKeyGod) & SHOT_KEY_GOD)) {
				if (((*(keydef->keyTimer) + sysTimes.vSystemLIRC5ms) & 0xffff) > keydef->shortTimer) {
					*(keydef->currentKeyGod) |= SHOT_KEY_GOD;
					if (keydef->fastAdjutMode)
						vKeyValue = keydef->shortKeyValue;
				}
			} else {
				if (!(*(keydef->currentKeyGod) & LONG_KEY_GOD)) {
					if (((*(keydef->keyTimer) + sysTimes.vSystemLIRC5ms) & 0xffff) > keydef->longTimer) {
						vKeyValue = keydef->longKeyValue;
						*(keydef->currentKeyGod) |= LONG_KEY_GOD;
						*(keydef->keyTimer) = 0;	//stop cnt
					}
				}
			}
		} else {
			setKeyPressedFlag(keydef);
		}
	}
}
/***********************************************************************************************************
  *  @brief		clear Power key registers
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void clearPowerKeyInfo(void) {
	*powerKeyInfo.currentKeyGod &= ~(SHOT_KEY_GOD | LONG_KEY_GOD);
	*powerKeyInfo.keyTimer = 0;
}
/***********************************************************************************************************
  *  @brief			??????????????????
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void fastKeyCheck(keyDectDef * keydef) {
	if (*(keydef->currentKeyGod) & LONG_KEY_GOD) {
		vKeyValue = keydef->shortKeyValue;
	}
}
/***********************************************************************************************************
  *  @brief			??????????????????
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
void funcKeyTask(void) {
//	if (FactoryMode == displayParams.DisplayModeIndex&&factoryParams.factoryIndex==WaitFuncKey){
//		switchKeyInfo.fastAdjutMode=1;
//		switchKeyInfo.longTimer=LONG_PRESSED_0S3;
//		powerKeyInfo.fastAdjutMode=1;
//		powerKeyInfo.longTimer=LONG_PRESSED_0S3;
//	}

	keyReleaseCheck(&powerKeyInfo);
	keyPressCheck(&powerKeyInfo);
//	keyReleaseCheck(&incKeyInfo);
//	keyPressCheck(&incKeyInfo);
//	keyReleaseCheck(&decKeyInfo);
//	keyPressCheck(&decKeyInfo);
	keyReleaseCheck(&switchKeyInfo);
	keyPressCheck(&switchKeyInfo);
}
/***********************************************************************************************************
  *  @brief				??????????????????A??????
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/
//void holdingKeyGroupA(void) {
//	fIsSwitchKeyGroupA = TRUE;
//	fastKeyCheck(&incKeyInfo);
//	if (FactoryMode == displayParams.DisplayModeIndex&&factoryParams.factoryIndex==WaitFuncKey){
//		fastKeyCheck(&switchKeyInfo);
//	}
//}
/***********************************************************************************************************
  *  @brief				??????????????????B??????
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :
  ************************************************************************************************************/

//void holdingKeyGroupB(void) {
//	fIsSwitchKeyGroupA = FALSE;
//	fastKeyCheck(&decKeyInfo);
//	if (FactoryMode == displayParams.DisplayModeIndex&&factoryParams.factoryIndex==WaitFuncKey){
//		fastKeyCheck(&powerKeyInfo);
//	}
//}
//
/***********************************************************************************************************
  *  @brief			??????????????????????????????
  *
  *  @param [in] :
  *
  *  @param [out] :
  *
  *  @return :
  *
  *  @note :			?????????????????????????????????????????????,??????????????????????????????
  ************************************************************************************************************/
//void holdingKeyFunc(void) {
//	fIsSwitchKeyGroupA ^= TRUE;
//	if (fIsSwitchKeyGroupA) {
//		holdingKeyGroupA();
//	} else {
//		holdingKeyGroupB();
//	}
//}
//
/***********************************************************************************************************
 *  @brief		set Power click registers
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :		invoke when IRQ  occurred
 ************************************************************************************************************/
void setPowerKeyPressedFlag(void) {
	setKeyPressedFlag(&powerKeyInfo);
}
/***********************************************************************************************************
 *  @brief		set Switch click registers
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :		invoke when IRQ  occurred
 ************************************************************************************************************/
void setSwitchKeyPressedFlag(void) {
	setKeyPressedFlag(&switchKeyInfo);
}
/***********************************************************************************************************
 *  @brief		set "Inc" click registers
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :		invoke when IRQ  occurred
 ************************************************************************************************************/

//void setIncKeyPressedFlag(void) {
//	setKeyPressedFlag(&incKeyInfo);
//}
/***********************************************************************************************************
 *  @brief		set "Dec" click registers
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :		invoked when IRQ  occurred
 ************************************************************************************************************/
//void setDecKeyPressedFlag(void) {
//	setKeyPressedFlag(&decKeyInfo);
//}

//void abandonAllDectInfo(){
////	vCodingChkIndex=0;
//	vRightLowTimeCnt=0;
//	vRightHiTimeCnt=0;
//	vLeftLowTimeCnt=0;
//	vLeftHiTimeCnt=0;
//	fIsRightFallEdge=0;
//	fIsLeftFallEdge=0;
//	fIsTurnLeft=0;
//	fIsTurnRight=0;
//	fIsRightRiseEdge=0;
//	fIsLeftRiseEdge=0;
//	fIsCodingTurnStart=0;
//}
///***********************************************************************************************************
// *  @brief 				clear all encoder detect information
// *
// *  @param [in] :
// *
// *  @param [out] :
// *
// *  @return :
// *
// *  @note :
// ************************************************************************************************************/
//void abandonAllDectInfo(encoderDectDef * dectDef, uint8 includeLastInfo) {
//	*dectDef->vRightLowTimeCnt = 0;
//	*dectDef->vRightHiTimeCnt = 0;
//	*dectDef->vLeftLowTimeCnt = 0;
//	*dectDef->vLeftHiTimeCnt = 0;
//	*dectDef->fIsRightFallEdge = 0;
//	*dectDef->fIsLeftFallEdge = 0;
//	*dectDef->fIsTurnLeft = 0;
//	*dectDef->fIsTurnRight = 0;
//	*dectDef->fIsRightRiseEdge = 0;
//	*dectDef->fIsLeftRiseEdge = 0;
//	*dectDef->fIsCodingTurnStart = 0;
//	if (includeLastInfo) {
//		*dectDef->vLastKeyTimer = 0;
//		*dectDef->vLastKeyValue = 0;
//	}
//}
///***********************************************************************************************************
//  *  @brief			???????????????????????????
//  *
//  *  @param [in] :
//  *
//  *  @param [out] :
//  *
//  *  @return :
//  *
//  *  @note :			???????????????????????????,?????????????????????????????????????????????.
//  ************************************************************************************************************/
//void encoderDect(encoderDectDef * dectDef) {
////	uint16 temp=0;
//	if (!(*dectDef->fIsCodingTurnStart)) {				//????????????
//		if (!(dectDef->LeftGPIOx->DI & dectDef->Left_GPIO_Pin)) {
//			*dectDef->fIsCodingTurnStart = 1;
//			*dectDef->vLeftLowTimeCnt = (~sysTimes.vTime1msCnt + 1);
//			*dectDef->fIsLeftFallEdge = 1;
//		}
//		if (!(dectDef->RightGPIOx->DI & dectDef->Right_GPIO_Pin)) {
//			*dectDef->fIsCodingTurnStart = 1;
//			*dectDef->vRightLowTimeCnt = (~sysTimes.vTime1msCnt + 1);
//			*dectDef->fIsRightFallEdge = 1;
//		}
//	} else {							//??????????????????
//		if ((dectDef->LeftGPIOx->DI & dectDef->Left_GPIO_Pin)) {
//			if (*dectDef->fIsLeftFallEdge && !(*dectDef->fIsLeftRiseEdge)) {
//				*dectDef->fIsLeftRiseEdge = 1;
//				*dectDef->vLeftHiTimeCnt = (~sysTimes.vTime1msCnt + 1);
//			}
//		} else {
//			if (!(*dectDef->fIsLeftFallEdge)) {
//				*dectDef->fIsLeftFallEdge = 1;
//				*dectDef->vLeftLowTimeCnt = (~sysTimes.vTime1msCnt + 1);
//			}
//		}
//		if ((dectDef->RightGPIOx->DI & dectDef->Right_GPIO_Pin)) {
//			if (*dectDef->fIsRightFallEdge && !(*dectDef->fIsRightRiseEdge)) {
//				*dectDef->fIsRightRiseEdge = 1;
//				*dectDef->vRightHiTimeCnt = (~sysTimes.vTime1msCnt + 1);
//			}
//		} else {
//			if (!(*dectDef->fIsRightFallEdge)) {
//				*dectDef->fIsRightFallEdge = 1;
//				*dectDef->vRightLowTimeCnt = (~sysTimes.vTime1msCnt + 1);
//			}
//		}
//		if ((dectDef->RightGPIOx->DI & dectDef->Right_GPIO_Pin) && (dectDef->LeftGPIOx->DI & dectDef->Left_GPIO_Pin) && *dectDef->fIsRightFallEdge
//				&& (*dectDef->fIsLeftFallEdge) && *dectDef->fIsRightRiseEdge && *dectDef->fIsLeftRiseEdge) {
//			if ((((*dectDef->vLeftHiTimeCnt + sysTimes.vTime1msCnt)&0xFFFF) > 1) && (((*dectDef->vRightHiTimeCnt + sysTimes.vTime1msCnt)&0xFFFF) > 1)) {
//				if ((((*dectDef->vLeftLowTimeCnt + sysTimes.vTime1msCnt)&0xFFFF) > 5) || ((*dectDef->vRightLowTimeCnt + sysTimes.vTime1msCnt)&0xFFFF) > 5) {			//????????????????????????????????????5ms
//					if (((*dectDef->vLeftLowTimeCnt + sysTimes.vTime1msCnt)&0xFFFF) > ((*dectDef->vRightLowTimeCnt + sysTimes.vTime1msCnt)&0xFFFF)) {
//						abandonAllDectInfo(dectDef,0);
//						vKeyValue = dectDef->vLeftKeyValue;
//						if(*dectDef->vLastKeyTimer){
//							if((*dectDef->vLastKeyValue&FAST_ADJUST_FALG_MASK)==vKeyValue){
//								if(((*dectDef->vLastKeyTimer+sysTimes.vSystemLIRC5ms)&0xffff)<FSAT_TIME_GAP){
//									vKeyValue|=FAST_ADJUST_FLAG;
//								}
//							}
//						}
//						*dectDef->vLastKeyTimer= 0x10000 - sysTimes.vSystemLIRC5ms;
//						*dectDef->vLastKeyValue= vKeyValue;
//						vDecTestCnt++;
//					} else if (((*dectDef->vRightLowTimeCnt + sysTimes.vTime1msCnt)&0xFFFF) > ((*dectDef->vLeftLowTimeCnt + sysTimes.vTime1msCnt)&0xFFFF)) {
//						abandonAllDectInfo(dectDef,0);
//						vKeyValue = dectDef->vRightKeyValue;
//						if(*dectDef->vLastKeyTimer){
//							if((*dectDef->vLastKeyValue&FAST_ADJUST_FALG_MASK)==vKeyValue){
////								temp=*dectDef->vLastKeyTimer+sysTimes.vSystemLIRC5ms;
//								if(((*dectDef->vLastKeyTimer+sysTimes.vSystemLIRC5ms)&0xffff)<FSAT_TIME_GAP){
//									vKeyValue|=FAST_ADJUST_FLAG;
//								}
//							}
//						}
//						*dectDef->vLastKeyTimer= 0x10000 - sysTimes.vSystemLIRC5ms;
//						*dectDef->vLastKeyValue= vKeyValue;
//						vIncTestCnt++;
//					} else {
//						abandonAllDectInfo(dectDef,1);
//					}
//				} else {
//					abandonAllDectInfo(dectDef,1);
//				}
//			}
//		} else if ((dectDef->RightGPIOx->DI & dectDef->Right_GPIO_Pin) && (dectDef->LeftGPIOx->DI & dectDef->Left_GPIO_Pin)) {
//			if (*dectDef->fIsLeftRiseEdge && !(*dectDef->fIsRightFallEdge)) {			//???????????????,??????????????????,????????????
//				if (((*dectDef->vLeftHiTimeCnt + sysTimes.vTime1msCnt)&0xFFFF) > 1) {
//					abandonAllDectInfo(dectDef,1);
//				}
//			}
//			if (*dectDef->fIsRightRiseEdge && !(*dectDef->fIsLeftFallEdge)) {			//???????????????,??????????????????,????????????
//				if (((*dectDef->vRightHiTimeCnt + sysTimes.vTime1msCnt)&0xFFFF) > 1) {
//					abandonAllDectInfo(dectDef,1);
//				}
//			}
//		}
//	}
//}
///***********************************************************************************************************
//  *  @brief 		??????????????????????????????
//  *
//  *  @param [in] :
//  *
//  *  @param [out] :
//  *
//  *  @return :
//  *
//  *  @note :		???????????????,????????????????????????,????????????1ms
//  ************************************************************************************************************/
//void encoderCheck(void) {
//
//	encoderDect(&fucnEncoderDef);
//	encoderDect(&brightEncoderDef);
//
//}			//function end
///*************************************************************************************************************
//  *  @brief 				??????????????????????????????????????????
//  *
//  *  @param [in] :
//  *
//  *  @param [out] :
//  *
//  *  @return :
//  *
//  *  @note :
//  ************************************************************************************************************/
//uint8 getEncoderStatus(void) {
//
//	return (*brightEncoderDef.fIsCodingTurnStart || *fucnEncoderDef.fIsCodingTurnStart) ? TRUE : FALSE;
//
//}

