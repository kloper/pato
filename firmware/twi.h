/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * twi.h - TWI slave driver
 *
 */

#ifndef _pato_twi_h_
#define _pato_twi_h_
#if defined(HAVE_TWI)

extern void twi_init(void);
extern void twi_slave_send(void);
extern packet_t *twi_slave_recv(void);
extern void *twi_outbuf(void);

#endif /* HAVE_TWI */
#endif /* _pato_twi_h_ */
