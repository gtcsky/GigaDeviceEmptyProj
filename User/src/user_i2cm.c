/*
 * user_i2cm.c
 *
 *  Created on: 2021Äê11ÔÂ8ÈÕ
 *      Author: Sky
 */
#include		"user_i2cm.h"
#include 	<stdio.h>
#include 	<string.h>

#define 	EEPROM_BLOCK0_ADDRESS    		0xA0
#define 	BUFFER_SIZE              				256


//uint8	i2c_buffer_write[BUFFER_SIZE]={0};
//
uint8	i2c_buffer_read[BUFFER_SIZE]={0};

uint16 	eeprom_address;

/*!
 \brief      configure the GPIO ports
 \param[in]  none
 \param[out] none
 \retval     none
 */

void gpio_config(void) {
	/* enable GPIOB clock */
	rcu_periph_clock_enable(RCU_GPIOB);
	/* enable I2C0 clock */
	rcu_periph_clock_enable(RCU_I2C0);

	/* connect PB6 to I2C0_SCL */
	gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_6);
	/* connect PB7 to I2C0_SDA */
	gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_7);
	/* configure GPIO pins of I2C0 */
	gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
}

/*!
 \brief      configure the I2C0 interfaces
 \param[in]  none
 \param[out] none
 \retval     none
 */
void i2c_config(void) {
	/* enable I2C clock */
	rcu_periph_clock_enable(RCU_I2C0);
	/* configure I2C clock */
	i2c_clock_config(I2C0, I2C0_SPEED, I2C_DTCY_2);
	/* configure I2C address */
	i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_SLAVE_ADDRESS7);
	/* enable I2C0 */
	i2c_enable(I2C0);
	/* enable acknowledge */
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

/*!
 \brief      initialize peripherals used by the I2C EEPROM driver
 \param[in]  none
 \param[out] none
 \retval     none
 */
void i2c_eeprom_init(void) {

	eeprom_address = EEPROM_BLOCK0_ADDRESS;

}



/*!
    \brief      write buffer of data to the I2C EEPROM
    \param[in]  p_buffer: pointer to the buffer  containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
void writeEepromData(uint8_t* p_buffer, uint8_t write_address, uint16_t number_of_byte) {

	uint8_t number_of_page = 0, number_of_single = 0, address = 0, count = 0;

	address = write_address % EEPROM_PAGE_SIZE;
	count = EEPROM_PAGE_SIZE - address;
	number_of_page = number_of_byte / EEPROM_PAGE_SIZE;
	number_of_single = number_of_byte % EEPROM_PAGE_SIZE;

	/* if write_address is EEPROM_PAGE_SIZE aligned  */
	if (0 == address) {
		while (number_of_page--) {
			eeprom_page_write(p_buffer, write_address, EEPROM_PAGE_SIZE);
			eeprom_wait_standby_state();
			write_address += EEPROM_PAGE_SIZE;
			p_buffer += EEPROM_PAGE_SIZE;
		}
		if (0 != number_of_single) {
			eeprom_page_write(p_buffer, write_address, number_of_single);
			eeprom_wait_standby_state();
		}
	} else {
		/* if write_address is not EEPROM_PAGE_SIZE aligned */
		if (number_of_byte < count) {
			eeprom_page_write(p_buffer, write_address, number_of_byte);
			eeprom_wait_standby_state();
		} else {
			number_of_byte -= count;
			number_of_page = number_of_byte / EEPROM_PAGE_SIZE;
			number_of_single = number_of_byte % EEPROM_PAGE_SIZE;

			if (0 != count) {
				eeprom_page_write(p_buffer, write_address, count);
				eeprom_wait_standby_state();
				write_address += count;
				p_buffer += count;
			}
			/* write page */
			while (number_of_page--) {
				eeprom_page_write(p_buffer, write_address, EEPROM_PAGE_SIZE);
				eeprom_wait_standby_state();
				write_address += EEPROM_PAGE_SIZE;
				p_buffer += EEPROM_PAGE_SIZE;
			}
			/* write single */
			if (0 != number_of_single) {
				eeprom_page_write(p_buffer, write_address, number_of_single);
				eeprom_wait_standby_state();
			}
		}
	}
}

/*!
    \brief      write one byte to the I2C EEPROM
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[out] none
    \retval     none
*/
void eeprom_byte_write(uint8_t* p_buffer, uint8_t write_address){
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, eeprom_address, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);

    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get(I2C0, I2C_FLAG_TBE));

    /* send the EEPROM's internal address to write to : only one byte address */
    i2c_data_transmit(I2C0, write_address);

    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

    /* send the byte to be written */
    i2c_data_transmit(I2C0, *p_buffer);

    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);

    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0)&0x0200);
}

