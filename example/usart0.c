/*
 * usart.c
 *
 *  Created on: 29 juil. 2013
 *      Author: ldo
 */

#include "usart0.h"

void
(*usart0_rx_vect) (void);

volatile uint8_t receive_char;

/* setup USART0
 * speed : 9600 Baud
 * 8-bits, no parity, 1-stop-bit */
void
usart_0_setup (void)
{
#if F_CPU == 16000000UL
  UBRR0 = 103; /* 9600 Baud at 16MHz */
#endif
#if F_CPU == 20000000UL
  UBRR1 = 129 /*9600 Baud at 20 MHz */
#endif
  UCSR0A = 0;
  UCSR0B = 0;

  /* 8-bits, no parity, 1-stop-bit */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/* ******************************************** RECEIVER */
void
usart_0_receiver_enable (void)
{
  DDR_USART0 &= ~(1 << DDR_RX); /* initialize pin RX as input pin */

  UCSR0B |= (1 << RXEN0);
}

void
usart_0_receiver_interrupt_enable (void
(*user_function) (void))
{
  DDR_USART0 &= ~(1 << DDR_RX); /* initialize pin RX as input pin */

  usart0_rx_vect = user_function;
  UCSR0B |= (1 << RXEN0) | (1 << RXCIE0); /* enable interrupt on the RXC0 flag */
  SREG |= (1 << SREG_I);
}

// receiver interrupt
#if defined (__AVR_ATmega2560__)
ISR( USART0_RX_vect)
#endif
#if defined (__AVR_ATmega328P__)
ISR( USART_RX_vect)
#endif
{
  /*uint8_t dustbin = 0;
   // If frame error or parity error or data overRun
   if ((UCSR1A & (1 << FE1)) || (UCSR1A & (1 << UPE1)) || (UCSR1A & (1 << DOR1)))
   {
   dustbin = UDR1;
   }
   // no error
   else*/
    {
      receive_char = UDR1;
    }

  usart0_rx_vect ();
}

uint8_t
usart_0_receive (void)
{
  // wait for data to be received
  while (!(UCSR0A & (1 << RXC0)))
    ;
  // get and return received data from buffer
  return UDR0;
}

/* ******************************************** TRANSMITTER */
void
usart_0_transmitter_enable (void)
{
  /* ATMEGA328P et ATMEGA2560 */
  DDRD |= (1 << PD1); /* initialize pin PD1 (TX) as output pin */

  UCSR0B |= (1 << TXEN0);
}

// transmit a char
void
usart_0_transmit (uint8_t data)
{
  // wait for empty transmit buffer
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  // put data info buffer, sends the data
  UDR0 = data;
}
