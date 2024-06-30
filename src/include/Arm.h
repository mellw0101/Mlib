#pragma once

#include "def.h"

#include <cstdint>
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

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

} // namespace Mlib::Arm
