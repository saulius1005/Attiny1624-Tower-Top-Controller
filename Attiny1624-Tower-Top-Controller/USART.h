/*
 * USART.h
 *
 * Created: 2025-03-03 21:26:54
 *  Author: Saulius
 */ 


#ifndef USART_H_
#define USART_H_

/**
 * @brief Macro to calculate USART baud rate in synchronous mode as Host SPI.
 * @param BAUD_RATE Desired baud rate.
 */

#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU / (2 * (float)BAUD_RATE / 64)) + 0.5) //synchronous mode as Host SPI
#define USART1_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (8 *(float)BAUD_RATE)) + 0.5) // double speed

#define PrintfBufferSize 30 //printf buffer size for USART1_printf()

/**
 * @brief Maximum count for consecutive errors before marking the system as faulty.
 */
#define CountForError 10

/**
 * @brief Timeout counter value for operations.
 * 
 * This constant defines the timeout threshold for operations. If the counter 
 * exceeds this value, it indicates a timeout has occurred.
 */
#define TIMEOUT_COUNTER 40000 ///< Timeout counter value for operations

/**
 * @brief Structure to hold communication status.
 *
 * This structure is used to track the state of communication for the transceiver. 
 * It includes flags and counters to handle errors and warnings.
 */
typedef struct {
    uint8_t error;         /**< Error flag (1 if an error occurs, 0 otherwise) */
    uint8_t errorCounter;  /**< Counter for error occurrences */
    uint8_t warning;       /**< Warning flag (1 if a warning occurs) */
} Communication;

/**
 * @brief Global variable for tracking communication status.
 *
 * This external variable is used to store the current communication status, 
 * including error and warning states.
 */
extern Communication Status;

#endif /* USART_H_ */