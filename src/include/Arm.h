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
#define MMIO_BASE               0xF8000000

/* Aggregate of all devices in the first GB */
#define DEV_RNG0_BASE           MMIO_BASE
#define DEV_RNG0_SIZE           SIZE_M(125)

#define RK3399_T_BASE_ADDR      DEV_RNG0_BASE
#define RK3399_T_USER_CON       (RK3399_T_BASE_ADDR + 0x0000)
#define RK3399_T_AUTO_CON       (RK3399_T_BASE_ADDR + 0x0004)
#define RK3399_T_DATA0          (RK3399_T_BASE_ADDR + 0x0020)

#define RK3399_T_ENABLE         0x01
#define RK3399_T_CHANNEL_SELECT 0x00 // Assuming channel 0 for simplicity
#define RK3399_T_CLOCK_DIV      0x00000003

class RK3399_T
{
public:
    RK3399_T()
    {
        base_addr = mapMemory(DEV_RNG0_BASE);
        //
        //  Set interleave and start conversion.
        //
        writeRegister(RK3399_T_USER_CON, 0x00000820);
    }

    ~RK3399_T()
    {
        if (reinterpret_cast<void *>(base_addr) != MAP_FAILED)
        {
            munmap(reinterpret_cast<void *>(base_addr), MAP_SIZE);
        }
    }

    s16
    readTemperature()
    {
        writeRegister(RK3399_T_USER_CON, readRegister(RK3399_T_USER_CON) | (1 << 5)); // Start conversion
        while (readRegister(RK3399_T_USER_CON) & (1 << 12))
        {
            // Wait for conversion to complete
            ;
        }
        // Read 12-bit ADC value
        s16 adc_value   = readRegister(RK3399_T_DATA0) & 0xFFF;
        s16 temperature = convertToTemperature(adc_value);
        return temperature;
    }

private:
    uintptr_t            base_addr;
    static constexpr u64 MAP_SIZE = 4096UL;
    static constexpr u64 MAP_MASK = MAP_SIZE - 1;

    uintptr_t
    mapMemory(uintptr_t base_addr)
    {
        int   mem_fd;
        void *mapped_base;

        mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (mem_fd == -1)
        {
            throw std::runtime_error("Cannot open /dev/mem");
        }

        mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, base_addr & ~MAP_MASK);
        if (mapped_base == MAP_FAILED)
        {
            close(mem_fd);
            throw std::runtime_error("mmap failed");
        }

        close(mem_fd);

        return reinterpret_cast<uintptr_t>(mapped_base);
    }

    void
    writeRegister(uint32_t reg, uint32_t value)
    {
        *(volatile uint32_t *)(base_addr + (reg & MAP_MASK)) = value;
    }

    uint32_t
    readRegister(uint32_t reg)
    {
        return *(volatile uint32_t *)(base_addr + (reg & MAP_MASK));
    }

    int16_t
    convertToTemperature(int16_t adc_value)
    {
        return adc_value * 5 / 1024 - 40;
    }
};
