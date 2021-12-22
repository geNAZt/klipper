#ifndef __ESP_IO_MUX_H
#define __ESP_IO_MUX_H

#include "internal/gpio.h"
#include <stdint.h>

/*
 * All ESP have a IO muxer to redirect different functions to the hardware PINs. This allows all pins to be reused and remapped
 * depending on what function is needed. We only use GPIO functions though
 */

#define IO_MUX_BASE_REG                0x3ff49000
#define IO_MUX_GPIO0_REG               (IO_MUX_BASE_REG + 0x44)
#define IO_MUX_GPIO1_REG               (IO_MUX_BASE_REG + 0x88)
#define IO_MUX_GPIO2_REG               (IO_MUX_BASE_REG + 0x40)
#define IO_MUX_GPIO3_REG               (IO_MUX_BASE_REG + 0x84)
#define IO_MUX_GPIO4_REG               (IO_MUX_BASE_REG + 0x48)
#define IO_MUX_GPIO5_REG               (IO_MUX_BASE_REG + 0x6C)
#define IO_MUX_GPIO6_REG               (IO_MUX_BASE_REG + 0x60)
#define IO_MUX_GPIO7_REG               (IO_MUX_BASE_REG + 0x64)
#define IO_MUX_GPIO8_REG               (IO_MUX_BASE_REG + 0x68)
#define IO_MUX_GPIO9_REG               (IO_MUX_BASE_REG + 0x54)
#define IO_MUX_GPIO10_REG              (IO_MUX_BASE_REG + 0x58)
#define IO_MUX_GPIO11_REG              (IO_MUX_BASE_REG + 0x5C)
#define IO_MUX_GPIO12_REG              (IO_MUX_BASE_REG + 0x34)
#define IO_MUX_GPIO13_REG              (IO_MUX_BASE_REG + 0x38)
#define IO_MUX_GPIO14_REG              (IO_MUX_BASE_REG + 0x30)
#define IO_MUX_GPIO15_REG              (IO_MUX_BASE_REG + 0x3C)
#define IO_MUX_GPIO16_REG              (IO_MUX_BASE_REG + 0x4C)
#define IO_MUX_GPIO17_REG              (IO_MUX_BASE_REG + 0x50)
#define IO_MUX_GPIO18_REG              (IO_MUX_BASE_REG + 0x70)
#define IO_MUX_GPIO19_REG              (IO_MUX_BASE_REG + 0x74)
#define IO_MUX_GPIO20_REG              (IO_MUX_BASE_REG + 0x78)
#define IO_MUX_GPIO21_REG              (IO_MUX_BASE_REG + 0x7C)
#define IO_MUX_GPIO22_REG              (IO_MUX_BASE_REG + 0x80)
#define IO_MUX_GPIO23_REG              (IO_MUX_BASE_REG + 0x8C)
#define IO_MUX_GPIO24_REG              (IO_MUX_BASE_REG + 0x90)
#define IO_MUX_GPIO25_REG              (IO_MUX_BASE_REG + 0x24)
#define IO_MUX_GPIO26_REG              (IO_MUX_BASE_REG + 0x28)
#define IO_MUX_GPIO27_REG              (IO_MUX_BASE_REG + 0x2C)
#define IO_MUX_GPIO32_REG              (IO_MUX_BASE_REG + 0x1C)
#define IO_MUX_GPIO33_REG              (IO_MUX_BASE_REG + 0x20)
#define IO_MUX_GPIO34_REG              (IO_MUX_BASE_REG + 0x14)
#define IO_MUX_GPIO35_REG              (IO_MUX_BASE_REG + 0x18)
#define IO_MUX_GPIO36_REG              (IO_MUX_BASE_REG + 0x04)
#define IO_MUX_GPIO37_REG              (IO_MUX_BASE_REG + 0x08)
#define IO_MUX_GPIO38_REG              (IO_MUX_BASE_REG + 0x0C)
#define IO_MUX_GPIO39_REG              (IO_MUX_BASE_REG + 0x10)

extern const uint32_t GPIO_PIN_MUX_REG[GPIO_NUM_MAX];

#define MCU_SEL_V 0x7
#define MCU_SEL_S 12

void io_mux_sel_pin_func(uint32_t pin, uint32_t func);

#endif // io_mux.h