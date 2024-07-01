#pragma once

// #include <Arm/arch/aarch64/arch.h>
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>
// #include "def.h"

/* Compute the number of elements in the given array */
#define ARRAY_SIZE(a)                    (sizeof(a) / sizeof((a)[0]))
#define MPIDR_AFFLVL2                    ULL(0x2)


#define SIZE_K(n)                        ((n) * 1024)
#define SIZE_M(n)                        ((n) * 1024 * 1024)
#define SRAM_TEXT_LIMIT                  (4 * 1024)
#define SRAM_DATA_LIMIT                  (4 * 1024)
#define SRAM_BIN_LIMIT                   (4 * 1024)

/*
 * The parts of the shared defined registers address with AP and M0,
 * let's note and mark the previous defines like this:
 */
#define GIC500_BASE                      (MMIO_BASE + 0x06E00000)
#define UART0_BASE                       (MMIO_BASE + 0x07180000)
#define UART1_BASE                       (MMIO_BASE + 0x07190000)
#define UART2_BASE                       (MMIO_BASE + 0x071A0000)
#define UART3_BASE                       (MMIO_BASE + 0x071B0000)

#define PMU_BASE                         (MMIO_BASE + 0x07310000)
#define PMUGRF_BASE                      (MMIO_BASE + 0x07320000)
#define SGRF_BASE                        (MMIO_BASE + 0x07330000)
#define PMUSRAM_BASE                     (MMIO_BASE + 0x073B0000)
#define PWM_BASE                         (MMIO_BASE + 0x07420000)

#define CIC_BASE                         (MMIO_BASE + 0x07620000)
#define PD_BUS0_BASE                     (MMIO_BASE + 0x07650000)
#define DCF_BASE                         (MMIO_BASE + 0x076A0000)
#define GPIO0_BASE                       (MMIO_BASE + 0x07720000)
#define GPIO1_BASE                       (MMIO_BASE + 0x07730000)
#define PMUCRU_BASE                      (MMIO_BASE + 0x07750000)
#define CRU_BASE                         (MMIO_BASE + 0x07760000)
#define GRF_BASE                         (MMIO_BASE + 0x07770000)
#define GPIO2_BASE                       (MMIO_BASE + 0x07780000)
#define GPIO3_BASE                       (MMIO_BASE + 0x07788000)
#define GPIO4_BASE                       (MMIO_BASE + 0x07790000)
#define WDT1_BASE                        (MMIO_BASE + 0x07840000)
#define WDT0_BASE                        (MMIO_BASE + 0x07848000)
#define TIMER_BASE                       (MMIO_BASE + 0x07850000)
#define STIME_BASE                       (MMIO_BASE + 0x07860000)
#define SRAM_BASE                        (MMIO_BASE + 0x078C0000)
#define SERVICE_NOC_0_BASE               (MMIO_BASE + 0x07A50000)
#define DDRC0_BASE                       (MMIO_BASE + 0x07A80000)
#define SERVICE_NOC_1_BASE               (MMIO_BASE + 0x07A84000)
#define DDRC1_BASE                       (MMIO_BASE + 0x07A88000)
#define SERVICE_NOC_2_BASE               (MMIO_BASE + 0x07A8C000)
#define SERVICE_NOC_3_BASE               (MMIO_BASE + 0x07A90000)
#define CCI500_BASE                      (MMIO_BASE + 0x07B00000)
#define COLD_BOOT_BASE                   (MMIO_BASE + 0x07FF0000)

/* Registers size */
#define GIC500_SIZE                      SIZE_M(2)
#define UART0_SIZE                       SIZE_K(64)
#define UART1_SIZE                       SIZE_K(64)
#define UART2_SIZE                       SIZE_K(64)
#define UART3_SIZE                       SIZE_K(64)
#define PMU_SIZE                         SIZE_K(64)
#define PMUGRF_SIZE                      SIZE_K(64)
#define SGRF_SIZE                        SIZE_K(64)
#define PMUSRAM_SIZE                     SIZE_K(64)
#define PMUSRAM_RSIZE                    SIZE_K(8)
#define PWM_SIZE                         SIZE_K(64)
#define CIC_SIZE                         SIZE_K(4)
#define DCF_SIZE                         SIZE_K(4)
#define GPIO0_SIZE                       SIZE_K(64)
#define GPIO1_SIZE                       SIZE_K(64)
#define PMUCRU_SIZE                      SIZE_K(64)
#define CRU_SIZE                         SIZE_K(64)
#define GRF_SIZE                         SIZE_K(64)
#define GPIO2_SIZE                       SIZE_K(32)
#define GPIO3_SIZE                       SIZE_K(32)
#define GPIO4_SIZE                       SIZE_K(32)
#define STIME_SIZE                       SIZE_K(64)
#define SRAM_SIZE                        SIZE_K(192)
#define SERVICE_NOC_0_SIZE               SIZE_K(192)
#define DDRC0_SIZE                       SIZE_K(32)
#define SERVICE_NOC_1_SIZE               SIZE_K(16)
#define DDRC1_SIZE                       SIZE_K(32)
#define SERVICE_NOC_2_SIZE               SIZE_K(16)
#define SERVICE_NOC_3_SIZE               SIZE_K(448)
#define CCI500_SIZE                      SIZE_M(1)
#define PD_BUS0_SIZE                     SIZE_K(448)

/* DDR Registers address */
#define CTL_BASE(ch)                     (DDRC0_BASE + (ch) * 0x8000)
#define CTL_REG(ch, n)                   (CTL_BASE(ch) + (n) * 0x4)

#define PI_OFFSET                        0x800
#define PI_BASE(ch)                      (CTL_BASE(ch) + PI_OFFSET)
#define PI_REG(ch, n)                    (PI_BASE(ch) + (n) * 0x4)

#define PHY_OFFSET                       0x2000
#define PHY_BASE(ch)                     (CTL_BASE(ch) + PHY_OFFSET)
#define PHY_REG(ch, n)                   (PHY_BASE(ch) + (n) * 0x4)

#define MSCH_BASE(ch)                    (SERVICE_NOC_1_BASE + (ch) * 0x8000)

/* Registers base address */
#define MMIO_BASE                        0xF8000000

/* Aggregate of all devices in the first GB */
#define DEV_RNG0_BASE                    MMIO_BASE
#define DEV_RNG0_SIZE                    SIZE_M(125)

#define RK_SIP_DDR_CFG                   0x82000008
#define DRAM_INIT                        0x00
#define DRAM_SET_RATE                    0x01
#define DRAM_ROUND_RATE                  0x02
#define DRAM_SET_AT_SR                   0x03
#define DRAM_GET_BW                      0x04
#define DRAM_GET_RATE                    0x05
#define DRAM_CLR_IRQ                     0x06
#define DRAM_SET_PARAM                   0x07
#define DRAM_SET_ODT_PD                  0x08

#define RK_SIP_HDCP_CONTROL              0x82000009
#define RK_SIP_HDCP_KEY_DATA64           0xC200000A

#define RK3399_PRIMARY_CPU               0x0

/* Special value used to verify platform parameters from BL2 to BL3-1 */
#define RK_BL31_PLAT_PARAM_VAL           0x0f1e2d3c4b5a6978ULL

/**************************************************************************
 * UART related constants
 **************************************************************************/
#define RK3399_BAUDRATE                  115200
#define RK3399_UART_CLOCK                24000000

/******************************************************************************
 * System counter frequency related constants
 ******************************************************************************/
#define SYS_COUNTER_FREQ_IN_TICKS        24000000

/* Base rockchip_platform compatible GIC memory map */
#define BASE_GICD_BASE                   (GIC500_BASE)
#define BASE_GICR_BASE                   (GIC500_BASE + SIZE_M(1))

/*****************************************************************************
 * CCI-400 related constants
 ******************************************************************************/
#define PLAT_RK_CCI_CLUSTER0_SL_IFACE_IX 0
#define PLAT_RK_CCI_CLUSTER1_SL_IFACE_IX 1

/******************************************************************************
 * sgi, ppi
 ******************************************************************************/
#define ARM_IRQ_SEC_PHY_TIMER            29

#define ARM_IRQ_SEC_SGI_0                8
#define ARM_IRQ_SEC_SGI_1                9
#define ARM_IRQ_SEC_SGI_2                10
#define ARM_IRQ_SEC_SGI_3                11
#define ARM_IRQ_SEC_SGI_4                12
#define ARM_IRQ_SEC_SGI_5                13
#define ARM_IRQ_SEC_SGI_6                14
#define ARM_IRQ_SEC_SGI_7                15

/*
 * Define a list of Group 1 Secure and Group 0 interrupts as per GICv3
 * terminology. On a GICv2 system or mode, the lists will be merged and treated
 * as Group 0 interrupts.
 */
#define PLAT_RK_GICV3_G1S_IRQS \
    INTR_PROP_DESC(ARM_IRQ_SEC_PHY_TIMER, GIC_HIGHEST_SEC_PRIORITY, INTR_GROUP1S, GIC_INTR_CFG_LEVEL)

#define PLAT_RK_GICV3_G0_IRQS \
    INTR_PROP_DESC(ARM_IRQ_SEC_SGI_6, GIC_HIGHEST_SEC_PRIORITY, INTR_GROUP0, GIC_INTR_CFG_LEVEL)


/*******************************************************************************
 * Platform memory map related constants
 ******************************************************************************/
/* TF text, ro, rw, Size: 1MB */
#define TZRAM_BASE                     (0x0)
#define TZRAM_SIZE                     (0x100000)

/*******************************************************************************
 * BL31 specific defines.
 ******************************************************************************/
/*
 * Put BL31 at the top of the Trusted RAM
 */
#define BL31_BASE                      (TZRAM_BASE + 0x40000)
#define BL31_LIMIT                     (TZRAM_BASE + TZRAM_SIZE)


#define PARAM_ADDR                     0xc0

#define PARAM_M0_FUNC                  0x00
#define PARAM_DRAM_FREQ                0x04
#define PARAM_DPLL_CON0                0x08
#define PARAM_DPLL_CON1                0x0c
#define PARAM_DPLL_CON2                0x10
#define PARAM_DPLL_CON3                0x14
#define PARAM_DPLL_CON4                0x18
#define PARAM_DPLL_CON5                0x1c
#define PARAM_FREQ_SELECT              0x20
#define PARAM_M0_DONE                  0x24
#define PARAM_M0_SIZE                  0x28
#define M0_DONE_FLAG                   0xf59ec39a


#define PMU_WKUP_CFG0                  0x00
#define PMU_WKUP_CFG1                  0x04
#define PMU_WKUP_CFG2                  0x08
#define PMU_WKUP_CFG3                  0x0c
#define PMU_WKUP_CFG4                  0x10
#define PMU_PWRDN_CON                  0x14
#define PMU_PWRDN_ST                   0x18
#define PMU_PLL_CON                    0x1c
#define PMU_PWRMODE_CON                0x20
#define PMU_SFT_CON                    0x24
#define PMU_INT_CON                    0x28
#define PMU_INT_ST                     0x2c
#define PMU_GPIO0_POS_INT_CON          0x30
#define PMU_GPIO0_NEG_INT_CON          0x34
#define PMU_GPIO1_POS_INT_CON          0x38
#define PMU_GPIO1_NEG_INT_CON          0x3c
#define PMU_GPIO0_POS_INT_ST           0x40
#define PMU_GPIO0_NEG_INT_ST           0x44
#define PMU_GPIO1_POS_INT_ST           0x48
#define PMU_GPIO1_NEG_INT_ST           0x4c
#define PMU_PWRDN_INTEN                0x50
#define PMU_PWRDN_STATUS               0x54
#define PMU_WAKEUP_STATUS              0x58
#define PMU_BUS_CLR                    0x5c
#define PMU_BUS_IDLE_REQ               0x60
#define PMU_BUS_IDLE_ST                0x64
#define PMU_BUS_IDLE_ACK               0x68
#define PMU_CCI500_CON                 0x6c
#define PMU_ADB400_CON                 0x70
#define PMU_ADB400_ST                  0x74
#define PMU_POWER_ST                   0x78
#define PMU_CORE_PWR_ST                0x7c
#define PMU_OSC_CNT                    0x80
#define PMU_PLLLOCK_CNT                0x84
#define PMU_PLLRST_CNT                 0x88
#define PMU_STABLE_CNT                 0x8c
#define PMU_DDRIO_PWRON_CNT            0x90
#define PMU_WAKEUP_RST_CLR_CNT         0x94
#define PMU_DDR_SREF_ST                0x98
#define PMU_SCU_L_PWRDN_CNT            0x9c
#define PMU_SCU_L_PWRUP_CNT            0xa0
#define PMU_SCU_B_PWRDN_CNT            0xa4
#define PMU_SCU_B_PWRUP_CNT            0xa8
#define PMU_GPU_PWRDN_CNT              0xac
#define PMU_GPU_PWRUP_CNT              0xb0
#define PMU_CENTER_PWRDN_CNT           0xb4
#define PMU_CENTER_PWRUP_CNT           0xb8
#define PMU_TIMEOUT_CNT                0xbc
#define PMU_CPU0APM_CON                0xc0
#define PMU_CPU1APM_CON                0xc4
#define PMU_CPU2APM_CON                0xc8
#define PMU_CPU3APM_CON                0xcc
#define PMU_CPU0BPM_CON                0xd0
#define PMU_CPU1BPM_CON                0xd4
#define PMU_NOC_AUTO_ENA               0xd8
#define PMU_PWRDN_CON1                 0xdc

#define PMUGRF_GPIO0A_IOMUX            0x00
#define PMUGRF_GPIO1A_IOMUX            0x10
#define PMUGRF_GPIO1C_IOMUX            0x18

#define PMUGRF_GPIO0A6_IOMUX_SHIFT     12
#define PMUGRF_GPIO0A6_IOMUX_PWM       0x1
#define PMUGRF_GPIO1C3_IOMUX_SHIFT     6
#define PMUGRF_GPIO1C3_IOMUX_PWM       0x1

#define CPU_AXI_QOS_ID_COREID          0x00
#define CPU_AXI_QOS_REVISIONID         0x04
#define CPU_AXI_QOS_PRIORITY           0x08
#define CPU_AXI_QOS_MODE               0x0c
#define CPU_AXI_QOS_BANDWIDTH          0x10
#define CPU_AXI_QOS_SATURATION         0x14
#define CPU_AXI_QOS_EXTCONTROL         0x18
#define CPU_AXI_QOS_NUM_REGS           0x07

#define CPU_AXI_CCI_M0_QOS_BASE        0xffa50000
#define CPU_AXI_CCI_M1_QOS_BASE        0xffad8000
#define CPU_AXI_DMAC0_QOS_BASE         0xffa64200
#define CPU_AXI_DMAC1_QOS_BASE         0xffa64280
#define CPU_AXI_DCF_QOS_BASE           0xffa64180
#define CPU_AXI_CRYPTO0_QOS_BASE       0xffa64100
#define CPU_AXI_CRYPTO1_QOS_BASE       0xffa64080
#define CPU_AXI_PMU_CM0_QOS_BASE       0xffa68000
#define CPU_AXI_PERI_CM1_QOS_BASE      0xffa64300
#define CPU_AXI_GIC_QOS_BASE           0xffa78000
#define CPU_AXI_SDIO_QOS_BASE          0xffa76000
#define CPU_AXI_SDMMC_QOS_BASE         0xffa74000
#define CPU_AXI_EMMC_QOS_BASE          0xffa58000
#define CPU_AXI_GMAC_QOS_BASE          0xffa5c000
#define CPU_AXI_USB_OTG0_QOS_BASE      0xffa70000
#define CPU_AXI_USB_OTG1_QOS_BASE      0xffa70080
#define CPU_AXI_USB_HOST0_QOS_BASE     0xffa60100
#define CPU_AXI_USB_HOST1_QOS_BASE     0xffa60180
#define CPU_AXI_GPU_QOS_BASE           0xffae0000
#define CPU_AXI_VIDEO_M0_QOS_BASE      0xffab8000
#define CPU_AXI_VIDEO_M1_R_QOS_BASE    0xffac0000
#define CPU_AXI_VIDEO_M1_W_QOS_BASE    0xffac0080
#define CPU_AXI_RGA_R_QOS_BASE         0xffab0000
#define CPU_AXI_RGA_W_QOS_BASE         0xffab0080
#define CPU_AXI_IEP_QOS_BASE           0xffa98000
#define CPU_AXI_VOP_BIG_R_QOS_BASE     0xffac8000
#define CPU_AXI_VOP_BIG_W_QOS_BASE     0xffac8080
#define CPU_AXI_VOP_LITTLE_QOS_BASE    0xffad0000
#define CPU_AXI_ISP0_M0_QOS_BASE       0xffaa0000
#define CPU_AXI_ISP0_M1_QOS_BASE       0xffaa0080
#define CPU_AXI_ISP1_M0_QOS_BASE       0xffaa8000
#define CPU_AXI_ISP1_M1_QOS_BASE       0xffaa8080
#define CPU_AXI_HDCP_QOS_BASE          0xffa90000
#define CPU_AXI_PERIHP_NSP_QOS_BASE    0xffad8080
#define CPU_AXI_PERILP_NSP_QOS_BASE    0xffad8180
#define CPU_AXI_PERILPSLV_NSP_QOS_BASE 0xffad8100

#define GRF_GPIO2A_IOMUX               0xe000
#define GRF_GPIO2B_IOMUX               0xe004
#define GRF_GPIO2C_IOMUX               0xe008
#define GRF_GPIO2D_IOMUX               0xe00c
#define GRF_GPIO3A_IOMUX               0xe010
#define GRF_GPIO3B_IOMUX               0xe014
#define GRF_GPIO3C_IOMUX               0xe018
#define GRF_GPIO3D_IOMUX               0xe01c
#define GRF_GPIO4A_IOMUX               0xe020
#define GRF_GPIO4B_IOMUX               0xe024
#define GRF_GPIO4C_IOMUX               0xe028
#define GRF_GPIO4D_IOMUX               0xe02c

#define GRF_GPIO2A_P                   0xe040
#define GRF_GPIO2B_P                   0xe044
#define GRF_GPIO2C_P                   0xe048
#define GRF_GPIO2D_P                   0xe04C
#define GRF_GPIO3A_P                   0xe050
#define GRF_GPIO3B_P                   0xe054
#define GRF_GPIO3C_P                   0xe058
#define GRF_GPIO3D_P                   0xe05C
#define GRF_GPIO4A_P                   0xe060
#define GRF_GPIO4B_P                   0xe064
#define GRF_GPIO4C_P                   0xe068
#define GRF_GPIO4D_P                   0xe06C

/* CRU */
#define CRU_DPLL_CON0                  0x40
#define CRU_DPLL_CON1                  0x44
#define CRU_DPLL_CON2                  0x48
#define CRU_DPLL_CON3                  0x4c
#define CRU_DPLL_CON4                  0x50
#define CRU_DPLL_CON5                  0x54

/* CRU_PLL_CON3 */
#define PLL_SLOW_MODE                  0
#define PLL_NORMAL_MODE                1
#define PLL_MODE(n)                    ((0x3 << (8 + 16)) | ((n) << 8))
#define PLL_POWER_DOWN(n)              ((0x1 << (0 + 16)) | ((n) << 0))

/* PMU CRU */
#define PMU_CRU_GATEDIS_CON0           0x130


#define CTL_REG_NUM                    332
#define PHY_REG_NUM                    959
#define PI_REG_NUM                     200

#define MSCH_ID_COREID                 0x0
#define MSCH_ID_REVISIONID             0x4
#define MSCH_DEVICECONF                0x8
#define MSCH_DEVICESIZE                0xc
#define MSCH_DDRTIMINGA0               0x10
#define MSCH_DDRTIMINGB0               0x14
#define MSCH_DDRTIMINGC0               0x18
#define MSCH_DEVTODEV0                 0x1c
#define MSCH_DDRMODE                   0x110
#define MSCH_AGINGX0                   0x1000

#define CIC_CTRL0                      0x0
#define CIC_CTRL1                      0x4
#define CIC_IDLE_TH                    0x8
#define CIC_CG_WAIT_TH                 0xc
#define CIC_STATUS0                    0x10
#define CIC_STATUS1                    0x14
#define CIC_CTRL2                      0x18
#define CIC_CTRL3                      0x1c
#define CIC_CTRL4                      0x20

/* DENALI_CTL_00 */
#define START                          1

/* DENALI_CTL_68 */
#define PWRUP_SREFRESH_EXIT            (1 << 16)

/* DENALI_CTL_274 */
#define MEM_RST_VALID                  1

#define PHY_DRV_ODT_Hi_Z               0x0
#define PHY_DRV_ODT_240                0x1
#define PHY_DRV_ODT_120                0x8
#define PHY_DRV_ODT_80                 0x9
#define PHY_DRV_ODT_60                 0xc
#define PHY_DRV_ODT_48                 0xd
#define PHY_DRV_ODT_40                 0xe
#define PHY_DRV_ODT_34_3               0xf

/*
 * sys_reg bitfield struct
 * [31] row_3_4_ch1
 * [30] row_3_4_ch0
 * [29:28] chinfo
 * [27] rank_ch1
 * [26:25] col_ch1
 * [24] bk_ch1
 * [23:22] cs0_row_ch1
 * [21:20] cs1_row_ch1
 * [19:18] bw_ch1
 * [17:16] dbw_ch1;
 * [15:13] ddrtype
 * [12] channelnum
 * [11] rank_ch0
 * [10:9] col_ch0
 * [8] bk_ch0
 * [7:6] cs0_row_ch0
 * [5:4] cs1_row_ch0
 * [3:2] bw_ch0
 * [1:0] dbw_ch0
 */
#define SYS_REG_ENC_ROW_3_4(n, ch)     ((n) << (30 + (ch)))
#define SYS_REG_DEC_ROW_3_4(n, ch)     (((n) >> (30 + (ch))) & 0x1)
#define SYS_REG_ENC_CHINFO(ch)         (1 << (28 + (ch)))
#define SYS_REG_DEC_CHINFO(n, ch)      (((n) >> (28 + (ch))) & 0x1)
#define SYS_REG_ENC_DDRTYPE(n)         ((n) << 13)
#define SYS_REG_DEC_DDRTYPE(n)         (((n) >> 13) & 0x7)
#define SYS_REG_ENC_NUM_CH(n)          (((n)-1) << 12)
#define SYS_REG_DEC_NUM_CH(n)          (1 + (((n) >> 12) & 0x1))
#define SYS_REG_ENC_RANK(n, ch)        (((n)-1) << (11 + (ch) * 16))
#define SYS_REG_DEC_RANK(n, ch)        (1 + (((n) >> (11 + (ch) * 16)) & 0x1))
#define SYS_REG_ENC_COL(n, ch)         (((n)-9) << (9 + (ch) * 16))
#define SYS_REG_DEC_COL(n, ch)         (9 + (((n) >> (9 + (ch) * 16)) & 0x3))
#define SYS_REG_ENC_BK(n, ch)          (((n) == 3 ? 0 : 1) << (8 + (ch) * 16))
#define SYS_REG_DEC_BK(n, ch)          (3 - (((n) >> (8 + (ch) * 16)) & 0x1))
#define SYS_REG_ENC_CS0_ROW(n, ch)     (((n)-13) << (6 + (ch) * 16))
#define SYS_REG_DEC_CS0_ROW(n, ch)     (13 + (((n) >> (6 + (ch) * 16)) & 0x3))
#define SYS_REG_ENC_CS1_ROW(n, ch)     (((n)-13) << (4 + (ch) * 16))
#define SYS_REG_DEC_CS1_ROW(n, ch)     (13 + (((n) >> (4 + (ch) * 16)) & 0x3))
#define SYS_REG_ENC_BW(n, ch)          ((2 >> (n)) << (2 + (ch) * 16))
#define SYS_REG_DEC_BW(n, ch)          (2 >> (((n) >> (2 + (ch) * 16)) & 0x3))
#define SYS_REG_ENC_DBW(n, ch)         ((2 >> (n)) << (0 + (ch) * 16))
#define SYS_REG_DEC_DBW(n, ch)         (2 >> (((n) >> (0 + (ch) * 16)) & 0x3))
#define DDR_STRIDE(n)                  mmio_write_32(SGRF_BASE + SGRF_SOC_CON3_7(4), (0x1f << (10 + 16)) | ((n) << 10))

#define GLB_SRST_FST_CFG_VAL           0xfdb9
#define GLB_SRST_SND_CFG_VAL           0xeca8

#define PMUCRU_PPLL_CON(n)             ((n) * 4)
#define CRU_PLL_CON(pll_id, n)         ((pll_id) * 0x20 + (n) * 4)
#define PLL_MODE_MSK                   0x03
#define PLL_MODE_SHIFT                 0x08
#define PLL_BYPASS_MSK                 0x01
#define PLL_BYPASS_SHIFT               0x01
#define PLL_PWRDN_MSK                  0x01
#define PLL_PWRDN_SHIFT                0x0
#define PLL_BYPASS                     BIT(1)
#define PLL_PWRDN                      BIT(0)

#define NO_PLL_BYPASS                  (0x00)
#define NO_PLL_PWRDN                   (0x00)

#define FBDIV(n)                       ((0xfff << 16) | n)
#define POSTDIV2(n)                    ((0x7 << (12 + 16)) | (n << 12))
#define POSTDIV1(n)                    ((0x7 << (8 + 16)) | (n << 8))
#define REFDIV(n)                      ((0x3F << 16) | n)
#define PLL_LOCK(n)                    ((n >> 31) & 0x1)

// #define PLL_SLOW_MODE                  BITS_WITH_WMASK(SLOW_MODE, PLL_MODE_MSK, PLL_MODE_SHIFT)

#define PLL_NOMAL_MODE                 BITS_WITH_WMASK(NORMAL_MODE, PLL_MODE_MSK, PLL_MODE_SHIFT)

#define PLL_BYPASS_MODE                BIT_WITH_WMSK(PLL_BYPASS_SHIFT)
#define PLL_NO_BYPASS_MODE             WMSK_BIT(PLL_BYPASS_SHIFT)

#define PLL_CON_COUNT                  0x06
#define CRU_CLKSEL_COUNT               108
#define CRU_CLKSEL_CON(n)              (0x100 + (n) * 4)

#define PMUCRU_CLKSEL_CONUT            0x06
#define PMUCRU_CLKSEL_OFFSET           0x080
#define REG_SIZE                       0x04
#define REG_SOC_WMSK                   0xffff0000
#define CLK_GATE_MASK                  0x01

#define PMUCRU_GATE_COUNT              0x03
#define CRU_GATE_COUNT                 0x23
#define PMUCRU_GATE_CON(n)             (0x100 + (n) * 4)
#define CRU_GATE_CON(n)                (0x300 + (n) * 4)

#define PMUCRU_RSTNHOLD_CON0           0x120

#define BIT_32(nr)                     (((uint32_t)(1U)) << (nr))
#define BIT_64(nr)                     (((uint64_t)(1ULL)) << (nr))

#define BIT                            BIT_64

typedef unsigned long u_register_t;


enum
{
    PRESETN_NOC_PMU_HOLD = 1,
    PRESETN_INTMEM_PMU_HOLD,
    HRESETN_CM0S_PMU_HOLD,
    HRESETN_CM0S_NOC_PMU_HOLD,
    DRESETN_CM0S_PMU_HOLD,
    POESETN_CM0S_PMU_HOLD,
    PRESETN_SPI3_HOLD,
    RESETN_SPI3_HOLD,
    PRESETN_TIMER_PMU_0_1_HOLD,
    RESETN_TIMER_PMU_0_HOLD,
    RESETN_TIMER_PMU_1_HOLD,
    PRESETN_UART_M0_PMU_HOLD,
    RESETN_UART_M0_PMU_HOLD,
    PRESETN_WDT_PMU_HOLD
};

#define PMUCRU_RSTNHOLD_CON1 0x124
enum
{
    PRESETN_I2C0_HOLD,
    PRESETN_I2C4_HOLD,
    PRESETN_I2C8_HOLD,
    PRESETN_MAILBOX_PMU_HOLD,
    PRESETN_RKPWM_PMU_HOLD,
    PRESETN_PMUGRF_HOLD,
    PRESETN_SGRF_HOLD,
    PRESETN_GPIO0_HOLD,
    PRESETN_GPIO1_HOLD,
    PRESETN_CRU_PMU_HOLD,
    PRESETN_INTR_ARB_HOLD,
    PRESETN_PVTM_PMU_HOLD,
    RESETN_I2C0_HOLD,
    RESETN_I2C4_HOLD,
    RESETN_I2C8_HOLD
};

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

#define CLST_L_CPUS_MSK (0xf)
#define CLST_B_CPUS_MSK (0x3)

enum pll_work_mode
{
    SLOW_MODE      = 0x00,
    NORMAL_MODE    = 0x01,
    DEEP_SLOW_MODE = 0x02,
};

enum glb_sft_reset
{
    PMU_RST_BY_FIRST_SFT,
    PMU_RST_BY_SECOND_SFT = BIT(2),
    PMU_RST_NOT_BY_SFT    = BIT(3),
};

struct pll_div
{
    uint32_t mhz;
    uint32_t refdiv;
    uint32_t fbdiv;
    uint32_t postdiv1;
    uint32_t postdiv2;
    uint32_t frac;
    uint32_t freq;
};

struct deepsleep_data_s
{
    uint32_t plls_con[END_PLL_ID][PLL_CON_COUNT];
    uint32_t cru_gate_con[CRU_GATE_COUNT];
    uint32_t pmucru_gate_con[PMUCRU_GATE_COUNT];
};

struct pmu_sleep_data
{
    uint32_t pmucru_rstnhold_con0;
    uint32_t pmucru_rstnhold_con1;
};

/**************************************************
 * pmugrf reg, offset
 **************************************************/
#define PMUGRF_OSREG(n)          (0x300 + (n) * 4)
#define PMUGRF_GPIO0A_P          0x040
#define PMUGRF_GPIO1A_P          0x050

/**************************************************
 * DCF reg, offset
 **************************************************/
#define DCF_DCF_CTRL             0x0
#define DCF_DCF_ADDR             0x8
#define DCF_DCF_ISR              0xc
#define DCF_DCF_TOSET            0x14
#define DCF_DCF_TOCMD            0x18
#define DCF_DCF_CMD_CFG          0x1c

/* DCF_DCF_ISR */
#define DCF_TIMEOUT              (1 << 2)
#define DCF_ERR                  (1 << 1)
#define DCF_DONE                 (1 << 0)

/* DCF_DCF_CTRL */
#define DCF_VOP_HW_EN            (1 << 2)
#define DCF_STOP                 (1 << 1)
#define DCF_START                (1 << 0)

#define CYCL_24M_CNT_US(us)      (24 * us)
#define CYCL_24M_CNT_MS(ms)      (ms * CYCL_24M_CNT_US(1000))
#define CYCL_32K_CNT_MS(ms)      (ms * 32)

/**************************************************
 * cru reg, offset
 **************************************************/
#define CRU_SOFTRST_CON(n)       (0x400 + (n) * 4)

#define CRU_DMAC0_RST            BIT_WITH_WMSK(3)
/* reset release*/
#define CRU_DMAC0_RST_RLS        WMSK_BIT(3)

