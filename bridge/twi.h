/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * twi.h - TWI side of the TWI-I2C bridge
 *
 */

#ifndef _pato_bridge_twi_h_
#define _pato_bridge_twi_h_

typedef struct _twi_packet_header {
   uint8_t rw:1;
   uint8_t addr:7;  
} twi_packet_header_t;

typedef struct _twi_config {
   uint16_t twps:2;
   uint16_t twbr:8;
} twi_config_t;

typedef struct _twi_wait_result {
   uint8_t status;
   uint8_t remaining;
} twi_wait_result_t;

extern void twi_init(void);
extern void twi_config(twi_config_t config);
extern void twi_master_send(twi_packet_header_t *data, uint8_t size,
			    uint8_t stop);
extern void twi_master_recv(twi_packet_header_t *data, uint8_t size,
			    uint8_t stop, uint8_t last_nack);
extern twi_wait_result_t twi_master_wait(void);

#endif /* _pato_bridge_twi_h_ */
