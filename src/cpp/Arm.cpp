#include "../include/Arm.h"

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

namespace Mlib::Arm
{
    void
    initialize()
    {
        // Open /proc/self/maps to read the memory map of the current process
        FILE *maps = fopen("/proc/self/maps", "r");
        if (maps == nullptr)
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        // Read the memory map line by line
        char  *line = nullptr;
        size_t len  = 0;
        while (getline(&line, &len, maps) != -1)
        {
            // Parse the line to get the start and end addresses of the mapped region
            unsigned long start, end;
            sscanf(line, "%lx-%lx", &start, &end);

            // Check if the mapped region contains the address of the function
            if (start <= (unsigned long)Arm::initialize && (unsigned long)Arm::initialize <= end)
            {
                // Get the page size of the system
                long page_size = sysconf(_SC_PAGESIZE);
                if (page_size == -1)
                {
                    perror("sysconf");
                    exit(EXIT_FAILURE);
                }

                // Calculate the start of the page containing the function
                unsigned long page_start = start + (((unsigned long)Arm::initialize - start) & ~MAP_MASK);

                // Get the protection flags of the page
                unsigned long prot;
                if (mincore((void *)page_start, 1, (u8 *)&prot) == -1)
                {
                    perror("mincore");
                    exit(EXIT_FAILURE);
                }

                // Check if the page is executable
                if (prot & PROT_EXEC)
                {
                    printf("The function is in an executable page\n");
                }
                else
                {
                    printf("The function is not in an executable page\n");
                }

                break;
            }
        }

        // Free the line buffer
        free(line);

        // Close /proc/self/maps
        fclose(maps);
    }


} // namespace Mlib::Arm
