/*
 * USARTVar.h
 *
 * Created: 2025-03-03 21:30:22
 *  Author: Saulius
 */ 


#ifndef USARTVAR_H_
#define USARTVAR_H_

/**
 * @brief Communication status structure.
 * 
 * This structure holds flags and counters used to track the status of the 
 * communication system. The structure includes the following fields:
 * 
 * - `.error`: A flag indicating if an error has occurred.
 * - `.errorCounter`: A counter that increments when warnings occur.
 * - `.warning`: A flag indicating if there is a warning.
 */
Communication Status = {
    .error = 0,           ///< Error flag, initialized to 0 (no error).
    .errorCounter = 0,    ///< Error counter, initialized to 0.
    .warning = 0          ///< Warning flag, initialized to 0 (no warning).
};

#endif /* USARTVAR_H_ */