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


/*
 * The parts of the shared defined registers address with AP and M0,
 * let's note and mark the previous defines like this:
 */
#define GIC500_BASE                (MMIO_BASE + 0x06E00000)
#define UART0_BASE                 (MMIO_BASE + 0x07180000)
#define UART1_BASE                 (MMIO_BASE + 0x07190000)
// #define UART2_BASE                 (MMIO_BASE + 0x071A0000)
#define UART3_BASE                 (MMIO_BASE + 0x071B0000)

// #define PMU_BASE                   (MMIO_BASE + 0x07310000)
// #define PMUGRF_BASE                (MMIO_BASE + 0x07320000)
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

/**************************************************
 * pmugrf reg, offset
 **************************************************/
#define PMUGRF_OSREG(n)            (0x300 + (n) * 4)
#define PMUGRF_GPIO0A_P            0x040
#define PMUGRF_GPIO1A_P            0x050

#define CIC_CTRL0                  0x0
#define CIC_CTRL1                  0x4
#define CIC_IDLE_TH                0x8
#define CIC_CG_WAIT_TH             0xc
#define CIC_STATUS0                0x10
#define CIC_STATUS1                0x14
#define CIC_CTRL2                  0x18
#define CIC_CTRL3                  0x1c
#define CIC_CTRL4                  0x20

#define PMU_WKUP_CFG0              0x00
#define PMU_WKUP_CFG1              0x04
#define PMU_WKUP_CFG2              0x08
#define PMU_WKUP_CFG3              0x0c
#define PMU_WKUP_CFG4              0x10
#define PMU_PWRDN_CON              0x14
#define PMU_PWRDN_ST               0x18
#define PMU_PLL_CON                0x1c
#define PMU_PWRMODE_CON            0x20
#define PMU_SFT_CON                0x24
#define PMU_INT_CON                0x28
#define PMU_INT_ST                 0x2c
#define PMU_GPIO0_POS_INT_CON      0x30
#define PMU_GPIO0_NEG_INT_CON      0x34
#define PMU_GPIO1_POS_INT_CON      0x38
#define PMU_GPIO1_NEG_INT_CON      0x3c
#define PMU_GPIO0_POS_INT_ST       0x40
#define PMU_GPIO0_NEG_INT_ST       0x44
#define PMU_GPIO1_POS_INT_ST       0x48
#define PMU_GPIO1_NEG_INT_ST       0x4c
#define PMU_PWRDN_INTEN            0x50
#define PMU_PWRDN_STATUS           0x54
#define PMU_WAKEUP_STATUS          0x58
#define PMU_BUS_CLR                0x5c
#define PMU_BUS_IDLE_REQ           0x60
#define PMU_BUS_IDLE_ST            0x64
#define PMU_BUS_IDLE_ACK           0x68
#define PMU_CCI500_CON             0x6c
#define PMU_ADB400_CON             0x70
#define PMU_ADB400_ST              0x74
#define PMU_POWER_ST               0x78
#define PMU_CORE_PWR_ST            0x7c
#define PMU_OSC_CNT                0x80
#define PMU_PLLLOCK_CNT            0x84
#define PMU_PLLRST_CNT             0x88
#define PMU_STABLE_CNT             0x8c
#define PMU_DDRIO_PWRON_CNT        0x90
#define PMU_WAKEUP_RST_CLR_CNT     0x94
#define PMU_DDR_SREF_ST            0x98
#define PMU_SCU_L_PWRDN_CNT        0x9c
#define PMU_SCU_L_PWRUP_CNT        0xa0
#define PMU_SCU_B_PWRDN_CNT        0xa4
#define PMU_SCU_B_PWRUP_CNT        0xa8
#define PMU_GPU_PWRDN_CNT          0xac
#define PMU_GPU_PWRUP_CNT          0xb0
#define PMU_CENTER_PWRDN_CNT       0xb4
#define PMU_CENTER_PWRUP_CNT       0xb8
#define PMU_TIMEOUT_CNT            0xbc
#define PMU_CPU0APM_CON            0xc0
#define PMU_CPU1APM_CON            0xc4
#define PMU_CPU2APM_CON            0xc8
#define PMU_CPU3APM_CON            0xcc
#define PMU_CPU0BPM_CON            0xd0
#define PMU_CPU1BPM_CON            0xd4
#define PMU_NOC_AUTO_ENA           0xd8
#define PMU_PWRDN_CON1             0xdc


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

