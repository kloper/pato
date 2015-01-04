/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * print.h - printf functionality for Pato
 *
 */

#ifndef _pato_print_h_
#define _pato_print_h_

#define PATO_PRINT_BUFFER_SIZE 80

typedef struct _print_buffer {
   uint16_t addr;
   uint8_t  buffer[PATO_PRINT_BUFFER_SIZE];
} print_buffer_t;

extern print_buffer_t g_print_buffer;

extern uint8_t hd44780_print_commit(void);
extern uint8_t hd44780_print_set_addr(uint16_t addr);
extern uint16_t hd44780_print_get_addr();
extern uint8_t hd44780_print_put(uint8_t arg0, uint8_t arg1);




#endif /* _pato_print_h_ */
