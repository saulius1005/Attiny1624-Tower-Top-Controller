/**
 * @file ADC.c
 * @brief Implementation of ADC initialization and solar cell voltage/current measurements.
 *
 * Contains functions for setting up the ADC and reading values from solar voltage and current sensors.
 * Supports filtering and reference voltage correction based on the internal VDD measurement.
 *
 * @author Saulius
 * @date 2024-09-02
 */

#include "Settings.h"
#include "ADCVar.h"

/**
 * @brief Initializes ADC0 peripheral with burst averaging and timing settings.
 *
 * - Enables the ADC.
 * - Sets the ADC clock prescaler.
 * - Applies timebase for proper sampling setup.
 * - Uses 1024-sample accumulation for noise reduction.
 * - Configures burst mode.
 */
void ADC0_init() {
	ADC0.CTRLA = ADC_ENABLE_bm; ///< Enable ADC
	ADC0.CTRLB = ADC_PRESC_DIV10_gc; ///< Set ADC clock prescaler to divide by 10
	ADC0.CTRLC = (TIMEBASE_VALUE << ADC_TIMEBASE_gp); ///< Set ADC timebase
	ADC0.CTRLE = 0; ///< Set sampling length to 0.5 ADC cycles
	ADC0.CTRLF = ADC_SAMPNUM_ACC1024_gc; ///< Accumulate 1024 samples for each result
	ADC0.COMMAND = ADC_MODE_BURST_SCALING_gc; ///< Use burst mode for better averaging
	while (ADC0.STATUS & ADC_ADCBUSY_bm); ///< Wait until ADC is ready
}

/**
 * @brief Performs a single ADC read on the specified channel.
 *
 * Starts a conversion and waits for completion. Returns the average result of 1024 samples.
 *
 * @param channel The ADC multiplexer positive input channel.
 * @return 16-bit averaged ADC result.
 */
uint16_t ADC0_Read(uint8_t channel) {
	ADC0.MUXPOS = channel;
	ADC0.COMMAND |= ADC_START_IMMEDIATE_gc; ///< Start conversion
	while (ADC0.STATUS & ADC_ADCBUSY_bm); ///< Wait for ADC to become ready
	while (!(ADC0.INTFLAGS & ADC_SAMPRDY_bm)); ///< Wait until the result is ready
	ADC0.INTFLAGS = ADC_SAMPRDY_bm; ///< Clear the sample ready flag
	return ADC0.SAMPLE; ///< Return averaged result
}

/**
 * @brief Reads either voltage or current from a solar cell based on the selected channel.
 *
 * - For current: Uses internal 1.024V reference to measure VDD/10, calculates true range for TMCS1100 sensor,
 *   then reads current with VDD as reference and applies correction.
 * - For voltage: Uses 2.048V reference for AMC1311-based voltage sensor and applies scaling.
 *
 * @param channel Selects between voltage and current input channels.
 */
void ReadSolarCells(solarrcells_t channel) {
	ADC_VALUES *voltageORcurrent = (channel == Voltage) ? &ReadVoltage : &ReadCurrent;

	if (channel == Current) {
		// Current measurement depends on MCU VDD; we need to measure VDD to calibrate range.
		ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_1024MV_gc;
		float mcuVoltage = 0.0025 * ADC0_Read(ADC_MUXPOS_VDDDIV10_gc); ///< VDD = 10 × ADC result × 1.024 / 4096
		uint16_t tmc1100Range = round(mcuVoltage * 250); ///< Calibrated TMCS1100 range depending on VDD
		ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_VDD_gc;
		uint16_t current = ((float)tmc1100Range / 4096) * ADC0_Read(channel);
		voltageORcurrent->Result = current > TMCS1100_ZERO_I ? current - TMCS1100_ZERO_I : current;
	}
	else {
		// Voltage measurement uses a fixed 2.048V reference, independent of VDD.
		ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_2048MV_gc;
		voltageORcurrent->Result = AMC1311_COEF * ADC0_Read(channel);
	}
}
