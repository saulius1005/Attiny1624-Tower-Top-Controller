/**
 * @file CLK.c
 * @brief Clock configuration functions for the system.
 *
 * This file contains functions to initialize and configure the external high-frequency
 * crystal oscillator and external clock input for the system.
 * 
 * @author Saulius
 * @date 2025-03-01 20:26:48
 */

#include "Settings.h"

void CLOCK_XOSCHF_clock_init()
{
	/* Wait for external clock change to stable */
	while (CLKCTRL.MCLKSTATUS & CLKCTRL_EXTS_bm)
	{};
	/* Enable external (20 MHz) clock input */
	ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_EXTCLK_gc | CLKCTRL_CLKSEL_OSC20M_gc);

	/* Disable the main clock prescaler for full-speed operation. */
	ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc & ~CLKCTRL_PEN_bm);

	/* Wait for system oscillator change to complete */
	while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
	{};
	/* Configuration complete;*/
}

void CLOCK_INHF_clock_init() {
	/* Enable the internal oscillator with a frequency of 20 MHz. */
	ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSC20M_gc /*| CLKCTRL_CLKOUT_bm*/);
	// For SO14, it is impossible to enable CLKOUT due to the lack of a dedicated pin.

	/* Disable the main clock prescaler for full-speed operation. */
	ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc & ~CLKCTRL_PEN_bm);

	/* Wait for the oscillator change to complete. */
	while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm) {};
}