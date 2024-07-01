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
// #define CRU_BASE                   (MMIO_BASE + 0x07760000)
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

#define assert(e)                  ((void)0)

namespace Mlib::Arm
{

#define CRU_PLL_CON(pll_id, n) ((pll_id) * 0x20 + (n) * 4)

#define GET_GPIO_PORT(pin)     (pin / 32)
#define GET_GPIO_NUM(pin)      (pin % 32)
#define CRU_PMU_CLKGATE_CON(n) (0x100 + n * 4)

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

    static inline u32
    mmio_read_32(uintptr_t addr)
    {
        return *(volatile u32 *)addr;
    }

    static inline void
    mmio_write_32(uintptr_t addr, u32 value)
    {
        *(volatile u32 *)addr = value;
    }

    static inline uintptr_t
    mapMemory(uintptr_t base_addr, u64 MAP_SIZE, u64 MAP_MASK)
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

    uint32_t ddr_get_rate();

    class RK3399_T
    {
    public:
        RK3399_T()
        {
            base_addr = mapMemory(MMIO_BASE, MAP_SIZE, MAP_MASK);
            printf("Base address: %p\n", reinterpret_cast<void *>(base_addr));
            u32 base_addr_data = mmio_read_32(base_addr);
            printf("Base address data: %u\n", base_addr_data);
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
        static constexpr uint64_t MAP_SIZE    = 4096UL;
        static constexpr uint64_t MAP_MASK    = MAP_SIZE - 1;
        static constexpr u16      CTL_REG_NUM = 332;
        static constexpr u8       PI_REG_NUM  = 200;
        static constexpr u32      MMIO_BASE   = 0xF8000000;
        static constexpr u32      CRU_BASE    = (MMIO_BASE + 0x07760000);

        int16_t
        convertToTemperature(int16_t adc_value)
        {
            return adc_value * 5 / 1024 - 40;
        }

        struct rk3399_ddr_pctl_regs
        {
            u32 denali_ctl[CTL_REG_NUM];
        };

        struct rk3399_ddr_publ_regs
        {
            /*
             * PHY registers from 0 to 90 for slice1.
             * These are used to restore slice1-4 on resume.
             */
            uint32_t phy0[91];
            /*
             * PHY registers from 512 to 895.
             * Only registers 0-37 of each 128 register range are used.
             */
            uint32_t phy512[3][38];
            uint32_t phy896[63];
        };

        struct rk3399_ddr_pi_regs
        {
            uint32_t denali_pi[PI_REG_NUM];
        };

        union noc_ddrtiminga0
        {
            u32 d32;
            struct
            {
                u32 acttoact : 6;
                u32 reserved0 : 2;
                u32 rdtomiss : 6;
                u32 reserved1 : 2;
                u32 wrtomiss : 6;
                u32 reserved2 : 2;
                u32 readlatency : 8;
            } b;
        };

        union noc_ddrtimingb0
        {
            uint32_t d32;
            struct
            {
                unsigned rdtowr : 5;
                unsigned reserved0 : 3;
                unsigned wrtord : 5;
                unsigned reserved1 : 3;
                unsigned rrd : 4;
                unsigned reserved2 : 4;
                unsigned faw : 6;
                unsigned reserved3 : 2;
            } b;
        };

        union noc_ddrtimingc0
        {
            uint32_t d32;
            struct
            {
                unsigned burstpenalty : 4;
                unsigned reserved0 : 4;
                unsigned wrtomwr : 6;
                unsigned reserved1 : 18;
            } b;
        };

        union noc_devtodev0
        {
            uint32_t d32;
            struct
            {
                unsigned busrdtord : 3;
                unsigned reserved0 : 1;
                unsigned busrdtowr : 3;
                unsigned reserved1 : 1;
                unsigned buswrtord : 3;
                unsigned reserved2 : 1;
                unsigned buswrtowr : 3;
                unsigned reserved3 : 17;
            } b;
        };

        union noc_ddrmode
        {
            uint32_t d32;
            struct
            {
                unsigned autoprecharge : 1;
                unsigned bypassfiltering : 1;
                unsigned fawbank : 1;
                unsigned burstsize : 2;
                unsigned mwrsize : 2;
                unsigned reserved2 : 1;
                unsigned forceorder : 8;
                unsigned forceorderstate : 8;
                unsigned reserved3 : 8;
            } b;
        };

        struct rk3399_msch_timings
        {
            union noc_ddrtiminga0 ddrtiminga0;
            union noc_ddrtimingb0 ddrtimingb0;
            union noc_ddrtimingc0 ddrtimingc0;
            union noc_devtodev0   devtodev0;
            union noc_ddrmode     ddrmode;
            uint32_t              agingx0;
        };

        struct rk3399_sdram_channel
        {
            unsigned char rank;
            /* col = 0, means this channel is invalid */
            unsigned char col;
            /* 3:8bank, 2:4bank */
            unsigned char bk;
            /* channel buswidth, 2:32bit, 1:16bit, 0:8bit */
            unsigned char bw;
            /* die buswidth, 2:32bit, 1:16bit, 0:8bit */
            unsigned char dbw;
            /* row_3_4 = 1: 6Gb or 12Gb die
             * row_3_4 = 0: normal die, power of 2
             */
            unsigned char       row_3_4;
            unsigned char       cs0_row;
            unsigned char       cs1_row;
            uint32_t            ddrconfig;
            rk3399_msch_timings noc_timings;
        };

        struct rk3399_sdram_params
        {
            rk3399_sdram_channel        ch[2];
            uint32_t                    ddr_freq;
            unsigned char               dramtype;
            unsigned char               num_channels;
            unsigned char               stride;
            unsigned char               odt;
            struct rk3399_ddr_pctl_regs pctl_regs;
            struct rk3399_ddr_pi_regs   pi_regs;
            struct rk3399_ddr_publ_regs phy_regs;
            uint32_t                    rx_cal_dqs[2][4];
        };

        static u32
        get_cs_die_capability(rk3399_sdram_params *ram_config, u8 channel, u8 cs)
        {
            rk3399_sdram_channel *ch = &ram_config->ch[channel];

            u32 bandwidth;
            u32 die_bandwidth;
            u32 die;
            u32 cs_cap;
            u32 row;

            row           = cs == 0 ? ch->cs0_row : ch->cs1_row;
            bandwidth     = 8 * (1 << ch->bw);
            die_bandwidth = 8 * (1 << ch->dbw);
            die           = bandwidth / die_bandwidth;
            cs_cap        = (1 << (row + ((1 << ch->bk) / 4 + 1) + ch->col + (bandwidth / 16)));
            if (ch->row_3_4)
            {
                cs_cap = cs_cap * 3 / 4;
            }

            return (cs_cap / die);
        }
    };

} // namespace Mlib::Arm
