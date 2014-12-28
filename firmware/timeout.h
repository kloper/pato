/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * timeout.h - use 8-bit counter #0 to track time
 *
 */

#ifndef _pato_timeout_h_
#define _pato_timeout_h_

extern void to_start(uint8_t prescale);
extern uint8_t to_check(void);

#endif /* _pato_timeout_h_ */
