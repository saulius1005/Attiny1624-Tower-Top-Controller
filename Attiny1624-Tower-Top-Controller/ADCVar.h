/**
 * @file ADCVar.h
 * @brief ADC measurement buffers for current and voltage readings.
 *
 * This file defines global variables used to store filtered ADC values for
 * current and voltage measurements.
 *
 * @author Saulius
 * @date 2025-03-03
 */

#ifndef ADCVAR_H_
#define ADCVAR_H_

/**
 * @brief Filtered and raw ADC data for current measurement.
 *
 * Stores the most recent current measurement and applies filtering
 * using a circular buffer of previous values.
 */
ADC_VALUES ReadCurrent = {
	.Result = 0,    ///< Most recent filtered current measurement result
	.Filter = {0},  ///< Circular buffer for filtering current readings
	.index = 0      ///< Current index in the filter buffer
};

/**
 * @brief Filtered and raw ADC data for voltage measurement.
 *
 * Stores the most recent voltage measurement and applies filtering
 * using a circular buffer of previous values.
 */
ADC_VALUES ReadVoltage = {
	.Result = 0,    ///< Most recent filtered voltage measurement result
	.Filter = {0},  ///< Circular buffer for filtering voltage readings
	.index = 0      ///< Current index in the filter buffer
};

#endif /* ADCVAR_H_ */