/* Compute the number of elements in the given array */
#define ARRAY_SIZE(a)          (sizeof(a) / sizeof((a)[0]))
#define MPIDR_AFFLVL2          ULL(0x2)


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

    static inline void
    mmio_write_64(uintptr_t addr, uint64_t value)
    {
        *(volatile uint64_t *)addr = value;
    }

    static inline uint64_t
    mmio_read_64(uintptr_t addr)
    {
        return *(volatile uint64_t *)addr;
    }

    static inline void
    mmio_clrbits_32(uintptr_t addr, uint32_t clear)
    {
        mmio_write_32(addr, mmio_read_32(addr) & ~clear);
    }

    static inline void
    mmio_setbits_32(uintptr_t addr, uint32_t set)
    {
        mmio_write_32(addr, mmio_read_32(addr) | set);
    }


    static inline void
    mmio_clrsetbits_32(uintptr_t addr, uint32_t clear, uint32_t set)
    {
        mmio_write_32(addr, (mmio_read_32(addr) & ~clear) | set);
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
            u32 exit_low_power_status = exit_low_power();
            printLowPowerStatus(exit_low_power_status);
            // u32 base_addr_data = mmio_read_32(base_addr);
            // printf("Base address data: %u\n", base_addr_data);
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
        static constexpr uint64_t MAP_SIZE        = 4096UL;
        static constexpr uint64_t MAP_MASK        = MAP_SIZE - 1;
        static constexpr u16      CTL_REG_NUM     = 332;
        static constexpr u8       PI_REG_NUM      = 200;
        static constexpr u32      MMIO_BASE       = 0xF8000000;
        static constexpr u32      CRU_BASE        = (MMIO_BASE + 0x07760000);
        static constexpr u32      UART2_BASE      = (MMIO_BASE + 0x071A0000);
        static constexpr u32      PMU_BASE        = (MMIO_BASE + 0x07310000);
        static constexpr u32      PMUGRF_BASE     = (MMIO_BASE + 0x07320000);
        static constexpr u32      SRAM_TEXT_LIMIT = (4 * 1024);
        static constexpr u32      SRAM_DATA_LIMIT = (4 * 1024);
        static constexpr u32      SRAM_BIN_LIMIT  = (4 * 1024);

        int16_t
        convertToTemperature(int16_t adc_value)
        {
            return adc_value * 5 / 1024 - 40;
        }

        void
        printLowPowerStatus(u32 status)
        {
            printf("Low Power Status:\n");
            printf("Channel 1:\n");
            printf("External Self-Refresh: %s\n", (status & (1 << 12)) ? "Enabled" : "Disabled");
            printf("Stdby Mode: %s\n", (status & (1 << 11)) ? "Enabled" : "Disabled");
            printf("Self-Refresh with Controller and Memory Clock Gate: %s\n",
                   (status & (1 << 10)) ? "Enabled" : "Disabled");
            printf("Self-Refresh: %s\n", (status & (1 << 9)) ? "Enabled" : "Disabled");
            printf("Power-Down: %s\n", (status & (1 << 8)) ? "Enabled" : "Disabled");
            printf("Channel 0:\n");
            printf("External Self-Refresh: %s\n", (status & (1 << 4)) ? "Enabled" : "Disabled");
            printf("Stdby Mode: %s\n", (status & (1 << 3)) ? "Enabled" : "Disabled");
            printf("Self-Refresh with Controller and Memory Clock Gate: %s\n",
                   (status & (1 << 2)) ? "Enabled" : "Disabled");
            printf("Self-Refresh: %s\n", (status & (1 << 1)) ? "Enabled" : "Disabled");
            printf("Power-Down: %s\n", (status & (1 << 0)) ? "Enabled" : "Disabled");
        }

        /*
         * return: bit12: channel 1, external self-refresh
         *         bit11: channel 1, stdby_mode
         *         bit10: channel 1, self-refresh with controller and memory clock gate
         *         bit9: channel 1, self-refresh
         *         bit8: channel 1, power-down
         *
         *         bit4: channel 1, external self-refresh
         *         bit3: channel 0, stdby_mode
         *         bit2: channel 0, self-refresh with controller and memory clock gate
         *         bit1: channel 0, self-refresh
         *         bit0: channel 0, power-down
         */
        uint32_t
        exit_low_power()
        {
            uint32_t low_power = 0;
            uint32_t channel_mask;
            uint32_t tmp, i;

            channel_mask = (mmio_read_32(PMUGRF_BASE + PMUGRF_OSREG(2)) >> 28) & 0x3;
            for (i = 0; i < 2; i++)
            {
                if (!(channel_mask & (1 << i)))
                {
                    continue;
                }

                //
                //  exit stdby mode
                //
                mmio_write_32(CIC_BASE + CIC_CTRL1, (1 << (i + 16)) | (0 << i));
                //
                //  exit external self-refresh
                //
                tmp = i ? 12 : 8;
                low_power |= ((mmio_read_32(PMU_BASE + PMU_SFT_CON) >> tmp) & 0x1) << (4 + 8 * i);
                mmio_clrbits_32(PMU_BASE + PMU_SFT_CON, 1 << tmp);
                while (!(mmio_read_32(PMU_BASE + PMU_DDR_SREF_ST) & (1 << i)))
                    ;
                //
                //  exit auto low-power
                //
                mmio_clrbits_32(CTL_REG(i, 101), 0x7);
                //
                //  lp_cmd to exit
                //
                if (((mmio_read_32(CTL_REG(i, 100)) >> 24) & 0x7f) != 0x40)
                {
                    while (mmio_read_32(CTL_REG(i, 200)) & 0x1)
                        ;
                    mmio_clrsetbits_32(CTL_REG(i, 93), 0xffu << 24, 0x69 << 24);
                    while (((mmio_read_32(CTL_REG(i, 100)) >> 24) & 0x7f) != 0x40)
                        ;
                }
            }
            return low_power;
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

        struct lat_adj_pair
        {
            uint32_t cl;
            uint32_t rdlat_adj;
            uint32_t cwl;
            uint32_t wrlat_adj;
        };

        enum
        {
            DDR3   = 3,
            LPDDR2 = 5,
            LPDDR3 = 6,
            LPDDR4 = 7,
            FAIL   = 0xff
        };

        static constexpr struct lat_adj_pair lpddr4_lat_adj[] = {
            { 6,    5,    4,    2},
            {10,    9,    6,    4},
            {14,  0xc,    8,    6},
            {20, 0x11,  0xa,    8},
            {24, 0x15,  0xc,  0xa},
            {28, 0x18,  0xe,  0xc},
            {32, 0x1b, 0x10,  0xe},
            {36, 0x1e, 0x12, 0x10}
        };

        static u32
        get_rdlat_adj(u32 dram_type, u32 cl)
        {
            const lat_adj_pair *p;

            u32 cnt;
            u32 i;

            p   = lpddr4_lat_adj;
            cnt = ARRAY_SIZE(lpddr4_lat_adj);

            for (i = 0; i < cnt; i++)
            {
                if (cl == p[i].cl)
                {
                    return p[i].rdlat_adj;
                }
            }
            /* fail */
            return FAIL;
        }
    };

} // namespace Mlib::Arm
