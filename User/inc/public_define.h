/*
 * public_define.h
 *
 *  Created on: 2021年11月4日
 *      Author: Sky
 */
#include  "stdint.h"
#include "gd32e23x.h"
#include "gd32e230c_eval.h"
#ifndef PROJECTS_80_EMPTY_PROJ_USER_INC_PUBLIC_DEFINE_H_
#define PROJECTS_80_EMPTY_PROJ_USER_INC_PUBLIC_DEFINE_H_

    /* exact-width signed integer types */
typedef   signed          char int8;
typedef   signed short     int int16;
typedef   signed           int int32;

    /* exact-width unsigned integer types */
typedef unsigned          char uint8;
typedef unsigned          char UINT8;
typedef unsigned short     int uint16;
typedef unsigned short     int UINT16;
typedef unsigned           int uint32;
typedef unsigned           int UINT32;


#define	EEPROM_INSIDE							1
#define	RGB_INSIDE								1
#define	LIGHTEFFECT_INSIDE						1


#define	POWER_DOWN_MODE_TIMEOUT			3				//3*50ms=150ms
#define	NORMAL_MODE_TIMEOUT				1000
#define	USER_DOG_CONST						1600				//1600*5ms=8s
#define	PWM_FRQ_CONST						2400				//72Mhz/2400=30Khz

#define	ADC_CONST								(4096)
#define	SYSTEM_VDD							3.3

#define	DEFAULT_TIME0_CH0_PWM				0
#define	DEFAULT_TIME0_CH3_PWM				0

typedef struct{
	volatile	uint16 	vSystemLIRC5ms;
	volatile	uint16 	vSystem5ms;
	volatile	uint16	vSystem1s;
	volatile	uint16	vTime1msCnt;
	volatile	uint16	vTimerCnt;
	volatile	uint16	vTimeToSleep;
	volatile	uint8	fTime50ms;
	volatile	uint8	fTime100ms;
	volatile	uint8	fTime500ms;
	volatile	uint8	fTime1s;
	volatile	uint8	fTime1ms;
}userSysTimesDef;

typedef enum{
	IdleIamgeDisplay=0,
	ModeTDisplay=1,
	CountDownDisplay=2,
	ChargingAtPowerDown=3,
	FactoryMode=4
}displayModeEnum;

typedef enum{
	HuesSetting=0,
	SaturationSetting=1,
	BrightnessSetting=2,
	ColorTempSetting=3,
	PreinstallEffect=4,
	Style1Setting=4,
	Style2Setting=5,
	Style3Setting=6
}settingIndexEnum;

typedef enum {
	FLASH_BUSY = 1,
	FLASH_COMPLETE
} FLASH_Status;

typedef struct{
	__IO uint32_t CTRL; 					//offset 0x00	GPIO port control register
	__IO uint32_t OMODE;					//offset 0x04	GPIO port output mode register
	__IO uint32_t OSPD;  					//offset 0x08	 GPIO port output speed register
	__IO uint32_t PUD;  					//offset 0x0C	GPIO port pull-up/pull-down register
	__IO uint32_t ISTAT; 					//offset 0x10	GPIO port input status register
	__IO uint32_t OCTL; 					//offset 0x14	GPIO port output control register״̬
	__IO uint32_t BOP;						//offset 0x18	GPIO port bit operation register
	__IO uint32_t LOCK;					//offset 0x1C	GPIO port configuration lock register
	__IO uint32_t AFSEL0;					//offset 0x20	GPIO alternate function selected register 0
	__IO uint32_t AFSEL1;					//offset 0x24  GPIO alternate function selected register 1
	__IO uint32_t BC;						//offset 0x28 	GPIO bit clear registe
	__IO uint32_t TG;						//offset 0x2c 	GPIO port bit toggle register
}GPIO_TypeDef;

#define	pGPIOA				 ((GPIO_TypeDef *) GPIOA)
#define	pGPIOB				 ((GPIO_TypeDef *) GPIOB)
#define	pGPIOC				 ((GPIO_TypeDef *) GPIOC)
#define	pGPIOF				 ((GPIO_TypeDef *) GPIOF)

