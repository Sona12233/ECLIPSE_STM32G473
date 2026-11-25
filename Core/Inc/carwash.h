#ifndef CARWASH_H
#define CARWASH_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t credit_ms;
    uint32_t pause_remaining_ms;
    uint32_t last_update_ms;
    uint8_t relay_mask;
} Carwash;

void Carwash_Init(Carwash *wash, uint32_t now_ms);

/*
 * Call once per main-loop pass. Inputs are already validated: added_credit_ms
 * is new paid time, stop_pressed is a new debounced press, and requested_relays
 * is zero when no new program button was pressed. Each bit represents a relay
 * (bit 0 is relay 1). The result is the desired output mask for this pass.
 */
uint8_t Carwash_Update(Carwash *wash, uint32_t now_ms,
                       uint32_t added_credit_ms, uint8_t requested_relays,
                       bool stop_pressed, uint32_t pause_ms);

#endif
