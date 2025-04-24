/**
 * @file Filter.c
 * @brief Implementation of a simple FIR filter for solar cell voltage and current measurements.
 *
 * Applies a fixed-length moving average (FIR) filter to smooth out readings from solar sensors.
 * This is used to reduce noise in both voltage and current measurements.
 *
 * @author Saulius
 * @date 2025-03-06
 */

#include "Settings.h"

/**
 * @brief Applies FIR (Finite Impulse Response) filtering to the selected ADC channel.
 *
 * Reads a new ADC value (voltage or current), stores it in the filter buffer, and calculates the average.
 *
 * - Voltage or current value is selected based on the channel input.
 * - The new measurement is added to a circular buffer.
 * - The filtered result is calculated as the average of all samples in the buffer.
 *
 * @param channel Specifies whether to process voltage or current (Voltage or Current).
 */
void FIR(solarrcells_t channel) {
	ReadSolarCells(channel); ///< Read raw measurement from the selected ADC channel

	// Select target ADC_VALUES structure based on the channel (voltage or current)
	ADC_VALUES *voltageORcurrent = (channel == Voltage) ? &ReadVoltage : &ReadCurrent;

	// Store the latest measurement into the filter buffer at the current index
	voltageORcurrent->Filter[voltageORcurrent->index] = voltageORcurrent->Result;

	// Update the index (circular buffer behavior)
	voltageORcurrent->index = (voltageORcurrent->index + 1) % FIR_STEPS;

	// Calculate the sum of all values in the buffer
	uint32_t sum = 0; ///< uint32_t is sufficient to store sum of up to 65535 uint16_t values
	for (uint8_t i = 0; i < FIR_STEPS; i++) {
		sum += voltageORcurrent->Filter[i];
	}

	// Store the filtered result as the average of the buffer
	voltageORcurrent->Result = sum / FIR_STEPS;
}
