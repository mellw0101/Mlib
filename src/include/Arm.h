#pragma once

#include "def.h"

#include <cstdint>
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

uintptr_t get_base_mem_addr(uintptr_t physical_addr);
