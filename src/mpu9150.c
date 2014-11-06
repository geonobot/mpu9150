/*
 * mpu9150.c
 *
 *  Created on: 12 sept. 2013
 *      Author: ldo
 *
 * The AK8975 3-Axis Compass is connected to the MPU-9150 through the MPU’s Auxiliary I C Bus.
 * In order to access this compass directly, the MPU-9150 should be put into Pass-Through Mode.
 */

#include "mpu9150.h"

static double acc_sensitivity = ACC_SENSITIVITY / 8.0; /* default value 2048 LSB/g for full-range 16g */
static double gyro_sensitivity = GYRO_SENSITIVITY / 8.0; /* default value 16.4 LSB per °/s for full range 200°/s */

static uint8_t slave_address = MPU9150_ADDRESS;

/**
 * \fn
 * \brief
 * \param ado (0 or 1) select MPU9150 slave address
 * */
void
mpu9150_setup (uint8_t ado, mpu9150_bandwidth_dlpf dlpf)
{
  volatile uint8_t buffer[2];

  twi_master_setup (); /* i2c to master mode */

  slave_address = MPU9150_ADDRESS + ado;

  /* reset entire device */
  buffer[0] = PWR_MGMT_1;
  buffer[1] = 0x80;
  twi_write_bytes (slave_address, 2, buffer);
  _delay_ms (100);

  /* out of sleep mode to start the sensor and set clock gyro */
  buffer[0] = PWR_MGMT_1;
  buffer[1] = 0x01;
  twi_write_bytes (slave_address, 2, buffer);

  /* configure digital low-pass filter (dlpf) bandwidth */
  buffer[0] = CONFIG;
  buffer[1] = dlpf;
  twi_write_bytes (slave_address, 2, buffer);

  /* enable FIFO buffer */
  /*buffer[0] = USER_CTRL;
   buffer[1] = 0x40;
   twi_write_bytes (slave_address, 2, buffer);

   _delay_ms (1);

   buffer[0] = FIFO_EN;
   buffer[1] = 0x08;
   twi_write_bytes (slave_address, 2, buffer);*/

}

/**
 * \fn void mpu9150_acc_setup (mpu9150_acc_measurement_range mr)
 * \brief setup accelerometer
 * \param mr measurement range (2, 4, 8 or 16g)
 * */
void
mpu9150_acc_setup (mpu9150_acc_measurement_range mr)
{
  volatile uint8_t buffer[2];

  /* Accelerometer configuration : full-scale range */
  buffer[0] = ACCEL_CONFIG;
  buffer[1] = (mr << 3);
  twi_write_bytes (slave_address, 2, buffer);

  acc_sensitivity = ACC_SENSITIVITY / (1 << mr); /* LSB/g */
}

/**
 * \fn void mpu9150_gyro_setup (mpu9150_gyro_measurement_range mr)
 * \brief setup gyroscope
 * \param mr measurement range (250, 500, 1000 or 2000°/s)
 * */
void
mpu9150_gyro_setup (mpu9150_gyro_measurement_range mr)
{
  volatile uint8_t buffer[2];

  /* Gyroscope configuration - full-scale range °/sec */
  buffer[0] = GYRO_CONFIG;
  buffer[1] = (mr << 3);
  twi_write_bytes (slave_address, 2, buffer);

  gyro_sensitivity = GYRO_SENSITIVITY / (1 << mr); /* LSB per °/sec */

  _delay_ms (1);
}

/*
 * \fn void mpu9150_compass_setup (void)
 * \brief setup compass, enable i2c by-pass
 * */
void
mpu9150_compass_setup (void)
{
  volatile uint8_t buffer[2];

  /* compass configuration : bypass mode
   * the host application processor will be able to directly access the auxiliary i2c bus */
  buffer[0] = INT_PIN_CFG;
  buffer[1] = I2C_BYPASS_EN;
  twi_write_bytes (slave_address, 2, buffer);
  _delay_ms (1);
}

/*
 * \fn uint8_t mpu9150_read_device_id (void)
 * \brief read device id
 * \return device id
 * */
uint8_t
mpu9150_read_device_id (void)
{
  volatile uint8_t device_id;
  volatile uint8_t reg = WHO_AM_I;

  twi_read_bytes (slave_address, &reg, 1, &device_id);

  return device_id;
}

/*
 * \fn int16_t mpu9150_read_temperature (void)
 * \return temperature in degrees C = (TEMP_OUT signed / 340) + 35
 * */
int16_t
mpu9150_read_temperature (void)
{
  volatile uint8_t buffer[2];
  volatile uint8_t reg = TEMP_OUT_H;

  twi_read_bytes (slave_address, &reg, 2, buffer);

  int16_t temperature;
  temperature = ((buffer[0] << 8) + buffer[1]);
  temperature /= 340;
  temperature += 35;
  return temperature;
}

/**
 * \fn xyz mpu9150_read_acc_xyz (void)
 * \brief read x-, y-, z-axis acceleration
 * \return x, y and z-axis acceleration value in g
 * */
xyz
mpu9150_read_acc_xyz (void)
{
  volatile uint8_t reg;
  volatile uint8_t buffer[6];
  xyz acc;

  reg = ACCEL_XOUT_H;
  twi_read_bytes (slave_address, &reg, 6, buffer);

  acc.x = (double) (buffer[1] + (buffer[0] << 8)) / acc_sensitivity;
  acc.y = (double) (buffer[3] + (buffer[2] << 8)) / acc_sensitivity;
  acc.z = (double) (buffer[5] + (buffer[4] << 8)) / acc_sensitivity;

  return acc;
}

/**
 * \fn xyz mpu9150_read_gyro_xyz (void)
 * \brief read x-, y-, z-axis angular speed
 * \return x, y and z-axis angular speed value in °/sec */
xyz
mpu9150_read_gyro_xyz (void)
{
  volatile uint8_t reg = GYRO_XOUT_H;
  volatile uint8_t buffer[6];
  xyz gyro;

  twi_read_bytes (slave_address, &reg, 6, buffer);

  gyro.x = (double) ((buffer[0] << 8) + buffer[1]) / gyro_sensitivity;
  gyro.y = (double) ((buffer[2] << 8) + buffer[3]) / gyro_sensitivity;
  gyro.z = (double) ((buffer[4] << 8) + buffer[5]) / gyro_sensitivity;

  return gyro;
}

/**
 * \fn xyz mpu9150_read_gyro_xyz (void)
 * \brief read x, y and z-axis magnetic field
 * \return x, y and z-axis magnetic field value in µT */
xyz
mpu9150_read_compass_xyz (void)
{
  return ak8975c_read_xyz ();
}
