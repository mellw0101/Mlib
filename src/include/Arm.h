#pragma once

#include "def.h"

#include <cstdint>
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

#define RK3399_T_BASE_ADDR         MMIO_BASE
#define RK3399_T_USER_CON          (RK3399_T_BASE_ADDR + 0x0000)
#define RK3399_T_AUTO_CON          (RK3399_T_BASE_ADDR + 0x0004)
#define RK3399_T_DATA0             (RK3399_T_BASE_ADDR + 0x0020)

#define RK3399_T_ENABLE            0x01
#define RK3399_T_CHANNEL_SELECT    0x00 // Assuming channel 0 for simplicity
#define RK3399_T_CLOCK_DIV         0x00000003

#define SIZE_K(n)                  ((n) * 1024)
#define SIZE_M(n)                  ((n) * 1024 * 1024)
#define SRAM_TEXT_LIMIT            (4 * 1024)
#define SRAM_DATA_LIMIT            (4 * 1024)
#define SRAM_BIN_LIMIT             (4 * 1024)

/*
 * The parts of the shared defined registers address with AP and M0,
 * let's note and mark the previous defines like this:
 */
#define GIC500_BASE                (MMIO_BASE + 0x06E00000)
#define UART0_BASE                 (MMIO_BASE + 0x07180000)
#define UART1_BASE                 (MMIO_BASE + 0x07190000)
#define UART2_BASE                 (MMIO_BASE + 0x071A0000)
#define UART3_BASE                 (MMIO_BASE + 0x071B0000)

#define PMU_BASE                   (MMIO_BASE + 0x07310000)
#define PMUGRF_BASE                (MMIO_BASE + 0x07320000)
#define SGRF_BASE                  (MMIO_BASE + 0x07330000)
#define PMUSRAM_BASE               (MMIO_BASE + 0x073B0000)
#define PWM_BASE                   (MMIO_BASE + 0x07420000)

#define CIC_BASE                   (MMIO_BASE + 0x07620000)
#define PD_BUS0_BASE               (MMIO_BASE + 0x07650000)
#define DCF_BASE                   (MMIO_BASE + 0x076A0000)
#define GPIO0_BASE                 (MMIO_BASE + 0x07720000)
#define GPIO1_BASE                 (MMIO_BASE + 0x07730000)
#define PMUCRU_BASE                (MMIO_BASE + 0x07750000)
#define CRU_BASE                   (MMIO_BASE + 0x07760000)
#define GRF_BASE                   (MMIO_BASE + 0x07770000)
#define GPIO2_BASE                 (MMIO_BASE + 0x07780000)
#define GPIO3_BASE                 (MMIO_BASE + 0x07788000)
#define GPIO4_BASE                 (MMIO_BASE + 0x07790000)
#define WDT1_BASE                  (MMIO_BASE + 0x07840000)
#define WDT0_BASE                  (MMIO_BASE + 0x07848000)
#define TIMER_BASE                 (MMIO_BASE + 0x07850000)
#define STIME_BASE                 (MMIO_BASE + 0x07860000)
#define SRAM_BASE                  (MMIO_BASE + 0x078C0000)
#define SERVICE_NOC_0_BASE         (MMIO_BASE + 0x07A50000)
#define DDRC0_BASE                 (MMIO_BASE + 0x07A80000)
#define SERVICE_NOC_1_BASE         (MMIO_BASE + 0x07A84000)
#define DDRC1_BASE                 (MMIO_BASE + 0x07A88000)
#define SERVICE_NOC_2_BASE         (MMIO_BASE + 0x07A8C000)
#define SERVICE_NOC_3_BASE         (MMIO_BASE + 0x07A90000)
#define CCI500_BASE                (MMIO_BASE + 0x07B00000)
#define COLD_BOOT_BASE             (MMIO_BASE + 0x07FF0000)

/* Registers size */
#define GIC500_SIZE                SIZE_M(2)
#define UART0_SIZE                 SIZE_K(64)
#define UART1_SIZE                 SIZE_K(64)
#define UART2_SIZE                 SIZE_K(64)
#define UART3_SIZE                 SIZE_K(64)
#define PMU_SIZE                   SIZE_K(64)
#define PMUGRF_SIZE                SIZE_K(64)
#define SGRF_SIZE                  SIZE_K(64)
#define PMUSRAM_SIZE               SIZE_K(64)
#define PMUSRAM_RSIZE              SIZE_K(8)
#define PWM_SIZE                   SIZE_K(64)
#define CIC_SIZE                   SIZE_K(4)
#define DCF_SIZE                   SIZE_K(4)
#define GPIO0_SIZE                 SIZE_K(64)
#define GPIO1_SIZE                 SIZE_K(64)
#define PMUCRU_SIZE                SIZE_K(64)
#define CRU_SIZE                   SIZE_K(64)
#define GRF_SIZE                   SIZE_K(64)
#define GPIO2_SIZE                 SIZE_K(32)
#define GPIO3_SIZE                 SIZE_K(32)
#define GPIO4_SIZE                 SIZE_K(32)
#define STIME_SIZE                 SIZE_K(64)
#define SRAM_SIZE                  SIZE_K(192)
#define SERVICE_NOC_0_SIZE         SIZE_K(192)
#define DDRC0_SIZE                 SIZE_K(32)
#define SERVICE_NOC_1_SIZE         SIZE_K(16)
#define DDRC1_SIZE                 SIZE_K(32)
#define SERVICE_NOC_2_SIZE         SIZE_K(16)
#define SERVICE_NOC_3_SIZE         SIZE_K(448)
#define CCI500_SIZE                SIZE_M(1)
#define PD_BUS0_SIZE               SIZE_K(448)

