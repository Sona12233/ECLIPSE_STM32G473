/*
 * payment_pulse.c
 *
 *  Created on: Nov 23, 2026
 *      Author: sonam
 */

#include "payment_pulse.h"
#include "main.h"

#include <stdbool.h>

/* Bench starting value. Check the fastest valid pulses from each acceptor. */
#define MIN_PULSE_GAP_MS 5U

typedef struct {
    /* The interrupt adds pulses. The main loop reads and clears this count. */
    volatile uint32_t pending_count;

    /* Only the interrupt uses these two fields. */
    uint32_t last_accepted_ms;
    bool has_accepted_pulse;

} PaymentPulseInput;

/* Static variables start at zero, so the driver needs no init function. */
static PaymentPulseInput bill_input;
static PaymentPulseInput coin_input;
static PaymentPulseInput cashless_input;
static PaymentPulseInput sense_input;

static void record_falling_edge(PaymentPulseInput* input) {
    const uint32_t now_ms = HAL_GetTick();

    /* Reject repeat edges close to the last accepted edge on this input.
       Unsigned subtraction also works when the tick crosses through zero. */
    if (input->has_accepted_pulse &&
        (now_ms - input->last_accepted_ms) < MIN_PULSE_GAP_MS)
        return;

    input->last_accepted_ms = now_ms;
    input->has_accepted_pulse = true;

    /* A full counter stays full instead of wrapping back to zero. */
    if (input->pending_count < UINT32_MAX)
        ++input->pending_count;
}

/* The STM32G4 HAL calls this after clearing the pending EXTI flag. */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    switch (pin) {
        case BILL_Pin:
            record_falling_edge(&bill_input);
            break;

        case COIN_Pin:
            record_falling_edge(&coin_input);
            break;

        case CASHLESS_Pin:
            record_falling_edge(&cashless_input);
            break;

        case SENSE_Pin:
            record_falling_edge(&sense_input);
            break;

        default:
            break;
    }
}

PaymentPulseCounts PaymentPulse_TakeCounts(void) {
    /* An interrupt must not add a pulse between copying and clearing. */
    const uint32_t saved_interrupt_mask = __get_PRIMASK();
    __disable_irq();

    const PaymentPulseCounts counts = {
        .bill = bill_input.pending_count,
        .coin = coin_input.pending_count,
        .cashless = cashless_input.pending_count,
        .sense = sense_input.pending_count
    };

    bill_input.pending_count = 0U;
    coin_input.pending_count = 0U;
    cashless_input.pending_count = 0U;
    sense_input.pending_count = 0U;

    /* Restore the interrupt state that the caller had on entry. */
    __set_PRIMASK(saved_interrupt_mask);

    return counts;
}

bool PaymentPulse_UpdateTotal(uint32_t* new_total) {
    /* Static keeps the total between calls. It starts at zero on reset. */
    static uint32_t payment_total = 0U;

    const uint32_t amount_per_pulse = 100U;

    /* Read and clear the four pending counters in one call. */
    const PaymentPulseCounts pulses = PaymentPulse_TakeCounts();

    /* Start the addition with 64 bits so the combined count cannot wrap. */
    const uint64_t pulse_count =
        (uint64_t)pulses.bill + pulses.coin + pulses.cashless + pulses.sense;

    if (pulse_count == 0U) return false;

    /* Limit the addition to the number of whole pulses that still fit. */
    const uint32_t pulses_that_fit =
        (UINT32_MAX - payment_total) / amount_per_pulse;

    if (pulses_that_fit == 0U) return false;

    const uint32_t pulses_to_add =
        (pulse_count > pulses_that_fit) ? pulses_that_fit : (uint32_t)pulse_count;

    payment_total += pulses_to_add * amount_per_pulse;
    *new_total = payment_total;
    return true;
}
