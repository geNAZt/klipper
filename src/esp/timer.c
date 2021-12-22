// I2C functions on ESP.
//
// Copyright (C) 2021,2022 Fabian Faßbender <fabian.fassbender42@googlemail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include "board/misc.h" // timer_read_time
#include "internal.h"

uint32_t timer_read_time(void)
{
    uint32_t lo, hi;
    uint32_t lo_start = REG_READ(TIMER_GROUP_0_LACT_LOW);
    uint32_t div = REG_GET_FIELD(TIMER_GROUP_0_LACT_CONFIG, TIMG_LACT_DIVIDER);

    /* The peripheral doesn't have a bit to indicate that the update is done, so we poll the
     * lower 32 bit part of the counter until it changes, or a timeout expires.
     */
    REG_WRITE(TIMER_GROUP_0_LACT_UPDATE, 1);

    do {
        lo = REG_READ(TIMER_GROUP_0_LACT_LOW);
    } while (lo == lo_start && div-- > 0);

    /* Since this function is called without a critical section, verify that LO and HI
     * registers are consistent. That is, if an interrupt happens between reading LO and
     * HI registers, and esp_timer_impl_get_time is called from an ISR, then try to
     * detect this by the change in LO register value, and re-read both registers.
     */
    do {
        lo_start = lo;
        hi = REG_READ(TIMER_GROUP_0_LACT_HIGH);
        lo = REG_READ(TIMER_GROUP_0_LACT_LOW);
    } while (lo != lo_start);

    timer_64b_reg_t result = {
        .lo = lo,
        .hi = hi
    };
    return result.val;
}