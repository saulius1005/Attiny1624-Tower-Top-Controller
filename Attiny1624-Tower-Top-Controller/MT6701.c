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
void MT6701_SSI_Angle() {
    uint32_t received_data = 0;

    PORTA.OUTCLR = PIN7_bm; ///< Pull CSN low to start communication   
    for (uint8_t i = 0; i < 3; i++) { ///< 3 bytes (24 bits) of data
        USART0_sendChar('o'); ///< Send dummy data (8 bits) for clock generation
		while (!(USART0.STATUS & USART_TXCIF_bm)) {} ///< Repeat until the full frame is received
		USART0.STATUS = USART_TXCIF_bm; ///< Clear frame flag before data collection
        received_data <<= 8; ///< Shift previous data left by 8 bits
        received_data |= USART0_readChar(); ///< Read 8 bits of received data and Append current received byte
    }
    PORTA.OUTSET = PIN7_bm; ///< Pull CSN high (USART SPI mode does not have integrated SS control)
    
    MT6701.CRCError = MT6701CRC(&received_data); ///< Verify and remove CRC from received data
    MT6701.MagneticFieldStatus = (received_data & 0xF) & 0x3; ///< Extract magnetic field status
    MT6701.PushButtonStatus = (received_data & 0xF) & 0x4; ///< Extract push button status
    MT6701.TrackStatus = (received_data & 0xF) & 0x8; ///< Extract track status
    MT6701.Angle = (double)(received_data >> 4) / 45.51111111; ///< Compute angle in degrees (received_data >> 4) / 16384 * 360
}