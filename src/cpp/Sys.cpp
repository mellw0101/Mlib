/// @file Sys.cpp
#include "../include/Sys.h"

namespace Mlib::Sys
{
    s8
    run_binary(const std::string &binary_path, const std::vector<std::string> &args,
               const std::vector<std::string> &env_vars)
    {
        s8 result = 0;

        // Convert vector of arguments to array of C-strings
        std::vector<char *> argv;
        argv.push_back(const_cast<char *>(binary_path.c_str()));
        for (const std::string &arg : args)
        {
            argv.push_back(const_cast<char *>(arg.c_str()));
        }
        //
        //  Null-terminate the array
        //
        argv.push_back(nullptr);

        //
        //  Convert vector of environment variables to array of C-strings
        //
        std::vector<char *> envp;
        for (const std::string &env_var : env_vars)
        {
            envp.push_back(const_cast<char *>(env_var.c_str()));
        }
        //
        //  Null-terminate the array
        //
        envp.push_back(nullptr);

        //
        //  Fork a new process
        //
        pid_t pid = fork();
        if (pid == -1)
        {
            throw std::runtime_error("Failed to fork process");
        }
        else if (pid == 0)
        {
            //
            //  Child process: set environment variables if any
            //
            if (!env_vars.empty())
            {
                execvpe(binary_path.c_str(), argv.data(), envp.data());
            }
            else
            {
                execvp(binary_path.c_str(), argv.data());
            }

            //
            //  If execvpe or execvp returns, an error occurred
            //
            throw std::runtime_error("Error: execvp failed to execute '" + binary_path + '\'');
            exit(EXIT_FAILURE);
        }
        else
        {
            //
            //  Parent process: wait for the child to finish
            //
            s32 status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                result = WEXITSTATUS(status);
                if (result != 0)
                {
                    throw std::runtime_error("Process exited with status " + std::to_string(result));
                }
            }
            else
            {
                throw std::runtime_error("Process did not exit normally");
            }
        }

        return result;
    }

    Prompt ::Prompt(const std::string &prompt)
    {
        std::string input;
        std::cout << prompt;
        getline(std::cin, input);
        ss.str(input);
    }

    Prompt ::operator std::string() const
    {
        return ss.str();
    }

    template <typename T>
    Singleton<T> *Singleton<T>::instance = nullptr;


    MSRReader ::MSRReader(u32 cpu)
    {
        STRING path = "/dev/cpu/" + std::to_string(cpu) + "/msr";
        msr_file.open(path, IOS::in | IOS::binary);
        if (!msr_file.is_open())
        {
            throw std::runtime_error("Failed to open MSR file");
        }
    }

    MSRReader ::~MSRReader()
    {
        if (msr_file.is_open())
        {
            msr_file.close();
        }
    }

    u64
    MSRReader ::read(u32 msr)
    {
        if (!msr_file.is_open())
        {
            throw std::runtime_error("MSR file is not open");
        }

        msr_file.seekg(msr, IOS::beg);
        u64 value = 0;
        msr_file.read(reinterpret_cast<s8 *>(&value), sizeof(value));

        if (msr_file.fail())
        {
            throw std::runtime_error("Failed to read MSR");
        }

        return value;
    }

    s32
    launch_child_process(C_s8 *command)
    {
        pid_t             pid;
        posix_spawnattr_t attr;

        STRING command_path = "/bin/" + STRING(command);

        char *argv[] = {const_cast<char *>(command_path.c_str()), NULL};

        posix_spawnattr_init(&attr);
        posix_spawnattr_setpgroup(&attr, 0);

        int result = posix_spawn(&pid, command_path.c_str(), NULL, &attr, argv, environ);
        posix_spawnattr_destroy(&attr);

        if (result == 0)
        {
            return 0;
        }
        else
        {
            return result;
        }
    }

    u64
    retriveSysLogicCores()
    {
        u64 sys_cores = sysconf(_SC_NPROCESSORS_ONLN);
        if (sys_cores == -1)
        {
            throw std::runtime_error("Failed to retrieve system logic cores" + ERRNO_STR);
        }
        return sys_cores;
    }

    u64
    read_msr_value_to_watts(MSRReader *msr_reader, u32 msr_address)
    {
        u64 raw_value = msr_reader->read(msr_address);
        //
        //  Assuming the raw value is in milliwatts, convert to watts
        //
        f64 power_in_watts = raw_value / 1000.0;

        if (power_in_watts <= 0)
        {
            throw std::runtime_error("Failed to read MSR value to watts");
        }

        return power_in_watts;
    }

    bool
    prompt_yes_no(const char *str, bool default_response, bool verbose_prompt)
    {
        while (true)
        {
            const char *yes_no_prompt = default_response ? "Y/n" : "y/N";
            printf("%s [%s]%s: ", str, yes_no_prompt,
                   verbose_prompt ? (default_response ? " Press enter to answer 'Yes'" : " Press enter to answer 'No'")
                                  : "");
            fflush(stdout);

            char input[3];
            if (fgets(input, 3, stdin) == nullptr)
            {
                return default_response;
            }

            if (*input == '\n')
            {
                return default_response;
            }

            switch (*input)
            {
                case 'Y' :
                case 'y' :
                {
                    return true;
                }
                case 'N' :
                case 'n' :
                {
                    return false;
                }
                default :
                {
                    printf("Only [%s] is allowed.\n", yes_no_prompt);
                    fflush(stdout);

                    if (input[1] != '\n')
                    {
                        for (int c; (c = fgetc(stdin)) != '\n' && c != EOF;)
                            ;
                    }
                }
            }
        }
    }


} // namespace Mlib::Sys
