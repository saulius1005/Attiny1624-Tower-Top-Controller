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


/*void ReadSolarCells(solarrcells_t channel){

	ADC_VALUES *voltageORcurrent = (channel == Voltage) ? &ReadVoltage : &ReadCurrent; //selecting object where data will be saved according to selected channel (Voltage or Current)

	ADC0.MUXPOS = channel;
	
	ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_1024MV_gc;  // 1.024V 
	if(ADC0_Read()>= 0xff0){ //if more or equal to 4080 ADC steps
		ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_2048MV_gc; //2.048V
		if (ADC0_Read()>= 0xff0){
			ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_4096MV_gc; //4.096V
			if(ADC0_Read()>= 0xff0){
				ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_VDD_gc; //ref voltage is 5V (TMC1100a4 = 400mV/A min: 0.125A, max: 12A), sollar cells max is ScI 10.64A+-3% ~11A, 11 * 0.4 = 4.4V > 4.096Vref
				if(channel == Current)
					voltageORcurrent->Result = (double)ADC0_Read()/3.2768; //Assume Vdd is 5.0V. 32.768 same as /327.68 * 10
			}
			else
			voltageORcurrent->Result = (channel == Voltage) ? ADC0_Read() : ADC0_Read()*0.25; //voltage measurements ends near 2.5V, but linear voltage output near 2V (keep this only for error detections)
		}
		else
		voltageORcurrent->Result = (channel == Voltage) ?  ADC0_Read()*0.5 : ADC0_Read()*0.125; //voltage linear measurement maximum value is 2.0V

	}
	else		
		voltageORcurrent->Result = (channel == Voltage) ?  ADC0_Read()*0.25 : ADC0_Read()*0.0625;  // Read ADC value, scale it, and round the result

}*/

void ReadSolarCells(solarrcells_t channel) {
	ADC_VALUES *voltageORcurrent = (channel == Voltage) ? &ReadVoltage : &ReadCurrent;
	ADC0.MUXPOS = channel;

	// Vref pasirinkimo seka (atitinka 1.024V, 2.048V, 4.096V, Vdd (5V))
	const uint8_t vref_options[4] = {ADC_REFSEL_1024MV_gc, ADC_REFSEL_2048MV_gc, ADC_REFSEL_4096MV_gc, ADC_REFSEL_VDD_gc};
	
	uint8_t vref_index = 0;
	uint16_t adcValue;

	// Pasirenkame optimalø Vref
	do {
		ADC0.CTRLC = (ADC0.CTRLC & ~ADC_REFSEL_gm) | vref_options[vref_index];
		adcValue = ADC0_Read();
	} while (adcValue >= 0xFF0 && ++vref_index < 4);

	// Srovës skaièiavimas
	if (channel == Current) {
		voltageORcurrent->Result = (vref_index < 3) ? (adcValue >> (4 - vref_index)) : ((double)adcValue / 3.2768);
	}
	// Átampos skaièiavimas
	else if (vref_index < 3) {
		voltageORcurrent->Result = adcValue >> (2 - vref_index); // 1.024V ? /4, 2.048V ? /2, 4.096V ? /1
	}
}
