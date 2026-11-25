/*
 * payment_pulse.h
 *
 *  Created on: Nov 23, 2026
 *      Author: sonam
 */

#ifndef INC_PAYMENT_PULSE_H_
#define INC_PAYMENT_PULSE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t bill;
    uint32_t coin;
    uint32_t cashless;
    uint32_t sense;
} PaymentPulseCounts;

// Call from the main loop. Returns and clears all four pending counts.
PaymentPulseCounts PaymentPulse_TakeCounts(void);

// Call from the main loop. Writes the new total and returns true when it changes.
bool PaymentPulse_UpdateTotal(uint32_t *new_total);


#endif /* INC_PAYMENT_PULSE_H_ */
