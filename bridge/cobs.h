/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * cobs.h - Consistent Overhead Byte Stuffing primitives
 *
 * See http://stuartcheshire.org/papers/COBSforToN.pdf
 *
 */

#ifndef _pato_bridge_cobs_h_
#define _pato_bridge_cobs_h_

extern uint8_t *cobs_encode(uint8_t *buffer, uint8_t size);
extern uint8_t *cobs_decode(uint8_t *buffer, uint16_t size);

#endif /* _pato_bridge_cobs_h_ */
