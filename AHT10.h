#include "frdm_bsp.h"
#include "delays.h"
#include <stdio.h>

#ifndef _CZUJNIK_H
#define _CZUJNIK_H

/*
 * Sensor constants
 * I2C address, initialization and begin measurment commands
 */
#define AHT_ADD 0x38 
#define CMD_INIT 0xe1
#define CMD_POM 0xac

/** 
 * @brief I2C initialization
 */
void I2C_Init(void);

/**
 * @brief Basic I2C write
 * @param Slave address
 * @param Data 
 */
void I2C_Write(uint8_t address, uint8_t data);

/**
 * @brief I2C read from block of registers
 * @param Slave device address
 * @param Start register
 * @param Number of registers to read
 * @param Data from slave device 
 */
void I2C_Read(uint8_t address, uint8_t reg, uint8_t size, uint8_t* data);

/**
 * @brief AHT10 initialization 
 */
void AHT10_Init(void);

/**
 * @brief Writes binary data into supplied addresses
 * @param Raw temperature reading
 * @param Raw humidity reading
 */
void AHT10_GetRawReadings(uint32_t *t_data, uint32_t *h_data);

/**
 * @brief Converts binary data into Celcius and humidity percentage 
 * @param Temperature converted to celcius
 * @param Humidity converted to percentage
 * @param Raw temperature reading
 * @param Raw humidity reading
 */
void AHT10_ConvertReadings(double *temp_C,double *hum_P,uint32_t temp_raw,uint32_t wilg_raw);

#endif
