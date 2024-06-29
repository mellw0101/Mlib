#include "../include/Arm.h"

uintptr_t
get_base_mem_addr()
{
    int       mem_fd;
    void     *mapped_base;
    uintptr_t base_addr = BASE_ADDR;

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

    return reinterpret_cast<uintptr_t>(mapped_base) + (base_addr & MAP_MASK);
}
