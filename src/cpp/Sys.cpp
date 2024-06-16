/// @file Sys.cpp
#include "../include/Sys.h"

namespace Mlib::Sys {
    s8
    run_binary(const string& binary_path, const vector<string>& args)
    {
        s8 result = 0;

        // Convert vector of arguments to array of C-strings
        vector<char*> argv;
        argv.push_back(const_cast<char*>(binary_path.c_str()));
        for (const string& arg : args)
        {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        // Null-terminate the array
        argv.push_back(nullptr);

        // Fork a new process
        pid_t pid = fork();
        if (pid == -1)
        {
            throw runtime_error("Failed to fork process");
        }
        else if (pid == 0)
        {
            // Child process: execute the binary
            execvp(binary_path.c_str(), argv.data());
            // If execvp returns, an error occurred
            // throw an exception and exit the child process
            throw runtime_error("Error: execvp failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent process: wait for the child to finish
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                result = WEXITSTATUS(status);
                if (result != 0)
                {
                    throw runtime_error("Process exited with status " + to_string(result));
                }
            }
            else
            {
                throw runtime_error("Process did not exit normally");
            }
        }
        return result;
    }

    /// @class @c Prompt

#pragma region Prompt

    Prompt::Prompt(const string& prompt)
    {
        string input;
        cout << prompt;
        getline(cin, input);
        ss.str(input);
    }


    Prompt::operator string() const
    {
        return ss.str();
    }

#pragma endregion
} // namespace Mlib::Sys
