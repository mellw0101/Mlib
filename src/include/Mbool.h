#pragma once

#ifdef TRUE
  #undef TRUE
#endif
#ifdef FALSE
  #undef FALSE
#endif
#ifdef __cplusplus
  #define TRUE true
  #define FALSE false
#else
  #define TRUE 1
  #define FALSE 0
#endif