/// @file Sys.h
#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <spawn.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "def.h"

namespace Mlib::Sys
{
    s8 run_binary(C_STRING &binary_path, C_VECTOR<STRING> &args, C_VECTOR<STRING> &env_vars = {});

    class Prompt
    {
    public:
        Prompt(C_STRING &prompt);
        operator STRING() const;

    private:
        STRINGSTREAM ss;
    };

    template <typename T>
    class Singleton
    {
    private:
        static Singleton *instance;
        T                 value;

        Singleton(T val)
            : value(val)
        {}

    public:
        static Singleton *
        Instance(T val)
        {
            if (instance == nullptr)
            {
                instance = new Singleton(val);
            }
            return instance;
        }

        void
        destroy()
        {
            if (instance != nullptr)
            {
                delete instance;
                instance = nullptr;
            }
        }

        T
        getValue() const
        {
            return value;
        }

        void
        setValue(T val)
        {
            value = val;
        }
    };

    namespace CpuMsrAddr
    {
        struct RYZEN_3900_X
        {
            static constexpr u32 prf_lmt_r    = 0xC001029A; // (Core Performance Limit Reasons)
            static constexpr u32 pwr_rep      = 0xC0010293; // (Power Reporting)
            static constexpr u32 pwr_lmt_stat = 0xC0010295; // (Power Limit Status)
        };
    } // namespace CpuMsrAddr

    class MSRReader
    {
        IFSTREAM msr_file;

    public:
        MSRReader(unsigned int cpu);

        ~MSRReader();

        uint64_t read(uint32_t msr);
    };

    s32 launch_child_process(C_s8 *command);

    u64 retriveSysLogicCores();

    u64 read_msr_value_to_watts(MSRReader *msr_reader, u32 msr_address);

    bool prompt_yes_no(const char *str);

} // namespace Mlib::Sys