typedef struct {
	__IO	uint32_t CTL0;			// 0x00U) /*!< TIMER control register 0 */
	__IO uint32_t CTL1;			//+ 0x04U) /*!< TIMER control register 1 */
	__IO uint32_t SMCFG;			//+ 0x08U) /*!< TIMER slave mode configuration register */
	__IO uint32_t DMAINTEN;		//+ 0x0CU) /*!< TIMER DMA and interrupt enable register */
	__IO uint32_t INTF;			// + 0x10U) /*!< TIMER interrupt flag register */
	__IO uint32_t SWEVG;			//+ 0x14U) /*!< TIMER software event generation register */
	__IO uint32_t CHCTL0;			// + 0x18U) /*!< TIMER channel control register 0 */
	__IO uint32_t CHCTL1;			// + 0x1CU) /*!< TIMER channel control register 1 */
	__IO uint32_t CHCTL2;			// + 0x20U) /*!< TIMER channel control register 2 */
	__IO uint32_t CNT;				// + 0x24U) /*!< TIMER counter register */
	__IO uint32_t PSC;				// + 0x28U) /*!< TIMER prescaler register */
	__IO uint32_t CAR;				// + 0x2CU) /*!< TIMER counter auto reload register */
	__IO uint32_t CREP;			// + 0x30U) /*!< TIMER counter repetition register */
	__IO uint32_t CH0CV;			// + 0x34U) /*!< TIMER channel 0 capture/compare value register */
	__IO uint32_t CH1CV;			// + 0x38U) /*!< TIMER channel 1 capture/compare value register */
	__IO uint32_t CH2CV;			// + 0x3CU) /*!< TIMER channel 2 capture/compare value register */
	__IO uint32_t CH3CV;			// + 0x40U) /*!< TIMER channel 3 capture/compare value register */
	__IO uint32_t CCHP;			// + 0x44U) /*!< TIMER complementary channel protection register */
	__IO uint32_t DMACFG;		// + 0x48U) /*!< TIMER DMA configuration register */
	__IO uint32_t DMATB;			// + 0x4CU) /*!< TIMER DMA transfer buffer register */
	__IO uint32_t IRMP;			// + 0x50U) /*!< TIMER channel input remap register */
	__IO uint32_t CFG;				// + 0xFCU)
	/*!< TIMER configuration register */
} TIME_TypeDef;

#define	pTIMER0	((TIME_TypeDef *)TIMER0)
#define	pTIMER2	((TIME_TypeDef *)TIMER2)
#define	pTIMER5	((TIME_TypeDef *)TIMER5)
#define	pTIMER13	((TIME_TypeDef *)TIMER13)
#define	pTIMER14	((TIME_TypeDef *)TIMER14)
#define	pTIMER15	((TIME_TypeDef *)TIMER15)
#define	pTIMER16	((TIME_TypeDef *)TIMER16)




#define	MAX_ColorTemp							75//56
#define	MIN_ColorTemp							32
#define	LED_REAL_START_DUTY					4		// LED 实际亮度起步4%

#define	MAX_HUES								360
#define	MAX_SATURATION						100
#define	MAX_BRIGHTNESS						100
#define	MIN_HUES								0
#define	MIN_SATURATION						1
#define	MIN_BRIGHTNESS						1
#define	DEFAULT_HUES							0
#define	DEFAULT_BRIGHTNESS					100//5
#define	DEFAULT_SATURATION					100
#if(RGB_INSIDE==1)
	#define	DEFAULT_ARROR_INDEX				0	//3
#else
	#define	DEFAULT_ARROR_INDEX				3	//3
#endif
#define	DEFAULT_COLOR_TEMP					44//44
#define	DEFAULT_STYLE1_VALUE					0
#define	DEFAULT_STYLE2_VALUE					0
#define	DEFAULT_STYLE3_VALUE					0


typedef uint8 (* daCallbacksType)(uint8 chl);
typedef struct{
	uint32	vSingleChlAdcConverting;
	uint32	vChlResultGot;
	uint32	vConvertingIndex;
	uint16 	aDaValues[18];
	uint32	vPospendConvertInfo;
	daCallbacksType callbacks[18];
}adcConvertStruct;


#pragma pack(1)
typedef struct {
	UINT16 checkCode;				//
	UINT8 DisplayModeIndex;			//	参考值: displayModeEnum
	UINT8 brightness;					//	亮度值	(0~100)%
	UINT8 colorTemperature;			//	33~65(00K)
	UINT8 arrowIndex;					//	光标序号	(0~6)		参考值:settingIndexEnum
	UINT8 battIndex;					//	电量标志	(0~6)
#if(LIGHTEFFECT_INSIDE==1)
	UINT8 style1Value;				//	0~2对应闪光灯标志A,B,C 三种模式
	UINT8 style2Value;				//	0~2对应循环标志A,B,C 三种模式
	UINT8 style3Value;				//	0~2对应锁标志A,B,C 三种模式
	UINT8 fIsEffectMode;				//	自定义特效模式+预设特效模式=1.普通模式=0
	UINT8 preinstallEffectNo;			//	预设特效编号
#endif
#if(RGB_INSIDE==1)
	UINT8 saturation;					//	饱和值	(0~100)
	UINT16 hues;						//	色调值	(0~360)
#endif
} displayParamsStruct;
#pragma pack()


#endif /* PROJECTS_80_EMPTY_PROJ_USER_INC_PUBLIC_DEFINE_H_ */
