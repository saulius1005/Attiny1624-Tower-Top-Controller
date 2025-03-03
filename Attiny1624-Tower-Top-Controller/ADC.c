/*
 * ADC.c
 *
 * Created: 2024-09-02 21:45:41
 *  Author: Saulius
 */ 
 #include "Settings.h"
 #include "ADCVar.h"

 void ADC0_init(){
	 ADC0.CTRLA = ADC_ENABLE_bm; //adc enable
	 ADC0.CTRLB = ADC_PRESC_DIV4_gc; // 20M / 4 = 5MHz ADC clock (MAX adc clock is 6M)
	 ADC0.CTRLC = (TIMEBASE_VALUE << ADC_TIMEBASE_gp)
	 | ADC_REFSEL_4096MV_gc; //4.096V reference
	 ADC0.CTRLE = 0; //How long to take a one sample (ADC clock cycles) so it will take 0+ 0.5 = half cycle
	 ADC0.CTRLF = ADC_SAMPNUM_NONE_gc ; //samples:
	 ADC0.COMMAND = ADC_MODE_SINGLE_12BIT_gc; // 1 sample
	 while (ADC0.STATUS & ADC_ADCBUSY_bm); // wait untill all settings will be set up

 }

 uint16_t ADC0_Read(uint8_t ch){
	 ADC0.COMMAND |= ADC_START_IMMEDIATE_gc; //start conversion
	 ADC0.MUXPOS = ch;
	 while (ADC0.STATUS & ADC_ADCBUSY_bm); // wait untill all settings will be set up
	 while (!(ADC0.INTFLAGS & ADC_SAMPRDY_bm));
	 ADC0.INTFLAGS = ADC_SAMPRDY_bm; //clear result ready flag
	 return ADC0.SAMPLE; //return average ADC value of 64 measurements
 }
