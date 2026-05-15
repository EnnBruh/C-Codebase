#ifndef ENN_BASE_H
#define ENN_BASE_H

#ifdef __cplusplus
        extern "C" {
#endif

/* ---------- Platform Detection ---------- */

#define ENN_UNKNOWN      0x0

#define ENN_WINDOWS      0x1
#define ENN_LINUX        0x2
#define ENN_APPLE        0x3

#define ENN_GCC          0x4
#define ENN_CLANG        0x5
#define ENN_MSVC         0x6

#define ENN_C89          0x7
#define ENN_C99          0x8
#define ENN_C11          0x9
#define ENN_C17          0xA
#define ENN_C23          0xB
#define ENN_CPP          0xC

#ifndef ENN_PLATFORM
#       if   defined(_WIN32) || defined(_WIN64)
#               define ENN_PLATFORM ENN_WINDOWS
#       elif defined(__linux__)
#               define ENN_PLATFORM ENN_LINUX
#       elif defined(__APPLE__)
#               define ENN_PLATFORM ENN_APPLE
#       else
#               define ENN_PLATFORM ENN_UNKNOWN
#       endif
#endif


#ifndef ENN_COMPILER
#       if   defined(__clang__)
#               define ENN_COMPILER ENN_CLANG
#       elif defined(__GNUC__)
#               define ENN_COMPILER ENN_GCC
#       elif defined(_MSC_VER) && !defined(_INTEL_COMPILER)
#               define ENN_COMPILER ENN_MSVC
#       else
#               define ENN_COMPILER ENN_UNKNOWN
#       endif
#endif


#ifndef ENN_STD_VERSION
#       if   defined(__STDC__)
#               if !defined(__STDC_VERSION__)
#                       if !defined(__cplusplus)
#                               define ENN_STD_VERSION ENN_C89
#                       else
#                               define ENN_STD_VERSION ENN_CPP
#                       endif
#               elif   __STDC_VERSION__ == 199901L
#                       define ENN_STD_VERSION ENN_C99
#               elif __STDC_VERSION__ == 201112L
#                       define ENN_STD_VERSION ENN_C11
#               elif __STDC_VERSION__ == 201710L
#                       define ENN_STD_VERSION ENN_C17
#               elif __STDC_VERSION__ == 202311L
#                       define ENN_STD_VERSION ENN_C23
#               else
#                       define ENN_STD_VERSION ENN_UNKNOWN
#               endif
#       else
#               define ENN_STD_VERSION ENN_UNKNOWN
#       endif
#endif

#if ENN_PLATFORM == ENN_UNKNWON
#       warning "Could not detect platform or using an unsupported platform. Currently supported platforms are Windows, ~Linux, ~Apple Based."
#endif

#if ENN_COMPILER == ENN_UNKNWON
#       warning "Could not detect compiler or using an unsupported compiler. Currently supported compilers are GCC, Clang, LLVM, MSVC."
#endif

#if ENN_STD_VERSION == ENN_UNKNWON
#       warning "Could not detect C Standard version or using an unsupported version. Currently supported versions are C99, C11, C17, C23."
#endif

#if ENN_PLATFORM == ENN_LINUX || ENN_PLATFORM == ENN_APPLE
#       error "Linux or Apple Based OS are not currently supported."
#endif

#if ENN_STD_VERSION <= ENN_C89
#       error "Please compile using C99 standard or above. C89 is not currently supported."
#endif

/* ---------- Macro Helpers ---------- */

#define ENN_FILE        __FILE__
#define ENN_LINE        __LINE__
#define ENN_FUNCTION    __func__

#if   ENN_COMPILER == ENN_MSVC
#        define ENN_FORCE_INLINE __forceinline
#elif ENN_COMPILER == ENN_GCC || ENN_COMPILER == ENN_CLANG
#        define ENN_FORCE_INLINE inline __attribute__((always_inline))
#else
#        define ENN_FORCE_INLINE inline
#endif

#define ENNDEF_PUBLIC   static ENN_FORCE_INLINE
#define ENNDEF_PRIVATE  extern

#define TO_STR_(_X) #_X
#define TO_STR(_X) TO_STR_(_X)

#define JOIN_(_X, _Y)                           _X##_Y
#define JOIN(_X, _Y)                            JOIN_(_X, _Y)

#define SEQ_N_()                63, 62, 61, 60,                         \
        59, 58, 57, 56, 55, 54, 53, 52, 51, 50,                         \
        49, 48, 47, 46, 45, 44, 43, 42, 41, 40,                         \
        39, 38, 37, 36, 35, 34, 33, 32, 31, 30,                         \
        29, 28, 27, 26, 25, 24, 23, 22, 21, 20,                         \
        19, 18, 17, 16, 15, 14, 13, 12, 11, 10,                         \
        9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define GET_NUM_ARGS__(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,         \
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,               \
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,               \
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,               \
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,               \
        _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,               \
        _61, _62, _63, _64, N, ...) N

#define GET_NUM_ARGS_(...)                      GET_NUM_ARGS__(__VA_ARGS__)
#define GET_NUM_ARGS(...)                       GET_NUM_ARGS_(_0, ##__VA_ARGS__, SEQ_N_())

#define SEQ_1_()          0, 0, 0, 0,                                   \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                   \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                   \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                   \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                   \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                   \
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0

#define HAS_ONE_ARG_(...)                       GET_NUM_ARGS__(__VA_ARGS__)
#define HAS_ONE_ARG(...)                        HAS_ONE_ARG_(_0, ##__VA_ARGS__, SEQ_1_())

#define EXPAND(macro)                           macro

/* ---------- Standard Defines ---------- */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <tgmath.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

typedef uint8_t         u8;
typedef uint16_t        u16;
typedef uint32_t        u32;
typedef uint64_t        u64;

typedef int8_t          i8;
typedef int16_t         i16;
typedef int32_t         i32;
typedef int64_t         i64;

typedef float           f32;
typedef double          f64;
typedef long double     f128;

typedef u8              byte;
typedef size_t          usize;

#define U8_MIN          UINT8_MIN
#define U16_MIN         UINT16_MIN
#define U32_MIN         UINT32_MIN
#define U64_MIN         UINT64_MIN

#define U8_MAX          UINT8_MAX
#define U16_MAX         UINT16_MAX
#define U32_MAX         UINT32_MAX
#define U64_MAX         UINT64_MAX

#define I8_MIN          INT8_MIN
#define I16_MIN         INT16_MIN
#define I32_MIN         INT32_MIN
#define I64_MIN         INT64_MIN

#define I8_MAX          INT8_MAX
#define I16_MAX         INT16_MAX
#define I32_MAX         INT32_MAX
#define I64_MAX         INT64_MAX

/* ---------- Debug & Logging Helpers ---------- */

#ifndef ENN_MAX_LOG_BUFF_SIZE
#       define ENN_MAX_LOG_BUFF_SIZE 2048
#endif

struct _Log {
        char    _buff[ENN_MAX_LOG_BUFF_SIZE];
        i32     _buff_len;
        FILE*   _stream;
};

struct _StackFrame {
        const char*     _file_name;
        const char*     _function_name;
        i32             _line;
};

#ifndef ENN_MAX_CALLSTACK_SIZE
#       define ENN_MAX_CALLSTACK_SIZE 12
#endif

struct _CallStack {
        struct _StackFrame      _calls[ENN_MAX_CALLSTACK_SIZE];
        i32                     _size;
        i32                     _stackoverflow;
};

extern struct _Log _internal_general_logs;
#ifdef ENN_DEBUG_MODE
        extern struct _CallStack _internal_call_stack;
        extern struct _Log _internal_memory_logs;
#endif

ENNDEF_PRIVATE void debug_init(void);
ENNDEF_PRIVATE void debug_term(void);

#ifndef ENN_DEBUG_MODE
#        define DEBUG_DUMP_GENERAL_LOGS()                                                                                                                \
                do {                                                                                                                                    \
                        fwrite(_internal_general_logs._buff, _internal_general_logs._buff_len, (sizeof (char)), _internal_general_logs._stream);        \
                        _internal_general_logs._buff_len = 0;                                                                                           \
                } while (0)
#else
#       define DEBUG_DUMP_GENERAL_LOGS()                                                                                                                \
                do {                                                                                                                                    \
                        fwrite(_internal_general_logs._buff, _internal_general_logs._buff_len, (sizeof (char)), _internal_general_logs._stream);        \
                        fwrite(_internal_general_logs._buff, _internal_general_logs._buff_len, (sizeof (char)), stdout);                                \
                        _internal_general_logs._buff_len = 0;                                                                                           \
                } while (0)
#endif

#ifdef ENN_DEBUG_MODE
#       define DEBUG_DUMP_MEMORY_LOGS()                                                                                                                 \
                do {                                                                                                                                    \
                        fwrite(_internal_memory_logs._buff, _internal_memory_logs._buff_len, (sizeof (char)), _internal_memory_logs._stream);           \
                        _internal_memory_logs._buff_len = 0;                                                                                            \
                } while (0)
#endif

#ifndef ENN_APP_DIRECTORY
#       define ENN_APP_DIRECTORY "App"
#endif

#ifndef ENN_DATA_PATH
#       define ENN_DATA_PATH "/Data/"
#endif

#ifndef ENN_LOGS_PATH
#       define ENN_LOGS_PATH "/Data/Logs/"
#endif

#ifndef ENN_GENERAL_LOG_FILENAME_FORMAT
#       define ENN_GENERAL_LOG_FILENAME_FORMAT "%Y-%m-%d-%H-%M-%S.log"
#endif

#ifndef ENN_MEMORY_LOG_FILENAME_FORMAT
#       define ENN_MEMORY_LOG_FILENAME_FORMAT "%Y-%m-%d-%H-%M-%S.memlog"
#endif

#ifndef LOG
#       ifndef ENN_LOG_TIME_FORMAT
#               define ENN_LOG_TIME_FORMAT "[%H:%M:%S]: "
#       endif

#       ifdef ENN_DEBUG_MODE
#               define LOG(...)                                                                                                                                                                                              \
                        do {                                                                                                                                                                                                 \
                                const i32 _to_be_written = snprintf(NULL, 0, __VA_ARGS__);                                                                                                                                   \
                                if (_to_be_written >= ENN_MAX_LOG_BUFF_SIZE) break;                                                                                                                                          \
                                const struct tm _current_time = *localtime(&(time_t) { time(NULL) });                                                                                                                        \
                                if (_internal_general_logs._buff_len + (sizeof ENN_LOG_TIME_FORMAT) * 2 >= ENN_MAX_LOG_BUFF_SIZE)                                                                                            \
                                DEBUG_DUMP_GENERAL_LOGS();                                                                                                                                                                   \
                                _internal_general_logs._buff_len += strftime(_internal_general_logs._buff + _internal_general_logs._buff_len, (sizeof ENN_LOG_TIME_FORMAT) * 2, ENN_LOG_TIME_FORMAT, &_current_time);        \
                                if (_internal_general_logs._buff_len + _to_be_written >= ENN_MAX_LOG_BUFF_SIZE)                                                                                                              \
                                DEBUG_DUMP_GENERAL_LOGS();                                                                                                                                                                   \
                                _internal_general_logs._buff_len += sprintf(_internal_general_logs._buff + _internal_general_logs._buff_len, __VA_ARGS__);                                                                   \
                                _internal_general_logs._buff[_internal_general_logs._buff_len++] = '\n';                                                                                                                     \
                                DEBUG_DUMP_GENERAL_LOGS();                                                                                                                                                                   \
                        } while (0)                                                                                                                                                                                      
#       else
#               define LOG(...)                                                                                                                                                                                              \
                        do {                                                                                                                                                                                                 \
                                const i32 _to_be_written = snprintf(NULL, 0, __VA_ARGS__);                                                                                                                                   \
                                if (_to_be_written >= ENN_MAX_LOG_BUFF_SIZE) break;                                                                                                                                          \
                                const struct tm _current_time = *localtime(&(time_t) { time(NULL) });                                                                                                                        \
                                if (_internal_general_logs._buff_len + (sizeof ENN_LOG_TIME_FORMAT) * 2 >= ENN_MAX_LOG_BUFF_SIZE)                                                                                            \
                                DEBUG_DUMP_GENERAL_LOGS();                                                                                                                                                                   \
                                _internal_general_logs._buff_len += strftime(_internal_general_logs._buff + _internal_general_logs._buff_len, (sizeof ENN_LOG_TIME_FORMAT) * 2, ENN_LOG_TIME_FORMAT, &_current_time);        \
                                if (_internal_general_logs._buff_len + _to_be_written >= ENN_MAX_LOG_BUFF_SIZE)                                                                                                              \
                                DEBUG_DUMP_GENERAL_LOGS();                                                                                                                                                                   \
                                _internal_general_logs._buff_len += sprintf(_internal_general_logs._buff + _internal_general_logs._buff_len, __VA_ARGS__);                                                                   \
                                _internal_general_logs._buff[_internal_general_logs._buff_len++] = '\n';                                                                                                                     \
                        } while (0)
#       endif
#endif

#ifndef LOG_WARN
#       define LOG_WARN(...)                                                                                                                                                                                                                                    \
                do {                                                                                                                                                                                                                                            \
                        LOG("Warning at " ENN_FILE ":" TO_STR(ENN_LINE) "\n\t" __VA_ARGS__);                                                                                                                                                                    \
                } while (0)
#endif

#ifndef LOG_ERR
#       define LOG_ERR(...)                                                                                                                                                                                                                                     \
                do {                                                                                                                                                                                                                                            \
                        LOG("Error at " ENN_FILE ":" TO_STR(ENN_LINE) "\n\t" __VA_ARGS__);                                                                                                                                                                      \
                        DEBUG_DUMP_CALLSTACK;                                                                                                                                                                                                                   \
                        debug_term();                                                                                                                                                                                                                           \
                        exit(1);                                                                                                                                                                                                                                \
                } while (0)
#endif

#ifndef ASSERT
#       ifndef ASSERT_1
#               define ASSERT_1(expr)                                                                                                                                                                                                                           \
                        do {                                                                                                                                                                                                                                    \
                                if (!(expr)) {                                                                                                                                                                                                                  \
                                        LOG("Asserion Failed at " ENN_FILE ":" TO_STR(ENN_LINE) "\n\tASSERT(" TO_STR(expr) ");");                                                                                                                               \
                                        DEBUG_DUMP_CALLSTACK();                                                                                                                                                                                                 \
                                        debug_term();                                                                                                                                                                                                           \
                                        exit(1);                                                                                                                                                                                                                \
                                }                                                                                                                                                                                                                               \
                        } while (0)
#       endif

#       ifndef ASSERT_0
#               define ASSERT_0(expr, ...)                                                                                                                                                                                                                      \
                        do {                                                                                                                                                                                                                                    \
                                if (!(expr)) {                                                                                                                                                                                                                  \
                                        LOG("Asserion Failed at " ENN_FILE ":" TO_STR(ENN_LINE) "\n\tASSERT(" TO_STR(expr) ");\n\t" __VA_ARGS__);                                                                                                               \
                                        DEBUG_DUMP_CALLSTACK();                                                                                                                                                                                                 \
                                        debug_term();                                                                                                                                                                                                           \
                                        exit(1);                                                                                                                                                                                                                \
                                }                                                                                                                                                                                                                               \
                        } while (0)
#       endif

#       define ASSERT(...) EXPAND(JOIN(ASSERT_, HAS_ONE_ARG(__VA_ARGS__))(__VA_ARGS__))
#endif

#ifndef DEBUG_TRACE
#       define DEBUG_TRACE_0()                                                                                                                                                                                                                                  \
                do {                                                                                                                                                                                                                                            \
                        if (_internal_call_stack._size + 1 >= ENN_MAX_CALLSTACK_SIZE) {                                                                                                                                                                         \
                                memmove(_internal_call_stack._calls, _internal_call_stack._calls + 1, _internal_call_stack._size * (sizeof (struct _StackFrame)));                                                                                              \
                                --_internal_call_stack._size;                                                                                                                                                                                                   \
                                ++_internal_call_stack._stackoverflow;                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        _internal_call_stack._calls[_internal_call_stack._size++] = (struct _StackFrame) {                                                                                                                                                      \
                                ._file_name     = ENN_FILE,                                                                                                                                                                                                     \
                                ._line          = ENN_LINE,                                                                                                                                                                                                     \
                                ._function_name = ENN_FUNCTION };                                                                                                                                                                                               \
                } while (0)

#       define DEBUG_TRACE_1(_func_name)                                                                                                                                                                                                                        \
                do {                                                                                                                                                                                                                                            \
                        if (_internal_call_stack._size + 1 >= ENN_MAX_CALLSTACK_SIZE) {                                                                                                                                                                         \
                                memmove(_internal_call_stack._calls, _internal_call_stack._calls + 1, _internal_call_stack._size * (sizeof (struct _StackFrame)));                                                                                              \
                                --_internal_call_stack._size;                                                                                                                                                                                                   \
                                ++_internal_call_stack._stackoverflow;                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        _internal_call_stack._calls[_internal_call_stack._size++] = (struct _StackFrame) {                                                                                                                                                      \
                                ._file_name     = ENN_FILE,                                                                                                                                                                                                     \
                                ._line          = ENN_LINE,                                                                                                                                                                                                     \
                                ._function_name = TO_STR(_func_name) };                                                                                                                                                                                         \
                } while (0)

#       ifdef ENN_DEBUG_MODE
#               define DEBUG_TRACE(...) EXPAND(JOIN(DEBUG_TRACE_, HAS_ONE_ARG(__VA_ARGS__))(__VA_ARGS__))
#       else
#               define DEBUG_TRACE(...) do { } while (0)
#       endif
#endif

#ifndef DEBUG_UNTRACE
#       ifdef ENN_DEBUG_MODE
#               define DEBUG_UNTRACE()                                                                                                                                                                                                                           \
                        do {                                                                                                                                                                                                                                     \
                                if (_internal_call_stack._size == 0) {                                                                                                                                                                                           \
                                        DEBUG_ASSERT(_internal_call_stack._stackoverflow > 0, "Requested pop from empty call stack");                                                                                                                            \
                                        --_internal_call_stack._stackoverflow;                                                                                                                                                                                   \
                                } else --_internal_call_stack._size;                                                                                                                                                                                             \
                        } while (0)
#       else
#               define DEBUG_UNTRACE() do { } while (0)
#       endif
#endif

#ifndef DEBUG_DUMP_CALLSTACK
#       ifdef ENN_DEBUG_MODE
#               define DEBUG_DUMP_CALLSTACK()                                                                                                                                                                                                                     \
                        do {                                                                                                                                                                                                                                      \
                                i32 _to_be_written = snprintf(NULL, 0, "\nCall Stack:\n");                                                                                                                                                                        \
                                if (_internal_general_logs._buff_len + _to_be_written >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_GENERAL_LOGS();                                                                                                                        \
                                _internal_general_logs._buff_len += sprintf(_internal_general_logs._buff + _internal_general_logs._buff_len, "\nCall Stack:\n");                                                                                                  \
                                for (i32 _i = _internal_call_stack._size - 1; _i >= 0; --_i) {                                                                                                                                                                    \
                                        const i32 _to_be_written = snprintf(NULL, 0, "\t#%" PRIi32 " %s:%" PRIi32 " %s\n", _internal_call_stack._size - _i,                                                                                                       \
                                                _internal_call_stack._calls[_i]._file_name,                                                                                                                                                                       \
                                                _internal_call_stack._calls[_i]._line,                                                                                                                                                                            \
                                                _internal_call_stack._calls[_i]._function_name);                                                                                                                                                                  \
                                        if (_internal_general_logs._buff_len + _to_be_written >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_GENERAL_LOGS();                                                                                                                \
                                        _internal_general_logs._buff_len += sprintf(_internal_general_logs._buff + _internal_general_logs._buff_len, "\t#%" PRIi32 " %s:%" PRIi32 " %s\n", _internal_call_stack._size - _i,                                       \
                                                _internal_call_stack._calls[_i]._file_name,                                                                                                                                                                       \
                                                _internal_call_stack._calls[_i]._line,                                                                                                                                                                            \
                                                _internal_call_stack._calls[_i]._function_name);                                                                                                                                                                  \
                                }                                                                                                                                                                                                                                 \
                        } while (0)
#       else
#               define DEBUG_DUMP_CALLSTACK() do { } while (0)
#       endif
#endif

#ifndef DEBUG_LOG
#       ifdef ENN_DEBUG_MODE
#               define DEBUG_LOG(...) LOG(__VA_ARGS__)
#       else
#               define DEBUG_LOG(...) do { } while (0)
#       endif
#endif

#ifndef DEBUG_LOG_WARN
#       ifdef ENN_DEBUG_MODE
#               define DEBUG_LOG_WARN(...) LOG_WARN(__VA_ARGS__)
#       else
#               define DEBUG_LOG_WARN(...) do { } while (0)
#       endif
#endif

#ifndef DEBUG_ASSERT
#       ifdef ENN_DEBUG_MODE
#               define DEBUG_ASSERT(...) ASSERT(__VA_ARGS__)
#       else
#               define DEBUG_ASSERT(...) do { } while (0)
#       endif
#endif

#define KiB(_sz) ((i64)(_sz) << 10)
#define MiB(_sz) ((i64)(_sz) << 20)
#define GiB(_sz) ((i64)(_sz) << 30)

#define BiK(_sz) ((f64)(_sz) / 1024)
#define BiM(_sz) ((f64)(_sz) / 1024 / 1024)
#define BiG(_sz) ((f64)(_sz) / 1024 / 1024 / 1024)

#ifdef ENN_DEBUG_MODE
        ENNDEF_PRIVATE void* debug_mem_malloc(usize sz, const char* file, i32 line);
        ENNDEF_PRIVATE void* debug_mem_calloc(usize cnt, usize sz, const char* file, i32 line);
        ENNDEF_PRIVATE void* debug_mem_realloc(void* ptr, usize new_sz, const char* file, i32 line);
        ENNDEF_PRIVATE void  debug_mem_free(void* ptr, const char* file, i32 line);
        ENNDEF_PRIVATE void  debug_mem_evidence_dump(void);

#       ifndef ENN_DEBUG_MEMORY_IMPL
#               define malloc(_sz)              debug_mem_malloc((_sz), ENN_FILE, ENN_LINE);
#               define calloc(_cnt, _sz)        debug_mem_calloc((_cnt), (_sz), ENN_FILE, ENN_LINE);
#               define realloc(_ptr, _sz)       debug_mem_realloc((_ptr), (_sz), ENN_FILE, ENN_LINE);
#               define free(_ptr)               debug_mem_free((_ptr), ENN_FILE, ENN_LINE);
#       endif
#endif

/* ---------- Data Types & Type Generic Structures ---------- */

#ifndef swap
#       define swap(_X, _Y)                                                                                                                                                                                                                                     \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(swap);                                                                                                                                                                                                                      \
                        DEBUG_ASSERT((sizeof (_X)) == (sizeof (_Y)), "Requested swap on types of different sizes");                                                                                                                                             \
                        byte __swap_buff[(sizeof (_X))];                                                                                                                                                                                                        \
                        memcpy(__swap_buff, &(_Y), (sizeof (_X)));                                                                                                                                                                                              \
                        memcpy(&(_Y), &(_X), (sizeof (_X)));                                                                                                                                                                                                    \
                        memcpy(&(_X), __swap_buff, (sizeof (_X)));                                                                                                                                                                                              \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)
#endif

typedef enum ENN_CMP {
        ENN_EQUAL       = 0x1,
        ENN_BIGGER      = 0x2,
        ENN_SMALLER     = 0x3,
        ENN_ERR         = 0x0
} ENN_CMP;

#ifndef max
#       define max_2(_X, _Y) (((_X) > (_Y)) ? (_X) : (_Y))
#       define max_3(_X, _Y, _cmp) ((EXPAND(((_cmp)((_X), (_Y)))) == ENN_BIGGER) ? (_X) : (_Y))
#       define max(...) EXPAND(JOIN(max_, GET_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__))
#endif

#ifndef min
#       define min_2(_X, _Y) (((_X) < (_Y)) ? (_X) : (_Y))
#       define min_3(_X, _Y, _cmp) ((EXPAND(((_cmp)((_X), (_Y)))) == ENN_SMALLER) ? (_X) : (_Y))
#       define min(...) EXPAND(JOIN(min_, GET_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__))
#endif

#define ENN_SMALLEST_POW2_GREATER_THAN(_X) ((i64)((!((_X) & ((_X) - 1))) ? (_X) : (0x8000000000000000 >> (__builtin_clzll((i64)(_X)) - 1))))

#ifndef ENN_VECTOR_IMPL
#       define ENN_VECTOR_IMPL

#       ifndef ENN_VECTOR_INIT_CAPACITY
#               define ENN_VECTOR_INIT_CAPACITY 8
#       endif

#       define vector(T)                                                                                                                                                                                                                                        \
                struct {                                                                                                                                                                                                                                        \
                        T*      data;                                                                                                                                                                                                                           \
                        i32     capacity;                                                                                                                                                                                                                       \
                        i32     start;                                                                                                                                                                                                                          \
                        i32     end;                                                                                                                                                                                                                            \
                }

#       define vector_new() { .data = NULL, .capacity = 0, .start = 0, .end = 0 }
#       define vector_size(_vec) ((_vec).end - (_vec).start)

#       define vector_clear(_vec)                                                                                                                                                                                                                                \
                do                                                                                                                                                                                                                                               \
                {                                                                                                                                                                                                                                                \
                        DEBUG_TRACE(vector_clear);                                                                                                                                                                                                               \
                        (_vec).start = 0;                                                                                                                                                                                                                        \
                        (_vec).end = 0;                                                                                                                                                                                                                          \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                         \
                } while (0)

#       define vector_destroy(_vec)                                                                                                                                                                                                                             \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_destroy);                                                                                                                                                                                                            \
                        free((_vec).data);                                                                                                                                                                                                                      \
                        (_vec).data     = NULL;                                                                                                                                                                                                                 \
                        (_vec).capacity = 0;                                                                                                                                                                                                                    \
                        (_vec).start    = 0;                                                                                                                                                                                                                    \
                        (_vec).end      = 0;                                                                                                                                                                                                                    \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_duplicate(_src, _dest)                                                                                                                                                                                                                    \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_duplicate);                                                                                                                                                                                                          \
                        DEBUG_ASSERT((sizeof (*(_src).data)) == (sizeof (*(_dest).data)), "Duplication request on vectors %s and %s with different type sizes", TO_STR(_src), TO_STR(_dest));                                                                   \
                        free((_dest).data);                                                                                                                                                                                                                     \
                        (_dest).capacity = (_src).capacity;                                                                                                                                                                                                     \
                        (_dest).end      = (_src).end;                                                                                                                                                                                                          \
                        (_dest).start    = (_src).start;                                                                                                                                                                                                        \
                        (_dest).data     = calloc((_dest).capacity, (sizeof (*(_dest).data)));                                                                                                                                                                  \
                        memcpy((_dest).data, (_src).data, vector_size(_dest) * (sizeof (*(_dest).data)));                                                                                                                                                       \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_reserve(_vec, _count)                                                                                                                                                                                                                     \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_reserve);                                                                                                                                                                                                            \
                        if ((_vec).capacity >= (_count)) {                                                                                                                                                                                                      \
                                DEBUG_LOG_WARN("Unnecessary reserve request for vector %s", TO_STR(_vec));                                                                                                                                                      \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        const i32 _new_capacity = (i32)(ENN_SMALLEST_POW2_GREATER_THAN(_count));                                                                                                                                                                \
                        if ((_vec).capacity == 0) {                                                                                                                                                                                                             \
                                (_vec).capacity = (_new_capacity);                                                                                                                                                                                              \
                                (_vec).data = calloc((_vec).capacity, (sizeof (*(_vec).data)));                                                                                                                                                                 \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        (_vec).capacity = (_new_capacity);                                                                                                                                                                                                      \
                        (_vec).data = realloc((_vec).data, (sizeof (*(_vec).data)) * (_vec).capacity);                                                                                                                                                          \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_shrink(_vec)                                                                                                                                                                                                                              \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_shrink);                                                                                                                                                                                                             \
                        if ((_vec).capacity == 0) {                                                                                                                                                                                                             \
                                DEBUG_LOG_WARN("Unnecessary shrinking request for vector '%s' with 0 capacity", TO_STR(_vec));                                                                                                                                  \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        if ((_vec).capacity == (i32)(ENN_SMALLEST_POW2_GREATER_THAN(vector_size(_vec)))) {                                                                                                                                                      \
                                DEBUG_LOG_WARN("Unnecessary shrinking request for already shrunk vector '%s'", TO_STR(_vec));                                                                                                                                   \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        if (vector_size(_vec) == 0) (_vec).capacity = (i32)(ENN_SMALLEST_POW2_GREATER_THAN(ENN_VECTOR_INIT_CAPACITY));                                                                                                                          \
                        else (_vec).capacity = (i32)(ENN_SMALLEST_POW2_GREATER_THAN(vector_size(_vec)));                                                                                                                                                        \
                        (_vec).data = realloc((_vec).data, (sizeof (*(_vec).data)) * (_vec).capacity);                                                                                                                                                          \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)


#       define vector_push_back(_vec, _elem)                                                                                                                                                                                                                    \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_push_back);                                                                                                                                                                                                          \
                        DEBUG_ASSERT((sizeof (*(_vec).data)) == (sizeof (_elem)), "Requested element '%s' push_back into vector '%s' of different size\n\tsizeof(%s) = %zu != sizeof(%s) = %zu",                                                                \
                                                                                TO_STR(_elem), TO_STR(_vec), TO_STR(_elem), (sizeof (_elem)), TO_STR(_vec), (sizeof *(_vec).data));                                                                             \
                        if ((_vec).capacity == 0) {                                                                                                                                                                                                             \
                                (_vec).capacity = (i32)(ENN_SMALLEST_POW2_GREATER_THAN(ENN_VECTOR_INIT_CAPACITY));                                                                                                                                              \
                                (_vec).data = calloc((_vec).capacity, (sizeof (*(_vec).data)));                                                                                                                                                                 \
                        } else if (vector_size(_vec) + 1 >= (_vec).capacity) {                                                                                                                                                                                  \
                                (_vec).capacity <<= 1;                                                                                                                                                                                                          \
                                (_vec).data = realloc((_vec).data, (sizeof (*(_vec).data)) * (_vec).capacity);                                                                                                                                                  \
                        }                                                                                                                                                                                                                                       \
                        memcpy(&((_vec).data[(_vec).end++]), &(_elem), (sizeof (_elem)));                                                                                                                                                                       \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_pop_back(_vec)                                                                                                                                                                                                                            \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_pop_back);                                                                                                                                                                                                           \
                        DEBUG_ASSERT(vector_size(_vec) > 0, "Requested pop_back from empty vector '%s'", TO_STR(_vec));                                                                                                                                         \
                        --(_vec).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_remove_at_address(_vec, _ptr)                                                                                                                                                                                                             \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_remove_at_address);                                                                                                                                                                                                  \
                        DEBUG_ASSERT(vector_size(_vec) > 0, "Requested removal from empty vector '%s'", TO_STR(_vec));                                                                                                                                          \
                        DEBUG_ASSERT((_ptr) != NULL, "Requested removal at address '%s' which is NULL from vector '%s'", TO_STR(_ptr), TO_STR(_vec));                                                                                                           \
                        DEBUG_ASSERT((sizeof (*(_vec).data)) == (sizeof (*(_ptr))), "Requested removal at address '%s' from vector '%s' of different size", TO_STR(_ptr), TO_STR(_vec));                                                                        \
                        DEBUG_ASSERT(((_ptr) >= (_vec).data + (_vec).start) && ((_ptr) < (_vec).data + (_vec).end), "Requested removal at address '%s' which is outside of vector '%s'", TO_STR(_ptr), TO_STR(_vec));                                           \
                        swap((*(_ptr)), (_vec).data[(_vec).end - 1]);                                                                                                                                                                                           \
                        --(_vec).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_remove_at_index(_vec, _idx)                                                                                                                                                                                                               \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_remove_at_index);                                                                                                                                                                                                    \
                        DEBUG_ASSERT(vector_size(_vec) > 0, "Requested removal on empty vector '%s'", TO_STR(_vec));                                                                                                                                            \
                        DEBUG_ASSERT(((_idx) >= (_vec).start) && ((_idx) < (_vec).end), "Requested removal at index '%s' which is outside of vector '%s'", TO_STR(_idx), TO_STR(_vec));                                                                         \
                        swap((_vec).data[(_idx)], (_vec).data[(_vec).end - 1]);                                                                                                                                                                                 \
                        --(_vec).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_remove_at_address_keep_order(_vec, _ptr)                                                                                                                                                                                                  \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_remove_at_address_keep_order);                                                                                                                                                                                       \
                        DEBUG_ASSERT(vector_size(_vec) > 0, "Requested removal from empty vector '%s'", TO_STR(_vec));                                                                                                                                          \
                        DEBUG_ASSERT((_ptr) != NULL, "Requested removal at address '%s' which is NULL from vector '%s'", TO_STR(_ptr), TO_STR(_vec));                                                                                                           \
                        DEBUG_ASSERT((sizeof (*(_vec).data)) == (sizeof (*(_ptr))), "Requested removal at address '%s' from vector '%s' of different size", TO_STR(_ptr), TO_STR(_vec));                                                                        \
                        DEBUG_ASSERT(((_ptr) >= (_vec).data + (_vec).start) && ((_ptr) < (_vec).data + (_vec).end), "Requested removal at address '%s' which is outside of vector '%s'", TO_STR(_ptr), TO_STR(_vec));                                           \
                        memmove((_ptr), (_ptr) + 1, (size_t)(((_vec).data + (_vec).end) - ((_ptr) + 1)) * sizeof(*(_vec).data));                                                                                                                                \
                        --(_vec).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_remove_at_index_keep_order(_vec, _idx)                                                                                                                                                                                                    \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_remove_at_index_keep_order);                                                                                                                                                                                         \
                        DEBUG_ASSERT(vector_size(_vec) > 0, "Requested removal on empty vector '%s'", TO_STR(_vec));                                                                                                                                            \
                        DEBUG_ASSERT(((_idx) >= (_vec).start) && ((_idx) < (_vec).end), "Requested removal at index '%s' which is outside of vector '%s'", TO_STR(_idx), TO_STR(_vec));                                                                         \
                        memmove(&(_vec).data[(_idx)], &(_vec).data[(_idx) + 1], (size_t)((_vec).end - (_idx) - 1) * sizeof(*(_vec).data));                                                                                                                      \
                        --(_vec).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_sort(_vec, _cmp, _start, _end)                                                                                                                                                                                                            \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_sort);                                                                                                                                                                                                               \
                        if (vector_size(_vec) == 0) {                                                                                                                                                                                                           \
                                DEBUG_LOG_WARN("Unnecessary sort request on empty vector '%s'", TO_STR(_vec));                                                                                                                                                  \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        if ((_start) >= (_end)) {                                                                                                                                                                                                               \
                                DEBUG_LOG_WARN("Requested sort on vector '%s' with bounds '%s' and '%s' which do not create a valid sequence for sorting", TO_STR(_vec), TO_STR(_start), TO_STR(_end));                                                         \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        DEBUG_ASSERT(((_start) >= (_vec).start) && ((_start) <= (_vec).end), "Requested sort starting at '%s' which is outside of vector '%s' bounds", TO_STR(_start), TO_STR(_vec));                                                           \
                        DEBUG_ASSERT(((_end) >= (_vec).start) && ((_end) <= (_vec).end), "Requested sort ending at '%s' which is outside of vector '%s' bounds", TO_STR(_end), TO_STR(_vec));                                                                   \
                                                                                                                                                                                                                                                                \
                        i32 __internal_stack[64]; /* Only need log2(2^31) for call stack so 62 for left and right index */                                                                                                                                      \
                        i32 __internal_stack_top = -1;                                                                                                                                                                                                          \
                        __internal_stack[++__internal_stack_top] = (_start);                                                                                                                                                                                    \
                        __internal_stack[++__internal_stack_top] = (_end) - 1;                                                                                                                                                                                  \
                                                                                                                                                                                                                                                                \
                        while (__internal_stack_top >= 0) {                                                                                                                                                                                                     \
                                i32 __right = __internal_stack[__internal_stack_top--];                                                                                                                                                                         \
                                i32 __left  = __internal_stack[__internal_stack_top--];                                                                                                                                                                         \
                                if (__left >= __right) continue;                                                                                                                                                                                                \
                                                                                                                                                                                                                                                                \
                                i32 __mid = __left + (__right - __left) / 2;                                                                                                                                                                                    \
                                if (__mid != __right)                                                                                                                                                                                                           \
                                        swap((_vec).data[__mid], (_vec).data[__right]);                                                                                                                                                                         \
                                                                                                                                                                                                                                                                \
                                i32 __i = __left - 1;                                                                                                                                                                                                           \
                                for (i32 __j = __left; __j < __right; ++__j) {                                                                                                                                                                                  \
                                        if (_cmp((_vec).data[__j], (_vec).data[__right]) == ENN_SMALLER) {                                                                                                                                                      \
                                                ++__i;                                                                                                                                                                                                          \
                                                if (__i != __j)                                                                                                                                                                                                 \
                                                        swap((_vec).data[__i], (_vec).data[__j]);                                                                                                                                                               \
                                        }                                                                                                                                                                                                                       \
                                }                                                                                                                                                                                                                               \
                                ++__i;                                                                                                                                                                                                                          \
                                if (__i != __right)                                                                                                                                                                                                             \
                                        swap((_vec).data[__i], (_vec).data[__right]);                                                                                                                                                                           \
                                                                                                                                                                                                                                                                \
                                i32 __left_size = (__i - 1) - __left;                                                                                                                                                                                           \
                                i32 __right_size = __right - (__i + 1);                                                                                                                                                                                         \
                                if (__left_size > __right_size) {                                                                                                                                                                                               \
                                        if (__left_size > 0)  { __internal_stack[++__internal_stack_top] = __left; __internal_stack[++__internal_stack_top] = __i - 1; }                                                                                        \
                                        if (__right_size > 0) { __internal_stack[++__internal_stack_top] = __i + 1; __internal_stack[++__internal_stack_top] = __right; }                                                                                       \
                                } else {                                                                                                                                                                                                                        \
                                        if (__right_size > 0) { __internal_stack[++__internal_stack_top] = __i + 1; __internal_stack[++__internal_stack_top] = __right; }                                                                                       \
                                        if (__left_size > 0)  { __internal_stack[++__internal_stack_top] = __left; __internal_stack[++__internal_stack_top] = __i - 1; }                                                                                        \
                                }                                                                                                                                                                                                                               \
                        }                                                                                                                                                                                                                                       \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define vector_binary_search(_vec, _cmp, _elem, _pos, _start, _end)                                                                                                                                                                                      \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(vector_binary_search);                                                                                                                                                                                                      \
                        if (vector_size(_vec) == 0) {                                                                                                                                                                                                           \
                                DEBUG_LOG_WARN("Requested search for value '%s' in empty vector %s", TO_STR(_elem), TO_STR(_vec));                                                                                                                              \
                                (_pos) = (_vec).end;                                                                                                                                                                                                            \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        if ((_start) >= (_end)) {                                                                                                                                                                                                               \
                                DEBUG_LOG_WARN("Requested search on vector '%s' for value '%s' with bounds '%s' and '%s' which do not create a valid sequence for search", TO_STR(_vec), TO_STR(_elem), TO_STR(_start), TO_STR(_end));                          \
                                (_pos) = (_vec).end;                                                                                                                                                                                                            \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        DEBUG_ASSERT((sizeof (*(_vec).data)) == (sizeof (_elem)), "Requested binary search for element '%s' in vector '%s' of different size", TO_STR(_elem), TO_STR(_vec));                                                                    \
                        DEBUG_ASSERT(((_start) >= (_vec).start) && ((_start) <= (_vec).end), "Requested binary search starting at '%s' which is outside of vector '%s' bounds", TO_STR(_start), TO_STR(_vec));                                                  \
                        DEBUG_ASSERT(((_end) >= (_vec).start) && ((_end) <= (_vec).end), "Requested binary search ending at '%s' which is outside of vector '%s' bounds", TO_STR(_end), TO_STR(_vec));                                                          \
                        if ((_start) + 1 == (_end)) {                                                                                                                                                                                                           \
                                if ((_cmp)((_vec).data[(_start)], (_elem)) == ENN_EQUAL) (_pos) = (_start);                                                                                                                                                     \
                                else (_pos) = (_vec).end;                                                                                                                                                                                                       \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        i32 _left = (_start), _right = (_end) - 1;                                                                                                                                                                                              \
                        i32 _mid = (_vec).end;                                                                                                                                                                                                                  \
                        ENN_CMP _cmp_res = ENN_ERR;                                                                                                                                                                                                             \
                        (_pos) = (_vec).end;                                                                                                                                                                                                                    \
                        while (_left <= _right) {                                                                                                                                                                                                               \
                                _mid = (_left + _right) >> 1;                                                                                                                                                                                                   \
                                _cmp_res = (_cmp)((_vec).data[_mid], (_elem));                                                                                                                                                                                  \
                                if (_cmp_res == ENN_EQUAL) { (_pos) = _mid; break; }                                                                                                                                                                            \
                                if (_cmp_res == ENN_BIGGER) _right = _mid - 1;                                                                                                                                                                                  \
                                else _left = _mid + 1;                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#endif

#ifndef ENN_DEQUE_IMPL
#       define ENN_DEQUE_IMPL

#       ifndef ENN_DEQUE_INIT_CAPACITY
#               define ENN_DEQUE_INIT_CAPACITY 16
#       endif

#       define deque(T)                                                                                                                                                                                                                                         \
                struct {                                                                                                                                                                                                                                        \
                        T*      data;                                                                                                                                                                                                                           \
                        i32     capacity;                                                                                                                                                                                                                       \
                        i32     start;                                                                                                                                                                                                                          \
                        i32     end;                                                                                                                                                                                                                            \
                }

#       define deque_new() { .data = NULL, .capacity = 0, .start = 0, .end = 0 }
#       define deque_size(_deq) ((_deq).end - (_deq).start)

#       define deque_clear(_deq)                                                                                                                                                                                                                                \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_clear);                                                                                                                                                                                                               \
                        (_deq).start = (_deq).capacity >> 1;                                                                                                                                                                                                    \
                        (_deq).end   = (_deq).capacity >> 1;                                                                                                                                                                                                    \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_destroy(_deq)                                                                                                                                                                                                                              \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_destroy);                                                                                                                                                                                                             \
                        free((_deq).data);                                                                                                                                                                                                                      \
                        (_deq).data     = NULL;                                                                                                                                                                                                                 \
                        (_deq).capacity = 0;                                                                                                                                                                                                                    \
                        (_deq).start    = 0;                                                                                                                                                                                                                    \
                        (_deq).end      = 0;                                                                                                                                                                                                                    \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_reserve(_deq, _count)                                                                                                                                                                                                                      \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_reserve);                                                                                                                                                                                                             \
                        if ((_deq).capacity >= (_count)) {                                                                                                                                                                                                      \
                                DEBUG_LOG_WARN("Unnecessary reserve request for deque %s", TO_STR(_deq));                                                                                                                                                       \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        const i32 _new_capacity = (i32)(ENN_SMALLEST_POW2_GREATER_THAN(_count));                                                                                                                                                                \
                        if ((_deq).capacity == 0) {                                                                                                                                                                                                             \
                                (_deq).capacity = (_new_capacity);                                                                                                                                                                                              \
                                (_deq).data = calloc((_deq).capacity, (sizeof (*(_deq).data)));                                                                                                                                                                 \
                                (_deq).start = (_deq).capacity >> 1;                                                                                                                                                                                            \
                                (_deq).end   = (_deq).capacity >> 1;                                                                                                                                                                                            \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        if (deque_size(_deq) == 0) {                                                                                                                                                                                                            \
                                void* _tmp = (void*)(_deq).data;                                                                                                                                                                                                \
                                (_deq).capacity = (_new_capacity);                                                                                                                                                                                              \
                                (_deq).data = calloc((_deq).capacity, (sizeof (*(_deq).data)));                                                                                                                                                                 \
                                free(_tmp);                                                                                                                                                                                                                     \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        const i32 _cap_change = (_new_capacity - (_deq).capacity) >> 1;                                                                                                                                                                         \
                        byte* _tmp = (byte*)(_deq).data;                                                                                                                                                                                                        \
                        (_deq).data = calloc((_new_capacity), (sizeof (*(_deq).data)));                                                                                                                                                                         \
                        memcpy((_deq).data + (_deq).start + _cap_change, _tmp + (_deq).start * (sizeof (*(_deq).data)), (deque_size(_deq) + 1) * (sizeof (*(_deq).data)));                                                                                      \
                        (_deq).start += _cap_change;                                                                                                                                                                                                            \
                        (_deq).end   += _cap_change;                                                                                                                                                                                                            \
                        (_deq).capacity = _new_capacity;                                                                                                                                                                                                        \
                        free(_tmp);                                                                                                                                                                                                                             \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_shrink(_deq)                                                                                                                                                                                                                               \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_shrink);                                                                                                                                                                                                              \
                        if ((_deq).capacity == 0) {                                                                                                                                                                                                             \
                                DEBUG_LOG_WARN("Unnecessary shrinking request for deque '%s' with 0 capacity", TO_STR(_deq));                                                                                                                                   \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        if ((_deq).capacity == (i32)(ENN_SMALLEST_POW2_GREATER_THAN(deque_size(_deq)))) {                                                                                                                                                       \
                                DEBUG_LOG_WARN("Unnecessary shrinking request for already shrunk deque '%s'", TO_STR(_deq));                                                                                                                                    \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        const i32 _new_capacity = ((deque_size(_deq) == 0) ? ((i32)(ENN_SMALLEST_POW2_GREATER_THAN(ENN_DEQUE_INIT_CAPACITY))) : ((i32)(ENN_SMALLEST_POW2_GREATER_THAN(deque_size(_deq)))));                                                     \
                        const i32 _cap_change = (_new_capacity - (_deq).capacity) >> 1;                                                                                                                                                                         \
                        byte* _tmp = (byte*)(_deq).data;                                                                                                                                                                                                        \
                        (_deq).data = calloc((_new_capacity), (sizeof (*(_deq).data)));                                                                                                                                                                         \
                        memcpy((_deq).data + ((_deq).start + _cap_change), _tmp + (_deq).start * (sizeof (*(_deq).data)), (deque_size(_deq) + 1) * (sizeof (*(_deq).data)));                                                                                    \
                        (_deq).end   += _cap_change;                                                                                                                                                                                                            \
                        (_deq).start += _cap_change;                                                                                                                                                                                                            \
                        (_deq).capacity = (_new_capacity);                                                                                                                                                                                                      \
                        free(_tmp);                                                                                                                                                                                                                             \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_push_back(_deq, _elem)                                                                                                                                                                                                                     \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_push_back);                                                                                                                                                                                                           \
                        DEBUG_ASSERT((sizeof (*(_deq).data)) == (sizeof (_elem)), "Requested element '%s' push_back into deque '%s' of different size", TO_STR(_elem), TO_STR(_deq));                                                                           \
                        if ((_deq).capacity == 0) {                                                                                                                                                                                                             \
                                (_deq).capacity = (i32)(ENN_SMALLEST_POW2_GREATER_THAN(ENN_DEQUE_INIT_CAPACITY));                                                                                                                                               \
                                (_deq).data = calloc((_deq).capacity, (sizeof (*(_deq).data)));                                                                                                                                                                 \
                                (_deq).start = (_deq).capacity >> 1;                                                                                                                                                                                            \
                                (_deq).end   = (_deq).capacity >> 1;                                                                                                                                                                                            \
                        } else if ((_deq).end + 1 >= (_deq).capacity) {                                                                                                                                                                                         \
                                (_deq).capacity <<= 1;                                                                                                                                                                                                          \
                                (_deq).data = realloc((_deq).data, (_deq).capacity * (sizeof (*(_deq).data)));                                                                                                                                                  \
                        }                                                                                                                                                                                                                                       \
                        (_deq).data[(_deq).end++] = (_elem);                                                                                                                                                                                                    \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_push_front(_deq, _elem)                                                                                                                                                                                                                    \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_push_front);                                                                                                                                                                                                          \
                        DEBUG_ASSERT((sizeof (*(_deq).data)) == (sizeof (_elem)), "Requested element '%s' push_front into deque '%s' of different size", TO_STR(_elem), TO_STR(_deq));                                                                          \
                        if ((_deq).capacity == 0) {                                                                                                                                                                                                             \
                                (_deq).capacity = (i32)(ENN_SMALLEST_POW2_GREATER_THAN(ENN_DEQUE_INIT_CAPACITY));                                                                                                                                               \
                                (_deq).data = calloc((_deq).capacity, (sizeof (*(_deq).data)));                                                                                                                                                                 \
                                (_deq).start = (_deq).capacity >> 1;                                                                                                                                                                                            \
                                (_deq).end   = (_deq).capacity >> 1;                                                                                                                                                                                            \
                        } else if ((_deq).start - 1 <= 0) {                                                                                                                                                                                                     \
                                const i32 _new_capacity = ((deque_size(_deq) == 0) ? ((i32)(ENN_SMALLEST_POW2_GREATER_THAN(ENN_DEQUE_INIT_CAPACITY))) : ((i32)(ENN_SMALLEST_POW2_GREATER_THAN(deque_size(_deq)))));                                             \
                                const i32 _cap_change = (_new_capacity - (_deq).capacity) >> 1;                                                                                                                                                                 \
                                byte* _tmp = (byte*)(_deq).data;                                                                                                                                                                                                \
                                (_deq).data = calloc((_new_capacity), (sizeof (*(_deq).data)));                                                                                                                                                                 \
                                memcpy((_deq).data + ((_deq).start + _cap_change), _tmp + (_deq).start * (sizeof (*(_deq).data)), (deque_size(_deq) + 1) * (sizeof (*(_deq).data)));                                                                            \
                                (_deq).end   += _cap_change;                                                                                                                                                                                                    \
                                (_deq).start += _cap_change;                                                                                                                                                                                                    \
                                (_deq).capacity = (_new_capacity);                                                                                                                                                                                              \
                                free(_tmp);                                                                                                                                                                                                                     \
                        }                                                                                                                                                                                                                                       \
                        (_deq).data[--(_deq).start] = (_elem);                                                                                                                                                                                                  \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_pop_back(_deq)                                                                                                                                                                                                                             \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_pop_back);                                                                                                                                                                                                            \
                        DEBUG_ASSERT(deque_size(_deq) > 0, "Requested pop_back from empty deque '%s'", TO_STR(_deq));                                                                                                                                           \
                        --(_deq).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_pop_front(_deq)                                                                                                                                                                                                                            \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_pop_front);                                                                                                                                                                                                           \
                        DEBUG_ASSERT(deque_size(_deq) > 0, "Requested pop_front from empty deque '%s'", TO_STR(_deq));                                                                                                                                          \
                        ++(_deq).start;                                                                                                                                                                                                                         \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_remove_at_address(_deq, _ptr)                                                                                                                                                                                                              \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_remove_at_address);                                                                                                                                                                                                   \
                        DEBUG_ASSERT(deque_size(_deq) > 0, "Requested removal from empty deque '%s'", TO_STR(_deq));                                                                                                                                            \
                        DEBUG_ASSERT((_ptr) != NULL, "Requested removal at address '%s' which is NULL from deque '%s'", TO_STR(_ptr), TO_STR(_deq));                                                                                                            \
                        DEBUG_ASSERT((sizeof (*(_deq).data)) == (sizeof (*(_ptr))), "Requested removal at address '%s' from deque '%s' of different size", TO_STR(_ptr), TO_STR(_deq));                                                                         \
                        DEBUG_ASSERT(((_ptr) >= (_deq).data + (_deq).start) && ((_ptr) < (_deq).data + (_deq).end), "Requested removal at address '%s' which is outside of deque '%s'", TO_STR(_ptr), TO_STR(_deq));                                            \
                        swap((*(_ptr)), (_deq).data[(_deq).end - 1]);                                                                                                                                                                                           \
                        --(_deq).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_remove_at_index(_deq, _idx)                                                                                                                                                                                                                \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_remove_at_index);                                                                                                                                                                                                     \
                        DEBUG_ASSERT(deque_size(_deq) > 0, "Requested removal on empty deque '%s'", TO_STR(_deq));                                                                                                                                              \
                        DEBUG_ASSERT(((_idx) >= (_deq).start) && ((_idx) < (_deq).end), "Requested removal at index '%s' which is outside of deque '%s'", TO_STR(_idx), TO_STR(_deq));                                                                          \
                        swap((_deq).data[(_idx)], (_deq).data[(_deq).end - 1]);                                                                                                                                                                                 \
                        --(_deq).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_remove_at_address_keep_order(_deq, _ptr)                                                                                                                                                                                                   \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_remove_at_address_keep_order);                                                                                                                                                                                        \
                        DEBUG_ASSERT(deque_size(_deq) > 0, "Requested removal from empty deque '%s'", TO_STR(_deq));                                                                                                                                            \
                        DEBUG_ASSERT((_ptr) != NULL, "Requested removal at address '%s' which is NULL from deque '%s'", TO_STR(_ptr), TO_STR(_deq));                                                                                                            \
                        DEBUG_ASSERT((sizeof (*(_deq).data)) == (sizeof (*(_ptr))), "Requested removal at address '%s' from deque '%s' of different size", TO_STR(_ptr), TO_STR(_deq));                                                                         \
                        DEBUG_ASSERT(((_ptr) >= (_deq).data + (_deq).start) && ((_ptr) < (_deq).data + (_deq).end), "Requested removal at address '%s' which is outside of deque '%s'", TO_STR(_ptr), TO_STR(_deq));                                            \
                        memmove((_ptr), (_ptr) + 1, (size_t)(((_deq).data + (_deq).end) - ((_ptr) + 1)) * sizeof(*(_deq).data));                                                                                                                                \
                        --(_deq).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_remove_at_index_keep_order(_deq, _idx)                                                                                                                                                                                                     \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_remove_at_index_keep_order);                                                                                                                                                                                          \
                        DEBUG_ASSERT(deque_size(_deq) > 0, "Requested removal on empty deque '%s'", TO_STR(_deq));                                                                                                                                              \
                        DEBUG_ASSERT(((_idx) >= (_deq).start) && ((_idx) < (_deq).end), "Requested removal at index '%s' which is outside of deque '%s'", TO_STR(_idx), TO_STR(_deq));                                                                          \
                        memmove(&(_deq).data[(_idx)], &(_deq).data[(_idx) + 1], (size_t)((_deq).end - (_idx) - 1) * sizeof(*(_deq).data));                                                                                                                      \
                        --(_deq).end;                                                                                                                                                                                                                           \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_sort(_deq, _cmp, _start, _end)                                                                                                                                                                                                             \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_sort);                                                                                                                                                                                                                \
                        if (deque_size(_deq) == 0) {                                                                                                                                                                                                            \
                                DEBUG_LOG_WARN("Unnecessary sort request on empty deque '%s'", TO_STR(_deq));                                                                                                                                                   \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        if ((_start) >= (_end)) {                                                                                                                                                                                                               \
                                DEBUG_LOG_WARN("Requested sort on deque '%s' with bounds '%s' and '%s' which do not create a valid sequence for sorting", TO_STR(_deq), TO_STR(_start), TO_STR(_end));                                                          \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        DEBUG_ASSERT(((_start) >= (_deq).start) && ((_start) <= (_deq).end), "Requested sort starting at '%s' which is outside of deque '%s' bounds", TO_STR(_start), TO_STR(_deq));                                                            \
                        DEBUG_ASSERT(((_end) >= (_deq).start) && ((_end) <= (_deq).end), "Requested sort ending at '%s' which is outside of deque '%s' bounds", TO_STR(_end), TO_STR(_deq));                                                                    \
                                                                                                                                                                                                                                                                \
                        i32 __internal_stack[64]; /* Only need log2(2^31) for call stack so 62 for left and right index */                                                                                                                                      \
                        i32 __internal_stack_top = -1;                                                                                                                                                                                                          \
                        __internal_stack[++__internal_stack_top] = (_start);                                                                                                                                                                                    \
                        __internal_stack[++__internal_stack_top] = (_end) - 1;                                                                                                                                                                                  \
                                                                                                                                                                                                                                                                \
                        while (__internal_stack_top >= 0) {                                                                                                                                                                                                     \
                                i32 __right = __internal_stack[__internal_stack_top--];                                                                                                                                                                         \
                                i32 __left  = __internal_stack[__internal_stack_top--];                                                                                                                                                                         \
                                if (__left >= __right) continue;                                                                                                                                                                                                \
                                                                                                                                                                                                                                                                \
                                i32 __mid = __left + (__right - __left) / 2;                                                                                                                                                                                    \
                                if (__mid != __right)                                                                                                                                                                                                           \
                                        swap((_deq).data[__mid], (_deq).data[__right]);                                                                                                                                                                         \
                                                                                                                                                                                                                                                                \
                                i32 __i = __left - 1;                                                                                                                                                                                                           \
                                for (i32 __j = __left; __j < __right; ++__j) {                                                                                                                                                                                  \
                                        if (_cmp((_deq).data[__j], (_deq).data[__right]) == ENN_SMALLER) {                                                                                                                                                      \
                                                ++__i;                                                                                                                                                                                                          \
                                                if (__i != __j)                                                                                                                                                                                                 \
                                                        swap((_deq).data[__i], (_deq).data[__j]);                                                                                                                                                               \
                                        }                                                                                                                                                                                                                       \
                                }                                                                                                                                                                                                                               \
                                ++__i;                                                                                                                                                                                                                          \
                                if (__i != __right)                                                                                                                                                                                                             \
                                        swap((_deq).data[__i], (_deq).data[__right]);                                                                                                                                                                           \
                                                                                                                                                                                                                                                                \
                                i32 __left_size = (__i - 1) - __left;                                                                                                                                                                                           \
                                i32 __right_size = __right - (__i + 1);                                                                                                                                                                                         \
                                if (__left_size > __right_size) {                                                                                                                                                                                               \
                                        if (__left_size > 0)  { __internal_stack[++__internal_stack_top] = __left; __internal_stack[++__internal_stack_top] = __i - 1; }                                                                                        \
                                        if (__right_size > 0) { __internal_stack[++__internal_stack_top] = __i + 1; __internal_stack[++__internal_stack_top] = __right; }                                                                                       \
                                } else {                                                                                                                                                                                                                        \
                                        if (__right_size > 0) { __internal_stack[++__internal_stack_top] = __i + 1; __internal_stack[++__internal_stack_top] = __right; }                                                                                       \
                                        if (__left_size > 0)  { __internal_stack[++__internal_stack_top] = __left; __internal_stack[++__internal_stack_top] = __i - 1; }                                                                                        \
                                }                                                                                                                                                                                                                               \
                        }                                                                                                                                                                                                                                       \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#       define deque_binary_search(_deq, _cmp, _elem, _pos, _start, _end)                                                                                                                                                                                       \
                do {                                                                                                                                                                                                                                            \
                        DEBUG_TRACE(deque_binary_search);                                                                                                                                                                                                       \
                        if (deque_size(_deq) == 0) {                                                                                                                                                                                                            \
                                DEBUG_LOG_WARN("Requested search for value '%s' in empty deque %s", TO_STR(_elem), TO_STR(_deq));                                                                                                                               \
                                (_pos) = (_deq).end;                                                                                                                                                                                                            \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        if ((_start) >= (_end)) {                                                                                                                                                                                                               \
                                DEBUG_LOG_WARN("Requested search on deque '%s' for value '%s' with bounds '%s' and '%s' which do not create a valid sequence for search", TO_STR(_deq), TO_STR(_elem), TO_STR(_start), TO_STR(_end));                           \
                                (_pos) = (_deq).end;                                                                                                                                                                                                            \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        DEBUG_ASSERT((sizeof (*(_deq).data)) == (sizeof (_elem)), "Requested binary search for element '%s' in deque '%s' of different size", TO_STR(_elem), TO_STR(_deq));                                                                     \
                        DEBUG_ASSERT(((_start) >= (_deq).start) && ((_start) <= (_deq).end), "Requested binary search starting at '%s' which is outside of deque '%s' bounds", TO_STR(_start), TO_STR(_deq));                                                   \
                        DEBUG_ASSERT(((_end) >= (_deq).start) && ((_end) <= (_deq).end), "Requested binary search ending at '%s' which is outside of deque '%s' bounds", TO_STR(_end), TO_STR(_deq));                                                           \
                        if ((_start) + 1 == (_end)) {                                                                                                                                                                                                           \
                                if ((_cmp)((_deq).data[(_start)], (_elem)) == ENN_EQUAL) (_pos) = (_start);                                                                                                                                                     \
                                else (_pos) = (_deq).end;                                                                                                                                                                                                       \
                                DEBUG_UNTRACE();                                                                                                                                                                                                                \
                                break;                                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        i32 _left = (_start), _right = (_end) - 1;                                                                                                                                                                                              \
                        i32 _mid = (_deq).end;                                                                                                                                                                                                                  \
                        ENN_CMP _cmp_res = ENN_ERR;                                                                                                                                                                                                             \
                        (_pos) = (_deq).end;                                                                                                                                                                                                                    \
                        while (_left <= _right) {                                                                                                                                                                                                               \
                                _mid = (_left + _right) >> 1;                                                                                                                                                                                                   \
                                _cmp_res = (_cmp)((_deq).data[_mid], (_elem));                                                                                                                                                                                  \
                                if (_cmp_res == ENN_EQUAL) { (_pos) = _mid; break; }                                                                                                                                                                            \
                                if (_cmp_res == ENN_BIGGER) _right = _mid - 1;                                                                                                                                                                                  \
                                else _left = _mid + 1;                                                                                                                                                                                                          \
                        }                                                                                                                                                                                                                                       \
                        DEBUG_UNTRACE();                                                                                                                                                                                                                        \
                } while (0)

#endif

// #define map(_key, _value)                                                                                                                                                                                                                                       \
//                 struct {                                                                                                                                                                                                                                        \
//                 } 

/* ---------- Math Helpers ---------- */

typedef struct f32vec2 { f32 x, y; }            f32vec2;
typedef struct f32vec3 { f32 x, y, z; }         f32vec3;
typedef struct f32vec4 { f32 x, y, z, w; }      f32vec4;

typedef struct f64vec2 { f64 x, y; }            f64vec2;
typedef struct f64vec3 { f64 x, y, z; }         f64vec3;
typedef struct f64vec4 { f64 x, y, z, w; }      f64vec4;

typedef struct f128vec2 { f128 x, y; }          f128vec2;
typedef struct f128vec3 { f128 x, y, z; }       f128vec3;
typedef struct f128vec4 { f128 x, y, z, w; }    f128vec4;

typedef struct i8vec2 { i8 x, y; }              i8vec2;
typedef struct i8vec3 { i8 x, y, z; }           i8vec3;
typedef struct i8vec4 { i8 x, y, z, w; }        i8vec4;

typedef struct i16vec2 { i16 x, y; }            i16vec2;
typedef struct i16vec3 { i16 x, y, z; }         i16vec3;
typedef struct i16vec4 { i16 x, y, z, w; }      i16vec4;

typedef struct i32vec2 { i32 x, y; }            i32vec2;
typedef struct i32vec3 { i32 x, y, z; }         i32vec3;
typedef struct i32vec4 { i32 x, y, z, w; }      i32vec4;

typedef struct i64vec2 { i64 x, y; }            i64vec2;
typedef struct i64vec3 { i64 x, y, z; }         i64vec3;
typedef struct i64vec4 { i64 x, y, z, w; }      i64vec4;

typedef struct u8uvec2 { u8 x, y; }             u8uvec2;
typedef struct u8uvec3 { u8 x, y, z; }          u8uvec3;
typedef struct u8uvec4 { u8 x, y, z, w; }       u8uvec4;

typedef struct u16uvec2 { u16 x, y; }           u16uvec2;
typedef struct u16uvec3 { u16 x, y, z; }        u16uvec3;
typedef struct u16uvec4 { u16 x, y, z, w; }     u16uvec4;

typedef struct u32uvec2 { u32 x, y; }           u32uvec2;
typedef struct u32uvec3 { u32 x, y, z; }        u32uvec3;
typedef struct u32uvec4 { u32 x, y, z, w; }     u32uvec4;

typedef struct u64uvec2 { u64 x, y; }           u64uvec2;
typedef struct u64uvec3 { u64 x, y, z; }        u64uvec3;
typedef struct u64uvec4 { u64 x, y, z, w; }     u64uvec4;

typedef f32                                     f32mat4[16];
typedef f32                                     f32mat3[9];
typedef f32                                     f32mat2[4];

typedef f64                                     f64mat4[16];
typedef f64                                     f64mat3[9];
typedef f64                                     f64mat2[4];

typedef f128                                    f128mat4[16];
typedef f128                                    f128mat3[9];
typedef f128                                    f128mat2[4];

typedef i8                                      i8mat4[16];
typedef i8                                      i8mat3[9];
typedef i8                                      i8mat2[4];

typedef i16                                     i16mat4[16];
typedef i16                                     i16mat3[9];
typedef i16                                     i16mat2[4];

typedef i32                                     i32mat4[16];
typedef i32                                     i32mat3[9];
typedef i32                                     i32mat2[4];

typedef i64                                     i64mat4[16];
typedef i64                                     i64mat3[9];
typedef i64                                     i64mat2[4];

typedef u8                                      u8mat4[16];
typedef u8                                      u8mat3[9];
typedef u8                                      u8mat2[4];

typedef u16                                     u16mat4[16];
typedef u16                                     u16mat3[9];
typedef u16                                     u16mat2[4];

typedef u32                                     u32mat4[16];
typedef u32                                     u32mat3[9];
typedef u32                                     u32mat2[4];

typedef u64                                     u64mat4[16];
typedef u64                                     u64mat3[9];
typedef u64                                     u64mat2[4];

/* ---------- Thread Helpers ---------- */

#if ENN_PLATFORM == ENN_WINDOWS

#       ifdef ENN_INCLUDE_OPTIONAL
#               include <winsock2.h>
#       endif

#       include <windows.h>

        typedef HANDLE Thread;
        typedef CRITICAL_SECTION Mutex;

#       define ENN_THREAD_FUNC          DWORD WINAPI
#       define ENN_THREAD_RETURN        DWORD

#else

#       include <pthread.h>

        typedef pthread_t Thread;
        typedef pthread_mutex_t Mutex;

#       define ENN_THREAD_FUNC          void*
#       define ENN_THREAD_RETURN        void*

#endif

ENNDEF_PUBLIC i32 thread_create(Thread* thread, ENN_THREAD_FUNC (*func)(void*), void* args) {
#       if ENN_PLATFORM == ENN_WINDOWS
                *thread = CreateThread(NULL, 0, func, args, 0, NULL);
                return (*thread != NULL) ? 0 : 1;
#       else
                return pthread_create(thread, NULL, func, args);
#       endif
}

ENNDEF_PUBLIC void thread_join(Thread thread) {
#       if ENN_PLATFORM == ENN_WINDOWS
                WaitForSingleObject(thread, INFINITE);
                CloseHandle(thread);
#       else
                pthread_join(thread, NULL);
#       endif
}

ENNDEF_PUBLIC void mutex_init(Mutex* mutex) {
#       if ENN_PLATFORM == ENN_WINDOWS
                InitializeCriticalSection(mutex);
#       else
                pthread_mutex_init(mutex, NULL);
#       endif
}

ENNDEF_PUBLIC void mutex_lock(Mutex* mutex) {
#       if ENN_PLATFORM == ENN_WINDOWS
                EnterCriticalSection(mutex);
#       else
                pthread_mutex_lock(mutex);
#       endif
}

ENNDEF_PUBLIC void mutex_unlock(Mutex* mutex) {
#       if ENN_PLATFORM == ENN_WINDOWS
                LeaveCriticalSection(mutex);
#       else
                pthread_mutex_unlock(mutex);
#       endif
}

ENNDEF_PUBLIC void mutex_destroy(Mutex* mutex) {
#       if ENN_PLATFORM == ENN_WINDOWS
                DeleteCriticalSection(mutex);
#       else
                pthread_mutex_destroy(mutex);
#       endif
}


/* ---------- Directories ---------- */

#include <dirent.h>

typedef struct dirent DirEntry;

ENNDEF_PUBLIC i32 directory_create(const char* dirpath) {
#       if ENN_PLATFORM == ENN_WINDOWS
                return mkdir(dirpath);
#       else
                return mkdir(dirpath, 0755);
#       endif
}

ENNDEF_PUBLIC DIR* directory_open(const char* dirpath) {
        return opendir(dirpath);
}

ENNDEF_PUBLIC i32 directory_close(DIR* dir) {
        return closedir(dir);
}

ENNDEF_PUBLIC DirEntry* directory_read(DIR* dir) {
        return readdir(dir);
}

/* ---------- File Helpers ---------- */

#include <sys/types.h>
#include <sys/stat.h>

ENNDEF_PUBLIC i32 file_read_cstring(const char *filepath, char **data) {
        DEBUG_TRACE();
        DEBUG_ASSERT(filepath != NULL);

        FILE *file = fopen(filepath, "rb");
        ASSERT(file != NULL, "[Base] Could not open file '%s'", filepath);

        fseek(file, 0, SEEK_END);
        i32 file_size = ftell(file);
        DEBUG_ASSERT(file_size != -1, "[Base] Could not get file size for '%s'", filepath);
        fseek(file, 0, SEEK_SET);

        *data = calloc(file_size + 1, (sizeof(char)));

        fread(*data, (sizeof(char)), file_size, file);
        fclose(file);

        DEBUG_UNTRACE();
        return file_size;
}

ENNDEF_PUBLIC i32 file_write_cstring(const char* filepath, char* data, i32 size) {
        DEBUG_TRACE();
        DEBUG_ASSERT(filepath != NULL);
        DEBUG_ASSERT(data != NULL);

        FILE* file = fopen(filepath, "wb");
        ASSERT(file != NULL, "[Base] Could not open file '%s'", filepath);

        i32 file_size = fwrite(data, (sizeof (char)), size, file);
        fclose(file);

        DEBUG_UNTRACE();
        return file_size;
}

ENNDEF_PUBLIC i32 file_remove(const char* filepath) {
        DEBUG_TRACE();
        DEBUG_ASSERT(filepath != NULL);
        DEBUG_UNTRACE();
        return remove(filepath);
}

ENNDEF_PUBLIC bool file_exists(const char* filepath) {
#       if ENN_PLATFORM == ENN_WINDOWS
                return GetFileAttributes(filepath) != INVALID_FILE_ATTRIBUTES;
#       else
                return access(filepath, F_OK) == 0;
#       endif
}

ENNDEF_PUBLIC time_t file_get_date(const char* filepath) {
#       if ENN_PLATFORM == ENN_WINDOWS
                struct _stat st;
                if (_stat(filepath, &st) != 0) return (time_t)0;
#       else 
                struct stat st;
                if (stat(filepath, &st) != 0) return (time_t)0;
#       endif
                return st.st_mtime;
}

ENNDEF_PUBLIC i32 file_get_size(const char* filepath) {
        DEBUG_TRACE();
        DEBUG_ASSERT(filepath != NULL);
        FILE* file = fopen(filepath, "rb");
        ASSERT(file != NULL, "[Base] Could not open file '%s'", filepath);

        fseek(file, 0, SEEK_END);
        i32 size = (i32)ftell(file);
        fclose(file);

        return size;
}

/* ---------- Text Serialization ---------- */

#ifndef ENN_DATAFILE_KEYPATH_SEPARATOR
#       define ENN_DATAFILE_KEYPATH_SEPARATOR           '|'
#endif

#ifndef ENN_DATAFILE_OPERATOR_ASSIGN
#       define ENN_DATAFILE_OPERATOR_ASSIGN             '='
#endif

#ifndef ENN_DATAFILE_STRING_IDENTIFIER
#       define ENN_DATAFILE_STRING_IDENTIFIER           '"'
#endif

#ifndef ENN_DATAFILE_OPERATOR_LIST_BEGIN
#       define ENN_DATAFILE_OPERATOR_LIST_BEGIN         '['
#endif

#ifndef ENN_DATAFILE_OPERATOR_LIST_END
#       define ENN_DATAFILE_OPERATOR_LIST_END           ']'
#endif

#ifndef ENN_DATAFILE_OPERATOR_LIST_CONTINUE
#       define ENN_DATAFILE_OPERATOR_LIST_CONTINUE      ','
#endif

#ifndef ENN_DATAFILE_LIST_ITEM_PADDING
#       define ENN_DATAFILE_LIST_ITEM_PADDING           4
#endif

#ifndef ENN_DATAFILE_FILE_EXTENSION
#       define ENN_DATAFILE_FILE_EXTENSION              ".enn"
#endif

typedef enum ENN_DATAFILE_DATA_TYPES {
        ENN_INT,
        ENN_REAL,
        ENN_STRING,
        ENN_LIST
} ENN_DATAFILE_DATA_TYPES;

#ifndef ENN_DATAFILE_MAX_STRING_SIZE
#       define ENN_DATAFILE_MAX_STRING_SIZE 64
#endif

typedef struct DataFileNode {
        char                                    key[ENN_DATAFILE_MAX_STRING_SIZE];

        ENN_DATAFILE_DATA_TYPES                 type;
        union {
        i32                             int_val;
        f32                             real_val;
        char                            string_val[ENN_DATAFILE_MAX_STRING_SIZE];
        vector(struct DataFileNode*)    children;
        }                                       data;

        struct DataFileNode*                    parent;
} DataFileNode;

typedef struct DataFile {
        DataFileNode*   root;
        char*           filepath;
} DataFile;

ENNDEF_PRIVATE void     datafile_create(DataFile* df);
ENNDEF_PRIVATE void     datafile_destroy(DataFile* df);

ENNDEF_PRIVATE void     datafile_read(DataFile* df, const char* filepath);
ENNDEF_PRIVATE void     datafile_write(DataFile* df, const char* filepath);

ENNDEF_PRIVATE i32      datafile_write_precalc(DataFile* df);

ENNDEF_PRIVATE void     datafile_read_abstract(DataFile* df, const char* buff);
ENNDEF_PRIVATE void     datafile_write_abstract(DataFile* df, const char* buff);

ENNDEF_PRIVATE void     datafile_put_i32(DataFile* df, char* keypath, i32 val);
ENNDEF_PRIVATE void     datafile_put_f32(DataFile* df, char* keypath, f32 val);
ENNDEF_PRIVATE void     datafile_put_cstring(DataFile* df, char* keypath, const char* val);

ENNDEF_PRIVATE i32      datafile_get_i32(DataFile* df, char* keypath);
ENNDEF_PRIVATE f32      datafile_get_f32(DataFile* df, char* keypath);
ENNDEF_PRIVATE char*    datafile_get_cstring(DataFile* df, char* keypath);

#ifdef __cplusplus
        }
#endif

#endif
