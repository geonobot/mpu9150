/*
 * usart.h
 *
 *  Created on: 29 juil. 2013
 *      Author: ldo
 */

#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#if defined (__AVR_ATmega328P__)
#define DDR_USART0 DDRD
#define DDR_RX PD0
#endif
#if defined (__AVR_ATmega2560__)
#define DDR_USART0 DDRE
#define DDR_RX PE0
#endif

extern volatile uint8_t receive_char;

void
usart_0_setup (void);
/* ******************************************** RECEIVER */
void
usart_0_receiver_enable (void);
void
usart_0_receiver_interrupt_enable (void
(*user_function) (void));

uint8_t
usart_0_receive (void);

/* ******************************************** TRANSMITTER */
void
usart_0_transmitter_enable (void);

// transmit a char
void
usart_0_transmit (uint8_t data);

#endif /* USART_H_ */
