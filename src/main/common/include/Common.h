
#ifndef COMMON_H
#define COMMON_H

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        NO_ERROR = 0, /* OK */
        ERROR_FAILED = 1, /* generic error */
        ERROR_OBJECT_NOT_FOUND = 2,
        ERROR_NULL_POINTER = 3,
        ERROR_INVALID_INPUT_PARAMETER = 4,
        ERROR_NOT_INITIALIZED = 5,
        ERROR_OUT_OF_RANGE = 6,
        ERROR_CORRUPTED_FILE = 7,
        ERROR_CREATE_ERROR = 8
    } ErrorCode_t;

    typedef enum {
        FALSE = 0,
        TRUE = 1
    } Bool_t;

    typedef char Char_t;
#define MAX_UINT8 0xFF
    typedef unsigned char Uint8_t;
    typedef signed char Int8_t;
#define MAX_UINT16 0xFFFF
    typedef unsigned short Uint16_t;
    typedef signed short Int16_t;
#define MAX_UINT32 0xFFFFFFFF
    typedef unsigned int Uint32_t;
    typedef signed int Int32_t;

    typedef unsigned long Ulong_t;
    typedef signed long Long_t;

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */

