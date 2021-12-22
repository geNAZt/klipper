// GPIO functions on ESP.
//
// Copyright (C) 2021,2022 Fabian Faßbender <fabian.fassbender42@googlemail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h"
#include "command.h" // shutdown
#include "gpio.h" // gpio_out_write
#include "board/irq.h" // irq_save
#include "sched.h" // sched_shutdown

/****************************************************************
 * Pin mappings
 ****************************************************************/

DECL_ENUMERATION_RANGE("pin", "gpio0", 0, GPIO_NUM_MAX - 1);

static gpio_dev_t* _gpio_dev = &GPIO;

void
_gpio_out_set(struct gpio_out g, uint8_t val)
{
    if (g.pin < 32)
    {
        if (val)
            REG_SET_BIT(GPIO_OUT_REG, g.bit);
        else
            REG_CLR_BIT(GPIO_OUT_REG, g.bit);
    } else
    {
        if (val)
            REG_SET_BIT(GPIO_OUT1_REG, g.bit);
        else
            REG_CLR_BIT(GPIO_OUT1_REG, g.bit);
    }
}

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (!GPIO_IS_VALID_OUTPUT_GPIO(pin))
        goto fail;

    uint32_t io_reg = GPIO_PIN_MUX_REG[pin];
    if (io_reg == 0) // No IO register known => pin not available
        goto fail;

    struct gpio_out g = { .pin=pin, .bit=GPIO2BIT(pin) };
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    irqstatus_t flag = irq_save();

    // This is impossible to be 0, otherwise gpio_out_setup failed and gave a gpio_out struct where is shouldn't
    uint32_t io_reg = GPIO_PIN_MUX_REG[g.pin];

    // We don't want interrupts from this pin when we change it
    gpio_intr(_gpio_dev, g.pin, 0);
    gpio_input_disable(io_reg);
    gpio_output_enable(_gpio_dev, g.pin);

    // Tell the IO muxer that this register is GPIO now
    io_mux_sel_pin_func(io_reg, 2);

    _gpio_out_set(g, val);
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    if (g.pin < 32)
    {
        REG_XOR_BIT(GPIO_OUT_REG, g.bit);
    } else
    {
        REG_XOR_BIT(GPIO_OUT1_REG, g.bit);
    }
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    irqstatus_t flag = irq_save();
    _gpio_out_set(g, val);
    irq_restore(flag);
}

struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (!GPIO_IS_VALID_GPIO(pin))
        goto fail;

    uint32_t io_reg = GPIO_PIN_MUX_REG[pin];
    if (io_reg == 0) // No IO register known => pin not available
        goto fail;

    struct gpio_in g = { .pin=pin, .bit=GPIO2BIT(pin) };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not a valid input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    irqstatus_t flag = irq_save();

    // This is impossible to be 0, otherwise gpio_in_setup failed and gave a gpio_in struct where is shouldn't
    uint32_t io_reg = GPIO_PIN_MUX_REG[g.pin];

    // We don't want interrupts from this pin when we change it
    gpio_intr(_gpio_dev, g.pin, 0);

    gpio_output_disable(_gpio_dev, g.pin);
    gpio_input_enable(io_reg);

    // Tell the IO muxer that this register is GPIO now
    io_mux_sel_pin_func(io_reg, 2);

    if (pull_up)
    {
        REG_RW_SET_MASK(GPIO_PIN_MUX_REG[g.pin], PULLUP_CTRL);
        if (!REG_GET_BIT(GPIO_PIN_MUX_REG[g.pin], PULLUP_CTRL)) // There are pins which don't have a pullup circuit
            goto fail;
    } else
    {
        REG_RW_CLR_MASK(GPIO_PIN_MUX_REG[g.pin], PULLUP_CTRL);
    }

    irq_restore(flag);
    return;

fail:
    shutdown("Not a valid input pin - pullup circuit missing");
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    if (g.pin < 32)
    {
        return !!(REG_GET_BIT(GPIO_IN_REG, g.bit));
    }

    return !!(REG_GET_BIT(GPIO_IN1_REG, g.bit));
}
