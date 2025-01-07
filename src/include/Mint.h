#pragma once

#ifdef Uchar
# undef Uchar
#endif
#ifdef Ushort
# undef Ushort
#endif
#ifdef Uint
# undef Uint
#endif
#ifdef Ulong
# undef Ulong
#endif
#ifdef UlongMIN
# undef UlongMIN
#endif
#ifdef UlongMAX
# undef UlongMAX
#endif
#ifdef PTR_SIZE
# undef PTR_SIZE
#endif
#define Uchar   unsigned char
#define Ushort  unsigned short int
#define Uint    unsigned int
#ifndef __WORDSIZE
# if defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(__aarch64__) || defined(_M_ARM64)
#   define __WORDSIZE 64
# elif defined(_WIN32)
#   define __WORDSIZE 32
# endif
#endif
#if __WORDSIZE == 64
# if (defined(__x86_64__) && !defined(__ILP32__)) || defined(__aarch64__) || defined(_M_ARM64)
#   define Ulong   unsigned long int
#   define UlongMIN (0UL)
#   define UlongMAX (18446744073709551615UL)
# else
#   define Ulong   unsigned long long int
#   define UlongMIN (0ULL)
#   define UlongMAX (18446744073709551615ULL)
# endif
#else
# define Ulong   unsigned long long int
# define UlongMIN (0ULL)
# define UlongMAX (18446744073709551615ULL)
#endif
#define PTR_SIZE __WORDSIZE