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

    s32
    launch_child_process(const s8 *command)
    {
        pid_t             pid;
        posix_spawnattr_t attr;

        std::string command_path = "/bin/" + std::string(command);

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

} // namespace Mlib::Sys