#define CRU_DMAC1_RST            BIT_WITH_WMSK(4)
/* reset release*/
#define CRU_DMAC1_RST_RLS        WMSK_BIT(4)

#define CRU_GLB_RST_CON          0x0510
#define CRU_GLB_SRST_FST         0x0500
#define CRU_GLB_SRST_SND         0x0504

#define CRU_CLKGATE_CON(n)       (0x300 + n * 4)
#define PCLK_GPIO2_GATE_SHIFT    3
#define PCLK_GPIO3_GATE_SHIFT    4
#define PCLK_GPIO4_GATE_SHIFT    5

/**************************************************
 * pmu cru reg, offset
 **************************************************/
#define CRU_PMU_RSTHOLD_CON(n)   (0x120 + n * 4)
/* reset hold*/
#define CRU_PMU_SGRF_RST_HOLD    BIT_WITH_WMSK(6)
/* reset hold release*/
#define CRU_PMU_SGRF_RST_RLS     WMSK_BIT(6)

#define CRU_PMU_WDTRST_MSK       (0x1 << 4)
#define CRU_PMU_WDTRST_EN        0x0

#define CRU_PMU_FIRST_SFTRST_MSK (0x3 << 2)
#define CRU_PMU_FIRST_SFTRST_EN  0x0

#define CRU_PMU_CLKGATE_CON(n)   (0x100 + n * 4)
#define PCLK_GPIO0_GATE_SHIFT    3
#define PCLK_GPIO1_GATE_SHIFT    4

#define CPU_BOOT_ADDR_WMASK      0xffff0000
#define CPU_BOOT_ADDR_ALIGN      16

#define GRF_IOMUX_2BIT_MASK      0x3
#define GRF_IOMUX_GPIO           0x0

#define GRF_GPIO4C2_IOMUX_SHIFT  4
#define GRF_GPIO4C2_IOMUX_PWM    0x1
#define GRF_GPIO4C6_IOMUX_SHIFT  12
#define GRF_GPIO4C6_IOMUX_PWM    0x1

#define PWM_CNT(n)               (0x0000 + 0x10 * (n))
#define PWM_PERIOD_HPR(n)        (0x0004 + 0x10 * (n))
#define PWM_DUTY_LPR(n)          (0x0008 + 0x10 * (n))
#define PWM_CTRL(n)              (0x000c + 0x10 * (n))

#define PWM_DISABLE              (0 << 0)
#define PWM_ENABLE               (1 << 0)

/* grf reg offset */
#define GRF_USBPHY0_CTRL0        0x4480
#define GRF_USBPHY0_CTRL2        0x4488
#define GRF_USBPHY0_CTRL3        0x448c
#define GRF_USBPHY0_CTRL12       0x44b0
#define GRF_USBPHY0_CTRL13       0x44b4
#define GRF_USBPHY0_CTRL15       0x44bc
#define GRF_USBPHY0_CTRL16       0x44c0

#define GRF_USBPHY1_CTRL0        0x4500
#define GRF_USBPHY1_CTRL2        0x4508
#define GRF_USBPHY1_CTRL3        0x450c
#define GRF_USBPHY1_CTRL12       0x4530
#define GRF_USBPHY1_CTRL13       0x4534
#define GRF_USBPHY1_CTRL15       0x453c
#define GRF_USBPHY1_CTRL16       0x4540

#define GRF_GPIO2A_IOMUX         0xe000
#define GRF_GPIO2A_P             0xe040
#define GRF_GPIO3A_P             0xe050
#define GRF_GPIO4A_P             0xe060
#define GRF_GPIO2D_HE            0xe18c
#define GRF_DDRC0_CON0           0xe380
#define GRF_DDRC0_CON1           0xe384
#define GRF_DDRC1_CON0           0xe388
#define GRF_DDRC1_CON1           0xe38c
#define GRF_SOC_CON_BASE         0xe200
#define GRF_SOC_CON(n)           (GRF_SOC_CON_BASE + (n) * 4)
#define GRF_IO_VSEL              0xe640

#define CRU_CLKSEL_CON0          0x0100
#define CRU_CLKSEL_CON6          0x0118
#define CRU_SDIO0_CON1           0x058c
#define PMUCRU_CLKSEL_CON0       0x0080
#define PMUCRU_CLKGATE_CON2      0x0108
#define PMUCRU_SOFTRST_CON0      0x0110
#define PMUCRU_GATEDIS_CON0      0x0130
#define PMUCRU_SOFTRST_CON(n)    (PMUCRU_SOFTRST_CON0 + (n) * 4)

#define __dead2                  __attribute__((__noreturn__))
#define __deprecated             __attribute__((__deprecated__))
#define __packed                 __attribute__((__packed__))
#define __used                   __attribute__((__used__))
#define __unused                 __attribute__((__unused__))
#define __maybe_unused           __attribute__((__unused__))
#define __aligned(x)             __attribute__((__aligned__(x)))
#define __section(x)             __attribute__((__section__(x)))
#define __fallthrough            __attribute__((__fallthrough__))
#if RECLAIM_INIT_CODE
/*
 * Add each function to a section that is unique so the functions can still
 * be garbage collected
 */
#    define __init __section(".text.init." __FILE__ "." __XSTRING(__LINE__))
#else
#    define __init
#endif

#define __printflike(fmtarg, firstvararg) __attribute__((__format__(__printf__, fmtarg, firstvararg)))

#define __weak_reference(sym, alias) \
    __asm__(".weak alias");          \
    __asm__(".equ alias, sym")

#define __STRING(x)    #x
#define __XSTRING(x)   __STRING(x)

#define __sramdata     __attribute__((section(".sram.data")))
#define __sramconst    __attribute__((section(".sram.rodata")))
#define __sramfunc     __attribute__((section(".sram.text")))

#define __pmusramdata  __attribute__((section(".pmusram.data")))
#define __pmusramconst __attribute__((section(".pmusram.rodata")))
#define __pmusramfunc  __attribute__((section(".pmusram.text")))

#define NOTICE(...)    printf("NOTICE:  " __VA_ARGS__)

