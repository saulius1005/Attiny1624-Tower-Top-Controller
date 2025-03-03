/**
 * @file USART.c
 * @brief Implementation of USART communication in SPI mode.
 * @author Saulius
 * @date 2025-02-10
 */

#include "Settings.h"
#include "USARTVar.h"

/**
 * @brief Initializes USART0 for SPI communication.
 *
 * This function configures USART0 to operate in SPI master mode with a baud rate of 500 kbps.
 * It enables the receiver (MISO) and transmitter (for sending dummy data), and sets
 * the SPI mode with data sampling on the trailing edge.
 */
void USART0_init() {
    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(500000); ///< Set baud rate to 0.5 Mbps
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; ///< Enable RX as MISO, TX for dummy data sending
    USART0.CTRLC = USART_CMODE_MSPI_gc | USART_UCPHA_bm; ///< Configure as Host SPI, data sampled on the trailing edge
}

/**
 * @brief Sends a character via USART0.
 *
 * @param c Character to be transmitted.
 *
 * This function waits for the data register to be empty before transmitting
 * the provided character.
 */
void USART0_sendChar(char c) {
    while (!(USART0.STATUS & USART_DREIF_bm)); ///< Wait for data register to be empty
    USART0.TXDATAL = c; ///< Send character
}

/**
 * @brief Reads a single character from USART0.
 * 
 * This function waits for a character to be received, and if no character is received within the timeout 
 * period, it sets a warning flag. 
 * 
 * @return The received character.
 */
char USART0_readChar() {
    USART0.STATUS = USART_RXCIF_bm; // Clear buffer before reading
    uint32_t timeout_counter = TIMEOUT_COUNTER; // Set a timeout counter
    while (!(USART0.STATUS & USART_RXCIF_bm)) { // Wait for data to be received
        if (--timeout_counter == 0) { // Timeout condition
            Status.warning = 1; // Set warning if timeout occurs
            break;
        }
    }
    return USART0.RXDATAL; // Return received character
}


/**
 * @brief Initializes USART1 with a baud rate of 500000.
 * 
 * This function configures USART1 for asynchronous communication, enabling both
 * transmission and reception at a baud rate of 0.5 Mbps with double-speed operation.
 */
void USART1_init() {
	USART1.BAUD = (uint16_t)USART1_BAUD_RATE(500000); // Set baud rate to 0.5 Mbps
	USART1.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_RXMODE_CLK2X_gc; // Enable RX, TX, double speed mode
	USART1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc; // Configure for 8-bit, no parity, 1 stop bit, asynchronous mode
}

/**
 * @brief Sends a single character via USART1.
 * 
 * This function waits until the USART1 data register is empty and then transmits a character.
 * 
 * @param c The character to send.
 */
void USART1_sendChar(char c) {
	while (!(USART1.STATUS & USART_DREIF_bm)); // Wait for data register to be empty
	USART1.TXDATAL = c; // Send character
}

/**
 * @brief Sends a string via USART1.
 * 
 * This function sends each character of the string one by one using the USART1_sendChar function.
 * 
 * @param str The string to send.
 */
void USART1_sendString(char *str) {
	for (size_t i = 0; i < strlen(str); i++) {
		USART1_sendChar(str[i]); // Send each character
	}
}

/**
 * @brief Sends a formatted string via USART1.
 * 
 * This function formats the input string with the provided arguments and sends it via USART1.
 * 
 * @param format The format string.
 * @param ... The arguments to be formatted into the string.
 */
void USART1_printf(const char *format, ...) {
	char buffer[30]; // Temporary buffer for formatted message
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args); // Format the message into the buffer
	va_end(args);
	USART1_sendString(buffer); // Use USART1 for sending
}