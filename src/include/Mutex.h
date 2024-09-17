#include "Debug.h"
#include "def.h"

/* Either lock or unlock the threadpools mutex, with full error reporting.  We
 * will not interviene on fail as I want to see for now atleast what it takes to
 * crash. */
void lock_pthread_mutex(pthread_mutex_t *mutex, bool lock);

/* RAII complient way to lock a pthread mutex.  This struct will lock
 * the mutex apon its creation, and unlock it when it goes out of scope. */
struct pthread_mutex_guard_t
{
    pthread_mutex_t *m = nullptr;
    explicit pthread_mutex_guard_t(pthread_mutex_t *mutex)
        : m(mutex)
    {
        if (m == nullptr)
        {
            logE("A 'nullptr' was passed to 'pthread_mutex_guard_t'.");
        }
        else
        {
            lock_pthread_mutex(m, true);
        }
    }
    ~pthread_mutex_guard_t(void)
    {
        if (m != nullptr)
        {
            lock_pthread_mutex(m, false);
        }
    }
    DEL_CM_CONSTRUCTORS(pthread_mutex_guard_t);
};
