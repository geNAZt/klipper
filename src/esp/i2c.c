// I2C functions on ESP.
//
// Copyright (C) 2021,2022 Fabian Faßbender <fabian.fassbender42@googlemail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include "driver/i2c.h"

#include "gpio.h"
#include "command.h" // shutdown / DECL_ENUMERATION_RANGE
#include "sched.h" // sched_shutdown

DECL_ENUMERATION_RANGE("i2c_bus", "i2c", 0, SOC_I2C_NUM - 1);

typedef volatile struct {
    uint8_t initialized;
    uint32_t frequency;
} i2c_bus_status_t;

typedef struct {
    uint8_t scl;
    uint8_t sda;
} i2c_bus_information_t;

static i2c_bus_information_t bus_info[SOC_I2C_NUM] = {
    {22,21},
#if SOC_I2C_NUM > 1
    {27,26},
#endif
};

static i2c_bus_status_t bus_status[SOC_I2C_NUM];

void
_i2c_init(uint8_t bus, uint32_t frequency)
{
    i2c_config_t conf = { };
    conf.mode = I2C_MODE_MASTER;
    conf.scl_io_num = (gpio_num_t) bus_info[bus].scl;
    conf.sda_io_num = (gpio_num_t) bus_info[bus].sda;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = frequency;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    esp_err_t ret = i2c_param_config((i2c_port_t) bus, &conf);
    if (ret != ESP_OK) {
        shutdown("i2c config setup failed");
    } else {
        ret = i2c_driver_install((i2c_port_t) bus, conf.mode, 0, 0, 0);
        if (ret != ESP_OK) {
            shutdown("i2c driver setup failed");
        } else {
            // Store that the bus has been initialized
            bus_status[bus].initialized = 1;
            i2c_set_timeout((i2c_port_t) bus, 0xFFFF);
        }
    }
}

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    if (bus >= SOC_I2C_NUM)
    {
        shutdown("i2c bus not present");
    }

    if (bus_status[bus].initialized == 0)
    {
        _i2c_init(bus, rate);
    }

    struct i2c_config c = { .bus=bus, .addr=addr };
    return c;
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    esp_err_t err = i2c_master_write_to_device((i2c_port_t) config.bus, config.addr, write, write_len, 5 / portTICK_RATE_MS);
    if (unlikely(err != ESP_OK)) {
        goto timeout;
    }

    return;
timeout:
    shutdown("I2C write timeout");
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg, uint8_t read_len, uint8_t *read)
{
    esp_err_t err = i2c_master_write_read_device((i2c_port_t) config.bus, config.addr, reg, reg_len, read, read_len, 5 / portTICK_RATE_MS);
    if (unlikely(err != ESP_OK)) {
        goto timeout;
    }

    return;
timeout:
    shutdown("I2C read timeout");
}