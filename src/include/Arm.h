#pragma once

#include "def.h"

#include <cstdint>
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

uintptr_t get_base_mem_addr(uintptr_t physical_addr);

// #define RK3399_T_BASE_ADDR      0x40038000
// #define RK3399_T_CLOCK_REG      (RK3399_T_BASE_ADDR + 0x00)
// #define RK3399_T_CTRL_REG       (RK3399_T_BASE_ADDR + 0x04)
// #define RK3399_T_DATA_REG       (RK3399_T_BASE_ADDR + 0x08)
//
// #define RK3399_T_ENABLE         0x01
// #define RK3399_T_CHANNEL_SELECT 0x00 // Assuming channel 0 for simplicity
// #define RK3399_T_CLOCK_DIV      0x00000003
//
// class RK3399_T
// {
// public:
//     RK3399_T()
//     {
//         writeRegister(RK3399_T_CLOCK_REG, RK3399_T_CLOCK_DIV);
//         writeRegister(RK3399_T_CTRL_REG, RK3399_T_ENABLE | RK3399_T_CHANNEL_SELECT);
//     }
//
//     s16
//     readTemperature()
//     {
//         s16 adc_value   = readRegister(RK3399_T_DATA_REG);
//         s16 temperature = convertToTemperature(adc_value);
//         return temperature;
//     }
//
// private:
//     void
//     writeRegister(u32 reg, u32 value)
//     {
//         *(volatile u32 *)(uintptr_t)reg = value;
//     }
//
//     u32
//     readRegister(u32 reg)
//     {
//         return *(volatile u32 *)(uintptr_t)reg;
//     }
//
//     s16
//     convertToTemperature(s16 adc_value)
//     {
//         return adc_value * 5 / 1024 - 40;
//     }
// };

#include <cstdint>
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

/* Registers base address */
#define MMIO_BASE 0xF8000000

uintptr_t get_base_mem_addr(uintptr_t physical_addr);

/* Aggregate of all devices in the first GB */
#define DEV_RNG0_BASE           MMIO_BASE
#define DEV_RNG0_SIZE           SIZE_M(125)

#define RK3399_T_BASE_ADDR      (MMIO_BASE + 0x38000)
#define RK3399_T_CLOCK_REG      (RK3399_T_BASE_ADDR + 0x00)
#define RK3399_T_CTRL_REG       (RK3399_T_BASE_ADDR + 0x04)
#define RK3399_T_DATA_REG       (RK3399_T_BASE_ADDR + 0x08)

#define RK3399_T_ENABLE         0x01
#define RK3399_T_CHANNEL_SELECT 0x00 // Assuming channel 0 for simplicity
#define RK3399_T_CLOCK_DIV      0x00000003

class RK3399_T
{
public:
    RK3399_T()
    {
        writeRegister(RK3399_T_CLOCK_REG, RK3399_T_CLOCK_DIV);
        writeRegister(RK3399_T_CTRL_REG, RK3399_T_ENABLE | RK3399_T_CHANNEL_SELECT);
    }

    int16_t
    readTemperature()
    {
        int16_t adc_value   = static_cast<int16_t>(readRegister(RK3399_T_DATA_REG));
        int16_t temperature = convertToTemperature(adc_value);
        return temperature;
    }

private:
    void
    writeRegister(uint32_t reg, uint32_t value)
    {
        *reinterpret_cast<volatile uint32_t *>(reg) = value;
    }

    uint32_t
    readRegister(uint32_t reg)
    {
        return *reinterpret_cast<volatile uint32_t *>(reg);
    }

    int16_t
    convertToTemperature(int16_t adc_value)
    {
        return adc_value * 5 / 1024 - 40;
    }
};