/* Define function for system instruction with type specifier */
#define DEFINE_SYSOP_TYPE_FUNC(_op, _type)       \
    static inline void _op##_type(void)          \
    {                                            \
        __asm__(#_op " " #_type : : : "memory"); \
    }

DEFINE_SYSOP_TYPE_FUNC(dmb, st)

extern uint32_t __bl31_sram_text_start, __bl31_sram_text_end;
extern uint32_t __bl31_sram_data_start, __bl31_sram_data_end;
extern uint32_t __bl31_sram_stack_start, __bl31_sram_stack_end;
extern uint32_t __bl31_sram_text_real_end, __bl31_sram_data_real_end;
extern uint32_t __sram_incbin_start, __sram_incbin_end;
extern uint32_t __sram_incbin_real_end;

/******************************************************************************
 * The register have write-mask bits, it is mean, if you want to set the bits,
 * you needs set the write-mask bits at the same time,
 * The write-mask bits is in high 16-bits.
 * The fllowing macro definition helps access write-mask bits reg efficient!
 ******************************************************************************/
#define REG_MSK_SHIFT 16

#ifndef WMSK_BIT
#    define WMSK_BIT(nr) BIT((nr) + REG_MSK_SHIFT)
#endif

/* set one bit with write mask */
#ifndef BIT_WITH_WMSK
#    define BIT_WITH_WMSK(nr) (BIT(nr) | WMSK_BIT(nr))
#endif

#ifndef BITS_SHIFT
#    define BITS_SHIFT(bits, shift) ((bits) << (shift))
#endif

#ifndef BITS_WITH_WMASK
#    define BITS_WITH_WMASK(bits, msk, shift) (BITS_SHIFT(bits, shift) | BITS_SHIFT(msk, (shift + REG_MSK_SHIFT)))
#endif

/******************************************************************************
 * Function and variable prototypes
 *****************************************************************************/
void plat_configure_mmu_el3(unsigned long total_base, unsigned long total_size, unsigned long, unsigned long,
                            unsigned long, unsigned long);

void rockchip_plat_mmu_el3(void);

/*******************************************************************************
 * Platform binary types for linking
 ******************************************************************************/
#define PLATFORM_LINKER_FORMAT "elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH   aarch64

/*******************************************************************************
 * Generic platform constants
 ******************************************************************************/

/* Size of cacheable stacks */
#if defined(IMAGE_BL1)
#    define PLATFORM_STACK_SIZE 0x440
#elif defined(IMAGE_BL2)
#    define PLATFORM_STACK_SIZE 0x400
#elif defined(IMAGE_BL31)
#    define PLATFORM_STACK_SIZE 0x800
#elif defined(IMAGE_BL32)
#    define PLATFORM_STACK_SIZE 0x440
#endif

#define FIRMWARE_WELCOME_STR          "Booting Trusted Firmware\n"

#define PLATFORM_MAX_AFFLVL           MPIDR_AFFLVL2
#define PLATFORM_SYSTEM_COUNT         U(1)
#define PLATFORM_CLUSTER_COUNT        U(2)
#define PLATFORM_CLUSTER0_CORE_COUNT  U(4)
#define PLATFORM_CLUSTER1_CORE_COUNT  U(2)
#define PLATFORM_CORE_COUNT           (PLATFORM_CLUSTER1_CORE_COUNT + PLATFORM_CLUSTER0_CORE_COUNT)
#define PLATFORM_MAX_CPUS_PER_CLUSTER U(4)
#define PLATFORM_NUM_AFFS             (PLATFORM_SYSTEM_COUNT + PLATFORM_CLUSTER_COUNT + PLATFORM_CORE_COUNT)
#define PLAT_RK_CLST_TO_CPUID_SHIFT   6
#define PLAT_MAX_PWR_LVL              MPIDR_AFFLVL2

/*
 * This macro defines the deepest retention state possible. A higher state
 * id will represent an invalid or a power down state.
 */
#define PLAT_MAX_RET_STATE            U(1)

/*
 * This macro defines the deepest power down states possible. Any state ID
 * higher than this is invalid.
 */
#define PLAT_MAX_OFF_STATE            U(2)

/*******************************************************************************
 * Platform specific page table and MMU setup constants
 ******************************************************************************/
#define PLAT_VIRT_ADDR_SPACE_SIZE     (1ULL << 32)
#define PLAT_PHY_ADDR_SPACE_SIZE      (1ULL << 32)
#define MAX_XLAT_TABLES               20
#define MAX_MMAP_REGIONS              25

/*******************************************************************************
 * Declarations and constants to access the mailboxes safely. Each mailbox is
 * aligned on the biggest cache line size in the platform. This is known only
 * to the platform as it might have a combination of integrated and external
 * caches. Such alignment ensures that two maiboxes do not sit on the same cache
 * line at any cache level. They could belong to different cpus/clusters &
 * get written while being protected by different locks causing corruption of
 * a valid mailbox address.
 ******************************************************************************/
#define CACHE_WRITEBACK_SHIFT         6
#define CACHE_WRITEBACK_GRANULE       (1 << CACHE_WRITEBACK_SHIFT)

#define U_(_x)                        (_x##U)
#define U(_x)                         U_(_x)
#define UL_(_x)                       (_x##UL)
#define UL(_x)                        UL_(_x)
#define ULL_(_x)                      (_x##ULL)
#define ULL(_x)                       ULL_(_x)
#define L_(_x)                        (_x##L)
#define L(_x)                         L_(_x)
#define LL_(_x)                       (_x##LL)
#define LL(_x)                        LL_(_x)

/*******************************************************************************
 * Number of power domains whose state this PSCI implementation can track
 ******************************************************************************/
#ifdef PLAT_NUM_PWR_DOMAINS
#    define PSCI_NUM_PWR_DOMAINS PLAT_NUM_PWR_DOMAINS
#else
#    define PSCI_NUM_PWR_DOMAINS (U(2) * PLATFORM_CORE_COUNT)
#endif

#define PSCI_NUM_NON_CPU_PWR_DOMAINS (PSCI_NUM_PWR_DOMAINS - PLATFORM_CORE_COUNT)

/* This is the power level corresponding to a CPU */
#define PSCI_CPU_PWR_LVL             U(0)

/*
 * The maximum power level supported by PSCI. Since PSCI CPU_SUSPEND
 * uses the old power_state parameter format which has 2 bits to specify the
 * power level, this constant is defined to be 3.
 */
#define PSCI_MAX_PWR_LVL             U(3)

/*******************************************************************************
 * Defines for runtime services function ids
 ******************************************************************************/
#define PSCI_VERSION                 U(0x84000000)
#define PSCI_CPU_SUSPEND_AARCH32     U(0x84000001)
#define PSCI_CPU_SUSPEND_AARCH64     U(0xc4000001)
#define PSCI_CPU_OFF                 U(0x84000002)
#define PSCI_CPU_ON_AARCH32          U(0x84000003)
#define PSCI_CPU_ON_AARCH64          U(0xc4000003)
#define PSCI_AFFINITY_INFO_AARCH32   U(0x84000004)
#define PSCI_AFFINITY_INFO_AARCH64   U(0xc4000004)
#define PSCI_MIG_AARCH32             U(0x84000005)
#define PSCI_MIG_AARCH64             U(0xc4000005)
#define PSCI_MIG_INFO_TYPE           U(0x84000006)
#define PSCI_MIG_INFO_UP_CPU_AARCH32 U(0x84000007)
#define PSCI_MIG_INFO_UP_CPU_AARCH64 U(0xc4000007)
#define PSCI_SYSTEM_OFF              U(0x84000008)
#define PSCI_SYSTEM_RESET            U(0x84000009)
#define PSCI_FEATURES                U(0x8400000A)
#define PSCI_NODE_HW_STATE_AARCH32   U(0x8400000d)
#define PSCI_NODE_HW_STATE_AARCH64   U(0xc400000d)
#define PSCI_SYSTEM_SUSPEND_AARCH32  U(0x8400000E)
#define PSCI_SYSTEM_SUSPEND_AARCH64  U(0xc400000E)
#define PSCI_SET_SUSPEND_MODE        U(0x8400000F)
#define PSCI_STAT_RESIDENCY_AARCH32  U(0x84000010)
#define PSCI_STAT_RESIDENCY_AARCH64  U(0xc4000010)
#define PSCI_STAT_COUNT_AARCH32      U(0x84000011)
#define PSCI_STAT_COUNT_AARCH64      U(0xc4000011)
#define PSCI_SYSTEM_RESET2_AARCH32   U(0x84000012)
#define PSCI_SYSTEM_RESET2_AARCH64   U(0xc4000012)
#define PSCI_MEM_PROTECT             U(0x84000013)
#define PSCI_MEM_CHK_RANGE_AARCH32   U(0x84000014)
#define PSCI_MEM_CHK_RANGE_AARCH64   U(0xc4000014)

/*
 * Number of PSCI calls (above) implemented
 */
#if ENABLE_PSCI_STAT
#    if PSCI_OS_INIT_MODE
#        define PSCI_NUM_CALLS U(30)
#    else
#        define PSCI_NUM_CALLS U(29)
#    endif
#else
#    if PSCI_OS_INIT_MODE
#        define PSCI_NUM_CALLS U(26)
#    else
#        define PSCI_NUM_CALLS U(25)
#    endif
#endif

/* The macros below are used to identify PSCI calls from the SMC function ID */
#define PSCI_FID_MASK           U(0xffe0)
#define PSCI_FID_VALUE          U(0)
#define is_psci_fid(_fid)       (((_fid) & PSCI_FID_MASK) == PSCI_FID_VALUE)

/*******************************************************************************
 * PSCI Migrate and friends
 ******************************************************************************/
#define PSCI_TOS_UP_MIG_CAP     0
#define PSCI_TOS_NOT_UP_MIG_CAP 1
#define PSCI_TOS_NOT_PRESENT_MP 2

/*******************************************************************************
 * PSCI CPU_SUSPEND 'power_state' parameter specific defines
 ******************************************************************************/
#define PSTATE_ID_SHIFT         U(0)

#if PSCI_EXTENDED_STATE_ID
#    define PSTATE_VALID_MASK U(0xB0000000)
#    define PSTATE_TYPE_SHIFT U(30)
#    define PSTATE_ID_MASK    U(0xfffffff)
#else
#    define PSTATE_VALID_MASK              U(0xFCFE0000)
#    define PSTATE_TYPE_SHIFT              U(16)
#    define PSTATE_PWR_LVL_SHIFT           U(24)
#    define PSTATE_ID_MASK                 U(0xffff)
#    define PSTATE_PWR_LVL_MASK            U(0x3)

#    define psci_get_pstate_pwrlvl(pstate) (((pstate) >> PSTATE_PWR_LVL_SHIFT) & PSTATE_PWR_LVL_MASK)
#    define psci_make_powerstate(state_id, type, pwrlvl)                                                          \
        (((state_id) & PSTATE_ID_MASK) << PSTATE_ID_SHIFT) | (((type) & PSTATE_TYPE_MASK) << PSTATE_TYPE_SHIFT) | \
            (((pwrlvl) & PSTATE_PWR_LVL_MASK) << PSTATE_PWR_LVL_SHIFT)
#endif /* __PSCI_EXTENDED_STATE_ID__ */

#define PSTATE_TYPE_STANDBY   U(0x0)
#define PSTATE_TYPE_POWERDOWN U(0x1)
#define PSTATE_TYPE_MASK      U(0x1)

/*******************************************************************************
 * PSCI CPU_FEATURES feature flag specific defines
 ******************************************************************************/
/* Features flags for CPU SUSPEND power state parameter format. Bits [1:1] */
#define FF_PSTATE_SHIFT       U(1)
#define FF_PSTATE_ORIG        U(0)
#define FF_PSTATE_EXTENDED    U(1)
#if PSCI_EXTENDED_STATE_ID
#    define FF_PSTATE FF_PSTATE_EXTENDED
#else
#    define FF_PSTATE FF_PSTATE_ORIG
#endif

/* Features flags for CPU SUSPEND OS Initiated mode support. Bits [0:0] */
#define FF_MODE_SUPPORT_SHIFT U(0)
#if PSCI_OS_INIT_MODE
#    define FF_SUPPORTS_OS_INIT_MODE U(1)
#else
#    define FF_SUPPORTS_OS_INIT_MODE U(0)
#endif

/*******************************************************************************
 * PSCI version
 ******************************************************************************/
#define PSCI_MAJOR_VER                (U(1) << 16)
#define PSCI_MINOR_VER                U(0x1)

/*******************************************************************************
 * PSCI error codes
 ******************************************************************************/
#define PSCI_E_SUCCESS                0
#define PSCI_E_NOT_SUPPORTED          -1
#define PSCI_E_INVALID_PARAMS         -2
#define PSCI_E_DENIED                 -3
#define PSCI_E_ALREADY_ON             -4
#define PSCI_E_ON_PENDING             -5
#define PSCI_E_INTERN_FAIL            -6
#define PSCI_E_NOT_PRESENT            -7
#define PSCI_E_DISABLED               -8
#define PSCI_E_INVALID_ADDRESS        -9

#define PSCI_INVALID_MPIDR            ~((u_register_t)0)

/*
 * SYSTEM_RESET2 macros
 */
#define PSCI_RESET2_TYPE_VENDOR_SHIFT U(31)
#define PSCI_RESET2_TYPE_VENDOR       (U(1) << PSCI_RESET2_TYPE_VENDOR_SHIFT)
#define PSCI_RESET2_TYPE_ARCH         (U(0) << PSCI_RESET2_TYPE_VENDOR_SHIFT)
#define PSCI_RESET2_SYSTEM_WARM_RESET (PSCI_RESET2_TYPE_ARCH | U(0))


/*******************************************************************************
 * Platform binary types for linking
 ******************************************************************************/
#define PLATFORM_LINKER_FORMAT        "elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH          aarch64

/*******************************************************************************
 * Generic platform constants
 ******************************************************************************/

/* Size of cacheable stacks */
#if defined(IMAGE_BL1)
#    define PLATFORM_STACK_SIZE 0x440
#elif defined(IMAGE_BL2)
#    define PLATFORM_STACK_SIZE 0x400
#elif defined(IMAGE_BL31)
#    define PLATFORM_STACK_SIZE 0x800
#elif defined(IMAGE_BL32)
#    define PLATFORM_STACK_SIZE 0x440
#endif

#define FIRMWARE_WELCOME_STR          "Booting Trusted Firmware\n"

#define PLATFORM_MAX_AFFLVL           MPIDR_AFFLVL2
#define PLATFORM_SYSTEM_COUNT         U(1)
#define PLATFORM_CLUSTER_COUNT        U(2)
#define PLATFORM_CLUSTER0_CORE_COUNT  U(4)
#define PLATFORM_CLUSTER1_CORE_COUNT  U(2)
#define PLATFORM_CORE_COUNT           (PLATFORM_CLUSTER1_CORE_COUNT + PLATFORM_CLUSTER0_CORE_COUNT)
#define PLATFORM_MAX_CPUS_PER_CLUSTER U(4)
#define PLATFORM_NUM_AFFS             (PLATFORM_SYSTEM_COUNT + PLATFORM_CLUSTER_COUNT + PLATFORM_CORE_COUNT)
#define PLAT_RK_CLST_TO_CPUID_SHIFT   6
#define PLAT_MAX_PWR_LVL              MPIDR_AFFLVL2

/*
 * This macro defines the deepest retention state possible. A higher state
 * id will represent an invalid or a power down state.
 */
#define PLAT_MAX_RET_STATE            U(1)

/*
 * This macro defines the deepest power down states possible. Any state ID
 * higher than this is invalid.
 */
#define PLAT_MAX_OFF_STATE            U(2)

/*******************************************************************************
 * Platform specific page table and MMU setup constants
 ******************************************************************************/
#define PLAT_VIRT_ADDR_SPACE_SIZE     (1ULL << 32)
#define PLAT_PHY_ADDR_SPACE_SIZE      (1ULL << 32)
#define MAX_XLAT_TABLES               20
#define MAX_MMAP_REGIONS              25

/*******************************************************************************
 * Declarations and constants to access the mailboxes safely. Each mailbox is
 * aligned on the biggest cache line size in the platform. This is known only
 * to the platform as it might have a combination of integrated and external
 * caches. Such alignment ensures that two maiboxes do not sit on the same cache
 * line at any cache level. They could belong to different cpus/clusters &
 * get written while being protected by different locks causing corruption of
 * a valid mailbox address.
 ******************************************************************************/
#define CACHE_WRITEBACK_SHIFT         6
#define CACHE_WRITEBACK_GRANULE       (1 << CACHE_WRITEBACK_SHIFT)

/*
 * Define GICD and GICC and GICR base
 */
#define PLAT_RK_GICD_BASE             BASE_GICD_BASE
#define PLAT_RK_GICR_BASE             BASE_GICR_BASE
#define PLAT_RK_GICC_BASE             0

#define PLAT_RK_UART_BASE             UART2_BASE
#define PLAT_RK_UART_CLOCK            RK3399_UART_CLOCK
#define PLAT_RK_UART_BAUDRATE         RK3399_BAUDRATE

#define PLAT_RK_CCI_BASE              CCI500_BASE

#define PLAT_RK_PRIMARY_CPU           0x0

#define PSRAM_DO_DDR_RESUME           1
#define PSRAM_CHECK_WAKEUP_CPU        0


#define ENPER_CS_TRAINING_FREQ        (666)
#define TDFI_LAT_THRESHOLD_FREQ       (928)
#define PHY_DLL_BYPASS_FREQ           (260)

#define ERROR(...)                    printf("ERROR:   " __VA_ARGS__)

#define M0_BINCODE_BASE               ((uintptr_t)rk3399m0_bin)
#define M0_PARAM_ADDR                 (M0_BINCODE_BASE + PARAM_ADDR)
#define M0PMU_BINCODE_BASE            ((uintptr_t)rk3399m0pmu_bin)

/* pmu_fw.c */
extern char rk3399m0_bin[];
extern char rk3399m0_bin_end[];

extern char rk3399m0pmu_bin[];
extern char rk3399m0pmu_bin_end[];

extern void m0_init(void);
extern void m0_start(void);
extern void m0_stop(void);
extern void m0_wait_done(void);
extern void m0_configure_execute_addr(uintptr_t addr);

/* Function to help build the psci capabilities bitfield */

static inline unsigned int
define_psci_cap(unsigned int x)
{
    return U(1) << (x & U(0x1f));
}

/* Power state helper functions */

static inline unsigned int
psci_get_pstate_id(unsigned int power_state)
{
    return ((power_state) >> PSTATE_ID_SHIFT) & PSTATE_ID_MASK;
}

static inline unsigned int
psci_get_pstate_type(unsigned int power_state)
{
    return ((power_state) >> PSTATE_TYPE_SHIFT) & PSTATE_TYPE_MASK;
}

static inline unsigned int
psci_check_power_state(unsigned int power_state)
{
    return ((power_state)&PSTATE_VALID_MASK);
}

/*
 * These are the states reported by the PSCI_AFFINITY_INFO API for the specified
 * CPU. The definitions of these states can be found in Section 5.7.1 in the
 * PSCI specification (ARM DEN 0022C).
 */
typedef enum
{
    AFF_STATE_ON         = U(0),
    AFF_STATE_OFF        = U(1),
    AFF_STATE_ON_PENDING = U(2)
} aff_info_state_t;

/*
 * These are the power states reported by PSCI_NODE_HW_STATE API for the
 * specified CPU. The definitions of these states can be found in Section 5.15.3
 * of PSCI specification (ARM DEN 0022C).
 */
#define HW_ON                0
#define HW_OFF               1
#define HW_STANDBY           2

/*
 * Macro to represent invalid affinity level within PSCI.
 */
#define PSCI_INVALID_PWR_LVL (PLAT_MAX_PWR_LVL + U(1))

/*
 * Type for representing the local power state at a particular level.
 */
typedef uint8_t plat_local_state_t;

/* The local state macro used to represent RUN state. */
#define PSCI_LOCAL_STATE_RUN U(0)

/*
 * Function to test whether the plat_local_state is RUN state
 */
static inline int
is_local_state_run(unsigned int plat_local_state)
{
    return (plat_local_state == PSCI_LOCAL_STATE_RUN) ? 1 : 0;
}

/*
 * Function to test whether the plat_local_state is RETENTION state
 */
static inline int
is_local_state_retn(unsigned int plat_local_state)
{
    return ((plat_local_state > PSCI_LOCAL_STATE_RUN) && (plat_local_state <= PLAT_MAX_RET_STATE)) ? 1 : 0;
}

/*
 * Function to test whether the plat_local_state is OFF state
 */
static inline int
is_local_state_off(unsigned int plat_local_state)
{
    return ((plat_local_state > PLAT_MAX_RET_STATE) && (plat_local_state <= PLAT_MAX_OFF_STATE)) ? 1 : 0;
}

/*****************************************************************************
 * This data structure defines the representation of the power state parameter
 * for its exchange between the generic PSCI code and the platform port. For
 * example, it is used by the platform port to specify the requested power
 * states during a power management operation. It is used by the generic code to
 * inform the platform about the target power states that each level should
 * enter.
 ****************************************************************************/
typedef struct psci_power_state
{
    /*
     * The pwr_domain_state[] stores the local power state at each level
     * for the CPU.
     */
    plat_local_state_t pwr_domain_state[PLAT_MAX_PWR_LVL + U(1)];
#if PSCI_OS_INIT_MODE
    /*
     * The highest power level at which the current CPU is the last running
     * CPU.
     */
    unsigned int last_at_pwrlvl;
#endif
} psci_power_state_t;

/*******************************************************************************
 * Structure used to store per-cpu information relevant to the PSCI service.
 * It is populated in the per-cpu data array. In return we get a guarantee that
 * this information will not reside on a cache line shared with another cpu.
 ******************************************************************************/
typedef struct psci_cpu_data
{
    /* State as seen by PSCI Affinity Info API */
    aff_info_state_t aff_info_state;

    /*
     * Highest power level which takes part in a power management
     * operation.
     */
    unsigned int target_pwrlvl;

    /* The local power state of this CPU */
    plat_local_state_t local_state;
} psci_cpu_data_t;

/*******************************************************************************
 * Structure populated by platform specific code to export routines which
 * perform common low level power management functions
 ******************************************************************************/
typedef struct plat_psci_ops
{
    void (*cpu_standby)(plat_local_state_t cpu_state);
    int (*pwr_domain_on)(u_register_t mpidr);
    void (*pwr_domain_off)(const psci_power_state_t *target_state);
    int (*pwr_domain_off_early)(const psci_power_state_t *target_state);
#if PSCI_OS_INIT_MODE
    int (*pwr_domain_validate_suspend)(const psci_power_state_t *target_state);
#endif
    void (*pwr_domain_suspend_pwrdown_early)(const psci_power_state_t *target_state);
    void (*pwr_domain_suspend)(const psci_power_state_t *target_state);
    void (*pwr_domain_on_finish)(const psci_power_state_t *target_state);
    void (*pwr_domain_on_finish_late)(const psci_power_state_t *target_state);
    void (*pwr_domain_suspend_finish)(const psci_power_state_t *target_state);
    void __dead2 (*pwr_domain_pwr_down_wfi)(const psci_power_state_t *target_state);
    void __dead2 (*system_off)(void);
    void __dead2 (*system_reset)(void);
    int (*validate_power_state)(unsigned int power_state, psci_power_state_t *req_state);
    int (*validate_ns_entrypoint)(uintptr_t ns_entrypoint);
    void (*get_sys_suspend_power_state)(psci_power_state_t *req_state);
    int (*get_pwr_lvl_state_idx)(plat_local_state_t pwr_domain_state, int pwrlvl);
    int (*translate_power_state_by_mpidr)(u_register_t mpidr, unsigned int power_state,
                                          psci_power_state_t *output_state);
    int (*get_node_hw_state)(u_register_t mpidr, unsigned int power_level);
    int (*mem_protect_chk)(uintptr_t base, u_register_t length);
    int (*read_mem_protect)(int *val);
    int (*write_mem_protect)(int val);
    int (*system_reset2)(int is_vendor, int reset_type, u_register_t cookie);
} plat_psci_ops_t;

/*******************************************************************************
 * Function & Data prototypes
 ******************************************************************************/
unsigned int psci_version(void);
int          psci_cpu_on(u_register_t target_cpu, uintptr_t entrypoint, u_register_t context_id);
int          psci_cpu_suspend(unsigned int power_state, uintptr_t entrypoint, u_register_t context_id);
int          psci_system_suspend(uintptr_t entrypoint, u_register_t context_id);
int          psci_cpu_off(void);
int          psci_affinity_info(u_register_t target_affinity, unsigned int lowest_affinity_level);
int          psci_migrate(u_register_t target_cpu);
int          psci_migrate_info_type(void);
u_register_t psci_migrate_info_up_cpu(void);
int          psci_node_hw_state(u_register_t target_cpu, unsigned int power_level);
int          psci_features(unsigned int psci_fid);
#if PSCI_OS_INIT_MODE
int psci_set_suspend_mode(unsigned int mode);
#endif
void __dead2 psci_power_down_wfi(void);
void         psci_arch_setup(void);

/*
 * Define GICD and GICC and GICR base
 */
#define PLAT_RK_GICD_BASE      BASE_GICD_BASE
#define PLAT_RK_GICR_BASE      BASE_GICR_BASE
#define PLAT_RK_GICC_BASE      0

#define PLAT_RK_UART_BASE      UART2_BASE
#define PLAT_RK_UART_CLOCK     RK3399_UART_CLOCK
#define PLAT_RK_UART_BAUDRATE  RK3399_BAUDRATE

#define PLAT_RK_CCI_BASE       CCI500_BASE

#define PLAT_RK_PRIMARY_CPU    0x0

#define PSRAM_DO_DDR_RESUME    1
#define PSRAM_CHECK_WAKEUP_CPU 0

/*
 * These are the power states reported by PSCI_NODE_HW_STATE API for the
 * specified CPU. The definitions of these states can be found in Section 5.15.3
 * of PSCI specification (ARM DEN 0022C).
 */
#define HW_ON                  0
#define HW_OFF                 1
#define HW_STANDBY             2

/*
 * Macro to represent invalid affinity level within PSCI.
 */
#define PSCI_INVALID_PWR_LVL   (PLAT_MAX_PWR_LVL + U(1))

/*
 * Type for representing the local power state at a particular level.
 */
typedef uint8_t plat_local_state_t;

/* The local state macro used to represent RUN state. */
#define PSCI_LOCAL_STATE_RUN U(0)


void plat_cci_init(void);
void plat_cci_enable(void);
void plat_cci_disable(void);

void plat_delay_timer_init(void);

void params_early_setup(u_register_t plat_params_from_bl2);

void plat_rockchip_gic_driver_init(void);
void plat_rockchip_gic_init(void);
void plat_rockchip_gic_cpuif_enable(void);
void plat_rockchip_gic_cpuif_disable(void);
void plat_rockchip_gic_pcpu_init(void);

void      plat_rockchip_pmu_init(void);
void      plat_rockchip_soc_init(void);
uintptr_t plat_get_sec_entrypoint(void);

void platform_cpu_warmboot(void);

struct bl_aux_gpio_info    *plat_get_rockchip_gpio_reset(void);
struct bl_aux_gpio_info    *plat_get_rockchip_gpio_poweroff(void);
struct bl_aux_gpio_info    *plat_get_rockchip_suspend_gpio(uint32_t *count);
struct bl_aux_rk_apio_info *plat_get_rockchip_suspend_apio(void);
void                        plat_rockchip_gpio_init(void);
void                        plat_rockchip_save_gpio(void);
void                        plat_rockchip_restore_gpio(void);

int rockchip_soc_cores_pwr_dm_on(unsigned long mpidr, uint64_t entrypoint);
int rockchip_soc_hlvl_pwr_dm_off(uint32_t lvl, plat_local_state_t lvl_state);
int rockchip_soc_cores_pwr_dm_off(void);
int rockchip_soc_sys_pwr_dm_suspend(void);
int rockchip_soc_cores_pwr_dm_suspend(void);
int rockchip_soc_hlvl_pwr_dm_suspend(uint32_t lvl, plat_local_state_t lvl_state);
int rockchip_soc_hlvl_pwr_dm_on_finish(uint32_t lvl, plat_local_state_t lvl_state);
int rockchip_soc_cores_pwr_dm_on_finish(void);
int rockchip_soc_sys_pwr_dm_resume(void);

int          rockchip_soc_hlvl_pwr_dm_resume(uint32_t lvl, plat_local_state_t lvl_state);
int          rockchip_soc_cores_pwr_dm_resume(void);
void __dead2 rockchip_soc_soft_reset(void);
void __dead2 rockchip_soc_system_off(void);
void __dead2 rockchip_soc_cores_pd_pwr_dn_wfi(const psci_power_state_t *target_state);
void __dead2 rockchip_soc_sys_pd_pwr_dn_wfi(void);

extern const unsigned char rockchip_power_domain_tree_desc[];

/*
 * Structure for specifying a single region of memory.
 */
typedef struct mmap_region
{
    unsigned long long base_pa;
    uintptr_t          base_va;
    size_t             size;
    unsigned int       attr;
} mmap_region_t;

/* Generic translation table APIs */
void init_xlat_tables(void);
void mmap_add_region(unsigned long long base_pa, uintptr_t base_va, size_t size, unsigned int attr);
void mmap_add(const mmap_region_t *mm);

extern void               *pmu_cpuson_entrypoint;
extern u_register_t        cpuson_entry_point[PLATFORM_CORE_COUNT];
extern uint32_t            cpuson_flags[PLATFORM_CORE_COUNT];
extern const mmap_region_t plat_rk_mmap[];

uint32_t rockchip_get_uart_base(void);
uint32_t rockchip_get_uart_baudrate(void);
uint32_t rockchip_get_uart_clock(void);

/* export related and operating SoC APIs */
void __dead2       soc_global_soft_reset(void);
void               disable_dvfs_plls(void);
void               disable_nodvfs_plls(void);
void               enable_dvfs_plls(void);
void               enable_nodvfs_plls(void);
void               prepare_abpll_for_ddrctrl(void);
void               restore_abpll(void);
void               clk_gate_con_save(void);
void               clk_gate_con_disable(void);
void               clk_gate_con_restore(void);
void               set_pmu_rsthold(void);
void               pmu_sgrf_rst_hld(void);
__pmusramfunc void pmu_sgrf_rst_hld_release(void);
__pmusramfunc void restore_pmu_rsthold(void);

typedef struct mem_region
{
    uintptr_t base;
    size_t    nbytes;
} mem_region_t;

/*
 * zero_normalmem all the regions defined in tbl.
 */
void clear_mem_regions(mem_region_t *tbl, size_t nregions);

/*
 * zero_normalmem all the regions defined in region. It dynamically
 * maps chunks of 'chunk_size' in 'va' virtual address and clears them.
 * For this reason memory regions must be multiple of chunk_size and
 * must be aligned to it as well. chunk_size and va can be selected
 * in a way that they minimize the number of entries used in the
 * translation tables.
 */
void clear_map_dyn_mem_regions(struct mem_region *regions, size_t nregions, uintptr_t va, size_t chunk);

/*
 * checks that a region (addr + nbytes-1) of memory is totally covered by
 * one of the regions defined in tbl. Caller must ensure that (addr+nbytes-1)
 * doesn't overflow.
 */
int mem_region_in_array_chk(mem_region_t *tbl, size_t nregions, uintptr_t addr, size_t nbytes);

/*
 * Fill a region of normal memory of size "length" in bytes with zero bytes.
 *
 * WARNING: This function can only operate on normal memory. This means that
 *          the MMU must be enabled when using this function. Otherwise, use
 *          zeromem.
 */
void zero_normalmem(void *mem, u_register_t length);

/*
 * Fill a region of memory of size "length" in bytes with null bytes.
 *
 * Unlike zero_normalmem, this function has no restriction on the type of
 * memory targeted and can be used for any device memory as well as normal
 * memory. This function must be used instead of zero_normalmem when MMU is
 * disabled.
 *
 * NOTE: When data cache and MMU are enabled, prefer zero_normalmem for faster
 *       zeroing.
 */
void zeromem(void *mem, u_register_t length);

/*
 * Utility function to return the address of a symbol. By default, the
 * compiler generates adr/adrp instruction pair to return the reference
 * to the symbol and this utility is used to override this compiler
 * generated to code to use `ldr` instruction.
 *
 * This helps when Position Independent Executable needs to reference a symbol
 * which is constant and does not depend on the execute address of the binary.
 */
#define DEFINE_LOAD_SYM_ADDR(_name)                                   \
    static inline u_register_t load_addr_##_name(void)                \
    {                                                                 \
        u_register_t v;                                               \
        __asm__ volatile("ldr %0, =" #_name : "=r"(v) : "X"(#_name)); \
        return v;                                                     \
    }

/* Helper to invoke the function defined by DEFINE_LOAD_SYM_ADDR() */
#define LOAD_ADDR_OF(_name)    (typeof(_name) *)load_addr_##_name()

/* mr0 for ddr3 */
#define DDR3_BL8               (0)
#define DDR3_BC4_8             (1)
#define DDR3_BC4               (2)
#define DDR3_CL(n)             (((((n)-4) & 0x7) << 4) | ((((n)-4) & 0x8) >> 1))
#define DDR3_WR(n)             (((n) & 0x7) << 9)
#define DDR3_DLL_RESET         (1 << 8)
#define DDR3_DLL_DERESET       (0 << 8)

/* mr1 for ddr3 */
#define DDR3_DLL_ENABLE        (0)
#define DDR3_DLL_DISABLE       (1)
#define DDR3_MR1_AL(n)         (((n) & 0x3) << 3)

#define DDR3_DS_40             (0)
#define DDR3_DS_34             (1 << 1)
#define DDR3_RTT_NOM_DIS       (0)
#define DDR3_RTT_NOM_60        (1 << 2)
#define DDR3_RTT_NOM_120       (1 << 6)
#define DDR3_RTT_NOM_40        ((1 << 2) | (1 << 6))
#define DDR3_TDQS              (1 << 11)

/* mr2 for ddr3 */
#define DDR3_MR2_CWL(n)        ((((n)-5) & 0x7) << 3)
#define DDR3_RTT_WR_DIS        (0)
#define DDR3_RTT_WR_60         (1 << 9)
#define DDR3_RTT_WR_120        (2 << 9)

/*
 * MR0 (Device Information)
 * 0:DAI complete, 1:DAI still in progress
 */
#define LPDDR2_DAI             (0x1)
/* 0:S2 or S4 SDRAM, 1:NVM */
#define LPDDR2_DI              (0x1 << 1)
/* 0:DNV not supported, 1:DNV supported */
#define LPDDR2_DNVI            (0x1 << 2)
#define LPDDR2_RZQI            (0x3 << 3)

/*
 * 00:RZQ self test not supported,
 * 01:ZQ-pin may connect to VDDCA or float
 * 10:ZQ-pin may short to GND.
 * 11:ZQ-pin self test completed, no error condition detected.
 */

/* MR1 (Device Feature) */
#define LPDDR2_BL4             (0x2)
#define LPDDR2_BL8             (0x3)
#define LPDDR2_BL16            (0x4)
#define LPDDR2_N_WR(n)         (((n)-2) << 5)

/* MR2 (Device Feature 2) */
#define LPDDR2_RL3_WL1         (0x1)
#define LPDDR2_RL4_WL2         (0x2)
#define LPDDR2_RL5_WL2         (0x3)
#define LPDDR2_RL6_WL3         (0x4)
#define LPDDR2_RL7_WL4         (0x5)
#define LPDDR2_RL8_WL4         (0x6)

/* MR3 (IO Configuration 1) */
#define LPDDR2_DS_34           (0x1)
#define LPDDR2_DS_40           (0x2)
#define LPDDR2_DS_48           (0x3)
#define LPDDR2_DS_60           (0x4)
#define LPDDR2_DS_80           (0x6)
/* optional */
#define LPDDR2_DS_120          (0x7)

/* MR4 (Device Temperature) */
#define LPDDR2_TREF_MASK       (0x7)
#define LPDDR2_4_TREF          (0x1)
#define LPDDR2_2_TREF          (0x2)
#define LPDDR2_1_TREF          (0x3)
#define LPDDR2_025_TREF        (0x5)
#define LPDDR2_025_TREF_DERATE (0x6)

#define LPDDR2_TUF             (0x1 << 7)

/* MR8 (Basic configuration 4) */
#define LPDDR2_S4              (0x0)
#define LPDDR2_S2              (0x1)
#define LPDDR2_N               (0x2)
/* Unit:MB */
#define LPDDR2_DENSITY(mr8)    (8 << (((mr8) >> 2) & 0xf))
#define LPDDR2_IO_WIDTH(mr8)   (32 >> (((mr8) >> 6) & 0x3))

/* MR10 (Calibration) */
#define LPDDR2_ZQINIT          (0xff)
#define LPDDR2_ZQCL            (0xab)
#define LPDDR2_ZQCS            (0x56)
#define LPDDR2_ZQRESET         (0xc3)

/* MR16 (PASR Bank Mask), S2 SDRAM Only */
#define LPDDR2_PASR_FULL       (0x0)
#define LPDDR2_PASR_1_2        (0x1)
#define LPDDR2_PASR_1_4        (0x2)
#define LPDDR2_PASR_1_8        (0x3)

/*
 * MR0 (Device Information)
 * 0:DAI complete,
 * 1:DAI still in progress
 */
#define LPDDR3_DAI             (0x1)
/*
 * 00:RZQ self test not supported,
 * 01:ZQ-pin may connect to VDDCA or float
 * 10:ZQ-pin may short to GND.
 * 11:ZQ-pin self test completed, no error condition detected.
 */
#define LPDDR3_RZQI            (0x3 << 3)
/*
 * 0:DRAM does not support WL(Set B),
 * 1:DRAM support WL(Set B)
 */
#define LPDDR3_WL_SUPOT        (1 << 6)
/*
 * 0:DRAM does not support RL=3,nWR=3,WL=1;
 * 1:DRAM supports RL=3,nWR=3,WL=1 for frequencies <=166
 */
#define LPDDR3_RL3_SUPOT       (1 << 7)

/* MR1 (Device Feature) */
#define LPDDR3_BL8             (0x3)
#define LPDDR3_N_WR(n)         ((n) << 5)

/* MR2 (Device Feature 2), WL Set A,default */
/* <=166MHz,optional*/
#define LPDDR3_RL3_WL1         (0x1)
/* <=400MHz*/
#define LPDDR3_RL6_WL3         (0x4)
/* <=533MHz*/
#define LPDDR3_RL8_WL4         (0x6)
/* <=600MHz*/
#define LPDDR3_RL9_WL5         (0x7)
/* <=667MHz,default*/
#define LPDDR3_RL10_WL6        (0x8)
/* <=733MHz*/
#define LPDDR3_RL11_WL6        (0x9)
/* <=800MHz*/
#define LPDDR3_RL12_WL6        (0xa)
/* <=933MHz*/
#define LPDDR3_RL14_WL8        (0xc)
/* <=1066MHz*/
#define LPDDR3_RL16_WL8        (0xe)

/* WL Set B, optional */
/* <=667MHz,default*/
#define LPDDR3_RL10_WL8        (0x8)
/* <=733MHz*/
#define LPDDR3_RL11_WL9        (0x9)
/* <=800MHz*/
#define LPDDR3_RL12_WL9        (0xa)
/* <=933MHz*/
#define LPDDR3_RL14_WL11       (0xc)
/* <=1066MHz*/
#define LPDDR3_RL16_WL13       (0xe)

/* 1:enable nWR programming > 9(default)*/
#define LPDDR3_N_WRE           (1 << 4)
/* 1:Select WL Set B*/
#define LPDDR3_WL_S            (1 << 6)
/* 1:enable*/
#define LPDDR3_WR_LEVEL        (1 << 7)

/* MR3 (IO Configuration 1) */
#define LPDDR3_DS_34           (0x1)
#define LPDDR3_DS_40           (0x2)
#define LPDDR3_DS_48           (0x3)
#define LPDDR3_DS_60           (0x4)
#define LPDDR3_DS_80           (0x6)
#define LPDDR3_DS_34D_40U      (0x9)
#define LPDDR3_DS_40D_48U      (0xa)
#define LPDDR3_DS_34D_48U      (0xb)

/* MR4 (Device Temperature) */
#define LPDDR3_TREF_MASK       (0x7)
/* SDRAM Low temperature operating limit exceeded */
#define LPDDR3_LT_EXED         (0x0)
#define LPDDR3_4_TREF          (0x1)
#define LPDDR3_2_TREF          (0x2)
#define LPDDR3_1_TREF          (0x3)
#define LPDDR3_05_TREF         (0x4)
#define LPDDR3_025_TREF        (0x5)
#define LPDDR3_025_TREF_DERATE (0x6)
/* SDRAM High temperature operating limit exceeded */
#define LPDDR3_HT_EXED         (0x7)

/* 1:value has changed since last read of MR4 */
#define LPDDR3_TUF             (0x1 << 7)

/* MR8 (Basic configuration 4) */
#define LPDDR3_S8              (0x3)
#define LPDDR3_DENSITY(mr8)    (8 << (((mr8) >> 2) & 0xf))
#define LPDDR3_IO_WIDTH(mr8)   (32 >> (((mr8) >> 6) & 0x3))

/* MR10 (Calibration) */
#define LPDDR3_ZQINIT          (0xff)
#define LPDDR3_ZQCL            (0xab)
#define LPDDR3_ZQCS            (0x56)
#define LPDDR3_ZQRESET         (0xc3)

/* MR11 (ODT Control) */
#define LPDDR3_ODT_60          (1)
#define LPDDR3_ODT_120         (2)

#define LPDDR4_RL24_NRTP10     (0x4)
#define LPDDR4_RL28_NRTP12     (0x5)
#define LPDDR4_RL32_NRTP14     (0x6)
#define LPDDR4_RL36_NRTP16     (0x7)
/* RL & nRTP for DBI-RD Disabled */
#define LPDDR4_RL12_NRTP8      (0x1)
#define LPDDR4_RL16_NRTP8      (0x2)
#define LPDDR4_RL22_NRTP8      (0x3)
#define LPDDR4_RL28_NRTP10     (0x4)
#define LPDDR4_RL32_NRTP12     (0x5)
#define LPDDR4_RL36_NRTP14     (0x6)
#define LPDDR4_RL40_NRTP16     (0x7)
/* WL Set A,default */
#define LPDDR4_A_WL4           (0x0)
#define LPDDR4_A_WL6           (0x1)
#define LPDDR4_A_WL8           (0x2)
#define LPDDR4_A_WL10          (0x3)
#define LPDDR4_A_WL12          (0x4)
#define LPDDR4_A_WL14          (0x5)
#define LPDDR4_A_WL16          (0x6)
#define LPDDR4_A_WL18          (0x7)
/* WL Set B, optional */
#define LPDDR4_B_WL4           (0x0 << 3)
#define LPDDR4_B_WL8           (0x1 << 3)
#define LPDDR4_B_WL12          (0x2 << 3)
#define LPDDR4_B_WL18          (0x3 << 3)
#define LPDDR4_B_WL22          (0x4 << 3)
#define LPDDR4_B_WL26          (0x5 << 3)
#define LPDDR4_B_WL30          (0x6 << 3)
#define LPDDR4_B_WL34          (0x7 << 3)
/* 1:Select WL Set B*/
#define LPDDR4_WL_B            (1 << 6)
/* 1:enable*/
#define LPDDR4_WR_LEVEL        (1 << 7)

/* MR3 */
#define LPDDR4_VDDQ_2_5        (0)
#define LPDDR4_VDDQ_3          (1)
#define LPDDR4_WRPST_0_5_TCK   (0 << 1)
#define LPDDR4_WRPST_1_5_TCK   (1 << 1)
#define LPDDR4_PPR_EN          (1 << 2)
/* PDDS */
#define LPDDR4_PDDS_240        (0x1 << 3)
#define LPDDR4_PDDS_120        (0x2 << 3)
#define LPDDR4_PDDS_80         (0x3 << 3)
#define LPDDR4_PDDS_60         (0x4 << 3)
#define LPDDR4_PDDS_48         (0x5 << 3)
#define LPDDR4_PDDS_40         (0x6 << 3)
#define LPDDR4_DBI_RD_EN       (1 << 6)
#define LPDDR4_DBI_WR_EN       (1 << 7)

/* MR11 (ODT Control) */
#define LPDDR4_DQODT_240       (1)
#define LPDDR4_DQODT_120       (2)
#define LPDDR4_DQODT_80        (3)
#define LPDDR4_DQODT_60        (4)
#define LPDDR4_DQODT_48        (5)
#define LPDDR4_DQODT_40        (6)
#define LPDDR4_DQODT_DIS       (0)
#define LPDDR4_CAODT_240       (1 << 4)
#define LPDDR4_CAODT_120       (2 << 4)
#define LPDDR4_CAODT_80        (3 << 4)
#define LPDDR4_CAODT_60        (4 << 4)
#define LPDDR4_CAODT_48        (5 << 4)
#define LPDDR4_CAODT_40        (6 << 4)
#define LPDDR4_CAODT_DIS       (0 << 4)

#define CTX_GPREGS_OFFSET      U(0x0)
#define CTX_GPREG_X0           U(0x0)
#define CTX_GPREG_X1           U(0x8)
#define CTX_GPREG_X2           U(0x10)
#define CTX_GPREG_X3           U(0x18)
#define CTX_GPREG_X4           U(0x20)
#define CTX_GPREG_X5           U(0x28)
#define CTX_GPREG_X6           U(0x30)
#define CTX_GPREG_X7           U(0x38)
#define CTX_GPREG_X8           U(0x40)
#define CTX_GPREG_X9           U(0x48)
#define CTX_GPREG_X10          U(0x50)
#define CTX_GPREG_X11          U(0x58)
#define CTX_GPREG_X12          U(0x60)
#define CTX_GPREG_X13          U(0x68)
#define CTX_GPREG_X14          U(0x70)
#define CTX_GPREG_X15          U(0x78)
#define CTX_GPREG_X16          U(0x80)
#define CTX_GPREG_X17          U(0x88)
#define CTX_GPREG_X18          U(0x90)
#define CTX_GPREG_X19          U(0x98)
#define CTX_GPREG_X20          U(0xa0)
#define CTX_GPREG_X21          U(0xa8)
#define CTX_GPREG_X22          U(0xb0)
#define CTX_GPREG_X23          U(0xb8)
#define CTX_GPREG_X24          U(0xc0)
#define CTX_GPREG_X25          U(0xc8)
#define CTX_GPREG_X26          U(0xd0)
#define CTX_GPREG_X27          U(0xd8)
#define CTX_GPREG_X28          U(0xe0)
#define CTX_GPREG_X29          U(0xe8)
#define CTX_GPREG_LR           U(0xf0)
#define CTX_GPREG_SP_EL0       U(0xf8)
#define CTX_GPREGS_END         U(0x100)

/*******************************************************************************
 * Constants that allow assembler code to access members of and the 'el3_state'
 * structure at their correct offsets. Note that some of the registers are only
 * 32-bits wide but are stored as 64-bit values for convenience
 ******************************************************************************/
#define CTX_EL3STATE_OFFSET    (CTX_GPREGS_OFFSET + CTX_GPREGS_END)
#define CTX_SCR_EL3            U(0x0)
#define CTX_ESR_EL3            U(0x8)
#define CTX_RUNTIME_SP         U(0x10)
#define CTX_SPSR_EL3           U(0x18)
#define CTX_ELR_EL3            U(0x20)
#define CTX_PMCR_EL0           U(0x28)
#define CTX_IS_IN_EL3          U(0x30)
#define CTX_MDCR_EL3           U(0x38)
/* Constants required in supporting nested exception in EL3 */
#define CTX_SAVED_ELR_EL3      U(0x40)

#define SMC_TYPE_YIELD         UL(0)
#define SMC_OK                 ULL(0)
#define SMC_UNK                -1
#define SMC_PREEMPTED          -2 /* Not defined by the SMCCC */
#define SMC_DENIED             -3 /* Not defined by the SMCCC */
#define SMC_INVALID_PARAM      -4 /* Not defined by the SMCCC */

/*******************************************************************************
 * Constants that allow assembler code to access members of and the 'gp_regs'
 * structure at their correct offsets.
 ******************************************************************************/
#define CTX_GPREGS_OFFSET      U(0x0)
#define CTX_GPREG_X0           U(0x0)
#define CTX_GPREG_X1           U(0x8)
#define CTX_GPREG_X2           U(0x10)
#define CTX_GPREG_X3           U(0x18)
#define CTX_GPREG_X4           U(0x20)
#define CTX_GPREG_X5           U(0x28)
#define CTX_GPREG_X6           U(0x30)
#define CTX_GPREG_X7           U(0x38)
#define CTX_GPREG_X8           U(0x40)
#define CTX_GPREG_X9           U(0x48)
#define CTX_GPREG_X10          U(0x50)
#define CTX_GPREG_X11          U(0x58)
#define CTX_GPREG_X12          U(0x60)
#define CTX_GPREG_X13          U(0x68)
#define CTX_GPREG_X14          U(0x70)
#define CTX_GPREG_X15          U(0x78)
#define CTX_GPREG_X16          U(0x80)
#define CTX_GPREG_X17          U(0x88)
#define CTX_GPREG_X18          U(0x90)
#define CTX_GPREG_X19          U(0x98)
#define CTX_GPREG_X20          U(0xa0)
#define CTX_GPREG_X21          U(0xa8)
#define CTX_GPREG_X22          U(0xb0)
#define CTX_GPREG_X23          U(0xb8)
#define CTX_GPREG_X24          U(0xc0)
#define CTX_GPREG_X25          U(0xc8)
#define CTX_GPREG_X26          U(0xd0)
#define CTX_GPREG_X27          U(0xd8)
#define CTX_GPREG_X28          U(0xe0)
#define CTX_GPREG_X29          U(0xe8)
#define CTX_GPREG_LR           U(0xf0)
#define CTX_GPREG_SP_EL0       U(0xf8)
#define CTX_GPREGS_END         U(0x100)

/*******************************************************************************
 * Constants that allow assembler code to access members of and the 'el3_state'
 * structure at their correct offsets. Note that some of the registers are only
 * 32-bits wide but are stored as 64-bit values for convenience
 ******************************************************************************/
#define CTX_EL3STATE_OFFSET    (CTX_GPREGS_OFFSET + CTX_GPREGS_END)
#define CTX_SCR_EL3            U(0x0)
#define CTX_ESR_EL3            U(0x8)
#define CTX_RUNTIME_SP         U(0x10)
#define CTX_SPSR_EL3           U(0x18)
#define CTX_ELR_EL3            U(0x20)
#define CTX_PMCR_EL0           U(0x28)
#define CTX_IS_IN_EL3          U(0x30)
#define CTX_MDCR_EL3           U(0x38)
/* Constants required in supporting nested exception in EL3 */
#define CTX_SAVED_ELR_EL3      U(0x40)
/*
 * General purpose flag, to save various EL3 states
 * FFH mode : Used to identify if handling nested exception
 * KFH mode : Used as counter value
 */
#define CTX_NESTED_EA_FLAG     U(0x48)
#if FFH_SUPPORT
#    define CTX_SAVED_ESR_EL3  U(0x50)
#    define CTX_SAVED_SPSR_EL3 U(0x58)
#    define CTX_SAVED_GPREG_LR U(0x60)
#    define CTX_EL3STATE_END   U(0x70) /* Align to the next 16 byte boundary */
#else
#    define CTX_EL3STATE_END U(0x50)   /* Align to the next 16 byte boundary */
#endif                                 /* FFH_SUPPORT */

/*******************************************************************************
 * Constants that allow assembler code to access members of and the
 * 'el1_sys_regs' structure at their correct offsets. Note that some of the
 * registers are only 32-bits wide but are stored as 64-bit values for
 * convenience
 ******************************************************************************/
#define CTX_EL1_SYSREGS_OFFSET (CTX_EL3STATE_OFFSET + CTX_EL3STATE_END)
#define CTX_SPSR_EL1           U(0x0)
#define CTX_ELR_EL1            U(0x8)
#define CTX_SCTLR_EL1          U(0x10)
#define CTX_TCR_EL1            U(0x18)
#define CTX_CPACR_EL1          U(0x20)
#define CTX_CSSELR_EL1         U(0x28)
#define CTX_SP_EL1             U(0x30)
#define CTX_ESR_EL1            U(0x38)
#define CTX_TTBR0_EL1          U(0x40)
#define CTX_TTBR1_EL1          U(0x48)
#define CTX_MAIR_EL1           U(0x50)
#define CTX_AMAIR_EL1          U(0x58)
#define CTX_ACTLR_EL1          U(0x60)
#define CTX_TPIDR_EL1          U(0x68)
#define CTX_TPIDR_EL0          U(0x70)
#define CTX_TPIDRRO_EL0        U(0x78)
#define CTX_PAR_EL1            U(0x80)
#define CTX_FAR_EL1            U(0x88)
#define CTX_AFSR0_EL1          U(0x90)
#define CTX_AFSR1_EL1          U(0x98)
#define CTX_CONTEXTIDR_EL1     U(0xa0)
#define CTX_VBAR_EL1           U(0xa8)
#define CTX_MDCCINT_EL1        U(0xb0)
#define CTX_MDSCR_EL1          U(0xb8)

#define CTX_AARCH64_END        U(0xc0) /* Align to the next 16 byte boundary */

/*
 * If the platform is AArch64-only, there is no need to save and restore these
 * AArch32 registers.
 */
#if CTX_INCLUDE_AARCH32_REGS
#    define CTX_SPSR_ABT    (CTX_AARCH64_END + U(0x0))
#    define CTX_SPSR_UND    (CTX_AARCH64_END + U(0x8))
#    define CTX_SPSR_IRQ    (CTX_AARCH64_END + U(0x10))
#    define CTX_SPSR_FIQ    (CTX_AARCH64_END + U(0x18))
#    define CTX_DACR32_EL2  (CTX_AARCH64_END + U(0x20))
#    define CTX_IFSR32_EL2  (CTX_AARCH64_END + U(0x28))
#    define CTX_AARCH32_END (CTX_AARCH64_END + U(0x30)) /* Align to the next 16 byte boundary */
#else
#    define CTX_AARCH32_END CTX_AARCH64_END
#endif                                                  /* CTX_INCLUDE_AARCH32_REGS */

/*
 * If the timer registers aren't saved and restored, we don't have to reserve
 * space for them in the context
 */
#if NS_TIMER_SWITCH
#    define CTX_CNTP_CTL_EL0      (CTX_AARCH32_END + U(0x0))
#    define CTX_CNTP_CVAL_EL0     (CTX_AARCH32_END + U(0x8))
#    define CTX_CNTV_CTL_EL0      (CTX_AARCH32_END + U(0x10))
#    define CTX_CNTV_CVAL_EL0     (CTX_AARCH32_END + U(0x18))
#    define CTX_CNTKCTL_EL1       (CTX_AARCH32_END + U(0x20))
#    define CTX_TIMER_SYSREGS_END (CTX_AARCH32_END + U(0x30)) /* Align to the next 16 byte boundary */
#else
#    define CTX_TIMER_SYSREGS_END CTX_AARCH32_END
#endif                                                        /* NS_TIMER_SWITCH */

#if ENABLE_FEAT_MTE2
#    define CTX_TFSRE0_EL1   (CTX_TIMER_SYSREGS_END + U(0x0))
#    define CTX_TFSR_EL1     (CTX_TIMER_SYSREGS_END + U(0x8))
#    define CTX_RGSR_EL1     (CTX_TIMER_SYSREGS_END + U(0x10))
#    define CTX_GCR_EL1      (CTX_TIMER_SYSREGS_END + U(0x18))
#    define CTX_MTE_REGS_END (CTX_TIMER_SYSREGS_END + U(0x20)) /* Align to the next 16 byte boundary */
#else
#    define CTX_MTE_REGS_END CTX_TIMER_SYSREGS_END
#endif                                                         /* ENABLE_FEAT_MTE2 */

#if ENABLE_FEAT_RAS
#    define CTX_DISR_EL1     (CTX_MTE_REGS_END + U(0x0))
#    define CTX_RAS_REGS_END (CTX_MTE_REGS_END + U(0x10)) /* Align to the next 16 byte boundary */
#else
#    define CTX_RAS_REGS_END CTX_MTE_REGS_END
#endif                                                    /* ENABLE_FEAT_RAS */

#if ENABLE_FEAT_S1PIE
#    define CTX_PIRE0_EL1      (CTX_RAS_REGS_END + U(0x0))
#    define CTX_PIR_EL1        (CTX_RAS_REGS_END + U(0x8))
#    define CTX_S1PIE_REGS_END (CTX_RAS_REGS_END + U(0x10)) /* Align to the next 16 byte boundary */
#else
#    define CTX_S1PIE_REGS_END CTX_RAS_REGS_END
#endif                                                      /* ENABLE_FEAT_S1PIE */

#if ENABLE_FEAT_S1POE
#    define CTX_POR_EL1        (CTX_S1PIE_REGS_END + U(0x0))
#    define CTX_S1POE_REGS_END (CTX_S1PIE_REGS_END + U(0x10)) /* Align to the next 16 byte boundary */
#else
#    define CTX_S1POE_REGS_END CTX_S1PIE_REGS_END
#endif                                                        /* ENABLE_FEAT_S1POE */

#if ENABLE_FEAT_S2POE
#    define CTX_S2POR_EL1      (CTX_S1POE_REGS_END + U(0x0))
#    define CTX_S2POE_REGS_END (CTX_S1POE_REGS_END + U(0x10)) /* Align to the next 16 byte boundary */
#else
#    define CTX_S2POE_REGS_END CTX_S1POE_REGS_END
#endif                                                        /* ENABLE_FEAT_S2POE */

#if ENABLE_FEAT_TCR2
#    define CTX_TCR2_EL1      (CTX_S2POE_REGS_END + U(0x0))
#    define CTX_TCR2_REGS_END (CTX_S2POE_REGS_END + U(0x10)) /* Align to the next 16 byte boundary */
#else
#    define CTX_TCR2_REGS_END CTX_S2POE_REGS_END
#endif                                                       /* ENABLE_FEAT_TCR2 */

#if ENABLE_TRF_FOR_NS
#    define CTX_TRFCR_EL1    (CTX_TCR2_REGS_END + U(0x0))
#    define CTX_TRF_REGS_END (CTX_TCR2_REGS_END + U(0x10)) /* Align to the next 16 byte boundary */
#else
#    define CTX_TRF_REGS_END CTX_TCR2_REGS_END
#endif                                                     /* ENABLE_TRF_FOR_NS */

#if ENABLE_FEAT_CSV2_2
#    define CTX_SCXTNUM_EL0     (CTX_TRF_REGS_END + U(0x0))
#    define CTX_SCXTNUM_EL1     (CTX_TRF_REGS_END + U(0x8))
#    define CTX_CSV2_2_REGS_END (CTX_TRF_REGS_END + U(0x10)) /* Align to the next 16 byte boundary */
#else
#    define CTX_CSV2_2_REGS_END CTX_TRF_REGS_END
#endif                                                       /* ENABLE_FEAT_CSV2_2 */

#if ENABLE_FEAT_GCS
#    define CTX_GCSCR_EL1    (CTX_CSV2_2_REGS_END + U(0x0))
#    define CTX_GCSCRE0_EL1  (CTX_CSV2_2_REGS_END + U(0x8))
#    define CTX_GCSPR_EL1    (CTX_CSV2_2_REGS_END + U(0x10))
#    define CTX_GCSPR_EL0    (CTX_CSV2_2_REGS_END + U(0x18))
#    define CTX_GCS_REGS_END (CTX_CSV2_2_REGS_END + U(0x20)) /* Align to the next 16 byte boundary */
#else
#    define CTX_GCS_REGS_END CTX_CSV2_2_REGS_END
#endif                                                       /* ENABLE_FEAT_GCS */

/*
 * End of EL1 system registers.
 */
#define CTX_EL1_SYSREGS_END CTX_GCS_REGS_END

/*******************************************************************************
 * Constants that allow assembler code to access members of and the 'fp_regs'
 * structure at their correct offsets.
 ******************************************************************************/
#define CTX_FPREGS_OFFSET   (CTX_EL1_SYSREGS_OFFSET + CTX_EL1_SYSREGS_END)
#if CTX_INCLUDE_FPREGS
#    define CTX_FP_Q0   U(0x0)
#    define CTX_FP_Q1   U(0x10)
#    define CTX_FP_Q2   U(0x20)
#    define CTX_FP_Q3   U(0x30)
#    define CTX_FP_Q4   U(0x40)
#    define CTX_FP_Q5   U(0x50)
#    define CTX_FP_Q6   U(0x60)
#    define CTX_FP_Q7   U(0x70)
#    define CTX_FP_Q8   U(0x80)
#    define CTX_FP_Q9   U(0x90)
#    define CTX_FP_Q10  U(0xa0)
#    define CTX_FP_Q11  U(0xb0)
#    define CTX_FP_Q12  U(0xc0)
#    define CTX_FP_Q13  U(0xd0)
#    define CTX_FP_Q14  U(0xe0)
#    define CTX_FP_Q15  U(0xf0)
#    define CTX_FP_Q16  U(0x100)
#    define CTX_FP_Q17  U(0x110)
#    define CTX_FP_Q18  U(0x120)
#    define CTX_FP_Q19  U(0x130)
#    define CTX_FP_Q20  U(0x140)
#    define CTX_FP_Q21  U(0x150)
#    define CTX_FP_Q22  U(0x160)
#    define CTX_FP_Q23  U(0x170)
#    define CTX_FP_Q24  U(0x180)
#    define CTX_FP_Q25  U(0x190)
#    define CTX_FP_Q26  U(0x1a0)
#    define CTX_FP_Q27  U(0x1b0)
#    define CTX_FP_Q28  U(0x1c0)
#    define CTX_FP_Q29  U(0x1d0)
#    define CTX_FP_Q30  U(0x1e0)
#    define CTX_FP_Q31  U(0x1f0)
#    define CTX_FP_FPSR U(0x200)
#    define CTX_FP_FPCR U(0x208)
#    if CTX_INCLUDE_AARCH32_REGS
#        define CTX_FP_FPEXC32_EL2 U(0x210)
#        define CTX_FPREGS_END     U(0x220) /* Align to the next 16 byte boundary */
#    else
#        define CTX_FPREGS_END U(0x210)     /* Align to the next 16 byte boundary */
#    endif                                  /* CTX_INCLUDE_AARCH32_REGS */
#else
#    define CTX_FPREGS_END U(0)
#endif                                      /* CTX_INCLUDE_FPREGS */

/*******************************************************************************
 * Registers related to CVE-2018-3639
 ******************************************************************************/
#define CTX_CVE_2018_3639_OFFSET  (CTX_FPREGS_OFFSET + CTX_FPREGS_END)
#define CTX_CVE_2018_3639_DISABLE U(0)
#define CTX_CVE_2018_3639_END     U(0x10) /* Align to the next 16 byte boundary */

/*******************************************************************************
 * Registers related to ARMv8.3-PAuth.
 ******************************************************************************/
#define CTX_PAUTH_REGS_OFFSET     (CTX_CVE_2018_3639_OFFSET + CTX_CVE_2018_3639_END)
#if CTX_INCLUDE_PAUTH_REGS
#    define CTX_PACIAKEY_LO    U(0x0)
#    define CTX_PACIAKEY_HI    U(0x8)
#    define CTX_PACIBKEY_LO    U(0x10)
#    define CTX_PACIBKEY_HI    U(0x18)
#    define CTX_PACDAKEY_LO    U(0x20)
#    define CTX_PACDAKEY_HI    U(0x28)
#    define CTX_PACDBKEY_LO    U(0x30)
#    define CTX_PACDBKEY_HI    U(0x38)
#    define CTX_PACGAKEY_LO    U(0x40)
#    define CTX_PACGAKEY_HI    U(0x48)
#    define CTX_PAUTH_REGS_END U(0x50) /* Align to the next 16 byte boundary */
#else
#    define CTX_PAUTH_REGS_END U(0)
#endif                                 /* CTX_INCLUDE_PAUTH_REGS */

/*******************************************************************************
 * Registers initialised in a per-world context.
 ******************************************************************************/
#define CTX_CPTR_EL3              U(0x0)
#define CTX_ZCR_EL3               U(0x8)
#define CTX_MPAM3_EL3             U(0x10)
#define CTX_PERWORLD_EL3STATE_END U(0x18)


/*
 * Common constants to help define the 'cpu_context' structure and its
 * members below.
 */
#define DWORD_SHIFT               U(3)
#define DEFINE_REG_STRUCT(name, num_regs) \
    typedef struct name                   \
    {                                     \
        uint64_t ctx_regs[num_regs];      \
    } __aligned(16) name##_t

/* Constants to determine the size of individual context structures */
#define CTX_GPREG_ALL       (CTX_GPREGS_END >> DWORD_SHIFT)
#define CTX_EL1_SYSREGS_ALL (CTX_EL1_SYSREGS_END >> DWORD_SHIFT)

#if CTX_INCLUDE_FPREGS
#    define CTX_FPREG_ALL (CTX_FPREGS_END >> DWORD_SHIFT)
#endif
#define CTX_EL3STATE_ALL      (CTX_EL3STATE_END >> DWORD_SHIFT)
#define CTX_CVE_2018_3639_ALL (CTX_CVE_2018_3639_END >> DWORD_SHIFT)
#if CTX_INCLUDE_PAUTH_REGS
#    define CTX_PAUTH_REGS_ALL (CTX_PAUTH_REGS_END >> DWORD_SHIFT)
#endif

/*
 * AArch64 general purpose register context structure. Usually x0-x18,
 * lr are saved as the compiler is expected to preserve the remaining
 * callee saved registers if used by the C runtime and the assembler
 * does not touch the remaining. But in case of world switch during
 * exception handling, we need to save the callee registers too.
 */
DEFINE_REG_STRUCT(gp_regs, CTX_GPREG_ALL);

/*
 * AArch64 EL1 system register context structure for preserving the
 * architectural state during world switches.
 */
DEFINE_REG_STRUCT(el1_sysregs, CTX_EL1_SYSREGS_ALL);

/*
 * AArch64 floating point register context structure for preserving
 * the floating point state during switches from one security state to
 * another.
 */
#if CTX_INCLUDE_FPREGS
DEFINE_REG_STRUCT(fp_regs, CTX_FPREG_ALL);
#endif

/*
 * Miscellaneous registers used by EL3 firmware to maintain its state
 * across exception entries and exits
 */
DEFINE_REG_STRUCT(el3_state, CTX_EL3STATE_ALL);

/* Function pointer used by CVE-2018-3639 dynamic mitigation */
DEFINE_REG_STRUCT(cve_2018_3639, CTX_CVE_2018_3639_ALL);

/* Registers associated to ARMv8.3-PAuth */
#if CTX_INCLUDE_PAUTH_REGS
DEFINE_REG_STRUCT(pauth, CTX_PAUTH_REGS_ALL);
#endif

/*
 * Macros to access members of any of the above structures using their
 * offsets
 */
#define read_ctx_reg(ctx, offset)       ((ctx)->ctx_regs[(offset) >> DWORD_SHIFT])
#define write_ctx_reg(ctx, offset, val) (((ctx)->ctx_regs[(offset) >> DWORD_SHIFT]) = (uint64_t)(val))

/*
 * Top-level context structure which is used by EL3 firmware to preserve
 * the state of a core at the next lower EL in a given security state and
 * save enough EL3 meta data to be able to return to that EL and security
 * state. The context management library will be used to ensure that
 * SP_EL3 always points to an instance of this structure at exception
 * entry and exit.
 */
typedef struct cpu_context
{
    gp_regs_t     gpregs_ctx;
    el3_state_t   el3state_ctx;
    el1_sysregs_t el1_sysregs_ctx;

#if CTX_INCLUDE_FPREGS
    fp_regs_t fpregs_ctx;
#endif
    cve_2018_3639_t cve_2018_3639_ctx;

#if CTX_INCLUDE_PAUTH_REGS
    pauth_t pauth_ctx;
#endif

#if CTX_INCLUDE_EL2_REGS
    el2_sysregs_t el2_sysregs_ctx;
#endif

} cpu_context_t;

/* Size of cpu_context array */
#define CPU_DATA_CONTEXT_NUM 3
/* Offset of cpu_ops_ptr, size 8 bytes */
#define CPU_DATA_CPU_OPS_PTR 0x18

/*
 * Per-World Context.
 * It stores registers whose values can be shared across CPUs.
 */
typedef struct per_world_context
{
    uint64_t ctx_cptr_el3;
    uint64_t ctx_zcr_el3;
    uint64_t ctx_mpam3_el3;
} per_world_context_t;

extern per_world_context_t per_world_context[CPU_DATA_CONTEXT_NUM];

/* Macros to access members of the 'cpu_context_t' structure */
#define get_el3state_ctx(h) (&((cpu_context_t *)h)->el3state_ctx)
#if CTX_INCLUDE_FPREGS
#    define get_fpregs_ctx(h) (&((cpu_context_t *)h)->fpregs_ctx)
#endif
#define get_el1_sysregs_ctx(h) (&((cpu_context_t *)h)->el1_sysregs_ctx)
#if CTX_INCLUDE_EL2_REGS
#    define get_el2_sysregs_ctx(h) (&((cpu_context_t *)h)->el2_sysregs_ctx)
#endif
#define get_gpregs_ctx(h)        (&((cpu_context_t *)h)->gpregs_ctx)
#define get_cve_2018_3639_ctx(h) (&((cpu_context_t *)h)->cve_2018_3639_ctx)
#if CTX_INCLUDE_PAUTH_REGS
#    define get_pauth_ctx(h) (&((cpu_context_t *)h)->pauth_ctx)
#endif

#if CTX_INCLUDE_PAUTH_REGS
CASSERT(CTX_PAUTH_REGS_OFFSET == __builtin_offsetof(cpu_context_t, pauth_ctx),
        assert_core_context_pauth_offset_mismatch);
#endif /* CTX_INCLUDE_PAUTH_REGS */

/*
 * Helper macro to set the general purpose registers that correspond to
 * parameters in an aapcs_64 call i.e. x0-x7
 */
#define set_aapcs_args0(ctx, x0)                              \
    do                                                        \
    {                                                         \
        write_ctx_reg(get_gpregs_ctx(ctx), CTX_GPREG_X0, x0); \
    }                                                         \
    while (0)
#define set_aapcs_args1(ctx, x0, x1)                          \
    do                                                        \
    {                                                         \
        write_ctx_reg(get_gpregs_ctx(ctx), CTX_GPREG_X1, x1); \
        set_aapcs_args0(ctx, x0);                             \
    }                                                         \
    while (0)
#define set_aapcs_args2(ctx, x0, x1, x2)                      \
    do                                                        \
    {                                                         \
        write_ctx_reg(get_gpregs_ctx(ctx), CTX_GPREG_X2, x2); \
        set_aapcs_args1(ctx, x0, x1);                         \
    }                                                         \
    while (0)
#define set_aapcs_args3(ctx, x0, x1, x2, x3)                  \
    do                                                        \
    {                                                         \
        write_ctx_reg(get_gpregs_ctx(ctx), CTX_GPREG_X3, x3); \
        set_aapcs_args2(ctx, x0, x1, x2);                     \
    }                                                         \
    while (0)
#define set_aapcs_args4(ctx, x0, x1, x2, x3, x4)              \
    do                                                        \
    {                                                         \
        write_ctx_reg(get_gpregs_ctx(ctx), CTX_GPREG_X4, x4); \
        set_aapcs_args3(ctx, x0, x1, x2, x3);                 \
    }                                                         \
    while (0)
#define set_aapcs_args5(ctx, x0, x1, x2, x3, x4, x5)          \
    do                                                        \
    {                                                         \
        write_ctx_reg(get_gpregs_ctx(ctx), CTX_GPREG_X5, x5); \
        set_aapcs_args4(ctx, x0, x1, x2, x3, x4);             \
    }                                                         \
    while (0)
#define set_aapcs_args6(ctx, x0, x1, x2, x3, x4, x5, x6)      \
    do                                                        \
    {                                                         \
        write_ctx_reg(get_gpregs_ctx(ctx), CTX_GPREG_X6, x6); \
        set_aapcs_args5(ctx, x0, x1, x2, x3, x4, x5);         \
    }                                                         \
    while (0)
#define set_aapcs_args7(ctx, x0, x1, x2, x3, x4, x5, x6, x7)  \
    do                                                        \
    {                                                         \
        write_ctx_reg(get_gpregs_ctx(ctx), CTX_GPREG_X7, x7); \
        set_aapcs_args6(ctx, x0, x1, x2, x3, x4, x5, x6);     \
    }                                                         \
    while (0)

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
#if CTX_INCLUDE_FPREGS
void fpregs_context_save(fp_regs_t *regs);
void fpregs_context_restore(fp_regs_t *regs);
#endif

/* Convenience macros to return from SMC handler */
#define SMC_RET0(_h)           \
    {                          \
        return (uint64_t)(_h); \
    }

#define SMC_RET1(_h, _x0)                                           \
    {                                                               \
        write_ctx_reg((get_gpregs_ctx(_h)), (CTX_GPREG_X0), (_x0)); \
        SMC_RET0(_h);                                               \
    }

/*
 * Description: depend on input parameter "timing_config",
 *		and calculate correspond "dram_type"
 *		spec timing to "pdram_timing"
 * parameters:
 *   input: timing_config
 *   output: pdram_timing
 * NOTE: MR ODT is set, need to disable by controller
 */
void dram_get_parameter(struct timing_related_config *timing_config, struct dram_timing_t *pdram_timing);


static const struct pll_div dpll_rates_table[] = {

  /* _mhz, _refdiv, _fbdiv, _postdiv1, _postdiv2 */
    {.mhz = 928, .refdiv = 1, .fbdiv = 116, .postdiv1 = 3, .postdiv2 = 1},
    {.mhz = 800, .refdiv = 1, .fbdiv = 100, .postdiv1 = 3, .postdiv2 = 1},
    {.mhz = 732, .refdiv = 1,  .fbdiv = 61, .postdiv1 = 2, .postdiv2 = 1},
    {.mhz = 666, .refdiv = 1, .fbdiv = 111, .postdiv1 = 4, .postdiv2 = 1},
    {.mhz = 600, .refdiv = 1,  .fbdiv = 50, .postdiv1 = 2, .postdiv2 = 1},
    {.mhz = 528, .refdiv = 1,  .fbdiv = 66, .postdiv1 = 3, .postdiv2 = 1},
    {.mhz = 400, .refdiv = 1,  .fbdiv = 50, .postdiv1 = 3, .postdiv2 = 1},
    {.mhz = 300, .refdiv = 1,  .fbdiv = 50, .postdiv1 = 4, .postdiv2 = 1},
    {.mhz = 200, .refdiv = 1,  .fbdiv = 50, .postdiv1 = 3, .postdiv2 = 2},
};

enum ddr3_speed_rate
{
    /* 5-5-5 */
    DDR3_800D = 0,
    /* 6-6-6 */
    DDR3_800E = 1,
    /* 6-6-6 */
    DDR3_1066E = 2,
    /* 7-7-7 */
    DDR3_1066F = 3,
    /* 8-8-8 */
    DDR3_1066G = 4,
    /* 7-7-7 */
    DDR3_1333F = 5,
    /* 8-8-8 */
    DDR3_1333G = 6,
    /* 9-9-9 */
    DDR3_1333H = 7,
    /* 10-10-10 */
    DDR3_1333J = 8,
    /* 8-8-8 */
    DDR3_1600G = 9,
    /* 9-9-9 */
    DDR3_1600H = 10,
    /* 10-10-10 */
    DDR3_1600J = 11,
    /* 11-11-11 */
    DDR3_1600K = 12,
    /* 10-10-10 */
    DDR3_1866J = 13,
    /* 11-11-11 */
    DDR3_1866K = 14,
    /* 12-12-12 */
    DDR3_1866L = 15,
    /* 13-13-13 */
    DDR3_1866M = 16,
    /* 11-11-11 */
    DDR3_2133K = 17,
    /* 12-12-12 */
    DDR3_2133L = 18,
    /* 13-13-13 */
    DDR3_2133M = 19,
    /* 14-14-14 */
    DDR3_2133N   = 20,
    DDR3_DEFAULT = 21,
};

#define max(a, b)          (((a) > (b)) ? (a) : (b))
#define range(mi, val, ma) (((ma) > (val)) ? (max(mi, val)) : (ma))

struct dram_timing_t
{
    /* unit MHz */
    uint32_t mhz;
    /* some timing unit is us */
    uint32_t tinit1;
    uint32_t tinit2;
    uint32_t tinit3;
    uint32_t tinit4;
    uint32_t tinit5;
    /* reset low, DDR3:200us */
    uint32_t trstl;
    /* reset high to CKE high, DDR3:500us  */
    uint32_t trsth;
    uint32_t trefi;
    /* base */
    uint32_t trcd;
    /* trp per bank */
    uint32_t trppb;
    /* trp all bank */
    uint32_t trp;
    uint32_t twr;
    uint32_t tdal;
    uint32_t trtp;
    uint32_t trc;
    uint32_t trrd;
    uint32_t tccd;
    uint32_t twtr;
    uint32_t trtw;
    uint32_t tras_max;
    uint32_t tras_min;
    uint32_t tfaw;
    uint32_t trfc;
    uint32_t tdqsck;
    uint32_t tdqsck_max;
    /* pd or sr */
    uint32_t txsr;
    uint32_t txsnr;
    uint32_t txp;
    uint32_t txpdll;
    uint32_t tdllk;
    uint32_t tcke;
    uint32_t tckesr;
    uint32_t tcksre;
    uint32_t tcksrx;
    uint32_t tdpd;
    /* mode register timing */
    uint32_t tmod;
    uint32_t tmrd;
    uint32_t tmrr;
    uint32_t tmrri;
    /* ODT */
    uint32_t todton;
    /* ZQ */
    uint32_t tzqinit;
    uint32_t tzqcs;
    uint32_t tzqoper;
    uint32_t tzqreset;
    /* Write Leveling */
    uint32_t twlmrd;
    uint32_t twlo;
    uint32_t twldqsen;
    /* CA Training */
    uint32_t tcackel;
    uint32_t tcaent;
    uint32_t tcamrd;
    uint32_t tcackeh;
    uint32_t tcaext;
    uint32_t tadr;
    uint32_t tmrz;
    uint32_t tcacd;
    /* mode register */
    uint32_t mr[4];
    uint32_t mr11;
    /* lpddr4 spec */
    uint32_t mr12;
    uint32_t mr13;
    uint32_t mr14;
    uint32_t mr16;
    uint32_t mr17;
    uint32_t mr20;
    uint32_t mr22;
    uint32_t tccdmw;
    uint32_t tppd;
    uint32_t tescke;
    uint32_t tsr;
    uint32_t tcmdcke;
    uint32_t tcscke;
    uint32_t tckelcs;
    uint32_t tcsckeh;
    uint32_t tckehcs;
    uint32_t tmrwckel;
    uint32_t tzqcal;
    uint32_t tzqlat;
    uint32_t tzqcke;
    uint32_t tvref_long;
    uint32_t tvref_short;
    uint32_t tvrcg_enable;
    uint32_t tvrcg_disable;
    uint32_t tfc_long;
    uint32_t tckfspe;
    uint32_t tckfspx;
    uint32_t tckehcmd;
    uint32_t tckelcmd;
    uint32_t tckelpd;
    uint32_t tckckel;
    /* other */
    uint32_t al;
    uint32_t cl;
    uint32_t cwl;
    uint32_t bl;
};

struct dram_info_t
{
    /* speed_rate only used when DDR3 */
    enum ddr3_speed_rate speed_rate;
    /* 1: use CS0, 2: use CS0 and CS1 */
    uint32_t cs_cnt;
    /* give the max per-die capability on each rank/cs */
    uint32_t per_die_capability[2];
};

struct timing_related_config
{
    struct dram_info_t dram_info[2];
    uint32_t           dram_type;
    /* MHz */
    uint32_t freq;
    uint32_t ch_cnt;
    uint32_t bl;
    /* 1:auto precharge, 0:never auto precharge */
    uint32_t ap;
    /*
     * 1:dll bypass, 0:dll normal
     * dram and controller dll bypass at the same time
     */
    uint32_t dllbp;
    /* 1:odt enable, 0:odt disable */
    uint32_t odt;
    /* 1:enable, 0:disabe */
    uint32_t rdbi;
    uint32_t wdbi;
    /* dram driver strength */
    uint32_t dramds;
    /* dram ODT, if odt=0, this parameter invalid */
    uint32_t dramodt;
    /*
     * ca ODT, if odt=0, this parameter invalid
     * it only used by LPDDR4
     */
    uint32_t caodt;
};


struct rk3399_sdram_default_config
{
    unsigned char bl;
    /* 1:auto precharge, 0:never auto precharge */
    unsigned char ap;
    /* dram driver strength */
    unsigned char dramds;
    /* dram ODT, if odt=0, this parameter invalid */
    unsigned char dramodt;
    /* ca ODT, if odt=0, this parameter invalid
     * only used by LPDDR4
     */
    unsigned char caodt;
    unsigned char burst_ref_cnt;
    /* zqcs period, unit(s) */
    unsigned char zqcsi;
};

struct drv_odt_lp_config
{
    uint32_t pd_idle;
    uint32_t sr_idle;
    uint32_t sr_mc_gate_idle;
    uint32_t srpd_lite_idle;
    uint32_t standby_idle;
    uint32_t odt_en;

    uint32_t dram_side_drv;
    uint32_t dram_side_dq_odt;
    uint32_t dram_side_ca_odt;
};

uint32_t ddr_set_rate(uint32_t hz);
uint32_t ddr_round_rate(uint32_t hz);
uint32_t ddr_get_rate(void);
uint32_t dram_set_odt_pd(uint32_t arg0, uint32_t arg1, uint32_t arg2);
void     dram_dfs_init(void);
void     ddr_prepare_for_sys_suspend(void);
void     ddr_prepare_for_sys_resume(void);


struct rk3399_dram_status
{
    uint32_t                     current_index;
    uint32_t                     index_freq[2];
    uint32_t                     boot_freq;
    uint32_t                     low_power_stat;
    struct timing_related_config timing_config;
    struct drv_odt_lp_config     drv_odt_lp_cfg;
};

struct rk3399_saved_status
{
    uint32_t freq;
    uint32_t low_power_stat;
    uint32_t odt;
};

uint32_t ddr_set_rate(uint32_t hz);
uint32_t ddr_round_rate(uint32_t hz);
uint32_t ddr_get_rate(void);
uint32_t dram_set_odt_pd(uint32_t arg0, uint32_t arg1, uint32_t arg2);
void     dram_dfs_init(void);
void     ddr_prepare_for_sys_suspend(void);
void     ddr_prepare_for_sys_resume(void);

static struct rk3399_dram_status  rk3399_dram_status;
static struct rk3399_saved_status rk3399_suspend_status;
static uint32_t                   wrdqs_delay_val[2][2][4];
static uint32_t                   rddqs_delay_ps;

static struct rk3399_sdram_default_config ddr3_default_config = {.bl = 8, .ap = 0, .burst_ref_cnt = 1, .zqcsi = 0};

static struct rk3399_sdram_default_config lpddr3_default_config = {.bl = 8, .ap = 0, .burst_ref_cnt = 1, .zqcsi = 0};

static struct rk3399_sdram_default_config lpddr4_default_config = {.bl            = 16,
                                                                   .ap            = 0,
                                                                   .caodt         = 240,
                                                                   .burst_ref_cnt = 1,
                                                                   .zqcsi         = 0};

enum
{
    DDR3   = 3,
    LPDDR2 = 5,
    LPDDR3 = 6,
    LPDDR4 = 7,
    UNUSED = 0xff
};

struct rk3399_ddr_pctl_regs
{
    uint32_t denali_ctl[CTL_REG_NUM];
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
    uint32_t d32;
    struct
    {
        unsigned acttoact : 6;
        unsigned reserved0 : 2;
        unsigned rdtomiss : 6;
        unsigned reserved1 : 2;
        unsigned wrtomiss : 6;
        unsigned reserved2 : 2;
        unsigned readlatency : 8;
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

extern struct rk3399_sdram_params sdram_config;

void dram_init(void);

static inline void
mmio_write_8(uintptr_t addr, uint8_t value)
{
    *(volatile uint8_t *)addr = value;
}

static inline uint8_t
mmio_read_8(uintptr_t addr)
{
    return *(volatile uint8_t *)addr;
}

static inline void
mmio_write_16(uintptr_t addr, uint16_t value)
{
    *(volatile uint16_t *)addr = value;
}

static inline uint16_t
mmio_read_16(uintptr_t addr)
{
    return *(volatile uint16_t *)addr;
}

static inline void
mmio_clrsetbits_16(uintptr_t addr, uint16_t clear, uint16_t set)
{
    mmio_write_16(addr, (mmio_read_16(addr) & ~clear) | set);
}

static inline void
mmio_write_32(uintptr_t addr, uint32_t value)
{
    *(volatile uint32_t *)addr = value;
}

static inline uint32_t
mmio_read_32(uintptr_t addr)
{
    return *(volatile uint32_t *)addr;
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


static uint32_t
get_cs_die_capability(struct rk3399_sdram_params *ram_config, uint8_t channel, uint8_t cs)
{
    struct rk3399_sdram_channel *ch = &ram_config->ch[channel];
    uint32_t                     bandwidth;
    uint32_t                     die_bandwidth;
    uint32_t                     die;
    uint32_t                     cs_cap;
    uint32_t                     row;

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

static void
get_dram_drv_odt_val(uint32_t dram_type, struct drv_odt_lp_config *drv_config)
{
    uint32_t tmp;
    uint32_t mr1_val, mr3_val, mr11_val;

    switch (dram_type)
    {
        case DDR3 :
            mr1_val = (mmio_read_32(CTL_REG(0, 133)) >> 16) & 0xffff;
            tmp     = ((mr1_val >> 1) & 1) | ((mr1_val >> 4) & 1);
            if (tmp)
            {
                drv_config->dram_side_drv = 34;
            }
            else
            {
                drv_config->dram_side_drv = 40;
            }
            tmp = ((mr1_val >> 2) & 1) | ((mr1_val >> 5) & 1) | ((mr1_val >> 7) & 1);
            if (tmp == 0)
            {
                drv_config->dram_side_dq_odt = 0;
            }
            else if (tmp == 1)
            {
                drv_config->dram_side_dq_odt = 60;
            }
            else if (tmp == 3)
            {
                drv_config->dram_side_dq_odt = 40;
            }
            else
            {
                drv_config->dram_side_dq_odt = 120;
            }
            break;
        case LPDDR3 :
            mr3_val  = mmio_read_32(CTL_REG(0, 138)) & 0xf;
            mr11_val = (mmio_read_32(CTL_REG(0, 139)) >> 24) & 0x3;
            if (mr3_val == 0xb)
            {
                drv_config->dram_side_drv = 3448;
            }
            else if (mr3_val == 0xa)
            {
                drv_config->dram_side_drv = 4048;
            }
            else if (mr3_val == 0x9)
            {
                drv_config->dram_side_drv = 3440;
            }
            else if (mr3_val == 0x4)
            {
                drv_config->dram_side_drv = 60;
            }
            else if (mr3_val == 0x3)
            {
                drv_config->dram_side_drv = 48;
            }
            else if (mr3_val == 0x2)
            {
                drv_config->dram_side_drv = 40;
            }
            else
            {
                drv_config->dram_side_drv = 34;
            }

            if (mr11_val == 1)
            {
                drv_config->dram_side_dq_odt = 60;
            }
            else if (mr11_val == 2)
            {
                drv_config->dram_side_dq_odt = 120;
            }
            else if (mr11_val == 0)
            {
                drv_config->dram_side_dq_odt = 0;
            }
            else
            {
                drv_config->dram_side_dq_odt = 240;
            }
            break;
        case LPDDR4 :
        default :
            mr3_val  = (mmio_read_32(CTL_REG(0, 138)) >> 3) & 0x7;
            mr11_val = (mmio_read_32(CTL_REG(0, 139)) >> 24) & 0xff;

            if ((mr3_val == 0) || (mr3_val == 7))
            {
                drv_config->dram_side_drv = 40;
            }
            else
            {
                drv_config->dram_side_drv = 240 / mr3_val;
            }

            tmp = mr11_val & 0x7;
            if ((tmp == 7) || (tmp == 0))
            {
                drv_config->dram_side_dq_odt = 0;
            }
            else
            {
                drv_config->dram_side_dq_odt = 240 / tmp;
            }

            tmp = (mr11_val >> 4) & 0x7;
            if ((tmp == 7) || (tmp == 0))
            {
                drv_config->dram_side_ca_odt = 0;
            }
            else
            {
                drv_config->dram_side_ca_odt = 240 / tmp;
            }
            break;
    }
}

static void
sdram_timing_cfg_init(struct timing_related_config *ptiming_config, struct rk3399_sdram_params *sdram_params,
                      struct drv_odt_lp_config *drv_config)
{
    uint32_t i, j;

    for (i = 0; i < sdram_params->num_channels; i++)
    {
        ptiming_config->dram_info[i].speed_rate = DDR3_DEFAULT;
        ptiming_config->dram_info[i].cs_cnt     = sdram_params->ch[i].rank;
        for (j = 0; j < sdram_params->ch[i].rank; j++)
        {
            ptiming_config->dram_info[i].per_die_capability[j] = get_cs_die_capability(sdram_params, i, j);
        }
    }
    ptiming_config->dram_type = sdram_params->dramtype;
    ptiming_config->ch_cnt    = sdram_params->num_channels;
    switch (sdram_params->dramtype)
    {
        case DDR3 :
            ptiming_config->bl = ddr3_default_config.bl;
            ptiming_config->ap = ddr3_default_config.ap;
            break;
        case LPDDR3 :
            ptiming_config->bl = lpddr3_default_config.bl;
            ptiming_config->ap = lpddr3_default_config.ap;
            break;
        case LPDDR4 :
            ptiming_config->bl   = lpddr4_default_config.bl;
            ptiming_config->ap   = lpddr4_default_config.ap;
            ptiming_config->rdbi = 0;
            ptiming_config->wdbi = 0;
            break;
        default :
            /* Do nothing in default case */
            break;
    }
    ptiming_config->dramds  = drv_config->dram_side_drv;
    ptiming_config->dramodt = drv_config->dram_side_dq_odt;
    ptiming_config->caodt   = drv_config->dram_side_ca_odt;
    ptiming_config->odt     = (mmio_read_32(PHY_REG(0, 5)) >> 16) & 0x1;
}

struct lat_adj_pair
{
    uint32_t cl;
    uint32_t rdlat_adj;
    uint32_t cwl;
    uint32_t wrlat_adj;
};

const struct lat_adj_pair ddr3_lat_adj[] = {
    { 6,   5,   5, 4},
    { 8,   7,   6, 5},
    {10,   9,   7, 6},
    {11,   9,   8, 7},
    {13, 0xb,   9, 8},
    {14, 0xb, 0xa, 9}
};

const struct lat_adj_pair lpddr3_lat_adj[] = {
    { 3,   2, 1, 0},
    { 6,   5, 3, 2},
    { 8,   7, 4, 3},
    { 9,   8, 5, 4},
    {10,   9, 6, 5},
    {11,   9, 6, 5},
    {12, 0xa, 6, 5},
    {14, 0xc, 8, 7},
    {16, 0xd, 8, 7}
};

const struct lat_adj_pair lpddr4_lat_adj[] = {
    { 6,    5,    4,    2},
    {10,    9,    6,    4},
    {14,  0xc,    8,    6},
    {20, 0x11,  0xa,    8},
    {24, 0x15,  0xc,  0xa},
    {28, 0x18,  0xe,  0xc},
    {32, 0x1b, 0x10,  0xe},
    {36, 0x1e, 0x12, 0x10}
};

static uint32_t
get_rdlat_adj(uint32_t dram_type, uint32_t cl)
{
    const struct lat_adj_pair *p;
    uint32_t                   cnt;
    uint32_t                   i;

    if (dram_type == DDR3)
    {
        p   = ddr3_lat_adj;
        cnt = ARRAY_SIZE(ddr3_lat_adj);
    }
    else if (dram_type == LPDDR3)
    {
        p   = lpddr3_lat_adj;
        cnt = ARRAY_SIZE(lpddr3_lat_adj);
    }
    else
    {
        p   = lpddr4_lat_adj;
        cnt = ARRAY_SIZE(lpddr4_lat_adj);
    }

    for (i = 0; i < cnt; i++)
    {
        if (cl == p[i].cl)
        {
            return p[i].rdlat_adj;
        }
    }
    /* fail */
    return 0xff;
}

static uint32_t
get_wrlat_adj(uint32_t dram_type, uint32_t cwl)
{
    const struct lat_adj_pair *p;
    uint32_t                   cnt;
    uint32_t                   i;

    if (dram_type == DDR3)
    {
        p   = ddr3_lat_adj;
        cnt = ARRAY_SIZE(ddr3_lat_adj);
    }
    else if (dram_type == LPDDR3)
    {
        p   = lpddr3_lat_adj;
        cnt = ARRAY_SIZE(lpddr3_lat_adj);
    }
    else
    {
        p   = lpddr4_lat_adj;
        cnt = ARRAY_SIZE(lpddr4_lat_adj);
    }

    for (i = 0; i < cnt; i++)
    {
        if (cwl == p[i].cwl)
        {
            return p[i].wrlat_adj;
        }
    }
    /* fail */
    return 0xff;
}

#define PI_REGS_DIMM_SUPPORT  (0)
#define PI_ADD_LATENCY        (0)
#define PI_DOUBLEFREEK        (1)

#define PI_PAD_DELAY_PS_VALUE (1000)
#define PI_IE_ENABLE_VALUE    (3000)
#define PI_TSEL_ENABLE_VALUE  (700)

static uint32_t
get_pi_rdlat_adj(struct dram_timing_t *pdram_timing)
{
    /*[DLLSUBTYPE2] == "STD_DENALI_HS" */
    uint32_t rdlat, delay_adder, ie_enable, hs_offset, tsel_adder, extra_adder, tsel_enable;

    ie_enable   = PI_IE_ENABLE_VALUE;
    tsel_enable = PI_TSEL_ENABLE_VALUE;

    rdlat       = pdram_timing->cl + PI_ADD_LATENCY;
    delay_adder = ie_enable / (1000000 / pdram_timing->mhz);
    if ((ie_enable % (1000000 / pdram_timing->mhz)) != 0)
    {
        delay_adder++;
    }
    hs_offset   = 0;
    tsel_adder  = 0;
    extra_adder = 0;
    /* rdlat = rdlat - (PREAMBLE_SUPPORT & 0x1); */
    tsel_adder = tsel_enable / (1000000 / pdram_timing->mhz);
    if ((tsel_enable % (1000000 / pdram_timing->mhz)) != 0)
    {
        tsel_adder++;
    }
    delay_adder = delay_adder - 1;
    if (tsel_adder > delay_adder)
    {
        extra_adder = tsel_adder - delay_adder;
    }
    else
    {
        extra_adder = 0;
    }
    if (PI_REGS_DIMM_SUPPORT && PI_DOUBLEFREEK)
    {
        hs_offset = 2;
    }
    else
    {
        hs_offset = 1;
    }

    if (delay_adder > (rdlat - 1 - hs_offset))
    {
        rdlat = rdlat - tsel_adder;
    }
    else
    {
        if ((rdlat - delay_adder) < 2)
        {
            rdlat = 2;
        }
        else
        {
            rdlat = rdlat - delay_adder - extra_adder;
        }
    }

    return rdlat;
}

static uint32_t
get_pi_wrlat(struct dram_timing_t *pdram_timing, struct timing_related_config *timing_config)
{
    uint32_t tmp;

    if (timing_config->dram_type == LPDDR3)
    {
        tmp = pdram_timing->cl;
        if (tmp >= 14)
        {
            tmp = 8;
        }
        else if (tmp >= 10)
        {
            tmp = 6;
        }
        else if (tmp == 9)
        {
            tmp = 5;
        }
        else if (tmp == 8)
        {
            tmp = 4;
        }
        else if (tmp == 6)
        {
            tmp = 3;
        }
        else
        {
            tmp = 1;
        }
    }
    else
    {
        tmp = 1;
    }

    return tmp;
}

static uint32_t
get_pi_wrlat_adj(struct dram_timing_t *pdram_timing, struct timing_related_config *timing_config)
{
    return get_pi_wrlat(pdram_timing, timing_config) + PI_ADD_LATENCY - 1;
}

static uint32_t
get_pi_tdfi_phy_rdlat(struct dram_timing_t *pdram_timing, struct timing_related_config *timing_config)
{
    /* [DLLSUBTYPE2] == "STD_DENALI_HS" */
    uint32_t cas_lat, delay_adder, ie_enable, hs_offset, ie_delay_adder;
    uint32_t mem_delay_ps, round_trip_ps;
    uint32_t phy_internal_delay, lpddr_adder, dfi_adder, rdlat_delay;

    ie_enable = PI_IE_ENABLE_VALUE;

    delay_adder = ie_enable / (1000000 / pdram_timing->mhz);
    if ((ie_enable % (1000000 / pdram_timing->mhz)) != 0)
    {
        delay_adder++;
    }
    delay_adder = delay_adder - 1;
    if (PI_REGS_DIMM_SUPPORT && PI_DOUBLEFREEK)
    {
        hs_offset = 2;
    }
    else
    {
        hs_offset = 1;
    }

    cas_lat = pdram_timing->cl + PI_ADD_LATENCY;

    if (delay_adder > (cas_lat - 1 - hs_offset))
    {
        ie_delay_adder = 0;
    }
    else
    {
        ie_delay_adder = ie_enable / (1000000 / pdram_timing->mhz);
        if ((ie_enable % (1000000 / pdram_timing->mhz)) != 0)
        {
            ie_delay_adder++;
        }
    }

    if (timing_config->dram_type == DDR3)
    {
        mem_delay_ps = 0;
    }
    else if (timing_config->dram_type == LPDDR4)
    {
        mem_delay_ps = 3600;
    }
    else if (timing_config->dram_type == LPDDR3)
    {
        mem_delay_ps = 5500;
    }
    else
    {
        NOTICE("get_pi_tdfi_phy_rdlat:dramtype unsupport\n");
        return 0;
    }
    round_trip_ps = 1100 + 500 + mem_delay_ps + 500 + 600;
    delay_adder   = round_trip_ps / (1000000 / pdram_timing->mhz);
    if ((round_trip_ps % (1000000 / pdram_timing->mhz)) != 0)
    {
        delay_adder++;
    }

    phy_internal_delay = 5 + 2 + 4;
    lpddr_adder        = mem_delay_ps / (1000000 / pdram_timing->mhz);
    if ((mem_delay_ps % (1000000 / pdram_timing->mhz)) != 0)
    {
        lpddr_adder++;
    }
    dfi_adder          = 0;
    phy_internal_delay = phy_internal_delay + 2;
    rdlat_delay        = delay_adder + phy_internal_delay + ie_delay_adder + lpddr_adder + dfi_adder;

    rdlat_delay = rdlat_delay + 2;
    return rdlat_delay;
}

static uint32_t
get_pi_todtoff_min(struct dram_timing_t *pdram_timing, struct timing_related_config *timing_config)
{
    uint32_t tmp, todtoff_min_ps;

    if (timing_config->dram_type == LPDDR3)
    {
        todtoff_min_ps = 2500;
    }
    else if (timing_config->dram_type == LPDDR4)
    {
        todtoff_min_ps = 1500;
    }
    else
    {
        todtoff_min_ps = 0;
    }
    /* todtoff_min */
    tmp = todtoff_min_ps / (1000000 / pdram_timing->mhz);
    if ((todtoff_min_ps % (1000000 / pdram_timing->mhz)) != 0)
    {
        tmp++;
    }
    return tmp;
}

static uint32_t
get_pi_todtoff_max(struct dram_timing_t *pdram_timing, struct timing_related_config *timing_config)
{
    uint32_t tmp, todtoff_max_ps;

    if ((timing_config->dram_type == LPDDR4) || (timing_config->dram_type == LPDDR3))
    {
        todtoff_max_ps = 3500;
    }
    else
    {
        todtoff_max_ps = 0;
    }

    /* todtoff_max */
    tmp = todtoff_max_ps / (1000000 / pdram_timing->mhz);
    if ((todtoff_max_ps % (1000000 / pdram_timing->mhz)) != 0)
    {
        tmp++;
    }
    return tmp;
}

static void
gen_rk3399_ctl_params_f0(struct timing_related_config *timing_config, struct dram_timing_t *pdram_timing)
{
    uint32_t i;
    uint32_t tmp, tmp1;

    for (i = 0; i < timing_config->ch_cnt; i++)
    {
        if (timing_config->dram_type == DDR3)
        {
            tmp = ((700000 + 10) * timing_config->freq + 999) / 1000;
            tmp += pdram_timing->txsnr + (pdram_timing->tmrd * 3) + pdram_timing->tmod + pdram_timing->tzqinit;
            mmio_write_32(CTL_REG(i, 5), tmp);

            mmio_clrsetbits_32(CTL_REG(i, 22), 0xffff, pdram_timing->tdllk);

            mmio_write_32(CTL_REG(i, 32), (pdram_timing->tmod << 8) | pdram_timing->tmrd);

            mmio_clrsetbits_32(CTL_REG(i, 59), 0xffffu << 16, (pdram_timing->txsr - pdram_timing->trcd) << 16);
        }
        else if (timing_config->dram_type == LPDDR4)
        {
            mmio_write_32(CTL_REG(i, 5), pdram_timing->tinit1 + pdram_timing->tinit3);
            mmio_write_32(CTL_REG(i, 32), (pdram_timing->tmrd << 8) | pdram_timing->tmrd);
            mmio_clrsetbits_32(CTL_REG(i, 59), 0xffffu << 16, pdram_timing->txsr << 16);
        }
        else
        {
            mmio_write_32(CTL_REG(i, 5), pdram_timing->tinit1);
            mmio_write_32(CTL_REG(i, 7), pdram_timing->tinit4);
            mmio_write_32(CTL_REG(i, 32), (pdram_timing->tmrd << 8) | pdram_timing->tmrd);
            mmio_clrsetbits_32(CTL_REG(i, 59), 0xffffu << 16, pdram_timing->txsr << 16);
        }
        mmio_write_32(CTL_REG(i, 6), pdram_timing->tinit3);
        mmio_write_32(CTL_REG(i, 8), pdram_timing->tinit5);
        mmio_clrsetbits_32(CTL_REG(i, 23), (0x7f << 16), ((pdram_timing->cl * 2) << 16));
        mmio_clrsetbits_32(CTL_REG(i, 23), (0x1f << 24), (pdram_timing->cwl << 24));
        mmio_clrsetbits_32(CTL_REG(i, 24), 0x3f, pdram_timing->al);
        mmio_clrsetbits_32(CTL_REG(i, 26), 0xffffu << 16, (pdram_timing->trc << 24) | (pdram_timing->trrd << 16));
        mmio_write_32(CTL_REG(i, 27), (pdram_timing->tfaw << 24) | (pdram_timing->trppb << 16) |
                                          (pdram_timing->twtr << 8) | pdram_timing->tras_min);

        mmio_clrsetbits_32(CTL_REG(i, 31), 0xffu << 24, max(4, pdram_timing->trtp) << 24);
        mmio_write_32(CTL_REG(i, 33), (pdram_timing->tcke << 24) | pdram_timing->tras_max);
        mmio_clrsetbits_32(CTL_REG(i, 34), 0xff, max(1, pdram_timing->tckesr));
        mmio_clrsetbits_32(
            CTL_REG(i, 39), (0x3f << 16) | (0xff << 8), (pdram_timing->twr << 16) | (pdram_timing->trcd << 8));
        mmio_clrsetbits_32(CTL_REG(i, 42), 0x1f << 16, pdram_timing->tmrz << 16);
        tmp = pdram_timing->tdal ? pdram_timing->tdal : (pdram_timing->twr + pdram_timing->trp);
        mmio_clrsetbits_32(CTL_REG(i, 44), 0xff, tmp);
        mmio_clrsetbits_32(CTL_REG(i, 45), 0xff, pdram_timing->trp);
        mmio_write_32(CTL_REG(i, 48), ((pdram_timing->trefi - 8) << 16) | pdram_timing->trfc);
        mmio_clrsetbits_32(CTL_REG(i, 52), 0xffff, pdram_timing->txp);
        mmio_clrsetbits_32(CTL_REG(i, 53), 0xffffu << 16, pdram_timing->txpdll << 16);
        mmio_clrsetbits_32(CTL_REG(i, 55), 0xf << 24, pdram_timing->tcscke << 24);
        mmio_clrsetbits_32(CTL_REG(i, 55), 0xff, pdram_timing->tmrri);
        mmio_write_32(CTL_REG(i, 56), (pdram_timing->tzqcke << 24) | (pdram_timing->tmrwckel << 16) |
                                          (pdram_timing->tckehcs << 8) | pdram_timing->tckelcs);
        mmio_clrsetbits_32(CTL_REG(i, 60), 0xffff, pdram_timing->txsnr);
        mmio_clrsetbits_32(
            CTL_REG(i, 62), 0xffffu << 16, (pdram_timing->tckehcmd << 24) | (pdram_timing->tckelcmd << 16));
        mmio_write_32(CTL_REG(i, 63), (pdram_timing->tckelpd << 24) | (pdram_timing->tescke << 16) |
                                          (pdram_timing->tsr << 8) | pdram_timing->tckckel);
        mmio_clrsetbits_32(CTL_REG(i, 64), 0xfff, (pdram_timing->tcmdcke << 8) | pdram_timing->tcsckeh);
        mmio_clrsetbits_32(CTL_REG(i, 92), 0xffff << 8, (pdram_timing->tcksrx << 16) | (pdram_timing->tcksre << 8));
        mmio_clrsetbits_32(CTL_REG(i, 108), 0x1 << 24, (timing_config->dllbp << 24));
        mmio_clrsetbits_32(CTL_REG(i, 122), 0x3ff << 16, (pdram_timing->tvrcg_enable << 16));
        mmio_write_32(CTL_REG(i, 123), (pdram_timing->tfc_long << 16) | pdram_timing->tvrcg_disable);
        mmio_write_32(
            CTL_REG(i, 124), (pdram_timing->tvref_long << 16) | (pdram_timing->tckfspx << 8) | pdram_timing->tckfspe);
        mmio_write_32(CTL_REG(i, 133), (pdram_timing->mr[1] << 16) | pdram_timing->mr[0]);
        mmio_clrsetbits_32(CTL_REG(i, 134), 0xffff, pdram_timing->mr[2]);
        mmio_clrsetbits_32(CTL_REG(i, 138), 0xffff, pdram_timing->mr[3]);
        mmio_clrsetbits_32(CTL_REG(i, 139), 0xffu << 24, pdram_timing->mr11 << 24);
        mmio_write_32(CTL_REG(i, 147), (pdram_timing->mr[1] << 16) | pdram_timing->mr[0]);
        mmio_clrsetbits_32(CTL_REG(i, 148), 0xffff, pdram_timing->mr[2]);
        mmio_clrsetbits_32(CTL_REG(i, 152), 0xffff, pdram_timing->mr[3]);
        mmio_clrsetbits_32(CTL_REG(i, 153), 0xffu << 24, pdram_timing->mr11 << 24);
        if (timing_config->dram_type == LPDDR4)
        {
            mmio_clrsetbits_32(CTL_REG(i, 140), 0xffffu << 16, pdram_timing->mr12 << 16);
            mmio_clrsetbits_32(CTL_REG(i, 142), 0xffffu << 16, pdram_timing->mr14 << 16);
            mmio_clrsetbits_32(CTL_REG(i, 145), 0xffffu << 16, pdram_timing->mr22 << 16);
            mmio_clrsetbits_32(CTL_REG(i, 154), 0xffffu << 16, pdram_timing->mr12 << 16);
            mmio_clrsetbits_32(CTL_REG(i, 156), 0xffffu << 16, pdram_timing->mr14 << 16);
            mmio_clrsetbits_32(CTL_REG(i, 159), 0xffffu << 16, pdram_timing->mr22 << 16);
        }
        mmio_clrsetbits_32(CTL_REG(i, 179), 0xfff << 8, pdram_timing->tzqinit << 8);
        mmio_write_32(CTL_REG(i, 180), (pdram_timing->tzqcs << 16) | (pdram_timing->tzqinit / 2));
        mmio_write_32(CTL_REG(i, 181), (pdram_timing->tzqlat << 16) | pdram_timing->tzqcal);
        mmio_clrsetbits_32(CTL_REG(i, 212), 0xff << 8, pdram_timing->todton << 8);

        if (timing_config->odt)
        {
            mmio_setbits_32(CTL_REG(i, 213), 1 << 16);
            if (timing_config->freq < 400)
            {
                tmp = 4 << 24;
            }
            else
            {
                tmp = 8 << 24;
            }
        }
        else
        {
            mmio_clrbits_32(CTL_REG(i, 213), 1 << 16);
            tmp = 2 << 24;
        }

        mmio_clrsetbits_32(CTL_REG(i, 216), 0x1f << 24, tmp);
        mmio_clrsetbits_32(
            CTL_REG(i, 221), (0x3 << 16) | (0xf << 8), (pdram_timing->tdqsck << 16) | (pdram_timing->tdqsck_max << 8));
        tmp = (get_wrlat_adj(timing_config->dram_type, pdram_timing->cwl) << 8) |
              get_rdlat_adj(timing_config->dram_type, pdram_timing->cl);
        mmio_clrsetbits_32(CTL_REG(i, 284), 0xffff, tmp);
        mmio_clrsetbits_32(CTL_REG(i, 82), 0xffffu << 16, (4 * pdram_timing->trefi) << 16);

        mmio_clrsetbits_32(CTL_REG(i, 83), 0xffff, (2 * pdram_timing->trefi) & 0xffff);

        if ((timing_config->dram_type == LPDDR3) || (timing_config->dram_type == LPDDR4))
        {
            tmp  = get_pi_wrlat(pdram_timing, timing_config);
            tmp1 = get_pi_todtoff_max(pdram_timing, timing_config);
            tmp  = (tmp > tmp1) ? (tmp - tmp1) : 0;
        }
        else
        {
            tmp = 0;
        }
        mmio_clrsetbits_32(CTL_REG(i, 214), 0x3f << 16, (tmp & 0x3f) << 16);

        if ((timing_config->dram_type == LPDDR3) || (timing_config->dram_type == LPDDR4))
        {
            /* min_rl_preamble = cl+TDQSCK_MIN -1 */
            tmp = pdram_timing->cl + get_pi_todtoff_min(pdram_timing, timing_config) - 1;
            /* todtoff_max */
            tmp1 = get_pi_todtoff_max(pdram_timing, timing_config);
            tmp  = (tmp > tmp1) ? (tmp - tmp1) : 0;
        }
        else
        {
            tmp = pdram_timing->cl - pdram_timing->cwl;
        }
        mmio_clrsetbits_32(CTL_REG(i, 215), 0x3f << 8, (tmp & 0x3f) << 8);

        mmio_clrsetbits_32(
            CTL_REG(i, 275), 0xff << 16, (get_pi_tdfi_phy_rdlat(pdram_timing, timing_config) & 0xff) << 16);

        mmio_clrsetbits_32(CTL_REG(i, 277), 0xffff, (2 * pdram_timing->trefi) & 0xffff);

        mmio_clrsetbits_32(CTL_REG(i, 282), 0xffff, (2 * pdram_timing->trefi) & 0xffff);

        mmio_write_32(CTL_REG(i, 283), 20 * pdram_timing->trefi);

        /* CTL_308 TDFI_CALVL_CAPTURE_F0:RW:16:10 */
        tmp1 = 20000 / (1000000 / pdram_timing->mhz) + 1;
        if ((20000 % (1000000 / pdram_timing->mhz)) != 0)
        {
            tmp1++;
        }
        tmp = (tmp1 >> 1) + (tmp1 % 2) + 5;
        mmio_clrsetbits_32(CTL_REG(i, 308), 0x3ff << 16, tmp << 16);

        /* CTL_308 TDFI_CALVL_CC_F0:RW:0:10 */
        tmp = tmp + 18;
        mmio_clrsetbits_32(CTL_REG(i, 308), 0x3ff, tmp);

        /* CTL_314 TDFI_WRCSLAT_F0:RW:8:8 */
        tmp1 = get_pi_wrlat_adj(pdram_timing, timing_config);
        if (timing_config->freq <= TDFI_LAT_THRESHOLD_FREQ)
        {
            if (tmp1 == 0)
            {
                tmp = 0;
            }
            else if (tmp1 < 5)
            {
                tmp = tmp1 - 1;
            }
            else
            {
                tmp = tmp1 - 5;
            }
        }
        else
        {
            tmp = tmp1 - 2;
        }
        mmio_clrsetbits_32(CTL_REG(i, 314), 0xff << 8, tmp << 8);

        /* CTL_314 TDFI_RDCSLAT_F0:RW:0:8 */
        if ((timing_config->freq <= TDFI_LAT_THRESHOLD_FREQ) && (pdram_timing->cl >= 5))
        {
            tmp = pdram_timing->cl - 5;
        }
        else
        {
            tmp = pdram_timing->cl - 2;
        }
        mmio_clrsetbits_32(CTL_REG(i, 314), 0xff, tmp);
    }
}

static void
gen_rk3399_ctl_params_f1(struct timing_related_config *timing_config, struct dram_timing_t *pdram_timing)
{
    uint32_t i;
    uint32_t tmp, tmp1;

    for (i = 0; i < timing_config->ch_cnt; i++)
    {
        if (timing_config->dram_type == DDR3)
        {
            tmp = ((700000 + 10) * timing_config->freq + 999) / 1000;
            tmp += pdram_timing->txsnr + (pdram_timing->tmrd * 3) + pdram_timing->tmod + pdram_timing->tzqinit;
            mmio_write_32(CTL_REG(i, 9), tmp);
            mmio_clrsetbits_32(CTL_REG(i, 22), 0xffffu << 16, pdram_timing->tdllk << 16);
            mmio_clrsetbits_32(CTL_REG(i, 34), 0xffffff00,
                               (pdram_timing->tmod << 24) | (pdram_timing->tmrd << 16) | (pdram_timing->trtp << 8));
            mmio_clrsetbits_32(CTL_REG(i, 60), 0xffffu << 16, (pdram_timing->txsr - pdram_timing->trcd) << 16);
        }
        else if (timing_config->dram_type == LPDDR4)
        {
            mmio_write_32(CTL_REG(i, 9), pdram_timing->tinit1 + pdram_timing->tinit3);
            mmio_clrsetbits_32(CTL_REG(i, 34), 0xffffff00,
                               (pdram_timing->tmrd << 24) | (pdram_timing->tmrd << 16) | (pdram_timing->trtp << 8));
            mmio_clrsetbits_32(CTL_REG(i, 60), 0xffffu << 16, pdram_timing->txsr << 16);
        }
        else
        {
            mmio_write_32(CTL_REG(i, 9), pdram_timing->tinit1);
            mmio_write_32(CTL_REG(i, 11), pdram_timing->tinit4);
            mmio_clrsetbits_32(CTL_REG(i, 34), 0xffffff00,
                               (pdram_timing->tmrd << 24) | (pdram_timing->tmrd << 16) | (pdram_timing->trtp << 8));
            mmio_clrsetbits_32(CTL_REG(i, 60), 0xffffu << 16, pdram_timing->txsr << 16);
        }
        mmio_write_32(CTL_REG(i, 10), pdram_timing->tinit3);
        mmio_write_32(CTL_REG(i, 12), pdram_timing->tinit5);
        mmio_clrsetbits_32(CTL_REG(i, 24), (0x7f << 8), ((pdram_timing->cl * 2) << 8));
        mmio_clrsetbits_32(CTL_REG(i, 24), (0x1f << 16), (pdram_timing->cwl << 16));
        mmio_clrsetbits_32(CTL_REG(i, 24), 0x3f << 24, pdram_timing->al << 24);
        mmio_clrsetbits_32(CTL_REG(i, 28), 0xffffff00,
                           (pdram_timing->tras_min << 24) | (pdram_timing->trc << 16) | (pdram_timing->trrd << 8));
        mmio_clrsetbits_32(
            CTL_REG(i, 29), 0xffffff, (pdram_timing->tfaw << 16) | (pdram_timing->trppb << 8) | pdram_timing->twtr);
        mmio_write_32(CTL_REG(i, 35), (pdram_timing->tcke << 24) | pdram_timing->tras_max);
        mmio_clrsetbits_32(CTL_REG(i, 36), 0xff, max(1, pdram_timing->tckesr));
        mmio_clrsetbits_32(CTL_REG(i, 39), (0xffu << 24), (pdram_timing->trcd << 24));
        mmio_clrsetbits_32(CTL_REG(i, 40), 0x3f, pdram_timing->twr);
        mmio_clrsetbits_32(CTL_REG(i, 42), 0x1f << 24, pdram_timing->tmrz << 24);
        tmp = pdram_timing->tdal ? pdram_timing->tdal : (pdram_timing->twr + pdram_timing->trp);
        mmio_clrsetbits_32(CTL_REG(i, 44), 0xff << 8, tmp << 8);
        mmio_clrsetbits_32(CTL_REG(i, 45), 0xff << 8, pdram_timing->trp << 8);
        mmio_write_32(CTL_REG(i, 49), ((pdram_timing->trefi - 8) << 16) | pdram_timing->trfc);
        mmio_clrsetbits_32(CTL_REG(i, 52), 0xffffu << 16, pdram_timing->txp << 16);
        mmio_clrsetbits_32(CTL_REG(i, 54), 0xffff, pdram_timing->txpdll);
        mmio_clrsetbits_32(CTL_REG(i, 55), 0xff << 8, pdram_timing->tmrri << 8);
        mmio_write_32(CTL_REG(i, 57), (pdram_timing->tmrwckel << 24) | (pdram_timing->tckehcs << 16) |
                                          (pdram_timing->tckelcs << 8) | pdram_timing->tcscke);
        mmio_clrsetbits_32(CTL_REG(i, 58), 0xf, pdram_timing->tzqcke);
        mmio_clrsetbits_32(CTL_REG(i, 61), 0xffff, pdram_timing->txsnr);
        mmio_clrsetbits_32(
            CTL_REG(i, 64), 0xffffu << 16, (pdram_timing->tckehcmd << 24) | (pdram_timing->tckelcmd << 16));
        mmio_write_32(CTL_REG(i, 65), (pdram_timing->tckelpd << 24) | (pdram_timing->tescke << 16) |
                                          (pdram_timing->tsr << 8) | pdram_timing->tckckel);
        mmio_clrsetbits_32(CTL_REG(i, 66), 0xfff, (pdram_timing->tcmdcke << 8) | pdram_timing->tcsckeh);
        mmio_clrsetbits_32(CTL_REG(i, 92), (0xffu << 24), (pdram_timing->tcksre << 24));
        mmio_clrsetbits_32(CTL_REG(i, 93), 0xff, pdram_timing->tcksrx);
        mmio_clrsetbits_32(CTL_REG(i, 108), (0x1 << 25), (timing_config->dllbp << 25));
        mmio_write_32(CTL_REG(i, 125), (pdram_timing->tvrcg_disable << 16) | pdram_timing->tvrcg_enable);
        mmio_write_32(
            CTL_REG(i, 126), (pdram_timing->tckfspx << 24) | (pdram_timing->tckfspe << 16) | pdram_timing->tfc_long);
        mmio_clrsetbits_32(CTL_REG(i, 127), 0xffff, pdram_timing->tvref_long);
        mmio_clrsetbits_32(CTL_REG(i, 134), 0xffffu << 16, pdram_timing->mr[0] << 16);
        mmio_write_32(CTL_REG(i, 135), (pdram_timing->mr[2] << 16) | pdram_timing->mr[1]);
        mmio_clrsetbits_32(CTL_REG(i, 138), 0xffffu << 16, pdram_timing->mr[3] << 16);
        mmio_clrsetbits_32(CTL_REG(i, 140), 0xff, pdram_timing->mr11);
        mmio_clrsetbits_32(CTL_REG(i, 148), 0xffffu << 16, pdram_timing->mr[0] << 16);
        mmio_write_32(CTL_REG(i, 149), (pdram_timing->mr[2] << 16) | pdram_timing->mr[1]);
        mmio_clrsetbits_32(CTL_REG(i, 152), 0xffffu << 16, pdram_timing->mr[3] << 16);
        mmio_clrsetbits_32(CTL_REG(i, 154), 0xff, pdram_timing->mr11);
        if (timing_config->dram_type == LPDDR4)
        {
            mmio_clrsetbits_32(CTL_REG(i, 141), 0xffff, pdram_timing->mr12);
            mmio_clrsetbits_32(CTL_REG(i, 143), 0xffff, pdram_timing->mr14);
            mmio_clrsetbits_32(CTL_REG(i, 146), 0xffff, pdram_timing->mr22);
            mmio_clrsetbits_32(CTL_REG(i, 155), 0xffff, pdram_timing->mr12);
            mmio_clrsetbits_32(CTL_REG(i, 157), 0xffff, pdram_timing->mr14);
            mmio_clrsetbits_32(CTL_REG(i, 160), 0xffff, pdram_timing->mr22);
        }
        mmio_write_32(CTL_REG(i, 182), ((pdram_timing->tzqinit / 2) << 16) | pdram_timing->tzqinit);
        mmio_write_32(CTL_REG(i, 183), (pdram_timing->tzqcal << 16) | pdram_timing->tzqcs);
        mmio_clrsetbits_32(CTL_REG(i, 184), 0x3f, pdram_timing->tzqlat);
        mmio_clrsetbits_32(CTL_REG(i, 188), 0xfff, pdram_timing->tzqreset);
        mmio_clrsetbits_32(CTL_REG(i, 212), 0xff << 16, pdram_timing->todton << 16);

        if (timing_config->odt)
        {
            mmio_setbits_32(CTL_REG(i, 213), (1 << 24));
            if (timing_config->freq < 400)
            {
                tmp = 4 << 24;
            }
            else
            {
                tmp = 8 << 24;
            }
        }
        else
        {
            mmio_clrbits_32(CTL_REG(i, 213), (1 << 24));
            tmp = 2 << 24;
        }
        mmio_clrsetbits_32(CTL_REG(i, 217), 0x1f << 24, tmp);
        mmio_clrsetbits_32(CTL_REG(i, 221), 0xf << 24, (pdram_timing->tdqsck_max << 24));
        mmio_clrsetbits_32(CTL_REG(i, 222), 0x3, pdram_timing->tdqsck);
        mmio_clrsetbits_32(CTL_REG(i, 291), 0xffff,
                           (get_wrlat_adj(timing_config->dram_type, pdram_timing->cwl) << 8) |
                               get_rdlat_adj(timing_config->dram_type, pdram_timing->cl));

        mmio_clrsetbits_32(CTL_REG(i, 84), 0xffff, (4 * pdram_timing->trefi) & 0xffff);

        mmio_clrsetbits_32(CTL_REG(i, 84), 0xffffu << 16, ((2 * pdram_timing->trefi) & 0xffff) << 16);

        if ((timing_config->dram_type == LPDDR3) || (timing_config->dram_type == LPDDR4))
        {
            tmp  = get_pi_wrlat(pdram_timing, timing_config);
            tmp1 = get_pi_todtoff_max(pdram_timing, timing_config);
            tmp  = (tmp > tmp1) ? (tmp - tmp1) : 0;
        }
        else
        {
            tmp = 0;
        }
        mmio_clrsetbits_32(CTL_REG(i, 214), 0x3f << 24, (tmp & 0x3f) << 24);

        if ((timing_config->dram_type == LPDDR3) || (timing_config->dram_type == LPDDR4))
        {
            /* min_rl_preamble = cl + TDQSCK_MIN - 1 */
            tmp = pdram_timing->cl + get_pi_todtoff_min(pdram_timing, timing_config);
            tmp--;
            /* todtoff_max */
            tmp1 = get_pi_todtoff_max(pdram_timing, timing_config);
            tmp  = (tmp > tmp1) ? (tmp - tmp1) : 0;
        }
        else
        {
            tmp = pdram_timing->cl - pdram_timing->cwl;
        }
        mmio_clrsetbits_32(CTL_REG(i, 215), 0x3f << 16, (tmp & 0x3f) << 16);

        mmio_clrsetbits_32(
            CTL_REG(i, 275), 0xffu << 24, (get_pi_tdfi_phy_rdlat(pdram_timing, timing_config) & 0xff) << 24);

        mmio_clrsetbits_32(CTL_REG(i, 284), 0xffffu << 16, ((2 * pdram_timing->trefi) & 0xffff) << 16);

        mmio_clrsetbits_32(CTL_REG(i, 289), 0xffff, (2 * pdram_timing->trefi) & 0xffff);

        mmio_write_32(CTL_REG(i, 290), 20 * pdram_timing->trefi);

        /* CTL_309 TDFI_CALVL_CAPTURE_F1:RW:16:10 */
        tmp1 = 20000 / (1000000 / pdram_timing->mhz) + 1;
        if ((20000 % (1000000 / pdram_timing->mhz)) != 0)
        {
            tmp1++;
        }
        tmp = (tmp1 >> 1) + (tmp1 % 2) + 5;
        mmio_clrsetbits_32(CTL_REG(i, 309), 0x3ff << 16, tmp << 16);

        /* CTL_309 TDFI_CALVL_CC_F1:RW:0:10 */
        tmp = tmp + 18;
        mmio_clrsetbits_32(CTL_REG(i, 309), 0x3ff, tmp);

        /* CTL_314 TDFI_WRCSLAT_F1:RW:24:8 */
        tmp1 = get_pi_wrlat_adj(pdram_timing, timing_config);
        if (timing_config->freq <= TDFI_LAT_THRESHOLD_FREQ)
        {
            if (tmp1 == 0)
            {
                tmp = 0;
            }
            else if (tmp1 < 5)
            {
                tmp = tmp1 - 1;
            }
            else
            {
                tmp = tmp1 - 5;
            }
        }
        else
        {
            tmp = tmp1 - 2;
        }

        mmio_clrsetbits_32(CTL_REG(i, 314), 0xffu << 24, tmp << 24);

        /* CTL_314 TDFI_RDCSLAT_F1:RW:16:8 */
        if ((timing_config->freq <= TDFI_LAT_THRESHOLD_FREQ) && (pdram_timing->cl >= 5))
        {
            tmp = pdram_timing->cl - 5;
        }
        else
        {
            tmp = pdram_timing->cl - 2;
        }
        mmio_clrsetbits_32(CTL_REG(i, 314), 0xff << 16, tmp << 16);
    }
}

static void
gen_rk3399_enable_training(uint32_t ch_cnt, uint32_t nmhz)
{
    uint32_t i, tmp;

    if (nmhz <= PHY_DLL_BYPASS_FREQ)
    {
        tmp = 0;
    }
    else
    {
        tmp = 1;
    }

    for (i = 0; i < ch_cnt; i++)
    {
        mmio_clrsetbits_32(CTL_REG(i, 305), 1 << 16, tmp << 16);
        mmio_clrsetbits_32(CTL_REG(i, 71), 1, tmp);
        mmio_clrsetbits_32(CTL_REG(i, 70), 1 << 8, 1 << 8);
    }
}

static void
gen_rk3399_disable_training(uint32_t ch_cnt)
{
    uint32_t i;

    for (i = 0; i < ch_cnt; i++)
    {
        mmio_clrbits_32(CTL_REG(i, 305), 1 << 16);
        mmio_clrbits_32(CTL_REG(i, 71), 1);
        mmio_clrbits_32(CTL_REG(i, 70), 1 << 8);
    }
}

static void
gen_rk3399_ctl_params(struct timing_related_config *timing_config, struct dram_timing_t *pdram_timing, uint32_t fn)
{
    if (fn == 0)
    {
        gen_rk3399_ctl_params_f0(timing_config, pdram_timing);
    }
    else
    {
        gen_rk3399_ctl_params_f1(timing_config, pdram_timing);
    }
}

static void
gen_rk3399_pi_params_f0(struct timing_related_config *timing_config, struct dram_timing_t *pdram_timing)
{
    uint32_t tmp, tmp1, tmp2;
    uint32_t i;

    for (i = 0; i < timing_config->ch_cnt; i++)
    {
        /* PI_02 PI_TDFI_PHYMSTR_MAX_F0:RW:0:32 */
        tmp = 4 * pdram_timing->trefi;
        mmio_write_32(PI_REG(i, 2), tmp);
        /* PI_03 PI_TDFI_PHYMSTR_RESP_F0:RW:0:16 */
        tmp = 2 * pdram_timing->trefi;
        mmio_clrsetbits_32(PI_REG(i, 3), 0xffff, tmp);
        /* PI_07 PI_TDFI_PHYUPD_RESP_F0:RW:16:16 */
        mmio_clrsetbits_32(PI_REG(i, 7), 0xffffu << 16, tmp << 16);

        /* PI_42 PI_TDELAY_RDWR_2_BUS_IDLE_F0:RW:0:8 */
        if (timing_config->dram_type == LPDDR4)
        {
            tmp = 2;
        }
        else
        {
            tmp = 0;
        }
        tmp = (pdram_timing->bl / 2) + 4 + (get_pi_rdlat_adj(pdram_timing) - 2) + tmp +
              get_pi_tdfi_phy_rdlat(pdram_timing, timing_config);
        mmio_clrsetbits_32(PI_REG(i, 42), 0xff, tmp);
        /* PI_43 PI_WRLAT_F0:RW:0:5 */
        if (timing_config->dram_type == LPDDR3)
        {
            tmp = get_pi_wrlat(pdram_timing, timing_config);
            mmio_clrsetbits_32(PI_REG(i, 43), 0x1f, tmp);
        }
        /* PI_43 PI_ADDITIVE_LAT_F0:RW:8:6 */
        mmio_clrsetbits_32(PI_REG(i, 43), 0x3f << 8, PI_ADD_LATENCY << 8);

        /* PI_43 PI_CASLAT_LIN_F0:RW:16:7 */
        mmio_clrsetbits_32(PI_REG(i, 43), 0x7f << 16, (pdram_timing->cl * 2) << 16);
        /* PI_46 PI_TREF_F0:RW:16:16 */
        mmio_clrsetbits_32(PI_REG(i, 46), 0xffffu << 16, pdram_timing->trefi << 16);
        /* PI_46 PI_TRFC_F0:RW:0:10 */
        mmio_clrsetbits_32(PI_REG(i, 46), 0x3ff, pdram_timing->trfc);
        /* PI_66 PI_TODTL_2CMD_F0:RW:24:8 */
        if (timing_config->dram_type == LPDDR3)
        {
            tmp = get_pi_todtoff_max(pdram_timing, timing_config);
            mmio_clrsetbits_32(PI_REG(i, 66), 0xffu << 24, tmp << 24);
        }
        /* PI_72 PI_WR_TO_ODTH_F0:RW:16:6 */
        if ((timing_config->dram_type == LPDDR3) || (timing_config->dram_type == LPDDR4))
        {
            tmp1 = get_pi_wrlat(pdram_timing, timing_config);
            tmp2 = get_pi_todtoff_max(pdram_timing, timing_config);
            if (tmp1 > tmp2)
            {
                tmp = tmp1 - tmp2;
            }
            else
            {
                tmp = 0;
            }
        }
        else if (timing_config->dram_type == DDR3)
        {
            tmp = 0;
        }
        mmio_clrsetbits_32(PI_REG(i, 72), 0x3f << 16, tmp << 16);
        /* PI_73 PI_RD_TO_ODTH_F0:RW:8:6 */
        if ((timing_config->dram_type == LPDDR3) || (timing_config->dram_type == LPDDR4))
        {
            /* min_rl_preamble = cl + TDQSCK_MIN - 1 */
            tmp1 = pdram_timing->cl;
            tmp1 += get_pi_todtoff_min(pdram_timing, timing_config);
            tmp1--;
            /* todtoff_max */
            tmp2 = get_pi_todtoff_max(pdram_timing, timing_config);
            if (tmp1 > tmp2)
            {
                tmp = tmp1 - tmp2;
            }
            else
            {
                tmp = 0;
            }
        }
        else if (timing_config->dram_type == DDR3)
        {
            tmp = pdram_timing->cl - pdram_timing->cwl;
        }
        mmio_clrsetbits_32(PI_REG(i, 73), 0x3f << 8, tmp << 8);
        /* PI_89 PI_RDLAT_ADJ_F0:RW:16:8 */
        tmp = get_pi_rdlat_adj(pdram_timing);
        mmio_clrsetbits_32(PI_REG(i, 89), 0xff << 16, tmp << 16);
        /* PI_90 PI_WRLAT_ADJ_F0:RW:16:8 */
        tmp = get_pi_wrlat_adj(pdram_timing, timing_config);
        mmio_clrsetbits_32(PI_REG(i, 90), 0xff << 16, tmp << 16);
        /* PI_91 PI_TDFI_WRCSLAT_F0:RW:16:8 */
        tmp1 = tmp;
        if (tmp1 == 0)
        {
            tmp = 0;
        }
        else if (tmp1 < 5)
        {
            tmp = tmp1 - 1;
        }
        else
        {
            tmp = tmp1 - 5;
        }
        mmio_clrsetbits_32(PI_REG(i, 91), 0xff << 16, tmp << 16);
        /* PI_95 PI_TDFI_CALVL_CAPTURE_F0:RW:16:10 */
        tmp1 = 20000 / (1000000 / pdram_timing->mhz) + 1;
        if ((20000 % (1000000 / pdram_timing->mhz)) != 0)
        {
            tmp1++;
        }
        tmp = (tmp1 >> 1) + (tmp1 % 2) + 5;
        mmio_clrsetbits_32(PI_REG(i, 95), 0x3ff << 16, tmp << 16);
        /* PI_95 PI_TDFI_CALVL_CC_F0:RW:0:10 */
        mmio_clrsetbits_32(PI_REG(i, 95), 0x3ff, tmp + 18);
        /* PI_102 PI_TMRZ_F0:RW:8:5 */
        mmio_clrsetbits_32(PI_REG(i, 102), 0x1f << 8, pdram_timing->tmrz << 8);
        /* PI_111 PI_TDFI_CALVL_STROBE_F0:RW:8:4 */
        tmp1 = 2 * 1000 / (1000000 / pdram_timing->mhz);
        if ((2 * 1000 % (1000000 / pdram_timing->mhz)) != 0)
        {
            tmp1++;
        }
        /* pi_tdfi_calvl_strobe=tds_train+5 */
        tmp = tmp1 + 5;
        mmio_clrsetbits_32(PI_REG(i, 111), 0xf << 8, tmp << 8);
        /* PI_116 PI_TCKEHDQS_F0:RW:16:6 */
        tmp = 10000 / (1000000 / pdram_timing->mhz);
        if ((10000 % (1000000 / pdram_timing->mhz)) != 0)
        {
            tmp++;
        }
        if (pdram_timing->mhz <= 100)
        {
            tmp = tmp + 1;
        }
        else
        {
            tmp = tmp + 8;
        }
        mmio_clrsetbits_32(PI_REG(i, 116), 0x3f << 16, tmp << 16);
        /* PI_125 PI_MR1_DATA_F0_0:RW+:8:16 */
        mmio_clrsetbits_32(PI_REG(i, 125), 0xffff << 8, pdram_timing->mr[1] << 8);
        /* PI_133 PI_MR1_DATA_F0_1:RW+:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 133), 0xffff, pdram_timing->mr[1]);
        /* PI_140 PI_MR1_DATA_F0_2:RW+:16:16 */
        mmio_clrsetbits_32(PI_REG(i, 140), 0xffffu << 16, pdram_timing->mr[1] << 16);
        /* PI_148 PI_MR1_DATA_F0_3:RW+:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 148), 0xffff, pdram_timing->mr[1]);
        /* PI_126 PI_MR2_DATA_F0_0:RW+:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 126), 0xffff, pdram_timing->mr[2]);
        /* PI_133 PI_MR2_DATA_F0_1:RW+:16:16 */
        mmio_clrsetbits_32(PI_REG(i, 133), 0xffffu << 16, pdram_timing->mr[2] << 16);
        /* PI_141 PI_MR2_DATA_F0_2:RW+:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 141), 0xffff, pdram_timing->mr[2]);
        /* PI_148 PI_MR2_DATA_F0_3:RW+:16:16 */
        mmio_clrsetbits_32(PI_REG(i, 148), 0xffffu << 16, pdram_timing->mr[2] << 16);
        /* PI_156 PI_TFC_F0:RW:0:10 */
        mmio_clrsetbits_32(PI_REG(i, 156), 0x3ff, pdram_timing->tfc_long);
        /* PI_158 PI_TWR_F0:RW:24:6 */
        mmio_clrsetbits_32(PI_REG(i, 158), 0x3f << 24, pdram_timing->twr << 24);
        /* PI_158 PI_TWTR_F0:RW:16:6 */
        mmio_clrsetbits_32(PI_REG(i, 158), 0x3f << 16, pdram_timing->twtr << 16);
        /* PI_158 PI_TRCD_F0:RW:8:8 */
        mmio_clrsetbits_32(PI_REG(i, 158), 0xff << 8, pdram_timing->trcd << 8);
        /* PI_158 PI_TRP_F0:RW:0:8 */
        mmio_clrsetbits_32(PI_REG(i, 158), 0xff, pdram_timing->trp);
        /* PI_157 PI_TRTP_F0:RW:24:8 */
        mmio_clrsetbits_32(PI_REG(i, 157), 0xffu << 24, pdram_timing->trtp << 24);
        /* PI_159 PI_TRAS_MIN_F0:RW:24:8 */
        mmio_clrsetbits_32(PI_REG(i, 159), 0xffu << 24, pdram_timing->tras_min << 24);
        /* PI_159 PI_TRAS_MAX_F0:RW:0:17 */
        tmp = pdram_timing->tras_max * 99 / 100;
        mmio_clrsetbits_32(PI_REG(i, 159), 0x1ffff, tmp);
        /* PI_160 PI_TMRD_F0:RW:16:6 */
        mmio_clrsetbits_32(PI_REG(i, 160), 0x3f << 16, pdram_timing->tmrd << 16);
        /*PI_160 PI_TDQSCK_MAX_F0:RW:0:4 */
        mmio_clrsetbits_32(PI_REG(i, 160), 0xf, pdram_timing->tdqsck_max);
        /* PI_187 PI_TDFI_CTRLUPD_MAX_F0:RW:8:16 */
        mmio_clrsetbits_32(PI_REG(i, 187), 0xffff << 8, (2 * pdram_timing->trefi) << 8);
        /* PI_188 PI_TDFI_CTRLUPD_INTERVAL_F0:RW:0:32 */
        mmio_clrsetbits_32(PI_REG(i, 188), 0xffffffff, 20 * pdram_timing->trefi);
    }
}

static void
gen_rk3399_pi_params_f1(struct timing_related_config *timing_config, struct dram_timing_t *pdram_timing)
{
    uint32_t tmp, tmp1, tmp2;
    uint32_t i;

    for (i = 0; i < timing_config->ch_cnt; i++)
    {
        /* PI_04 PI_TDFI_PHYMSTR_MAX_F1:RW:0:32 */
        tmp = 4 * pdram_timing->trefi;
        mmio_write_32(PI_REG(i, 4), tmp);
        /* PI_05 PI_TDFI_PHYMSTR_RESP_F1:RW:0:16 */
        tmp = 2 * pdram_timing->trefi;
        mmio_clrsetbits_32(PI_REG(i, 5), 0xffff, tmp);
        /* PI_12 PI_TDFI_PHYUPD_RESP_F1:RW:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 12), 0xffff, tmp);

        /* PI_42 PI_TDELAY_RDWR_2_BUS_IDLE_F1:RW:8:8 */
        if (timing_config->dram_type == LPDDR4)
        {
            tmp = 2;
        }
        else
        {
            tmp = 0;
        }
        tmp = (pdram_timing->bl / 2) + 4 + (get_pi_rdlat_adj(pdram_timing) - 2) + tmp +
              get_pi_tdfi_phy_rdlat(pdram_timing, timing_config);
        mmio_clrsetbits_32(PI_REG(i, 42), 0xff << 8, tmp << 8);
        /* PI_43 PI_WRLAT_F1:RW:24:5 */
        if (timing_config->dram_type == LPDDR3)
        {
            tmp = get_pi_wrlat(pdram_timing, timing_config);
            mmio_clrsetbits_32(PI_REG(i, 43), 0x1f << 24, tmp << 24);
        }
        /* PI_44 PI_ADDITIVE_LAT_F1:RW:0:6 */
        mmio_clrsetbits_32(PI_REG(i, 44), 0x3f, PI_ADD_LATENCY);
        /* PI_44 PI_CASLAT_LIN_F1:RW:8:7:=0x18 */
        mmio_clrsetbits_32(PI_REG(i, 44), 0x7f << 8, (pdram_timing->cl * 2) << 8);
        /* PI_47 PI_TREF_F1:RW:16:16 */
        mmio_clrsetbits_32(PI_REG(i, 47), 0xffffu << 16, pdram_timing->trefi << 16);
        /* PI_47 PI_TRFC_F1:RW:0:10 */
        mmio_clrsetbits_32(PI_REG(i, 47), 0x3ff, pdram_timing->trfc);
        /* PI_67 PI_TODTL_2CMD_F1:RW:8:8 */
        if (timing_config->dram_type == LPDDR3)
        {
            tmp = get_pi_todtoff_max(pdram_timing, timing_config);
            mmio_clrsetbits_32(PI_REG(i, 67), 0xff << 8, tmp << 8);
        }
        /* PI_72 PI_WR_TO_ODTH_F1:RW:24:6 */
        if ((timing_config->dram_type == LPDDR3) || (timing_config->dram_type == LPDDR4))
        {
            tmp1 = get_pi_wrlat(pdram_timing, timing_config);
            tmp2 = get_pi_todtoff_max(pdram_timing, timing_config);
            if (tmp1 > tmp2)
            {
                tmp = tmp1 - tmp2;
            }
            else
            {
                tmp = 0;
            }
        }
        else if (timing_config->dram_type == DDR3)
        {
            tmp = 0;
        }
        mmio_clrsetbits_32(PI_REG(i, 72), 0x3f << 24, tmp << 24);
        /* PI_73 PI_RD_TO_ODTH_F1:RW:16:6 */
        if ((timing_config->dram_type == LPDDR3) || (timing_config->dram_type == LPDDR4))
        {
            /* min_rl_preamble = cl + TDQSCK_MIN - 1 */
            tmp1 = pdram_timing->cl + get_pi_todtoff_min(pdram_timing, timing_config);
            tmp1--;
            /* todtoff_max */
            tmp2 = get_pi_todtoff_max(pdram_timing, timing_config);
            if (tmp1 > tmp2)
            {
                tmp = tmp1 - tmp2;
            }
            else
            {
                tmp = 0;
            }
        }
        else if (timing_config->dram_type == DDR3)
        {
            tmp = pdram_timing->cl - pdram_timing->cwl;
        }

        mmio_clrsetbits_32(PI_REG(i, 73), 0x3f << 16, tmp << 16);
        /*P I_89 PI_RDLAT_ADJ_F1:RW:24:8 */
        tmp = get_pi_rdlat_adj(pdram_timing);
        mmio_clrsetbits_32(PI_REG(i, 89), 0xffu << 24, tmp << 24);
        /* PI_90 PI_WRLAT_ADJ_F1:RW:24:8 */
        tmp = get_pi_wrlat_adj(pdram_timing, timing_config);
        mmio_clrsetbits_32(PI_REG(i, 90), 0xffu << 24, tmp << 24);
        /* PI_91 PI_TDFI_WRCSLAT_F1:RW:24:8 */
        tmp1 = tmp;
        if (tmp1 == 0)
        {
            tmp = 0;
        }
        else if (tmp1 < 5)
        {
            tmp = tmp1 - 1;
        }
        else
        {
            tmp = tmp1 - 5;
        }
        mmio_clrsetbits_32(PI_REG(i, 91), 0xffu << 24, tmp << 24);
        /*PI_96 PI_TDFI_CALVL_CAPTURE_F1:RW:16:10 */
        /* tadr=20ns */
        tmp1 = 20000 / (1000000 / pdram_timing->mhz) + 1;
        if ((20000 % (1000000 / pdram_timing->mhz)) != 0)
        {
            tmp1++;
        }
        tmp = (tmp1 >> 1) + (tmp1 % 2) + 5;
        mmio_clrsetbits_32(PI_REG(i, 96), 0x3ff << 16, tmp << 16);
        /* PI_96 PI_TDFI_CALVL_CC_F1:RW:0:10 */
        tmp = tmp + 18;
        mmio_clrsetbits_32(PI_REG(i, 96), 0x3ff, tmp);
        /*PI_103 PI_TMRZ_F1:RW:0:5 */
        mmio_clrsetbits_32(PI_REG(i, 103), 0x1f, pdram_timing->tmrz);
        /*PI_111 PI_TDFI_CALVL_STROBE_F1:RW:16:4 */
        /* tds_train=ceil(2/ns) */
        tmp1 = 2 * 1000 / (1000000 / pdram_timing->mhz);
        if ((2 * 1000 % (1000000 / pdram_timing->mhz)) != 0)
        {
            tmp1++;
        }
        /* pi_tdfi_calvl_strobe=tds_train+5 */
        tmp = tmp1 + 5;
        mmio_clrsetbits_32(PI_REG(i, 111), 0xf << 16, tmp << 16);
        /* PI_116 PI_TCKEHDQS_F1:RW:24:6 */
        tmp = 10000 / (1000000 / pdram_timing->mhz);
        if ((10000 % (1000000 / pdram_timing->mhz)) != 0)
        {
            tmp++;
        }
        if (pdram_timing->mhz <= 100)
        {
            tmp = tmp + 1;
        }
        else
        {
            tmp = tmp + 8;
        }
        mmio_clrsetbits_32(PI_REG(i, 116), 0x3f << 24, tmp << 24);
        /* PI_128 PI_MR1_DATA_F1_0:RW+:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 128), 0xffff, pdram_timing->mr[1]);
        /* PI_135 PI_MR1_DATA_F1_1:RW+:8:16 */
        mmio_clrsetbits_32(PI_REG(i, 135), 0xffff << 8, pdram_timing->mr[1] << 8);
        /* PI_143 PI_MR1_DATA_F1_2:RW+:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 143), 0xffff, pdram_timing->mr[1]);
        /* PI_150 PI_MR1_DATA_F1_3:RW+:8:16 */
        mmio_clrsetbits_32(PI_REG(i, 150), 0xffff << 8, pdram_timing->mr[1] << 8);
        /* PI_128 PI_MR2_DATA_F1_0:RW+:16:16 */
        mmio_clrsetbits_32(PI_REG(i, 128), 0xffffu << 16, pdram_timing->mr[2] << 16);
        /* PI_136 PI_MR2_DATA_F1_1:RW+:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 136), 0xffff, pdram_timing->mr[2]);
        /* PI_143 PI_MR2_DATA_F1_2:RW+:16:16 */
        mmio_clrsetbits_32(PI_REG(i, 143), 0xffffu << 16, pdram_timing->mr[2] << 16);
        /* PI_151 PI_MR2_DATA_F1_3:RW+:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 151), 0xffff, pdram_timing->mr[2]);
        /* PI_156 PI_TFC_F1:RW:16:10 */
        mmio_clrsetbits_32(PI_REG(i, 156), 0x3ff << 16, pdram_timing->tfc_long << 16);
        /* PI_162 PI_TWR_F1:RW:8:6 */
        mmio_clrsetbits_32(PI_REG(i, 162), 0x3f << 8, pdram_timing->twr << 8);
        /* PI_162 PI_TWTR_F1:RW:0:6 */
        mmio_clrsetbits_32(PI_REG(i, 162), 0x3f, pdram_timing->twtr);
        /* PI_161 PI_TRCD_F1:RW:24:8 */
        mmio_clrsetbits_32(PI_REG(i, 161), 0xffu << 24, pdram_timing->trcd << 24);
        /* PI_161 PI_TRP_F1:RW:16:8 */
        mmio_clrsetbits_32(PI_REG(i, 161), 0xff << 16, pdram_timing->trp << 16);
        /* PI_161 PI_TRTP_F1:RW:8:8 */
        mmio_clrsetbits_32(PI_REG(i, 161), 0xff << 8, pdram_timing->trtp << 8);
        /* PI_163 PI_TRAS_MIN_F1:RW:24:8 */
        mmio_clrsetbits_32(PI_REG(i, 163), 0xffu << 24, pdram_timing->tras_min << 24);
        /* PI_163 PI_TRAS_MAX_F1:RW:0:17 */
        mmio_clrsetbits_32(PI_REG(i, 163), 0x1ffff, pdram_timing->tras_max * 99 / 100);
        /* PI_164 PI_TMRD_F1:RW:16:6 */
        mmio_clrsetbits_32(PI_REG(i, 164), 0x3f << 16, pdram_timing->tmrd << 16);
        /* PI_164 PI_TDQSCK_MAX_F1:RW:0:4 */
        mmio_clrsetbits_32(PI_REG(i, 164), 0xf, pdram_timing->tdqsck_max);
        /* PI_189 PI_TDFI_CTRLUPD_MAX_F1:RW:0:16 */
        mmio_clrsetbits_32(PI_REG(i, 189), 0xffff, 2 * pdram_timing->trefi);
        /* PI_190 PI_TDFI_CTRLUPD_INTERVAL_F1:RW:0:32 */
        mmio_clrsetbits_32(PI_REG(i, 190), 0xffffffff, 20 * pdram_timing->trefi);
    }
}

static void
gen_rk3399_pi_params(struct timing_related_config *timing_config, struct dram_timing_t *pdram_timing, uint32_t fn)
{
    if (fn == 0)
    {
        gen_rk3399_pi_params_f0(timing_config, pdram_timing);
    }
    else
    {
        gen_rk3399_pi_params_f1(timing_config, pdram_timing);
    }
}

static void
gen_rk3399_set_odt(uint32_t odt_en)
{
    uint32_t drv_odt_val;
    uint32_t i;

    for (i = 0; i < rk3399_dram_status.timing_config.ch_cnt; i++)
    {
        drv_odt_val = (odt_en | (0 << 1) | (0 << 2)) << 16;
        mmio_clrsetbits_32(PHY_REG(i, 5), 0x7 << 16, drv_odt_val);
        mmio_clrsetbits_32(PHY_REG(i, 133), 0x7 << 16, drv_odt_val);
        mmio_clrsetbits_32(PHY_REG(i, 261), 0x7 << 16, drv_odt_val);
        mmio_clrsetbits_32(PHY_REG(i, 389), 0x7 << 16, drv_odt_val);
        drv_odt_val = (odt_en | (0 << 1) | (0 << 2)) << 24;
        mmio_clrsetbits_32(PHY_REG(i, 6), 0x7 << 24, drv_odt_val);
        mmio_clrsetbits_32(PHY_REG(i, 134), 0x7 << 24, drv_odt_val);
        mmio_clrsetbits_32(PHY_REG(i, 262), 0x7 << 24, drv_odt_val);
        mmio_clrsetbits_32(PHY_REG(i, 390), 0x7 << 24, drv_odt_val);
    }
}

static void
gen_rk3399_phy_dll_bypass(uint32_t mhz, uint32_t ch, uint32_t index, uint32_t dram_type)
{
    uint32_t sw_master_mode = 0;
    uint32_t rddqs_gate_delay, rddqs_latency, total_delay;
    uint32_t i;

    if (dram_type == DDR3)
    {
        total_delay = PI_PAD_DELAY_PS_VALUE;
    }
    else if (dram_type == LPDDR3)
    {
        total_delay = PI_PAD_DELAY_PS_VALUE + 2500;
    }
    else
    {
        total_delay = PI_PAD_DELAY_PS_VALUE + 1500;
    }
    /* total_delay + 0.55tck */
    total_delay += (55 * 10000) / mhz;
    rddqs_latency = total_delay * mhz / 1000000;
    total_delay -= rddqs_latency * 1000000 / mhz;
    rddqs_gate_delay = total_delay * 0x200 * mhz / 1000000;
    if (mhz <= PHY_DLL_BYPASS_FREQ)
    {
        sw_master_mode = 0xc;
        mmio_setbits_32(PHY_REG(ch, 514), 1);
        mmio_setbits_32(PHY_REG(ch, 642), 1);
        mmio_setbits_32(PHY_REG(ch, 770), 1);

        /* setting bypass mode slave delay */
        for (i = 0; i < 4; i++)
        {
            /* wr dq delay = -180deg + (0x60 / 4) * 20ps */
            mmio_clrsetbits_32(PHY_REG(ch, 1 + 128 * i), 0x7ff << 8, 0x4a0 << 8);
            /* rd dqs/dq delay = (0x60 / 4) * 20ps */
            mmio_clrsetbits_32(PHY_REG(ch, 11 + 128 * i), 0x3ff, 0xa0);
            /* rd rddqs_gate delay */
            mmio_clrsetbits_32(PHY_REG(ch, 2 + 128 * i), 0x3ff, rddqs_gate_delay);
            mmio_clrsetbits_32(PHY_REG(ch, 78 + 128 * i), 0xf, rddqs_latency);
        }
        for (i = 0; i < 3; i++)
        {
            /* adr delay */
            mmio_clrsetbits_32(PHY_REG(ch, 513 + 128 * i), 0x7ff << 16, 0x80 << 16);
        }

        if ((mmio_read_32(PHY_REG(ch, 86)) & 0xc00) == 0)
        {
            /*
             * old status is normal mode,
             * and saving the wrdqs slave delay
             */
            for (i = 0; i < 4; i++)
            {
                /* save and clear wr dqs slave delay */
                wrdqs_delay_val[ch][index][i] = 0x3ff & (mmio_read_32(PHY_REG(ch, 63 + i * 128)) >> 16);
                mmio_clrsetbits_32(PHY_REG(ch, 63 + i * 128), 0x03ff << 16, 0 << 16);
                /*
                 * in normal mode the cmd may delay 1cycle by
                 * wrlvl and in bypass mode making dqs also
                 * delay 1cycle.
                 */
                mmio_clrsetbits_32(PHY_REG(ch, 78 + i * 128), 0x07 << 8, 0x1 << 8);
            }
        }
    }
    else if (mmio_read_32(PHY_REG(ch, 86)) & 0xc00)
    {
        /* old status is bypass mode and restore wrlvl resume */
        for (i = 0; i < 4; i++)
        {
            mmio_clrsetbits_32(PHY_REG(ch, 63 + i * 128), 0x03ff << 16, (wrdqs_delay_val[ch][index][i] & 0x3ff) << 16);
            /* resume phy_write_path_lat_add */
            mmio_clrbits_32(PHY_REG(ch, 78 + i * 128), 0x07 << 8);
        }
    }

    /* phy_sw_master_mode_X PHY_86/214/342/470 4bits offset_8 */
    mmio_clrsetbits_32(PHY_REG(ch, 86), 0xf << 8, sw_master_mode << 8);
    mmio_clrsetbits_32(PHY_REG(ch, 214), 0xf << 8, sw_master_mode << 8);
    mmio_clrsetbits_32(PHY_REG(ch, 342), 0xf << 8, sw_master_mode << 8);
    mmio_clrsetbits_32(PHY_REG(ch, 470), 0xf << 8, sw_master_mode << 8);

    /* phy_adrctl_sw_master_mode PHY_547/675/803 4bits offset_16 */
    mmio_clrsetbits_32(PHY_REG(ch, 547), 0xf << 16, sw_master_mode << 16);
    mmio_clrsetbits_32(PHY_REG(ch, 675), 0xf << 16, sw_master_mode << 16);
    mmio_clrsetbits_32(PHY_REG(ch, 803), 0xf << 16, sw_master_mode << 16);
}

static void
gen_rk3399_phy_params(struct timing_related_config *timing_config, struct drv_odt_lp_config *drv_config,
                      struct dram_timing_t *pdram_timing, uint32_t fn)
{
    uint32_t tmp, i, div, j;
    uint32_t mem_delay_ps, pad_delay_ps, total_delay_ps, delay_frac_ps;
    uint32_t trpre_min_ps, gate_delay_ps, gate_delay_frac_ps;
    uint32_t ie_enable, tsel_enable, cas_lat, rddata_en_ie_dly, tsel_adder;
    uint32_t extra_adder, delta, hs_offset;

    for (i = 0; i < timing_config->ch_cnt; i++)
    {

        pad_delay_ps = PI_PAD_DELAY_PS_VALUE;
        ie_enable    = PI_IE_ENABLE_VALUE;
        tsel_enable  = PI_TSEL_ENABLE_VALUE;

        mmio_clrsetbits_32(PHY_REG(i, 896), (0x3 << 8) | 1, fn << 8);

        /* PHY_LOW_FREQ_SEL */
        /* DENALI_PHY_913 1bit offset_0 */
        if (timing_config->freq > 400)
        {
            mmio_clrbits_32(PHY_REG(i, 913), 1);
        }
        else
        {
            mmio_setbits_32(PHY_REG(i, 913), 1);
        }

        /* PHY_RPTR_UPDATE_x */
        /* DENALI_PHY_87/215/343/471 4bit offset_16 */
        tmp = 2500 / (1000000 / pdram_timing->mhz) + 3;
        if ((2500 % (1000000 / pdram_timing->mhz)) != 0)
        {
            tmp++;
        }
        mmio_clrsetbits_32(PHY_REG(i, 87), 0xf << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 215), 0xf << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 343), 0xf << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 471), 0xf << 16, tmp << 16);

        /* PHY_PLL_CTRL */
        /* DENALI_PHY_911 13bits offset_0 */
        /* PHY_LP4_BOOT_PLL_CTRL */
        /* DENALI_PHY_919 13bits offset_0 */
        tmp = (1 << 12) | (2 << 7) | (1 << 1);
        mmio_clrsetbits_32(PHY_REG(i, 911), 0x1fff, tmp);
        mmio_clrsetbits_32(PHY_REG(i, 919), 0x1fff, tmp);

        /* PHY_PLL_CTRL_CA */
        /* DENALI_PHY_911 13bits offset_16 */
        /* PHY_LP4_BOOT_PLL_CTRL_CA */
        /* DENALI_PHY_919 13bits offset_16 */
        tmp = (2 << 7) | (1 << 5) | (1 << 1);
        mmio_clrsetbits_32(PHY_REG(i, 911), 0x1fff << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 919), 0x1fff << 16, tmp << 16);

        /* PHY_TCKSRE_WAIT */
        /* DENALI_PHY_922 4bits offset_24 */
        if (pdram_timing->mhz <= 400)
        {
            tmp = 1;
        }
        else if (pdram_timing->mhz <= 800)
        {
            tmp = 3;
        }
        else if (pdram_timing->mhz <= 1000)
        {
            tmp = 4;
        }
        else
        {
            tmp = 5;
        }
        mmio_clrsetbits_32(PHY_REG(i, 922), 0xf << 24, tmp << 24);
        /* PHY_CAL_CLK_SELECT_0:RW8:3 */
        div = pdram_timing->mhz / (2 * 20);
        for (j = 2, tmp = 1; j <= 128; j <<= 1, tmp++)
        {
            if (div < j)
            {
                break;
            }
        }
        mmio_clrsetbits_32(PHY_REG(i, 947), 0x7 << 8, tmp << 8);

        if (timing_config->dram_type == DDR3)
        {
            mem_delay_ps = 0;
            trpre_min_ps = 1000;
        }
        else if (timing_config->dram_type == LPDDR4)
        {
            mem_delay_ps = 1500;
            trpre_min_ps = 900;
        }
        else if (timing_config->dram_type == LPDDR3)
        {
            mem_delay_ps = 2500;
            trpre_min_ps = 900;
        }
        else
        {
            ERROR("gen_rk3399_phy_params:dramtype unsupport\n");
            return;
        }
        total_delay_ps     = mem_delay_ps + pad_delay_ps;
        delay_frac_ps      = 1000 * total_delay_ps / (1000000 / pdram_timing->mhz);
        gate_delay_ps      = delay_frac_ps + 1000 - (trpre_min_ps / 2);
        gate_delay_frac_ps = gate_delay_ps % 1000;
        tmp                = gate_delay_frac_ps * 0x200 / 1000;
        /* PHY_RDDQS_GATE_SLAVE_DELAY */
        /* DENALI_PHY_77/205/333/461 10bits offset_16 */
        mmio_clrsetbits_32(PHY_REG(i, 77), 0x2ff << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 205), 0x2ff << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 333), 0x2ff << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 461), 0x2ff << 16, tmp << 16);

        tmp = gate_delay_ps / 1000;
        /* PHY_LP4_BOOT_RDDQS_LATENCY_ADJUST */
        /* DENALI_PHY_10/138/266/394 4bit offset_0 */
        mmio_clrsetbits_32(PHY_REG(i, 10), 0xf, tmp);
        mmio_clrsetbits_32(PHY_REG(i, 138), 0xf, tmp);
        mmio_clrsetbits_32(PHY_REG(i, 266), 0xf, tmp);
        mmio_clrsetbits_32(PHY_REG(i, 394), 0xf, tmp);
        /* PHY_GTLVL_LAT_ADJ_START */
        /* DENALI_PHY_80/208/336/464 4bits offset_16 */
        tmp = rddqs_delay_ps / (1000000 / pdram_timing->mhz) + 2;
        mmio_clrsetbits_32(PHY_REG(i, 80), 0xf << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 208), 0xf << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 336), 0xf << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 464), 0xf << 16, tmp << 16);

        cas_lat          = pdram_timing->cl + PI_ADD_LATENCY;
        rddata_en_ie_dly = ie_enable / (1000000 / pdram_timing->mhz);
        if ((ie_enable % (1000000 / pdram_timing->mhz)) != 0)
        {
            rddata_en_ie_dly++;
        }
        rddata_en_ie_dly = rddata_en_ie_dly - 1;
        tsel_adder       = tsel_enable / (1000000 / pdram_timing->mhz);
        if ((tsel_enable % (1000000 / pdram_timing->mhz)) != 0)
        {
            tsel_adder++;
        }
        if (rddata_en_ie_dly > tsel_adder)
        {
            extra_adder = rddata_en_ie_dly - tsel_adder;
        }
        else
        {
            extra_adder = 0;
        }
        delta = cas_lat - rddata_en_ie_dly;
        if (PI_REGS_DIMM_SUPPORT && PI_DOUBLEFREEK)
        {
            hs_offset = 2;
        }
        else
        {
            hs_offset = 1;
        }
        if (rddata_en_ie_dly > (cas_lat - 1 - hs_offset))
        {
            tmp = 0;
        }
        else if ((delta == 2) || (delta == 1))
        {
            tmp = rddata_en_ie_dly - 0 - extra_adder;
        }
        else
        {
            tmp = extra_adder;
        }
        /* PHY_LP4_BOOT_RDDATA_EN_TSEL_DLY */
        /* DENALI_PHY_9/137/265/393 4bit offset_16 */
        mmio_clrsetbits_32(PHY_REG(i, 9), 0xf << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 137), 0xf << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 265), 0xf << 16, tmp << 16);
        mmio_clrsetbits_32(PHY_REG(i, 393), 0xf << 16, tmp << 16);
        /* PHY_RDDATA_EN_TSEL_DLY */
        /* DENALI_PHY_86/214/342/470 4bit offset_0 */
        mmio_clrsetbits_32(PHY_REG(i, 86), 0xf, tmp);
        mmio_clrsetbits_32(PHY_REG(i, 214), 0xf, tmp);
        mmio_clrsetbits_32(PHY_REG(i, 342), 0xf, tmp);
        mmio_clrsetbits_32(PHY_REG(i, 470), 0xf, tmp);

        if (tsel_adder > rddata_en_ie_dly)
        {
            extra_adder = tsel_adder - rddata_en_ie_dly;
        }
        else
        {
            extra_adder = 0;
        }
        if (rddata_en_ie_dly > (cas_lat - 1 - hs_offset))
        {
            tmp = tsel_adder;
        }
        else
        {
            tmp = rddata_en_ie_dly - 0 + extra_adder;
        }
        /* PHY_LP4_BOOT_RDDATA_EN_DLY */
        /* DENALI_PHY_9/137/265/393 4bit offset_8 */
        mmio_clrsetbits_32(PHY_REG(i, 9), 0xf << 8, tmp << 8);
        mmio_clrsetbits_32(PHY_REG(i, 137), 0xf << 8, tmp << 8);
        mmio_clrsetbits_32(PHY_REG(i, 265), 0xf << 8, tmp << 8);
        mmio_clrsetbits_32(PHY_REG(i, 393), 0xf << 8, tmp << 8);
        /* PHY_RDDATA_EN_DLY */
        /* DENALI_PHY_85/213/341/469 4bit offset_24 */
        mmio_clrsetbits_32(PHY_REG(i, 85), 0xf << 24, tmp << 24);
        mmio_clrsetbits_32(PHY_REG(i, 213), 0xf << 24, tmp << 24);
        mmio_clrsetbits_32(PHY_REG(i, 341), 0xf << 24, tmp << 24);
        mmio_clrsetbits_32(PHY_REG(i, 469), 0xf << 24, tmp << 24);

        if (pdram_timing->mhz <= ENPER_CS_TRAINING_FREQ)
        {
            /*
             * Note:Per-CS Training is not compatible at speeds
             * under 533 MHz. If the PHY is running at a speed
             * less than 533MHz, all phy_per_cs_training_en_X
             * parameters must be cleared to 0.
             */

            /*DENALI_PHY_84/212/340/468 1bit offset_16 */
            mmio_clrbits_32(PHY_REG(i, 84), 0x1 << 16);
            mmio_clrbits_32(PHY_REG(i, 212), 0x1 << 16);
            mmio_clrbits_32(PHY_REG(i, 340), 0x1 << 16);
            mmio_clrbits_32(PHY_REG(i, 468), 0x1 << 16);
        }
        else
        {
            mmio_setbits_32(PHY_REG(i, 84), 0x1 << 16);
            mmio_setbits_32(PHY_REG(i, 212), 0x1 << 16);
            mmio_setbits_32(PHY_REG(i, 340), 0x1 << 16);
            mmio_setbits_32(PHY_REG(i, 468), 0x1 << 16);
        }
        gen_rk3399_phy_dll_bypass(pdram_timing->mhz, i, fn, timing_config->dram_type);
    }
}

