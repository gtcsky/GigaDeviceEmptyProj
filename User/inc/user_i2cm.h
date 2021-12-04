/*
 * user_i2cm.h
 *
 *  Created on: 2021Äê11ÔÂ8ÈÕ
 *      Author: Sky
 */

#ifndef PROJECTS_80_EMPTY_PROJ_USER_INC_USER_I2CM_H_
#define PROJECTS_80_EMPTY_PROJ_USER_INC_USER_I2CM_H_
#include		"public_define.h"

#define EEPROM_SIZE_2K		2
#define EEPROM_SIZE_4K		4
#define EEPROM_SIZE_8K		8
#define EEPROM_SIZE_16K	16
#define EEPROM_SIZE_32K	32
#define EEPROM_SIZE_64K	64
#define EEPROM_SIZE_128K	128
#define EEPROM_SIZE_256K	256
#define EEPROM_SIZE_512K	512

#define EEPROM_SIZE EEPROM_SIZE_2K

#if(EEPROM_SIZE == EEPROM_SIZE_2K)
	#define	EEPROM_PAGE_SIZE				8	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			32
#elif(EEPROM_SIZE == EEPROM_SIZE_4K)
	#define	EEPROM_PAGE_SIZE				16	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			32
#elif(EEPROM_SIZE == EEPROM_SIZE_8K)
	#define	EEPROM_PAGE_SIZE				16	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			64
#elif(EEPROM_SIZE == EEPROM_SIZE_16K)
	#define	EEPROM_PAGE_SIZE				16	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			128
#elif(EEPROM_SIZE == EEPROM_SIZE_32K)
	#define	EEPROM_PAGE_SIZE				32	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			128
#elif(EEPROM_SIZE == EEPROM_SIZE_64K)
	#define	EEPROM_PAGE_SIZE				32	//(Bytes)
	#define	EEPROM_TOTAL_PAGES			256
#endif






#define	SYSTEM_PARAM_START_ADDR	0x00

#define EEP_FIRST_PAGE         		0x00
#define I2C_OK                			0
#define I2C_FAIL               			1

#define I2C0_SPEED              		400000
#define I2C0_SLAVE_ADDRESS7     	0xA0
//#define I2C_PAGE_SIZE           		8

void	 eeprom_test(void);


/*!
 \brief      wait for EEPROM standby state
 \param[in]  none
 \param[out] none
 \retval     none
 */
void eeprom_wait_standby_state(void);


/*!
    \brief      write more than one byte to the EEPROM with a single write cycle
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
void eeprom_page_write(uint8_t* p_buffer, uint8_t write_address, uint8_t number_of_byte);

/*!
    \brief      write buffer of data to the I2C EEPROM
    \param[in]  p_buffer: pointer to the buffer  containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
void writeEepromData(uint8_t* p_buffer, uint8_t write_address, uint16_t number_of_byte);


/*!
    \brief      read data from the EEPROM
    \param[in]  p_buffer: pointer to the buffer that receives the data read from the EEPROM
    \param[in]  read_address: EEPROM's internal address to start reading from
    \param[in]  number_of_byte: number of bytes to reads from the EEPROM
    \param[out] none
    \retval     none
*/
void readEepromData(uint8_t* p_buffer, uint8_t read_address, uint16_t number_of_byte);

void i2c_init(void);
#endif /* PROJECTS_80_EMPTY_PROJ_USER_INC_USER_I2CM_H_ */