/*!
    \brief      write more than one byte to the EEPROM with a single write cycle
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
void eeprom_page_write(uint8_t* p_buffer, uint8_t write_address, uint8_t number_of_byte) {
	/* wait until I2C bus is idle */
	while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY))
		;

	/* send a start condition to I2C bus */
	i2c_start_on_bus(I2C0);

	/* wait until SBSEND bit is set */
	while (!i2c_flag_get(I2C0, I2C_FLAG_SBSEND))
		;

	/* send slave address to I2C bus */
	i2c_master_addressing(I2C0, eeprom_address, I2C_TRANSMITTER);

	/* wait until ADDSEND bit is set */
	while (!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND))
		;

	/* clear the ADDSEND bit */
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

	/* wait until the transmit data buffer is empty */
	while (SET != i2c_flag_get(I2C0, I2C_FLAG_TBE))
		;

	/* send the EEPROM's internal address to write to : only one byte address */
	i2c_data_transmit(I2C0, write_address);

	/* wait until BTC bit is set */
	while (!i2c_flag_get(I2C0, I2C_FLAG_BTC))
		;

	/* while there is data to be written */
	while (number_of_byte--) {
		i2c_data_transmit(I2C0, *p_buffer);

		/* point to the next byte to be written */
		p_buffer++;

		/* wait until BTC bit is set */
		while (!i2c_flag_get(I2C0, I2C_FLAG_BTC))
			;
	}
	/* send a stop condition to I2C bus */
	i2c_stop_on_bus(I2C0);

	/* wait until the stop condition is finished */
	while (I2C_CTL0(I2C0) & 0x0200)
		;
}

/*!
    \brief      read data from the EEPROM
    \param[in]  p_buffer: pointer to the buffer that receives the data read from the EEPROM
    \param[in]  read_address: EEPROM's internal address to start reading from
    \param[in]  number_of_byte: number of bytes to reads from the EEPROM
    \param[out] none
    \retval     none
*/
void readEepromData(uint8_t* p_buffer, uint8_t read_address, uint16_t number_of_byte){
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

    if(2 == number_of_byte){
        i2c_ackpos_config(I2C0,I2C_ACKPOS_NEXT);
    }

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, eeprom_address, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);

    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get( I2C0 , I2C_FLAG_TBE));

    /* enable I2C0*/
    i2c_enable(I2C0);

    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C0, read_address);

    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, eeprom_address, I2C_RECEIVER);

    if(number_of_byte < 3){
        /* disable acknowledge */
        i2c_ack_config(I2C0,I2C_ACK_DISABLE);
    }

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);

    if(1 == number_of_byte){
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C0);
    }

    /* while there is data to be read */
    while(number_of_byte){
        if(3 == number_of_byte){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

            /* disable acknowledge */
            i2c_ack_config(I2C0,I2C_ACK_DISABLE);
        }
        if(2 == number_of_byte){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C0);
        }

        /* wait until the RBNE bit is set and clear it */
        if(i2c_flag_get(I2C0, I2C_FLAG_RBNE)){
            /* read a byte from the EEPROM */
            *p_buffer = i2c_data_receive(I2C0);

            /* point to the next location where the byte read will be saved */
            p_buffer++;

            /* decrement the read bytes counter */
            number_of_byte--;
        }
    }

    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0)&0x0200);

    /* enable acknowledge */
    i2c_ack_config(I2C0,I2C_ACK_ENABLE);

    i2c_ackpos_config(I2C0,I2C_ACKPOS_CURRENT);
}

/*!
 \brief      wait for EEPROM standby state
 \param[in]  none
 \param[out] none
 \retval     none
 */
void eeprom_wait_standby_state(void) {
	__IO uint32_t
	val = 0;

	while (1) {
		/* wait until I2C bus is idle */
		while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

		/* send a start condition to I2C bus */
		i2c_start_on_bus(I2C0);

		/* wait until SBSEND bit is set */
		while (!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

		/* send slave address to I2C bus */
		i2c_master_addressing(I2C0, eeprom_address, I2C_TRANSMITTER);

		/* keep looping till the Address is acknowledged or the AE flag is set (address not acknowledged at time) */
		do {
			/* get the current value of the I2C_STAT0 register */
			val = I2C_STAT0(I2C0);

		} while (0 == (val & (I2C_STAT0_ADDSEND | I2C_STAT0_AERR)));

		/* check if the ADDSEND flag has been set */
		if (val & I2C_STAT0_ADDSEND) {

			/* clear ADDSEND flag */
			i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

			/* send a stop condition to I2C bus */
			i2c_stop_on_bus(I2C0);

			/* exit the function */
			return;

		} else {
			/* clear the bit of AE */
			i2c_flag_clear(I2C0, I2C_FLAG_AERR);
		}

		/* send a stop condition to I2C bus */
		i2c_stop_on_bus(I2C0);
		/* wait until the stop condition is finished */
		while (I2C_CTL0(I2C0) & 0x0200);
	}
}



void eeprom_test(void) {
		/* EEPROM data read */
		readEepromData(i2c_buffer_read, EEP_FIRST_PAGE, BUFFER_SIZE);
}
//void eeprom_test(void) {
//	uint8 * p = i2c_buffer_write;
//	/* configure GPIO */
//	gpio_config();
//
//	/* configure I2C */
//	i2c_config();
//
//	/* initialize EEPROM */
//	i2c_eeprom_init();
//
//	for (uint16 i = 0; i < BUFFER_SIZE; i++) {
//
//		*(p++) = i;
//
//	}
//
//	/* EEPROM data write */
//	writeEepromData(i2c_buffer_write, EEP_FIRST_PAGE, BUFFER_SIZE);
//
//	/* EEPROM data read */
//	readEepromData(i2c_buffer_read, EEP_FIRST_PAGE, BUFFER_SIZE);
//
//}

void i2c_init(void) {
	/* configure GPIO */
	gpio_config();

	/* configure I2C */
	i2c_config();

	/* initialize EEPROM */
	i2c_eeprom_init();

}