static int
to_get_clk_index(unsigned int mhz)
{
    int pll_cnt, i;

    pll_cnt = ARRAY_SIZE(dpll_rates_table);

    /* Assuming rate_table is in descending order */
    for (i = 0; i < pll_cnt; i++)
    {
        if (mhz >= dpll_rates_table[i].mhz)
        {
            break;
        }
    }

    /* if mhz lower than lowest frequency in table, use lowest frequency */
    if (i == pll_cnt)
    {
        i = pll_cnt - 1;
    }

    return i;
}

uint32_t
ddr_get_rate(void)
{
    uint32_t refdiv, postdiv1, fbdiv, postdiv2;

    refdiv   = mmio_read_32(CRU_BASE + CRU_PLL_CON(DPLL_ID, 1)) & 0x3f;
    fbdiv    = mmio_read_32(CRU_BASE + CRU_PLL_CON(DPLL_ID, 0)) & 0xfff;
    postdiv1 = (mmio_read_32(CRU_BASE + CRU_PLL_CON(DPLL_ID, 1)) >> 8) & 0x7;
    postdiv2 = (mmio_read_32(CRU_BASE + CRU_PLL_CON(DPLL_ID, 1)) >> 12) & 0x7;

    return (24 / refdiv * fbdiv / postdiv1 / postdiv2) * 1000 * 1000;
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
exit_low_power(void)
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

        /* exit stdby mode */
        mmio_write_32(CIC_BASE + CIC_CTRL1, (1 << (i + 16)) | (0 << i));
        /* exit external self-refresh */
        tmp = i ? 12 : 8;
        low_power |= ((mmio_read_32(PMU_BASE + PMU_SFT_CON) >> tmp) & 0x1) << (4 + 8 * i);
        mmio_clrbits_32(PMU_BASE + PMU_SFT_CON, 1 << tmp);
        while (!(mmio_read_32(PMU_BASE + PMU_DDR_SREF_ST) & (1 << i)))
            ;
        /* exit auto low-power */
        mmio_clrbits_32(CTL_REG(i, 101), 0x7);
        /* lp_cmd to exit */
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

void
resume_low_power(uint32_t low_power)
{
    uint32_t channel_mask;
    uint32_t tmp, i, val;

    channel_mask = (mmio_read_32(PMUGRF_BASE + PMUGRF_OSREG(2)) >> 28) & 0x3;
    for (i = 0; i < 2; i++)
    {
        if (!(channel_mask & (1 << i)))
        {
            continue;
        }

        /* resume external self-refresh */
        tmp = i ? 12 : 8;
        val = (low_power >> (4 + 8 * i)) & 0x1;
        mmio_setbits_32(PMU_BASE + PMU_SFT_CON, val << tmp);
        /* resume auto low-power */
        val = (low_power >> (8 * i)) & 0x7;
        mmio_setbits_32(CTL_REG(i, 101), val);
        /* resume stdby mode */
        val = (low_power >> (3 + 8 * i)) & 0x1;
        mmio_write_32(CIC_BASE + CIC_CTRL1, (1 << (i + 16)) | (val << i));
    }
}

static void
dram_low_power_config(void)
{
    uint32_t tmp, i;
    uint32_t ch_cnt    = rk3399_dram_status.timing_config.ch_cnt;
    uint32_t dram_type = rk3399_dram_status.timing_config.dram_type;

    if (dram_type == DDR3)
    {
        tmp = (2 << 16) | (0x7 << 8);
    }
    else
    {
        tmp = (3 << 16) | (0x7 << 8);
    }

    for (i = 0; i < ch_cnt; i++)
    {
        mmio_clrsetbits_32(CTL_REG(i, 101), 0x70f0f, tmp);
    }

    /* standby idle */
    mmio_write_32(CIC_BASE + CIC_CG_WAIT_TH, 0x640008);

    if (ch_cnt == 2)
    {
        mmio_write_32(GRF_BASE + GRF_DDRC1_CON1, (((0x1 << 4) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7)) << 16) |
                                                     ((0x1 << 4) | (0x0 << 5) | (0x1 << 6) | (0x1 << 7)));
        mmio_write_32(CIC_BASE + CIC_CTRL1, 0x002a0028);
    }

    mmio_write_32(GRF_BASE + GRF_DDRC0_CON1, (((0x1 << 4) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7)) << 16) |
                                                 ((0x1 << 4) | (0x0 << 5) | (0x1 << 6) | (0x1 << 7)));
    mmio_write_32(CIC_BASE + CIC_CTRL1, 0x00150014);
}

