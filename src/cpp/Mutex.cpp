#include "../include/Mutex.h"

// void lock_pthread_mutex(pthread_mutex_t *mutex, bool lock) {
//   int result = (lock == TRUE) ? pthread_mutex_lock(mutex) : pthread_mutex_unlock(mutex);
//   if (result == 0) {
//     return;
//   }
//   switch (result) {
//     case EAGAIN : {
//       logE("Mutex exceeded the maximum number of recursive locks.");
//       break;
//     }
//     case EDEADLK : {
//       logE("Mutex caused a deadlock or was already owned by this thread.");
//       break;
//     }
//     case EINVAL : {
//       logE("Mutex is not valid, indicating a fatal error.");
//       break;
//     }
//     case EPERM : {
//       logE("Mutex was locked by another thread, or not at all.");
//       break;
//     }
//     default : {
//       logE("Mutex encountered an unexpected error code: %d.", result);
//     }
//   }
// }
