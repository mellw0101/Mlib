#include "../include/Arm.h"

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

namespace Mlib::Arm
{
    uintptr_t
    get_base_mem_addr(uintptr_t physical_addr)
    {
        int   mem_fd;
        void *mapped_base;

        mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (mem_fd == -1)
        {
            throw std::runtime_error("Cannot open /dev/mem");
        }

        mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, physical_addr & ~MAP_MASK);
        if (mapped_base == MAP_FAILED)
        {
            close(mem_fd);
            throw std::runtime_error("mmap failed");
        }

        close(mem_fd);

        return reinterpret_cast<uintptr_t>(mapped_base) + (physical_addr & MAP_MASK);
    }

    uint32_t
    ddr_get_rate()
    {
        uint32_t refdiv, postdiv1, fbdiv, postdiv2;

        refdiv   = mmio_read_32(CRU_BASE + CRU_PLL_CON(DPLL_ID, 1)) & 0x3f;
        fbdiv    = mmio_read_32(CRU_BASE + CRU_PLL_CON(DPLL_ID, 0)) & 0xfff;
        postdiv1 = (mmio_read_32(CRU_BASE + CRU_PLL_CON(DPLL_ID, 1)) >> 8) & 0x7;
        postdiv2 = (mmio_read_32(CRU_BASE + CRU_PLL_CON(DPLL_ID, 1)) >> 12) & 0x7;

        return (24 / refdiv * fbdiv / postdiv1 / postdiv2) * 1000 * 1000;
    }

} // namespace Mlib::Arm