void
dram_dfs_init(void)
{
    uint32_t trefi0, trefi1, boot_freq;
    uint32_t rddqs_adjust, rddqs_slave;

    /* get sdram config for os reg */
    get_dram_drv_odt_val(sdram_config.dramtype, &rk3399_dram_status.drv_odt_lp_cfg);
    sdram_timing_cfg_init(&rk3399_dram_status.timing_config, &sdram_config, &rk3399_dram_status.drv_odt_lp_cfg);

    trefi0 = ((mmio_read_32(CTL_REG(0, 48)) >> 16) & 0xffff) + 8;
    trefi1 = ((mmio_read_32(CTL_REG(0, 49)) >> 16) & 0xffff) + 8;

    rk3399_dram_status.index_freq[0] = trefi0 * 10 / 39;
    rk3399_dram_status.index_freq[1] = trefi1 * 10 / 39;
    rk3399_dram_status.current_index = (mmio_read_32(CTL_REG(0, 111)) >> 16) & 0x3;
    if (rk3399_dram_status.timing_config.dram_type == DDR3)
    {
        rk3399_dram_status.index_freq[0] /= 2;
        rk3399_dram_status.index_freq[1] /= 2;
    }
    boot_freq                    = rk3399_dram_status.index_freq[rk3399_dram_status.current_index];
    boot_freq                    = dpll_rates_table[to_get_clk_index(boot_freq)].mhz;
    rk3399_dram_status.boot_freq = boot_freq;
    rk3399_dram_status.index_freq[rk3399_dram_status.current_index]             = boot_freq;
    rk3399_dram_status.index_freq[(rk3399_dram_status.current_index + 1) & 0x1] = 0;
    rk3399_dram_status.low_power_stat                                           = 0;
    /*
     * following register decide if NOC stall the access request
     * or return error when NOC being idled. when doing ddr frequency
     * scaling in M0 or DCF, we need to make sure noc stall the access
     * request, if return error cpu may data abort when ddr frequency
     * changing. it don't need to set this register every times,
     * so we init this register in function dram_dfs_init().
     */
    mmio_write_32(GRF_BASE + GRF_SOC_CON(0), 0xffffffff);
    mmio_write_32(GRF_BASE + GRF_SOC_CON(1), 0xffffffff);
    mmio_write_32(GRF_BASE + GRF_SOC_CON(2), 0xffffffff);
    mmio_write_32(GRF_BASE + GRF_SOC_CON(3), 0xffffffff);
    mmio_write_32(GRF_BASE + GRF_SOC_CON(4), 0x70007000);

    /* Disable multicast */
    mmio_clrbits_32(PHY_REG(0, 896), 1);
    mmio_clrbits_32(PHY_REG(1, 896), 1);
    dram_low_power_config();

    /*
     * If boot_freq isn't in the bypass mode, it can get the
     * rddqs_delay_ps from the result of gate training
     */
    if (((mmio_read_32(PHY_REG(0, 86)) >> 8) & 0xf) != 0xc)
    {

        /*
         * Select PHY's frequency set to current_index
         * index for get the result of gate Training
         * from registers
         */
        mmio_clrsetbits_32(PHY_REG(0, 896), 0x3 << 8, rk3399_dram_status.current_index << 8);
        rddqs_slave = (mmio_read_32(PHY_REG(0, 77)) >> 16) & 0x3ff;
        rddqs_slave = rddqs_slave * 1000000 / boot_freq / 512;

        rddqs_adjust   = mmio_read_32(PHY_REG(0, 78)) & 0xf;
        rddqs_adjust   = rddqs_adjust * 1000000 / boot_freq;
        rddqs_delay_ps = rddqs_slave + rddqs_adjust - (1000000 / boot_freq / 2);
    }
    else
    {
        rddqs_delay_ps = 3500;
    }
}

