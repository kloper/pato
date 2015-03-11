/** -*- C -*-
 * \file
 *
 * \brief stuff to make UART code compatible across different AVRs
 *
 * Copyright (c) 2014-2015 Dimitry Kloper <kloper@users.sf.net>. 
 * All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * official policies, either expressed or implied, of the Pato Project.
 */

#ifndef _pato_uart_compat_h_
#define _pato_uart_compat_h_
#if defined(HAVE_UART)

#if !defined(UCSR0A) && defined(UCSRA)
#define UCSR0A UCSRA
#endif
#if !defined(UCSR0B) && defined(UCSRB)
#define UCSR0B UCSRB
#endif
#if !defined(UCSR0C) && defined(UCSRC)
#define UCSR0C UCSRC
#endif
#if !defined(U2X0) && defined(U2X)
#define U2X0 U2X
#endif
#if !defined(UBRR0H) && defined(UBRRH)
#define UBRR0H UBRRH
#endif
#if !defined(UBRR0L) && defined(UBRRL)
#define UBRR0L UBRRL
#endif
#if !defined(UPM00) && defined(UPM0)
#define UPM00 UPM0
#endif
#if !defined(UPM01) && defined(UPM1)
#define UPM01 UPM1
#endif
#if !defined(USBS0) && defined(USBS)
#define USBS0 USBS
#endif
#if !defined(UCSZ00) && defined(UCSZ0)
#define UCSZ00 UCSZ0
#endif
#if !defined(UCSZ01) && defined(UCSZ1)
#define UCSZ01 UCSZ1
#endif
#if !defined(RXEN0) && defined(RXEN)
#define RXEN0 RXEN
#endif
#if !defined(TXEN0) && defined(TXEN)
#define TXEN0 TXEN
#endif
#if !defined(RXCIE0) && defined(RXCIE)
#define RXCIE0 RXCIE
#endif
#if !defined(UDRE0) && defined(UDRE)
#define UDRE0 UDRE
#endif
#if !defined(UDR0) && defined(UDR)
#define UDR0 UDR
#endif
#if !defined(RXC0) && defined(RXC)
#define RXC0 RXC
#endif
#if !defined(FE0) && defined(FE)
#define FE0 FE
#endif
#if !defined(DOR0) && defined(DOR)
#define DOR0 DOR
#endif
#if !defined(UPE0) && defined(UPE)
#define UPE0 UPE
#endif

#if defined(__AVR_ATtiny2313__)
#define UART_DDR DDRD
#define UART_PORT PORTD
#else 
#define UART_DDR DDRE
#define UART_PORT PORTE
#endif

#endif /* HAVE_UART */
#endif /* _pato_uart_compat_h_ */
