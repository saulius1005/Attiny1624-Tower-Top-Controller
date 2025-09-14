/*
 * Yswitches.c
 *
 * Created: 2025-03-03 23:22:12
 *  Author: Saulius
 */ 
 #include "Settings.h"

 uint8_t YEndSwitches(){
	return (!(PORTA.IN & PIN5_bm)) | ((!(PORTA.IN & PIN4_bm)) << 1); // Checking Y min and max values (PB0 and PA2 values) and it will be 0,1,2,3
 } 