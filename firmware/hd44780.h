/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * hd44780.h - interface to HD44780 controller
 *
 */

#ifndef _pato_hd44780_h_
#define _pato_hd44780_h_

#define HD44780_CTRL_PORT PORTA
#define HD44780_CTRL_DDR (_SFR_IO_ADDR(HD44780_CTRL_PORT) - 1)
#define HD44780_CTRL_OUT (_SFR_IO_ADDR(HD44780_CTRL_PORT))
#define HD44780_CTRL_IN  (_SFR_IO_ADDR(HD44780_CTRL_PORT) - 2)

#define HD44780_RS PA0
#define HD44780_RW PA1
#define HD44780_EN PA2
#define HD44780_RST PA3

#define HD44780_DATA_PORT PORTC
#define HD44780_DATA_DDR (_SFR_IO_ADDR(HD44780_DATA_PORT) - 1)
#define HD44780_DATA_OUT (_SFR_IO_ADDR(HD44780_DATA_PORT))
#define HD44780_DATA_IN  (_SFR_IO_ADDR(HD44780_DATA_PORT) - 2)

#define HD44780_DB4 PC4
#define HD44780_DB5 PC5
#define HD44780_DB6 PC6
#define HD44780_DB7 PC7
#define HD44780_BUSY PC7

#ifndef __ASSEMBLY__
extern void hd44780_reset(uint8_t cmd);
extern void hd44780_ir_write(uint8_t cmd);
extern uint8_t hd44780_wait_busy(void);
extern void hd44780_dr_write(uint8_t value);
extern uint8_t hd44780_dr_read(void);
#endif /* __ASM__ */

#endif /* _pato_hd44780_h_ */
