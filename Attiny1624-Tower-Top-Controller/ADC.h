/**
 * @file ADC.h
 * @brief Definitions for ADC configuration and solar cell measurement handling.
 *
 * Provides macros, type definitions, and external variable declarations used for
 * analog-to-digital conversion and filtering of voltage and current signals.
 *
 * @author Saulius
 * @date 2025-03-03
 */

#ifndef ADC_H_
#define ADC_H_

#include <math.h>

/**
 * @brief ADC time base value in microseconds.
 *
 * According to datasheet, the time base must be at least 1 µs.
 * This macro calculates the value dynamically based on the system clock.
 */
#define TIMEBASE_VALUE ((uint8_t) ceil(F_CPU * 0.000001))

/**
 * @brief Voltage scaling coefficient for AMC1311 voltage sensor.
 *
 * The AMC1311 has a full input range of 2V which corresponds to 300V (depending on external resistor divider).
 * With the MCU reference voltage at 2.048V, the actual measurable range is 307.2V.
 * This constant converts raw ADC values into a scaled integer voltage representation.
 */
#define AMC1311_COEF 0.75

/**
 * @brief Zero-current offset for TMCS1100 current sensor.
 *
 * The TMCS1100 sensor starts measuring current from ~0.125A.
 * This value (12) corresponds to 0.125A × 100 scaling factor and is used as a correction offset.
 */
#define TMCS1100_ZERO_I 12

/**
 * @brief Structure for storing filtered ADC values.
 *
 * Holds the latest result and a buffer of previous ADC readings for filtering (e.g., FIR).
 */
typedef struct {
	uint16_t Result;         ///< Last filtered ADC result
	uint16_t Filter[FIR_STEPS]; ///< FIR filter buffer
	uint8_t index;           ///< Index for the current position in the filter buffer
} ADC_VALUES;

/**
 * @brief Enum for selecting solar cell ADC input channels.
 *
 * Used to distinguish between voltage and current measurement channels.
 */
typedef enum {
	Voltage = ADC_MUXPOS_AIN2_gc,   ///< ADC channel for voltage measurement
	Current = ADC_MUXPOS_AIN11_gc   ///< ADC channel for current measurement
} solarrcells_t;

/**
 * @brief External ADC result holder for current measurements.
 */
extern ADC_VALUES ReadCurrent;

/**
 * @brief External ADC result holder for voltage measurements.
 */
extern ADC_VALUES ReadVoltage;

#endif /* ADC_H_ */
