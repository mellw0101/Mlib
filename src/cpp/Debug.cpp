#include "../include/Debug.h"


namespace Mlib::Debug
{
    Lout *Lout::LoutInstance = nullptr;

    Lout &
    Lout::Instance()
    {
        using namespace std;

        if (LoutInstance == nullptr)
        {
            lock_guard<mutex> lock(mutex);
            LoutInstance = new Lout();
        }
        return *LoutInstance;
    }

    void
    Lout::Destroy()
    {
        if (LoutInstance != nullptr)
        {
            delete LoutInstance;
            LoutInstance = nullptr;
        }
    }

} // namespace Mlib::Debug
