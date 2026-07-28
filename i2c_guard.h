/*
 * i2c_guard.h — attente I2C avec timeout, partage entre main.c et ssd1306.c
 * Evite le gel silencieux si un esclave reste bloque (lecon du projet proximite).
 */
#ifndef I2C_GUARD_H
#define I2C_GUARD_H

#include <stdbool.h>
#include <stdint.h>
#include "mcc_generated_files/i2c_host/i2c1.h"

static inline bool I2C_WaitIdle(void)
{
    uint32_t t = 400000UL;              /* quelques ms a 100 MIPS */
    while (I2C1_IsBusy() && --t) { }
    return (t != 0U);
}

#endif /* I2C_GUARD_H */