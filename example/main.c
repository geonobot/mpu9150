/*
 * main.c
 *
 *  Created on: 12 sept. 2013
 *      Author: ldo
 */

#include <util/delay.h>
#include "mpu9150.h"
#include "usart0.h"

void
pin_setup (void)
{

}

void
setup (void)
{
  usart_0_setup ();
  usart_0_transmitter_enable ();

  mpu9150_setup (0, DLPF_5_Hz);
  mpu9150_acc_setup (MR_2_g);
  mpu9150_gyro_setup (MR_250);
  mpu9150_compass_setup();
}

int
main (void)
{
  setup ();
  _delay_ms (1000);

  while (1)
    {
      usart_0_transmit (0xAA);
      usart_0_transmit (0xAA);

      uint8_t device_id = mpu9150_read_device_id ();
      usart_0_transmit (device_id);

      int16_t temperature = mpu9150_read_temperature ();
      usart_0_transmit (temperature >> 8);
      usart_0_transmit (temperature);

      xyz acc = mpu9150_read_acc_xyz ();
      double x = acc.x * 1000.0;
      double y = acc.y * 1000.0;
      double z = acc.z * 1000;
      usart_0_transmit ((int16_t) x >> 8);
      usart_0_transmit ((int16_t) x);
      usart_0_transmit ((int16_t) y >> 8);
      usart_0_transmit ((int16_t) y);
      usart_0_transmit ((int16_t) z >> 8);
      usart_0_transmit ((int16_t) z);

      xyz gyro = mpu9150_read_gyro_xyz ();
      usart_0_transmit ((int16_t) gyro.x >> 8);
      usart_0_transmit ((int16_t) gyro.x);
      usart_0_transmit ((int16_t) gyro.y >> 8);
      usart_0_transmit ((int16_t) gyro.y);
      usart_0_transmit ((int16_t) gyro.z >> 8);
      usart_0_transmit ((int16_t) gyro.z);

      uint8_t compass_device_id = ak8975c_read_device_id ();
      usart_0_transmit (compass_device_id);

      xyz compass = mpu9150_read_compass_xyz ();
      usart_0_transmit ((int16_t) compass.x >> 8);
      usart_0_transmit ((int16_t) compass.x);
      usart_0_transmit ((int16_t) compass.y >> 8);
      usart_0_transmit ((int16_t) compass.y);
      usart_0_transmit ((int16_t) compass.z >> 8);
      usart_0_transmit ((int16_t) compass.z);

      _delay_ms (100);
    }
}