/*
 * arg0: bit0-7: sr_idle; bit8-15:sr_mc_gate_idle; bit16-31: standby idle
 * arg1: bit0-11: pd_idle; bit 16-27: srpd_lite_idle
 * arg2: bit0: if odt en
 */
uint32_t
dram_set_odt_pd(uint32_t arg0, uint32_t arg1, uint32_t arg2)
{
    struct drv_odt_lp_config *lp_cfg    = &rk3399_dram_status.drv_odt_lp_cfg;
    uint32_t                 *low_power = &rk3399_dram_status.low_power_stat;
    uint32_t                  dram_type, ch_count, pd_tmp, sr_tmp, i;

    dram_type = rk3399_dram_status.timing_config.dram_type;
    ch_count  = rk3399_dram_status.timing_config.ch_cnt;

    lp_cfg->sr_idle         = arg0 & 0xff;
    lp_cfg->sr_mc_gate_idle = (arg0 >> 8) & 0xff;
    lp_cfg->standby_idle    = (arg0 >> 16) & 0xffff;
    lp_cfg->pd_idle         = arg1 & 0xfff;
    lp_cfg->srpd_lite_idle  = (arg1 >> 16) & 0xfff;

    rk3399_dram_status.timing_config.odt = arg2 & 0x1;

    exit_low_power();

    *low_power = 0;

    /* pd_idle en */
    if (lp_cfg->pd_idle)
    {
        *low_power |= ((1 << 0) | (1 << 8));
    }
    /* sr_idle en srpd_lite_idle */
    if (lp_cfg->sr_idle | lp_cfg->srpd_lite_idle)
    {
        *low_power |= ((1 << 1) | (1 << 9));
    }
    /* sr_mc_gate_idle */
    if (lp_cfg->sr_mc_gate_idle)
    {
        *low_power |= ((1 << 2) | (1 << 10));
    }
    /* standbyidle */
    if (lp_cfg->standby_idle)
    {
        if (rk3399_dram_status.timing_config.ch_cnt == 2)
        {
            *low_power |= ((1 << 3) | (1 << 11));
        }
        else
        {
            *low_power |= (1 << 3);
        }
    }

    pd_tmp = arg1;
    if (dram_type != LPDDR4)
    {
        pd_tmp = arg1 & 0xfff;
    }
    sr_tmp = arg0 & 0xffff;
    for (i = 0; i < ch_count; i++)
    {
        mmio_write_32(CTL_REG(i, 102), pd_tmp);
        mmio_clrsetbits_32(CTL_REG(i, 103), 0xffff, sr_tmp);
    }
    mmio_write_32(CIC_BASE + CIC_IDLE_TH, (arg0 >> 16) & 0xffff);

    return 0;
}

