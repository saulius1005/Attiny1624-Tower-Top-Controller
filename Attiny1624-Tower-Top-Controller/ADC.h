/*
 * ADC.h
 *
 * Created: 2025-03-03 21:22:45
 *  Author: Saulius
 */ 


#ifndef ADC_H_
#define ADC_H_

#define TIMEBASE_VALUE ((uint8_t) ceil(F_CPU*0.000001)) //ADC time base

typedef struct {
	uint16_t Result;
	uint16_t Filter[50];
	uint8_t index;
} ADC_VALUES;

typedef enum {
	Voltage = ADC_MUXPOS_AIN5_gc,
	Current = ADC_MUXPOS_AIN4_gc
}solarrcells_t;

extern ADC_VALUES ReadCurrent;
extern ADC_VALUES ReadVoltage;

#endif /* ADC_H_ */