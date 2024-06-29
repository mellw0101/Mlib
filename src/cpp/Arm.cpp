#include "../include/Arm.h"

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

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
