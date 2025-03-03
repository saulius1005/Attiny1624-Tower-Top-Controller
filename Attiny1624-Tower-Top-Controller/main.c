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
	USART1_init();

    while (1) 
    {
        MT6701_SSI_Angle(Elevation_Angle); ///< Read MT6701 sensor data
        _delay_ms(100); ///< Wait 100ms before the next read
        MT6701_SSI_Angle(Azimuth_Angle); ///< Read MT6701 sensor data

		uint8_t y = YEndSwitches();
		uint64_t combined = ((uint64_t)MT6701ELEVATION.Angle << 40) | ((uint64_t)MT6701AZIMUTH.Angle << 24) | ((uint32_t)ReadADC.SCU << 12) | ((uint16_t)ReadADC.SCI << 4) | y;

        _delay_ms(100); ///< Wait 100ms before the next read
		        // Send the combined data over USART0 in a formatted string
		        USART1_printf("<%04x%04x%03x%02x%x%02x>\r\n",
		        (uint16_t)MT6701ELEVATION.Angle,           ///< Elevation angle (4 digits)
		        (uint16_t)MT6701AZIMUTH.Angle,           ///< Azimuth angle (4 digits)
		        (uint16_t)ReadADC.SCU,           ///< Voltage (3 digits)
		        (uint8_t)ReadADC.SCI,            ///< Current (2 digits)
		        (uint8_t)y,            ///< End switch status (1 digit)
		        (uint8_t)crc8_cdma2000(combined)); ///< CRC value (1 byte)
    }
}