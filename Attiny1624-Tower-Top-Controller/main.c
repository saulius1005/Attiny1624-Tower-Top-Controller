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
	ADC0_init();

    while (1) 
    {
        MT6701_SSI_Angle(Elevation_Angle); ///< Read MT6701 sensor data
        MT6701_SSI_Angle(Azimuth_Angle); ///< Read MT6701 sensor data
		_delay_ms(10);
		ReadSolarCells(Voltage);
		_delay_ms(10);
		ReadSolarCells(Current);
		uint8_t y = YEndSwitches();
		uint64_t combined = ((uint64_t)MT6701ELEVATION.Angle << 44) | ((uint64_t)MT6701AZIMUTH.Angle << 28) | ((uint64_t)ReadVoltage.Result << 16) | ((uint32_t)ReadCurrent.Result << 4) | y;



		        // Send the combined data over USART0 in a formatted string
		        USART1_printf("<%04x%04x%03x%03x%x%02x>\r\n",
		        (uint16_t)MT6701ELEVATION.Angle,           ///< Elevation angle (4 digits)
		        (uint16_t)MT6701AZIMUTH.Angle,           ///< Azimuth angle (4 digits)
		        (uint16_t)ReadVoltage.Result,           ///< Voltage (3 digits)
		        (uint16_t)ReadCurrent.Result,            ///< Current (3 digits)
		        (uint8_t)y,            ///< End switch status (1 digit)
		        (uint8_t)crc8_cdma2000(combined)); ///< CRC value (1 byte)
        _delay_ms(100); ///< Wait 100ms before the next read
    }
}