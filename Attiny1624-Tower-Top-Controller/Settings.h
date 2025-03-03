/**
 * @file Settings.h
 * @brief Global settings and function declarations for the project.
 * @author Saulius
 * @date 2024-12-04
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define F_CPU 20000000

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 
#include "ADC.h"
#include "USART.h"
#include "MT6701.h"

/**
 * @brief Initializes general-purpose input/output (GPIO) settings.
 */
void GPIO_init();

/**
 * @brief Initializes the internal high-frequency clock.
 */
void CLOCK_INHF_clock_init();

/**
 * @brief Computes CRC for a given command.
 * @param command 32-bit input data for CRC calculation.
 * @return Computed CRC value.
 */
uint8_t CRC(uint32_t command);

/**
 * @brief Initializes USART0 for SPI communication.
 */
void USART0_init();

/**
 * @brief Sends a character via USART0.
 * @param c Character to be transmitted.
 */
void USART0_sendChar(char c);

/**
 * @brief Reads a character from USART0.
 * @return The received character.
 */
char USART0_readChar();

/**
 * @brief Verifies and removes CRC from received MT6701 sensor data.
 * @param data Pointer to the 32-bit sensor data.
 * @return 0 if CRC is correct, 1 if incorrect.
 */
uint8_t MT6701CRC(uint32_t *data);

/**
 * @brief Reads angle data from the MT6701 sensor using SSI protocol.
 */
void MT6701_SSI_Angle();

#endif /* SETTINGS_H_ */