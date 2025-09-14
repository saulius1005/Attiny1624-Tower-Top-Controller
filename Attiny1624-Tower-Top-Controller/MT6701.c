/**
 * @file MT6701.c
 * @brief Implementation of MT6701 sensor angle reading via SSI.
 * @author Saulius
 * @date 2025-02-28
 */

#include "Settings.h"
#include "MT6701Var.h"

/**
 * @brief Reads the angular position from the MT6701 sensor using SSI communication.
 *
 * This function initiates an SSI communication session by pulling the chip select (CSN) low,
 * transmitting dummy data to generate a clock signal, and receiving the corresponding data bits.
 * The received data is then processed to extract the angle, magnetic field status, push button status,
 * and track status.
 */
void MT6701_SSI_Angle(angleChannel_t channel) {
    uint32_t received_data = 0;
    PORTA.OUTCLR = channel; ///< Pull CSN low to start communication   
    for (uint8_t i = 0; i < 3; i++) { ///< 3 bytes (24 bits) of data
        USART0_sendChar('o'); ///< Send dummy data (8 bits) for clock generation
        received_data <<= 8; ///< Shift previous data left by 8 bits
        received_data |= USART0_readChar(); ///< Read 8 bits of received data and Append current received byte
    }
    PORTA.OUTSET = channel; ///< Pull CSN high (USART SPI mode does not have integrated SS control)
    
	    // Use a pointer to simplify the logic
	    AngleSensorStatus *sensor = (channel == Elevation_Angle) ? &MT6701ELEVATION : &MT6701AZIMUTH;

	    // Update sensor data
	    sensor->CRCError = MT6701CRC(&received_data);  // Verify and remove CRC from received data
	    sensor->MagneticFieldStatus = received_data & 0x3;  // Extract magnetic field status
	    sensor->PushButtonStatus = (received_data >> 2) & 0x1;  // Extract push button status
	    sensor->TrackStatus = (received_data >> 3) & 0x1;  // Extract track status
	    sensor->Angle = ((double)(received_data >> 4) / 0.4551111111)+0.5;  // Compute angle in degrees
}