/* DDR Registers address */
#define CTL_BASE(ch)               (DDRC0_BASE + (ch) * 0x8000)
#define CTL_REG(ch, n)             (CTL_BASE(ch) + (n) * 0x4)

#define PI_OFFSET                  0x800
#define PI_BASE(ch)                (CTL_BASE(ch) + PI_OFFSET)
#define PI_REG(ch, n)              (PI_BASE(ch) + (n) * 0x4)

#define PHY_OFFSET                 0x2000
#define PHY_BASE(ch)               (CTL_BASE(ch) + PHY_OFFSET)
#define PHY_REG(ch, n)             (PHY_BASE(ch) + (n) * 0x4)

#define MSCH_BASE(ch)              (SERVICE_NOC_1_BASE + (ch) * 0x8000)


// TSADC base address and register offsets
#define TSADC_BASE_ADDR            (MMIO_BASE + 0x0E600)
#define TSADC_USER_CON             (TSADC_BASE_ADDR + 0x0000)
#define TSADC_AUTO_CON             (TSADC_BASE_ADDR + 0x0004)
#define TSADC_INT_EN               (TSADC_BASE_ADDR + 0x0008)
#define TSADC_INT_PD               (TSADC_BASE_ADDR + 0x000c)
#define TSADC_DATA0                (TSADC_BASE_ADDR + 0x0020)
#define TSADC_DATA1                (TSADC_BASE_ADDR + 0x0024)
#define TSADC_COMP0_INT            (TSADC_BASE_ADDR + 0x0030)
#define TSADC_COMP1_INT            (TSADC_BASE_ADDR + 0x0034)
#define TSADC_COMP0_SHUT           (TSADC_BASE_ADDR + 0x0040)
#define TSADC_COMP1_SHUT           (TSADC_BASE_ADDR + 0x0044)
#define TSADC_HIGHT_INT_DEBOUNCE   (TSADC_BASE_ADDR + 0x0060)
#define TSADC_HIGHT_TSHUT_DEBOUNCE (TSADC_BASE_ADDR + 0x0064)
#define TSADC_AUTO_PERIOD          (TSADC_BASE_ADDR + 0x0068)
#define TSADC_AUTO_PERIOD_HT       (TSADC_BASE_ADDR + 0x006c)
#define TSADC_COMP0_LOW_INT        (TSADC_BASE_ADDR + 0x0080)
#define TSADC_COMP1_LOW_INT        (TSADC_BASE_ADDR + 0x0084)

// TSADC enable and clock divider
#define TSADC_ENABLE               0x00000200
#define TSADC_CLOCK_DIV            0x00000003

namespace Mlib::Arm
{
/* Registers base address */
#define MMIO_BASE              0xF8000000

#define CRU_PLL_CON(pll_id, n) ((pll_id) * 0x20 + (n) * 4)
#define CRU_BASE               (MMIO_BASE + 0x07760000)

    enum plls_id
    {
        ALPLL_ID = 0,
        ABPLL_ID,
        DPLL_ID,
        CPLL_ID,
        GPLL_ID,
        NPLL_ID,
        VPLL_ID,
        PPLL_ID,
        END_PLL_ID,
    };

    uintptr_t get_base_mem_addr(uintptr_t physical_addr);

    static inline uint32_t
    mmio_read_32(uintptr_t addr)
    {
        return *(volatile uint32_t *)addr;
    }

    uint32_t ddr_get_rate();


    class RK3399_T
    {
    public:
        RK3399_T()
        {
            base_addr = mapMemory(MMIO_BASE);
            //
            //  Set interleave and start conversion.
            //
            // writeRegister(RK3399_T_USER_CON, 0x00000820);
        }

        ~RK3399_T()
        {
            if (reinterpret_cast<void *>(base_addr) != MAP_FAILED)
            {
                s32 result = munmap(reinterpret_cast<void *>(base_addr), MAP_SIZE);
                if (result == -1)
                {
                    printf("Failed to unmap memory\n");
                }
            }
        }

        int16_t
        readTemperature()
        {
            // writeRegister(RK3399_T_USER_CON, readRegister(RK3399_T_USER_CON) | (1 << 5)); // Start conversion
            // while (readRegister(RK3399_T_USER_CON) & (1 << 12))
            // {
            //     // Wait for conversion to complete
            //     ;
            // }
            // // Read 12-bit ADC value
            // int16_t adc_value   = readRegister(RK3399_T_DATA0) & 0xFFF;
            // int16_t temperature = convertToTemperature(adc_value);
            // return temperature;
            return 0;
        }

    private:
        uintptr_t                 base_addr;
        static constexpr uint64_t MAP_SIZE = 4096UL;
        static constexpr uint64_t MAP_MASK = MAP_SIZE - 1;

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
                printf("Failed to map memory\n");
                return reinterpret_cast<uintptr_t>(MAP_FAILED);
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

    class TSADC
    {
    public:
        TSADC()
        {
            writeRegister(TSADC_USER_CON, TSADC_ENABLE);
            writeRegister(TSADC_AUTO_CON, TSADC_CLOCK_DIV);
        }

        int16_t
        readTemperature()
        {
            int16_t adc_value   = readRegister(TSADC_DATA0);
            int16_t temperature = convertToTemperature(adc_value);
            return temperature;
        }

    private:
        void
        writeRegister(uint32_t reg, uint32_t value)
        {
            *(volatile uint32_t *)(uintptr_t)reg = value;
        }

        uint32_t
        readRegister(uint32_t reg)
        {
            return *(volatile uint32_t *)(uintptr_t)reg;
        }

        int16_t
        convertToTemperature(int16_t adc_value)
        {
            return adc_value * 5 / 1024 - 40;
        }
    };


} // namespace Mlib::Arm
