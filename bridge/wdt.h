/* -*- asm -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * wdt.S - control WatchDog Timer for timeout purposes
 *
 */

#ifndef _pato_bridge_wdt_h_
#define _pato_bridge_wdt_h_


extern void wdt_enable(uint8_t prescale);
extern void wdt_disable();
extern void wdt_reset();

#endif /* _pato_bridge_wdt_h_ */