static void
m0_configure_ddr(struct pll_div pll_div, uint32_t ddr_index)
{
    mmio_write_32(M0_PARAM_ADDR + PARAM_DPLL_CON0, FBDIV(pll_div.fbdiv));
    mmio_write_32(M0_PARAM_ADDR + PARAM_DPLL_CON1,
                  POSTDIV2(pll_div.postdiv2) | POSTDIV1(pll_div.postdiv1) | REFDIV(pll_div.refdiv));

    mmio_write_32(M0_PARAM_ADDR + PARAM_DRAM_FREQ, pll_div.mhz);

    mmio_write_32(M0_PARAM_ADDR + PARAM_FREQ_SELECT, ddr_index << 4);
    dmbst();
    m0_configure_execute_addr(M0_BINCODE_BASE);
}

static uint32_t
prepare_ddr_timing(uint32_t mhz)
{
    uint32_t             index;
    struct dram_timing_t dram_timing;

    rk3399_dram_status.timing_config.freq = mhz;

    if (mhz < 300)
    {
        rk3399_dram_status.timing_config.dllbp = 1;
    }
    else
    {
        rk3399_dram_status.timing_config.dllbp = 0;
    }

    if (rk3399_dram_status.timing_config.odt == 1)
    {
        gen_rk3399_set_odt(1);
    }

    index = (rk3399_dram_status.current_index + 1) & 0x1;

    /*
     * checking if having available gate traiing timing for
     * target freq.
     */
    dram_get_parameter(&rk3399_dram_status.timing_config, &dram_timing);
    gen_rk3399_ctl_params(&rk3399_dram_status.timing_config, &dram_timing, index);
    gen_rk3399_pi_params(&rk3399_dram_status.timing_config, &dram_timing, index);
    gen_rk3399_phy_params(&rk3399_dram_status.timing_config, &rk3399_dram_status.drv_odt_lp_cfg, &dram_timing, index);
    rk3399_dram_status.index_freq[index] = mhz;

    return index;
}

uint32_t
ddr_set_rate(uint32_t hz)
{
    uint32_t low_power, index, ddr_index;
    uint32_t mhz = hz / (1000 * 1000);

    if (mhz == rk3399_dram_status.index_freq[rk3399_dram_status.current_index])
    {
        return mhz;
    }

    index = to_get_clk_index(mhz);
    mhz   = dpll_rates_table[index].mhz;

    ddr_index = prepare_ddr_timing(mhz);
    gen_rk3399_enable_training(rk3399_dram_status.timing_config.ch_cnt, mhz);
    if (ddr_index > 1)
    {
        goto out;
    }

    /*
     * Make sure the clock is enabled. The M0 clocks should be on all of the
     * time during S0.
     */
    m0_configure_ddr(dpll_rates_table[index], ddr_index);
    m0_start();
    m0_wait_done();
    m0_stop();

    if (rk3399_dram_status.timing_config.odt == 0)
    {
        gen_rk3399_set_odt(0);
    }

    rk3399_dram_status.current_index = ddr_index;
    low_power                        = rk3399_dram_status.low_power_stat;
    resume_low_power(low_power);
out:
    gen_rk3399_disable_training(rk3399_dram_status.timing_config.ch_cnt);
    return mhz;
}

uint32_t
ddr_round_rate(uint32_t hz)
{
    int      index;
    uint32_t mhz = hz / (1000 * 1000);

    index = to_get_clk_index(mhz);

    return dpll_rates_table[index].mhz * 1000 * 1000;
}

