/*
 * Filter.c
 *
 * Created: 2025-03-06 10:52:38
 *  Author: Saulius
 */ 
#include "Settings.h"


void FIR(solarrcells_t channel) {
	ReadSolarCells(channel); //Read Solar cells Voltage or current depending from selected channel
	ADC_VALUES *voltageORcurrent = (channel == Voltage) ? &ReadVoltage : &ReadCurrent; //If channel is for voltage we will use ReadVoltage object to store data otherwise ReadCurrent
	voltageORcurrent->Filter[voltageORcurrent->index] = voltageORcurrent->Result; //depending of selected object savo measured voltage or current to filter data storage array
	voltageORcurrent->index = (voltageORcurrent->index + 1) % FIR_STEPS; //and increasing its index nuber till reach defined FIR_STEPS value after that reseting index to 0
	uint32_t sum = 0; //uint32_t can hold max up to 65535 uint16_t values so maximum FIR_STEPS can be 65535 (way too much)
	for (uint8_t i = 0; i < FIR_STEPS; i++) {
		sum += voltageORcurrent->Filter[i]; // calculating average rsult of all array sum divided from defined FIR_STEPS value
	}
	voltageORcurrent->Result = sum / FIR_STEPS; //reuse Result variable to save final value
}