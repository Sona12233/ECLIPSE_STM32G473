#include "carwash.h"

void Carwash_Init(Carwash *wash, uint32_t now_ms)
{
    *wash = (Carwash){.last_update_ms = now_ms};
}

uint8_t Carwash_Update(Carwash *wash, uint32_t now_ms,
                       uint32_t added_credit_ms, uint8_t requested_relays,
                       bool stop_pressed, uint32_t pause_ms)
{
    /* Unsigned subtraction keeps elapsed time correct across tick rollover. */
    const uint32_t elapsed_ms = now_ms - wash->last_update_ms;
    wash->last_update_ms = now_ms;

    /* Charge the elapsed interval to the program that was already running. */
    if (wash->relay_mask != 0U) {
        if (elapsed_ms >= wash->credit_ms) {
            wash->credit_ms = 0U;
            wash->relay_mask = 0U;
        } else {
            wash->credit_ms -= elapsed_ms;
        }
    }

    if (elapsed_ms >= wash->pause_remaining_ms) {
        wash->pause_remaining_ms = 0U;
    } else {
        wash->pause_remaining_ms -= elapsed_ms;
    }

    /* Cap the sum so that a large credit cannot wrap around to zero. */
    const uint32_t credit_space_ms = UINT32_MAX - wash->credit_ms;
    wash->credit_ms += (added_credit_ms < credit_space_ms)
                       ? added_credit_ms : credit_space_ms;

    /* Stop takes priority over a simultaneous program button press. */
    if (stop_pressed) {
        if (wash->relay_mask != 0U) {
            wash->pause_remaining_ms = pause_ms;
        }
        wash->relay_mask = 0U;
    } else if (requested_relays != 0U && wash->credit_ms != 0U
               && wash->pause_remaining_ms == 0U) {
        wash->relay_mask = requested_relays;
    }

    return wash->relay_mask;
}