void
ddr_prepare_for_sys_suspend(void)
{
    uint32_t mhz = rk3399_dram_status.index_freq[rk3399_dram_status.current_index];

    /*
     * If we're not currently at the boot (assumed highest) frequency, we
     * need to change frequencies to configure out current index.
     */
    rk3399_suspend_status.freq = mhz;
    exit_low_power();
    rk3399_suspend_status.low_power_stat = rk3399_dram_status.low_power_stat;
    rk3399_suspend_status.odt            = rk3399_dram_status.timing_config.odt;
    rk3399_dram_status.low_power_stat    = 0;
    rk3399_dram_status.timing_config.odt = 1;
    if (mhz != rk3399_dram_status.boot_freq)
    {
        ddr_set_rate(rk3399_dram_status.boot_freq * 1000 * 1000);
    }

    /*
     * This will configure the other index to be the same frequency as the
     * current one. We retrain both indices on resume, so both have to be
     * setup for the same frequency.
     */
    prepare_ddr_timing(rk3399_dram_status.boot_freq);
}

void
ddr_prepare_for_sys_resume(void)
{
    /* Disable multicast */
    mmio_clrbits_32(PHY_REG(0, 896), 1);
    mmio_clrbits_32(PHY_REG(1, 896), 1);

    /* The suspend code changes the current index, so reset it now. */
    rk3399_dram_status.current_index     = (mmio_read_32(CTL_REG(0, 111)) >> 16) & 0x3;
    rk3399_dram_status.low_power_stat    = rk3399_suspend_status.low_power_stat;
    rk3399_dram_status.timing_config.odt = rk3399_suspend_status.odt;

    /*
     * Set the saved frequency from suspend if it's different than the
     * current frequency.
     */
    if (rk3399_suspend_status.freq != rk3399_dram_status.index_freq[rk3399_dram_status.current_index])
    {
        ddr_set_rate(rk3399_suspend_status.freq * 1000 * 1000);
        return;
    }

    gen_rk3399_set_odt(rk3399_dram_status.timing_config.odt);
    resume_low_power(rk3399_dram_status.low_power_stat);
}

uint32_t
ddr_smc_handler(uint64_t arg0, uint64_t arg1, uint64_t id, uint64_t arg2)
{
    switch (id)
    {
        case DRAM_SET_RATE :
            return ddr_set_rate((uint32_t)arg0);
        case DRAM_ROUND_RATE :
            return ddr_round_rate((uint32_t)arg0);
        case DRAM_GET_RATE :
            return ddr_get_rate();
        case DRAM_SET_ODT_PD :
            dram_set_odt_pd(arg0, arg1, arg2);
            break;
        default :
            break;
    }

    return 0;
}

uintptr_t
rockchip_plat_sip_handler(uint32_t smc_fid, u_register_t x1, u_register_t x2, u_register_t x3, u_register_t x4,
                          void *cookie, void *handle, u_register_t flags)
{
#ifdef PLAT_RK_DP_HDCP
    uint64_t x5, x6;
#endif

    switch (smc_fid)
    {
        case RK_SIP_DDR_CFG :
            SMC_RET1(handle, ddr_smc_handler(x1, x2, x3, x4));
#ifdef PLAT_RK_DP_HDCP
        case RK_SIP_HDCP_CONTROL :
            SMC_RET1(handle, dp_hdcp_ctrl(x1));
        case RK_SIP_HDCP_KEY_DATA64 :
            x5 = read_ctx_reg(get_gpregs_ctx(handle), CTX_GPREG_X5);
            x6 = read_ctx_reg(get_gpregs_ctx(handle), CTX_GPREG_X6);
            SMC_RET1(handle, dp_hdcp_store_key(x1, x2, x3, x4, x5, x6));
#endif
        default :
            ERROR("%s: unhandled SMC (0x%x)\n", __func__, smc_fid);
            SMC_RET1(handle, SMC_UNK);
    }
}

enum pmu_powerdomain_id
{
    PD_CPUL0 = 0,
    PD_CPUL1,
    PD_CPUL2,
    PD_CPUL3,
    PD_CPUB0,
    PD_CPUB1,
    PD_SCUL,
    PD_SCUB,
    PD_TCPD0,
    PD_TCPD1,
    PD_CCI,
    PD_PERILP,
    PD_PERIHP,
    PD_CENTER,
    PD_VIO,
    PD_GPU,
    PD_VCODEC,
    PD_VDU,
    PD_RGA,
    PD_IEP,
    PD_VO,
    PD_ISP0 = 22,
    PD_ISP1,
    PD_HDCP,
    PD_GMAC,
    PD_EMMC,
    PD_USB3,
    PD_EDP,
    PD_GIC,
    PD_SD,
    PD_SDIOAUDIO,
    PD_END
};

enum powerdomain_state
{
    PMU_POWER_ON = 0,
    PMU_POWER_OFF,
};

enum pmu_bus_id
{
    BUS_ID_GPU = 0,
    BUS_ID_PERILP,
    BUS_ID_PERIHP,
    BUS_ID_VCODEC,
    BUS_ID_VDU,
    BUS_ID_RGA,
    BUS_ID_IEP,
    BUS_ID_VOPB,
    BUS_ID_VOPL,
    BUS_ID_ISP0,
    BUS_ID_ISP1,
    BUS_ID_HDCP,
    BUS_ID_USB3,
    BUS_ID_PERILPM0,
    BUS_ID_CENTER,
    BUS_ID_CCIM0,
    BUS_ID_CCIM1,
    BUS_ID_VIO,
    BUS_ID_MSCH0,
    BUS_ID_MSCH1,
    BUS_ID_ALIVE,
    BUS_ID_PMU,
    BUS_ID_EDP,
    BUS_ID_GMAC,
    BUS_ID_EMMC,
    BUS_ID_CENTER1,
    BUS_ID_PMUM0,
    BUS_ID_GIC,
    BUS_ID_SD,
    BUS_ID_SDIOAUDIO,
};

enum pmu_bus_state
{
    BUS_ACTIVE,
    BUS_IDLE,
};

/* pmu_cpuapm bit */
enum pmu_cores_pm_by_wfi
{
    core_pm_en = 0,
    core_pm_int_wakeup_en,
    core_pm_resv,
    core_pm_sft_wakeup_en
};

enum pmu_wkup_cfg0
{
    PMU_GPIO0A_POSE_WKUP_EN = 0,
    PMU_GPIO0B_POSE_WKUP_EN = 8,
    PMU_GPIO0C_POSE_WKUP_EN = 16,
    PMU_GPIO0D_POSE_WKUP_EN = 24,
};

enum pmu_wkup_cfg1
{
    PMU_GPIO0A_NEGEDGE_WKUP_EN = 0,
    PMU_GPIO0B_NEGEDGE_WKUP_EN = 7,
    PMU_GPIO0C_NEGEDGE_WKUP_EN = 16,
    PMU_GPIO0D_NEGEDGE_WKUP_EN = 24,
};

enum pmu_wkup_cfg2
{
    PMU_GPIO1A_POSE_WKUP_EN = 0,
    PMU_GPIO1B_POSE_WKUP_EN = 7,
    PMU_GPIO1C_POSE_WKUP_EN = 16,
    PMU_GPIO1D_POSE_WKUP_EN = 24,
};

enum pmu_wkup_cfg3
{
    PMU_GPIO1A_NEGEDGE_WKUP_EN = 0,
    PMU_GPIO1B_NEGEDGE_WKUP_EN = 7,
    PMU_GPIO1C_NEGEDGE_WKUP_EN = 16,
    PMU_GPIO1D_NEGEDGE_WKUP_EN = 24,
};

/* pmu_wkup_cfg4 */
enum pmu_wkup_cfg4
{
    PMU_CLUSTER_L_WKUP_EN = 0,
    PMU_CLUSTER_B_WKUP_EN,
    PMU_GPIO_WKUP_EN,
    PMU_SDIO_WKUP_EN,

    PMU_SDMMC_WKUP_EN,
    PMU_TIMER_WKUP_EN = 6,
    PMU_USBDEV_WKUP_EN,

    PMU_SFT_WKUP_EN,
    PMU_M0_WDT_WKUP_EN,
    PMU_TIMEOUT_WKUP_EN,
    PMU_PWM_WKUP_EN,

    PMU_PCIE_WKUP_EN = 13,
};

enum pmu_pwrdn_con
{
    PMU_A53_L0_PWRDWN_EN = 0,
    PMU_A53_L1_PWRDWN_EN,
    PMU_A53_L2_PWRDWN_EN,
    PMU_A53_L3_PWRDWN_EN,

    PMU_A72_B0_PWRDWN_EN,
    PMU_A72_B1_PWRDWN_EN,
    PMU_SCU_L_PWRDWN_EN,
    PMU_SCU_B_PWRDWN_EN,

    PMU_TCPD0_PWRDWN_EN,
    PMU_TCPD1_PWRDWN_EN,
    PMU_CCI_PWRDWN_EN,
    PMU_PERILP_PWRDWN_EN,

    PMU_PERIHP_PWRDWN_EN,
    PMU_CENTER_PWRDWN_EN,
    PMU_VIO_PWRDWN_EN,
    PMU_GPU_PWRDWN_EN,

    PMU_VCODEC_PWRDWN_EN,
    PMU_VDU_PWRDWN_EN,
    PMU_RGA_PWRDWN_EN,
    PMU_IEP_PWRDWN_EN,

    PMU_VO_PWRDWN_EN,
    PMU_ISP0_PWRDWN_EN = 22,
    PMU_ISP1_PWRDWN_EN,

    PMU_HDCP_PWRDWN_EN,
    PMU_GMAC_PWRDWN_EN,
    PMU_EMMC_PWRDWN_EN,
    PMU_USB3_PWRDWN_EN,

    PMU_EDP_PWRDWN_EN,
    PMU_GIC_PWRDWN_EN,
    PMU_SD_PWRDWN_EN,
    PMU_SDIOAUDIO_PWRDWN_EN,
};

enum pmu_pwrdn_st
{
    PMU_A53_L0_PWRDWN_ST = 0,
    PMU_A53_L1_PWRDWN_ST,
    PMU_A53_L2_PWRDWN_ST,
    PMU_A53_L3_PWRDWN_ST,

    PMU_A72_B0_PWRDWN_ST,
    PMU_A72_B1_PWRDWN_ST,
    PMU_SCU_L_PWRDWN_ST,
    PMU_SCU_B_PWRDWN_ST,

    PMU_TCPD0_PWRDWN_ST,
    PMU_TCPD1_PWRDWN_ST,
    PMU_CCI_PWRDWN_ST,
    PMU_PERILP_PWRDWN_ST,

    PMU_PERIHP_PWRDWN_ST,
    PMU_CENTER_PWRDWN_ST,
    PMU_VIO_PWRDWN_ST,
    PMU_GPU_PWRDWN_ST,

    PMU_VCODEC_PWRDWN_ST,
    PMU_VDU_PWRDWN_ST,
    PMU_RGA_PWRDWN_ST,
    PMU_IEP_PWRDWN_ST,

    PMU_VO_PWRDWN_ST,
    PMU_ISP0_PWRDWN_ST = 22,
    PMU_ISP1_PWRDWN_ST,

    PMU_HDCP_PWRDWN_ST,
    PMU_GMAC_PWRDWN_ST,
    PMU_EMMC_PWRDWN_ST,
    PMU_USB3_PWRDWN_ST,

    PMU_EDP_PWRDWN_ST,
    PMU_GIC_PWRDWN_ST,
    PMU_SD_PWRDWN_ST,
    PMU_SDIOAUDIO_PWRDWN_ST,

};

enum pmu_pll_con
{
    PMU_PLL_PD_CFG = 0,
    PMU_SFT_PLL_PD = 8,
};

enum pmu_pwermode_con
{
    PMU_PWR_MODE_EN = 0,
    PMU_WKUP_RST_EN,
    PMU_INPUT_CLAMP_EN,
    PMU_OSC_DIS,

    PMU_ALIVE_USE_LF,
    PMU_PMU_USE_LF,
    PMU_POWER_OFF_REQ_CFG,
    PMU_CHIP_PD_EN,

    PMU_PLL_PD_EN,
    PMU_CPU0_PD_EN,
    PMU_L2_FLUSH_EN,
    PMU_L2_IDLE_EN,

    PMU_SCU_PD_EN,
    PMU_CCI_PD_EN,
    PMU_PERILP_PD_EN,
    PMU_CENTER_PD_EN,

    PMU_SREF0_ENTER_EN,
    PMU_DDRC0_GATING_EN,
    PMU_DDRIO0_RET_EN,
    PMU_DDRIO0_RET_DE_REQ,

    PMU_SREF1_ENTER_EN,
    PMU_DDRC1_GATING_EN,
    PMU_DDRIO1_RET_EN,
    PMU_DDRIO1_RET_DE_REQ,

    PMU_CLK_CENTER_SRC_GATE_EN = 26,
    PMU_CLK_PERILP_SRC_GATE_EN,

    PMU_CLK_CORE_SRC_GATE_EN,
    PMU_DDRIO_RET_HW_DE_REQ,
    PMU_SLP_OUTPUT_CFG,
    PMU_MAIN_CLUSTER,
};

enum pmu_sft_con
{
    PMU_WKUP_SFT = 0,
    PMU_INPUT_CLAMP_CFG,
    PMU_OSC_DIS_CFG,
    PMU_PMU_LF_EN_CFG,

    PMU_ALIVE_LF_EN_CFG,
    PMU_24M_EN_CFG,
    PMU_DBG_PWRUP_L0_CFG,
    PMU_WKUP_SFT_M0,

    PMU_DDRCTL0_C_SYSREQ_CFG,
    PMU_DDR0_IO_RET_CFG,

    PMU_DDRCTL1_C_SYSREQ_CFG = 12,
    PMU_DDR1_IO_RET_CFG,
    DBG_PWRUP_B0_CFG = 15,

    DBG_NOPWERDWN_L0_EN,
    DBG_NOPWERDWN_L1_EN,
    DBG_NOPWERDWN_L2_EN,
    DBG_NOPWERDWN_L3_EN,

    DBG_PWRUP_REQ_L_EN = 20,
    CLUSTER_L_CLK_SRC_GATING_CFG,
    L2_FLUSH_REQ_CLUSTER_L,
    ACINACTM_CLUSTER_L_CFG,

    DBG_NO_PWERDWN_B0_EN,
    DBG_NO_PWERDWN_B1_EN,

    DBG_PWRUP_REQ_B_EN = 28,
    CLUSTER_B_CLK_SRC_GATING_CFG,
    L2_FLUSH_REQ_CLUSTER_B,
    ACINACTM_CLUSTER_B_CFG,
};

enum pmu_int_con
{
    PMU_PMU_INT_EN = 0,
    PMU_PWRMD_WKUP_INT_EN,
    PMU_WKUP_GPIO0_NEG_INT_EN,
    PMU_WKUP_GPIO0_POS_INT_EN,
    PMU_WKUP_GPIO1_NEG_INT_EN,
    PMU_WKUP_GPIO1_POS_INT_EN,
};

enum pmu_int_st
{
    PMU_PWRMD_WKUP_INT_ST = 1,
    PMU_WKUP_GPIO0_NEG_INT_ST,
    PMU_WKUP_GPIO0_POS_INT_ST,
    PMU_WKUP_GPIO1_NEG_INT_ST,
    PMU_WKUP_GPIO1_POS_INT_ST,
};

enum pmu_gpio0_pos_int_con
{
    PMU_GPIO0A_POS_INT_EN = 0,
    PMU_GPIO0B_POS_INT_EN = 8,
    PMU_GPIO0C_POS_INT_EN = 16,
    PMU_GPIO0D_POS_INT_EN = 24,
};

enum pmu_gpio0_neg_int_con
{
    PMU_GPIO0A_NEG_INT_EN = 0,
    PMU_GPIO0B_NEG_INT_EN = 8,
    PMU_GPIO0C_NEG_INT_EN = 16,
    PMU_GPIO0D_NEG_INT_EN = 24,
};

enum pmu_gpio1_pos_int_con
{
    PMU_GPIO1A_POS_INT_EN = 0,
    PMU_GPIO1B_POS_INT_EN = 8,
    PMU_GPIO1C_POS_INT_EN = 16,
    PMU_GPIO1D_POS_INT_EN = 24,
};

enum pmu_gpio1_neg_int_con
{
    PMU_GPIO1A_NEG_INT_EN = 0,
    PMU_GPIO1B_NEG_INT_EN = 8,
    PMU_GPIO1C_NEG_INT_EN = 16,
    PMU_GPIO1D_NEG_INT_EN = 24,
};

enum pmu_gpio0_pos_int_st
{
    PMU_GPIO0A_POS_INT_ST = 0,
    PMU_GPIO0B_POS_INT_ST = 8,
    PMU_GPIO0C_POS_INT_ST = 16,
    PMU_GPIO0D_POS_INT_ST = 24,
};

enum pmu_gpio0_neg_int_st
{
    PMU_GPIO0A_NEG_INT_ST = 0,
    PMU_GPIO0B_NEG_INT_ST = 8,
    PMU_GPIO0C_NEG_INT_ST = 16,
    PMU_GPIO0D_NEG_INT_ST = 24,
};

enum pmu_gpio1_pos_int_st
{
    PMU_GPIO1A_POS_INT_ST = 0,
    PMU_GPIO1B_POS_INT_ST = 8,
    PMU_GPIO1C_POS_INT_ST = 16,
    PMU_GPIO1D_POS_INT_ST = 24,
};

enum pmu_gpio1_neg_int_st
{
    PMU_GPIO1A_NEG_INT_ST = 0,
    PMU_GPIO1B_NEG_INT_ST = 8,
    PMU_GPIO1C_NEG_INT_ST = 16,
    PMU_GPIO1D_NEG_INT_ST = 24,
};

/* pmu power down configure register 0x0050 */
enum pmu_pwrdn_inten
{
    PMU_A53_L0_PWR_SWITCH_INT_EN = 0,
    PMU_A53_L1_PWR_SWITCH_INT_EN,
    PMU_A53_L2_PWR_SWITCH_INT_EN,
    PMU_A53_L3_PWR_SWITCH_INT_EN,

    PMU_A72_B0_PWR_SWITCH_INT_EN,
    PMU_A72_B1_PWR_SWITCH_INT_EN,
    PMU_SCU_L_PWR_SWITCH_INT_EN,
    PMU_SCU_B_PWR_SWITCH_INT_EN,

    PMU_TCPD0_PWR_SWITCH_INT_EN,
    PMU_TCPD1_PWR_SWITCH_INT_EN,
    PMU_CCI_PWR_SWITCH_INT_EN,
    PMU_PERILP_PWR_SWITCH_INT_EN,

    PMU_PERIHP_PWR_SWITCH_INT_EN,
    PMU_CENTER_PWR_SWITCH_INT_EN,
    PMU_VIO_PWR_SWITCH_INT_EN,
    PMU_GPU_PWR_SWITCH_INT_EN,

    PMU_VCODEC_PWR_SWITCH_INT_EN,
    PMU_VDU_PWR_SWITCH_INT_EN,
    PMU_RGA_PWR_SWITCH_INT_EN,
    PMU_IEP_PWR_SWITCH_INT_EN,

    PMU_VO_PWR_SWITCH_INT_EN,
    PMU_ISP0_PWR_SWITCH_INT_EN = 22,
    PMU_ISP1_PWR_SWITCH_INT_EN,

    PMU_HDCP_PWR_SWITCH_INT_EN,
    PMU_GMAC_PWR_SWITCH_INT_EN,
    PMU_EMMC_PWR_SWITCH_INT_EN,
    PMU_USB3_PWR_SWITCH_INT_EN,

    PMU_EDP_PWR_SWITCH_INT_EN,
    PMU_GIC_PWR_SWITCH_INT_EN,
    PMU_SD_PWR_SWITCH_INT_EN,
    PMU_SDIOAUDIO_PWR_SWITCH_INT_EN,
};

enum pmu_wkup_status
{
    PMU_WKUP_BY_CLSTER_L_INT = 0,
    PMU_WKUP_BY_CLSTER_b_INT,
    PMU_WKUP_BY_GPIO_INT,
    PMU_WKUP_BY_SDIO_DET,

    PMU_WKUP_BY_SDMMC_DET,
    PMU_WKUP_BY_TIMER = 6,
    PMU_WKUP_BY_USBDEV_DET,

    PMU_WKUP_BY_M0_SFT,
    PMU_WKUP_BY_M0_WDT_INT,
    PMU_WKUP_BY_TIMEOUT,
    PMU_WKUP_BY_PWM,

    PMU_WKUP_BY_PCIE = 13,
};

enum pmu_bus_clr
{
    PMU_CLR_GPU = 0,
    PMU_CLR_PERILP,
    PMU_CLR_PERIHP,
    PMU_CLR_VCODEC,

    PMU_CLR_VDU,
    PMU_CLR_RGA,
    PMU_CLR_IEP,
    PMU_CLR_VOPB,

    PMU_CLR_VOPL,
    PMU_CLR_ISP0,
    PMU_CLR_ISP1,
    PMU_CLR_HDCP,

    PMU_CLR_USB3,
    PMU_CLR_PERILPM0,
    PMU_CLR_CENTER,
    PMU_CLR_CCIM1,

    PMU_CLR_CCIM0,
    PMU_CLR_VIO,
    PMU_CLR_MSCH0,
    PMU_CLR_MSCH1,

    PMU_CLR_ALIVE,
    PMU_CLR_PMU,
    PMU_CLR_EDP,
    PMU_CLR_GMAC,

    PMU_CLR_EMMC,
    PMU_CLR_CENTER1,
    PMU_CLR_PMUM0,
    PMU_CLR_GIC,

    PMU_CLR_SD,
    PMU_CLR_SDIOAUDIO,
};

/* PMU bus idle request register */
enum pmu_bus_idle_req
{
    PMU_IDLE_REQ_GPU = 0,
    PMU_IDLE_REQ_PERILP,
    PMU_IDLE_REQ_PERIHP,
    PMU_IDLE_REQ_VCODEC,

    PMU_IDLE_REQ_VDU,
    PMU_IDLE_REQ_RGA,
    PMU_IDLE_REQ_IEP,
    PMU_IDLE_REQ_VOPB,

    PMU_IDLE_REQ_VOPL,
    PMU_IDLE_REQ_ISP0,
    PMU_IDLE_REQ_ISP1,
    PMU_IDLE_REQ_HDCP,

    PMU_IDLE_REQ_USB3,
    PMU_IDLE_REQ_PERILPM0,
    PMU_IDLE_REQ_CENTER,
    PMU_IDLE_REQ_CCIM0,

    PMU_IDLE_REQ_CCIM1,
    PMU_IDLE_REQ_VIO,
    PMU_IDLE_REQ_MSCH0,
    PMU_IDLE_REQ_MSCH1,

    PMU_IDLE_REQ_ALIVE,
    PMU_IDLE_REQ_PMU,
    PMU_IDLE_REQ_EDP,
    PMU_IDLE_REQ_GMAC,

    PMU_IDLE_REQ_EMMC,
    PMU_IDLE_REQ_CENTER1,
    PMU_IDLE_REQ_PMUM0,
    PMU_IDLE_REQ_GIC,

    PMU_IDLE_REQ_SD,
    PMU_IDLE_REQ_SDIOAUDIO,
};

/* pmu bus idle status register */
enum pmu_bus_idle_st
{
    PMU_IDLE_ST_GPU = 0,
    PMU_IDLE_ST_PERILP,
    PMU_IDLE_ST_PERIHP,
    PMU_IDLE_ST_VCODEC,

    PMU_IDLE_ST_VDU,
    PMU_IDLE_ST_RGA,
    PMU_IDLE_ST_IEP,
    PMU_IDLE_ST_VOPB,

    PMU_IDLE_ST_VOPL,
    PMU_IDLE_ST_ISP0,
    PMU_IDLE_ST_ISP1,
    PMU_IDLE_ST_HDCP,

    PMU_IDLE_ST_USB3,
    PMU_IDLE_ST_PERILPM0,
    PMU_IDLE_ST_CENTER,
    PMU_IDLE_ST_CCIM0,

    PMU_IDLE_ST_CCIM1,
    PMU_IDLE_ST_VIO,
    PMU_IDLE_ST_MSCH0,
    PMU_IDLE_ST_MSCH1,

    PMU_IDLE_ST_ALIVE,
    PMU_IDLE_ST_PMU,
    PMU_IDLE_ST_EDP,
    PMU_IDLE_ST_GMAC,

    PMU_IDLE_ST_EMMC,
    PMU_IDLE_ST_CENTER1,
    PMU_IDLE_ST_PMUM0,
    PMU_IDLE_ST_GIC,

    PMU_IDLE_ST_SD,
    PMU_IDLE_ST_SDIOAUDIO,
};

enum pmu_bus_idle_ack
{
    PMU_IDLE_ACK_GPU = 0,
    PMU_IDLE_ACK_PERILP,
    PMU_IDLE_ACK_PERIHP,
    PMU_IDLE_ACK_VCODEC,

    PMU_IDLE_ACK_VDU,
    PMU_IDLE_ACK_RGA,
    PMU_IDLE_ACK_IEP,
    PMU_IDLE_ACK_VOPB,

    PMU_IDLE_ACK_VOPL,
    PMU_IDLE_ACK_ISP0,
    PMU_IDLE_ACK_ISP1,
    PMU_IDLE_ACK_HDCP,

    PMU_IDLE_ACK_USB3,
    PMU_IDLE_ACK_PERILPM0,
    PMU_IDLE_ACK_CENTER,
    PMU_IDLE_ACK_CCIM0,

    PMU_IDLE_ACK_CCIM1,
    PMU_IDLE_ACK_VIO,
    PMU_IDLE_ACK_MSCH0,
    PMU_IDLE_ACK_MSCH1,

    PMU_IDLE_ACK_ALIVE,
    PMU_IDLE_ACK_PMU,
    PMU_IDLE_ACK_EDP,
    PMU_IDLE_ACK_GMAC,

    PMU_IDLE_ACK_EMMC,
    PMU_IDLE_ACK_CENTER1,
    PMU_IDLE_ACK_PMUM0,
    PMU_IDLE_ACK_GIC,

    PMU_IDLE_ACK_SD,
    PMU_IDLE_ACK_SDIOAUDIO,
};

enum pmu_cci500_con
{
    PMU_PREQ_CCI500_CFG_SW = 0,
    PMU_CLR_PREQ_CCI500_HW,
    PMU_PSTATE_CCI500_0,
    PMU_PSTATE_CCI500_1,

    PMU_PSTATE_CCI500_2,
    PMU_QREQ_CCI500_CFG_SW,
    PMU_CLR_QREQ_CCI500_HW,
    PMU_QGATING_CCI500_CFG,

    PMU_PREQ_CCI500_CFG_SW_WMSK = 16,
    PMU_CLR_PREQ_CCI500_HW_WMSK,
    PMU_PSTATE_CCI500_0_WMSK,
    PMU_PSTATE_CCI500_1_WMSK,

    PMU_PSTATE_CCI500_2_WMSK,
    PMU_QREQ_CCI500_CFG_SW_WMSK,
    PMU_CLR_QREQ_CCI500_HW_WMSK,
    PMU_QGATING_CCI500_CFG_WMSK,
};

enum pmu_adb400_con
{
    PMU_PWRDWN_REQ_CXCS_SW = 0,
    PMU_PWRDWN_REQ_CORE_L_SW,
    PMU_PWRDWN_REQ_CORE_L_2GIC_SW,
    PMU_PWRDWN_REQ_GIC2_CORE_L_SW,

    PMU_PWRDWN_REQ_CORE_B_SW,
    PMU_PWRDWN_REQ_CORE_B_2GIC_SW,
    PMU_PWRDWN_REQ_GIC2_CORE_B_SW,

    PMU_CLR_CXCS_HW = 8,
    PMU_CLR_CORE_L_HW,
    PMU_CLR_CORE_L_2GIC_HW,
    PMU_CLR_GIC2_CORE_L_HW,

    PMU_CLR_CORE_B_HW,
    PMU_CLR_CORE_B_2GIC_HW,
    PMU_CLR_GIC2_CORE_B_HW,

    PMU_PWRDWN_REQ_CXCS_SW_WMSK = 16,
    PMU_PWRDWN_REQ_CORE_L_SW_WMSK,
    PMU_PWRDWN_REQ_CORE_L_2GIC_SW_WMSK,
    PMU_PWRDWN_REQ_GIC2_CORE_L_SW_WMSK,

    PMU_PWRDWN_REQ_CORE_B_SW_WMSK,
    PMU_PWRDWN_REQ_CORE_B_2GIC_SW_WMSK,
    PMU_PWRDWN_REQ_GIC2_CORE_B_SW_WMSK,

    PMU_CLR_CXCS_HW_WMSK = 24,
    PMU_CLR_CORE_L_HW_WMSK,
    PMU_CLR_CORE_L_2GIC_HW_WMSK,
    PMU_CLR_GIC2_CORE_L_HW_WMSK,

    PMU_CLR_CORE_B_HW_WMSK,
    PMU_CLR_CORE_B_2GIC_HW_WMSK,
    PMU_CLR_GIC2_CORE_B_HW_WMSK,
};

enum pmu_adb400_st
{
    PMU_PWRDWN_REQ_CXCS_SW_ST = 0,
    PMU_PWRDWN_REQ_CORE_L_SW_ST,
    PMU_PWRDWN_REQ_CORE_L_2GIC_SW_ST,
    PMU_PWRDWN_REQ_GIC2_CORE_L_SW_ST,

    PMU_PWRDWN_REQ_CORE_B_SW_ST,
    PMU_PWRDWN_REQ_CORE_B_2GIC_SW_ST,
    PMU_PWRDWN_REQ_GIC2_CORE_B_SW_ST,

    PMU_CLR_CXCS_HW_ST = 8,
    PMU_CLR_CORE_L_HW_ST,
    PMU_CLR_CORE_L_2GIC_HW_ST,
    PMU_CLR_GIC2_CORE_L_HW_ST,

    PMU_CLR_CORE_B_HW_ST,
    PMU_CLR_CORE_B_2GIC_HW_ST,
    PMU_CLR_GIC2_CORE_B_HW_ST,
};

enum pmu_pwrdn_con1
{
    PMU_VD_SCU_L_PWRDN_EN = 0,
    PMU_VD_SCU_B_PWRDN_EN,
    PMU_VD_CENTER_PWRDN_EN,
};

enum pmu_core_pwr_st
{
    L2_FLUSHDONE_CLUSTER_L = 0,
    STANDBY_BY_WFIL2_CLUSTER_L,

    L2_FLUSHDONE_CLUSTER_B = 10,
    STANDBY_BY_WFIL2_CLUSTER_B,
};

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

    int16_t
    readTemperature()
    {
        writeRegister(RK3399_T_USER_CON, readRegister(RK3399_T_USER_CON) | (1 << 5)); // Start conversion
        while (readRegister(RK3399_T_USER_CON) & (1 << 12))
        {
            // Wait for conversion to complete
            ;
        }
        // Read 12-bit ADC value
        int16_t adc_value   = readRegister(RK3399_T_DATA0) & 0xFFF;
        int16_t temperature = convertToTemperature(adc_value);
        return temperature;
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
