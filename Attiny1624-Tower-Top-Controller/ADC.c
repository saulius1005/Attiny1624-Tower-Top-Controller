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
	 ADC0.CTRLC = (TIMEBASE_VALUE << ADC_TIMEBASE_gp);
	 ADC0.CTRLE = 0; //How long to take a one sample (ADC clock cycles) so it will take 0+ 0.5 = half cycle
	 ADC0.CTRLF = ADC_SAMPNUM_NONE_gc ; //samples:
	 ADC0.COMMAND = ADC_MODE_SINGLE_12BIT_gc; // 1 sample
	 while (ADC0.STATUS & ADC_ADCBUSY_bm); // wait untill all settings will be set up

 }

 uint16_t ADC0_Read(){
	 ADC0.COMMAND |= ADC_START_IMMEDIATE_gc; //start conversion
	 while (ADC0.STATUS & ADC_ADCBUSY_bm); // wait untill all settings will be set up
	 while (!(ADC0.INTFLAGS & ADC_SAMPRDY_bm));
	 ADC0.INTFLAGS = ADC_SAMPRDY_bm; //clear result ready flag
	 return ADC0.SAMPLE; //return average ADC value of 1024 measurements
 }


void ReadSolarCells(solarrcells_t channel){

	ADC_VALUES *voltageORcurrent = (channel == Voltage) ? &ReadVoltage : &ReadCurrent; //selecting object where data will be saved according to selected channel (Voltage or Current)

	ADC0.MUXPOS = channel;
	
	ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_1024MV_gc;  // 1.024V 
	if(ADC0_Read()>= 0xff0){ //if more or equal to 4080 ADC steps
		ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_2048MV_gc; //2.048V
		if (ADC0_Read()>= 0xff0){
			ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_4096MV_gc; //4.096V
			if(ADC0_Read()>= 0xff0){
				ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_VDD_gc; //ref voltage is 5V (TMC1100a4 = 400mV/A min: 0.125A, max: 12A), sollar cells max is ScI 10.64A+-3% ~11A, 11 * 0.4 = 4.4V > 4.096Vref
				voltageORcurrent->Result = (double)ADC0_Read()/3.2768; //Assume Vdd is 5.0V. 32.768 same as /327.68 * 10
			}
			else
			voltageORcurrent->Result = ADC0_Read()>>2;
		}
		else
		voltageORcurrent->Result = ADC0_Read()>>3;
	}
	else
	voltageORcurrent->Result = ADC0_Read()>>4;  // Read ADC value, scale it, and round the result
}