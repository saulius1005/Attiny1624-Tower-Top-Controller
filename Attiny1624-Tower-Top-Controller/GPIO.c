/**
 * @file GPIO.c
 * @brief GPIO initialization for MT6701 sensor communication.
 * @author Saulius
 * @date 2024-12-04
 */

#include "Settings.h"

void GPIO_init(){
    PORTMUX.USARTROUTEA = PORTMUX_USART0_DEFAULT_gc | PORTMUX_USART1_DEFAULT_gc; ///< Set USART0 and UASRT1 to default pins

    PORTB.DIRSET = PIN1_bm | PIN2_bm; ///< Set PB1 as XCK (MT6701 CLK) out and PB2 as TX for dummy data sending
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm; ///< Enable pull-up for PB2 (USART0 TX)    
    PORTB.DIRCLR =  PIN3_bm; ///< PB3 as input (USART0 RX as MISO (MT6701 DO))	
    PORTB.PIN3CTRL = PORT_PULLUPEN_bm; ///< Enable pull-up for PB3 (USART0 RX)

    PORTA.DIRSET = PIN1_bm | PIN6_bm | PIN7_bm; ///< Set PA1 as (USART1 LED TX), PA6 as AZSS (MT6701 CSN),  PA7 as ELSS (MT6701 CSN)
	PORTA.PIN1CTRL = PORT_INVEN_bm; ///< Invert for PA1 (USART1 TX LED TX) to save LED life and consumed energy
	PORTA.DIRCLR = PIN4_bm | PIN5_bm; ///< Set PA4 as input (Y MAX), Set PB0 as input (Y MIN)
	PORTA.PIN4CTRL = PORT_PULLUPEN_bm;///< Enable pull-up for PA2 (Y MAX)
	PORTA.PIN5CTRL = PORT_PULLUPEN_bm; ///< Enable pull-up for PA5 (Y MIN)
    PORTA.OUTSET = PIN6_bm | PIN7_bm; ///< Keep All SS high

	PORTB.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc; ///< Turn off digital buffer for PB0 (SC Current)
	PORTA.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc; ///< Turn off digital buffer for PA2 (SC Voltage)
}
