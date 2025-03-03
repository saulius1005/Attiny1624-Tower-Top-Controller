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
	uint16_t SCU;
	uint16_t SCI;
} ADC_VALUES;

extern ADC_VALUES ReadADC;

#endif /* ADC_H_ */