/*
 * Attiny1624-Tower-Top-Controller.c
 *
 * Created: 2025-03-03 20:35:05
 * Author : Saulius
 */ 
#include "Settings.h"

/**
 * @brief Main function to initialize peripherals and read MT6701 sensor data.
 * 
 * This function initializes the system clock, GPIO, and USART0 communication.
 * It then enters an infinite loop where it continuously reads the MT6701 sensor
 * data every 100 milliseconds.
 *
 * @return int (not used, since the function never exits).
 */
int main(void)
{
	//CLOCK_XOSCHF_clock_init(); 
    CLOCK_INHF_clock_init(); ///< Initialize system clock
    GPIO_init(); ///< Initialize GPIO pins
    USART0_init(); ///< Initialize USART0 for SPI communication

    while (1) 
    {
        MT6701_SSI_Angle(); ///< Read MT6701 sensor data
        _delay_ms(100); ///< Wait 100ms before the next read
    }
}