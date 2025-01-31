/** @file Sys.h */
#pragma once

#include <cstring>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <spawn.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "Error.h"
#include "Http.h"
#include "def.h"

namespace Mlib::Sys {
  Uchar run_binary(C_STRING &binary_path, C_VECTOR<STRING> &args, C_VECTOR<STRING> &env_vars = {});

  class Prompt {
   public:
    Prompt(C_STRING &prompt);
    operator STRING() const;

   private:
    STRINGSTREAM ss;
  };

  template <typename T>
  class Singleton {
   private:
    static Singleton *instance;
    T                 value;

    Singleton(T val)
        : value(val) {
    }

   public:
    static Singleton *Instance(T val) {
      if (instance == nullptr) {
        instance = new Singleton(val);
      }
      return instance;
    }

    void destroy() {
      if (instance != nullptr) {
        delete instance;
        instance = nullptr;
      }
    }

    T getValue() const {
      return value;
    }

    void setValue(T val) {
      value = val;
    }
  };

  namespace CpuMsrAddr {
    struct RYZEN_3900_X {
      static constexpr u32 prf_lmt_r    = 0xC001029A; // (Core Performance Limit Reasons)
      static constexpr u32 pwr_rep      = 0xC0010293; // (Power Reporting)
      static constexpr u32 pwr_lmt_stat = 0xC0010295; // (Power Limit Status)
    };
  }

  class MSRReader {
    IFSTREAM msr_file;

   public:
    MSRReader(unsigned int cpu);

    ~MSRReader();

    Ulong read(Uint msr);
  };

  int           launch_child_process(const char *command);
  unsigned long retriveSysLogicCores();
  unsigned long read_msr_value_to_watts(MSRReader *msr_reader, Uint msr_address);

  /* Prompt for a answer to a prompt.
   * Return 'default_response' apon enter, else return 'true' if 'Y/y' or 'false' if 'N/n'.
   * If 'verbose_prompt' is set to 'true' the prompt will state "Press enter to answer 'default_response'".
   * By default 'default_response' is set to 'true' and 'verbose_prompt' is set to 'false'. */
  bool        prompt_yes_no(const char *str, bool default_response = true, bool verbose_prompt = false);
  const char *itoa(int num) _NO_THROW;
  void        run_bin(const char *bin, char *const *arg_arry, char *const *env_arry) __attribute_nonnull__((1, 2));
  void        get_dev_info(const char *path, Ulong *b_size);
  void        posix_run_bin(const char *bin, char **argv, char **envv);
}

Ulong get_current_time_ms(void);
void format_time(Ulong ms, char *buffer, Ulong buffer_size);