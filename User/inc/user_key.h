/*
 * user_key.h
 *
 *  Created on: 2019骞�8鏈�15鏃�
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_KEY_H_
#define USER_INC_USER_KEY_H_
#include	"public_define.h"

#define 	FUNC_KEY_SHORT_PRESSED    					0x51
#define 	FUNC_KEY_LONG_PRESSED    					0x52
#define 	FUNC_KEY_DOUBLE_CLICK    					0x53

#define 	POWER_KEY_SHORT_PRESSED   				0x01
#define 	POWER_KEY_LONG_PRESSED    				0x02
#define 	REMOTE_POWER_LONG_PRESSED    			0x03
#define  POWER_KEY_PRESSED   						0x04
#define	FACTORY_MODE_KEY							0X05

#define 	INC_KEY_SHORT_PRESSED    					0x11
#define 	INC_KEY_LONG_PRESSED    					0x12
#define  INC_KEY_PRESSED   							0x08

#define 	DEC_KEY_SHORT_PRESSED    					0x21
#define 	DEC_KEY_LONG_PRESSED    					0x22
#define 	DEC_KEY_PRESSED   							0x10

#define 	BRIGHTNESS_UP_KEY_SHORT_PRESSED    		0x31
#define 	BRIGHTNESS_UP_KEY_LONG_PRESSED    		0x32
#define	FAST_ADJUST_FLAG							0x80
#define	FAST_ADJUST_FALG_MASK					0X7F
#define	FSAT_TIME_GAP								(10)		//6*5=30MS
#define	FAST_ADJUST_STEP							(5)

#define 	BRIGHTNESS_DOWN_KEY_SHORT_PRESSED    	0x41
#define 	BRIGHTNESS_DOWN_KEY_LONG_PRESSED    	0x42

#define 	SWITCH_KEY_SHORT_PRESSED					0x51
#define 	SWITCH_KEY_LONG_PRESSED					0x52
#define 	SWITCH_KEY_PRESSED   						0x02

#define 	CLOSE_KEY_SHORT_PRESSED   	 				0x61
#define 	CLOSE_KEY_LONG_PRESSED    	 				0x62

#define 	SHORT_PRESSED_TIME   						2  		//4*5ms=20ms
#define 	LONG_PRESSED_0S2     						40		//40*5=200ms
#define 	LONG_PRESSED_0S3     						60		//60*5=300ms
#define 	LONG_PRESSED_0S5     						100		//100*5=500ms
#define 	LONG_PRESSED_1S							200		//200*5ms=1s
#define 	LONG_PRESSED_2S							400		//400*5ms=2s
#define 	LONG_PRESSED_3S							600		//600*5ms=3s
#define 	LONG_PRESSED_4S							800		//800*5ms=4s
#define 	LONG_PRESSED_5S							1000	//1000*5ms=5s

#define 	DOUBLE_CLICK_CONST						100;   		//100*5ms  鍦ㄧ涓�娆℃寜閿噴鏀惧��,鍊掕鏃�500ms浠ュ唴,濡傛灉鍙崇浜屾鎸夐敭浜х敓,灏卞綋鎴愭槸鍙屽嚮
extern	volatile	uint8 	vKeyValue;
extern	volatile	uint8 	vIsKeyPressed;
extern	uint8 	vIsFuncKeyGod;
extern	uint8	vDoubleClickCountDown;
extern	uint16	vFuncKeyTimer;

extern	uint8 	vIsPowerKeyGod;
extern	uint16	vPowerKeyTimer;

extern	uint8 	vIsIncKeyGod;
extern	uint16	vIncKeyTimer;

extern	uint16	vDecKeyTimer;
extern	uint8 	vIsDecKeyGod;

extern	uint16	vBrightnessUpKeyTimer;
extern	uint8 	vIsBrightnessUpKeyGod;

extern	uint16	vBrightnessDownKeyTimer;
extern	uint8 	vIsBrightnessDownKeyGod;

extern	uint8	fIsEncoderPending;

#define	FUNC_KEY_SHOT_PRRESS_GOD	0X01
#define	FUNC_KEY_LONG_PRRESS_GOD	0X02
#define	POWER_KEY_SHOT_PRRESS_GOD	0X01
#define	POWER_KEY_LONG_PRRESS_GOD	0X02

#define	INC_KEY_SHOT_PRRESS_GOD	0X01
#define	INC_KEY_LONG_PRRESS_GOD	0X02

#define	DEC_KEY_SHOT_PRRESS_GOD	0X01
#define	DEC_KEY_LONG_PRRESS_GOD	0X02

#define	BRIGHTNESS_UP_KEY_SHOT_PRRESS_GOD	0X01
#define	BRIGHTNESS_UP_KEY_LONG_PRRESS_GOD	0X02

#define	BRIGHTNESS_DOWN_KEY_SHOT_PRRESS_GOD	0X01
#define	BRIGHTNESS_DOWN_KEY_LONG_PRRESS_GOD	0X02

#define	CLOSED_KEY_SHOT_PRRESS_GOD	0X01
#define	CLOSED_KEY_LONG_PRRESS_GOD	0X02

extern	uint8 getEncoderStatus(void);
extern	void funcKeyTask(void);
extern	void keyDoubleClickChk(void);
extern	void setClosedKeyPressedFlag(void);
extern	void closedReleasedFunc(void);
extern	void setIncKeyPressedFlag(void);
extern	void setDecKeyPressedFlag(void);
extern	void holdingKeyFunc(void);
extern	void setBrightnessUpKeyPressedFlag(void);
extern	void setBrightnessDownKeyPressedFlag(void);
extern	void	 clearPowerKeyInfo(void);
extern	void encoderCheck(void);

/***********************************************************************************************************
 *  @brief		set Power click registers
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :		invoked when IRQ  occurred
 ************************************************************************************************************/
void setPowerKeyPressedFlag(void);

/***********************************************************************************************************
 *  @brief		set Switch click registers
 *
 *  @param [in] :
 *
 *  @param [out] :
 *
 *  @return :
 *
 *  @note :		invoked when IRQ  occurred
 ************************************************************************************************************/
void setSwitchKeyPressedFlag(void);
#endif /* USER_INC_USER_KEY_H_ */
