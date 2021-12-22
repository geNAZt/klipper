#include "internal.h"
#include <stdint.h>

void gpio_input_disable(uint32_t io_register) 
{
    REG_CLR_BIT(io_register, BIT9);
}

void gpio_input_enable(uint32_t io_register) 
{
    REG_SET_BIT(io_register, BIT9);
}

void gpio_output_enable(gpio_dev_t* dev, uint8_t pin)
{
    if (pin < 32) 
    {
        dev->enable_w1ts = (0x1 << pin);
    } else 
    {
        MODIFY_U32_REG_FIELD(hw->enable1_w1ts, data, (0x1 << (pin - 32)));
    }
}

void gpio_output_disable(gpio_dev_t* dev, uint8_t pin)
{
    if (pin < 32) 
    {
        hw->enable_w1tc = (0x1 << pin);
    } else 
    {
        MODIFY_U32_REG_FIELD(hw->enable1_w1tc, data, (0x1 << (pin - 32)));
    }

    // Ensure no other output signal is routed via GPIO matrix to this pin
    REG_WRITE(GPIO_FUNC0_OUT_SEL_CFG_REG + (gpio_num * 4), 128);
}

void gpio_intr(gpio_dev_t* dev, uint8_t pin, uint32_t intr)
{
    dev->pin[pin].int_type = intr;
}