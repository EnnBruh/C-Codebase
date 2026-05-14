#define ENN_DEBUG_MEMORY_IMPL

#include "Base/base_internal.h"

#ifdef ENN_DEBUG_MODE


struct _Log _internal_memory_logs;
enum ENN_MEM_OPERATION_TYPES {
        ENN_ALLOCATION,
        ENN_DEALLOCATION,
        ENN_HANDLED
};

struct _mem_evidence {
        void*                           address;
        usize                           size;

        const char*                     file;
        i32                             line;
        enum ENN_MEM_OPERATION_TYPES    type;

        struct tm                       time;
};

static vector(struct _mem_evidence) _internal_memory_evidence;

void* debug_mem_malloc(usize sz, const char* file, i32 line) {
        DEBUG_TRACE();


        sz = (usize)(ENN_SMALLEST_POW2_GREATER_THAN(sz));
        void* ptr = malloc(sz);
        DEBUG_ASSERT(ptr != NULL, "Malloc failed for size %zu at %s:%" PRIi32, sz, file, line);

        struct _mem_evidence allocation = {
                .address = ptr,
                .size    = sz,
                .file    = file,
                .line    = line,
                .type    = ENN_ALLOCATION,
                .time    = *localtime(&(time_t) { time(NULL) })
        };

        vector_push_back(_internal_memory_evidence, allocation);

        DEBUG_UNTRACE();
        return ptr;
}

void* debug_mem_calloc(usize cnt, usize sz, const char* file, i32 line) {
        DEBUG_TRACE();

        sz = (usize)(ENN_SMALLEST_POW2_GREATER_THAN((sz * cnt)));
        void* ptr = calloc(1, sz);
        DEBUG_ASSERT(ptr != NULL, "Calloc failed for size %zu at %s:%" PRIi32, sz, file, line);

        struct _mem_evidence allocation = {
                .address = ptr,
                .size    = sz,
                .file    = file,
                .line    = line,
                .type    = ENN_ALLOCATION,
                .time    = *localtime(&(time_t) { time(NULL) })
        };

        vector_push_back(_internal_memory_evidence, allocation);

        DEBUG_UNTRACE();
        return ptr;
}

void* debug_mem_realloc(void* ptr, usize new_sz, const char* file, i32 line) {
        DEBUG_TRACE();

        struct _mem_evidence deallocation = {
                .address = ptr,
                .size    = 0,
                .file    = file,
                .line    = line,
                .type    = ENN_DEALLOCATION,
                .time    = *localtime(&(time_t) { time(NULL) })
        };

        new_sz = (usize)(ENN_SMALLEST_POW2_GREATER_THAN(new_sz));
        void* new_ptr = realloc(ptr, new_sz);
        DEBUG_ASSERT(ptr != NULL, "Realloc failed for size %zu at %s:%" PRIi32, new_sz, file, line);

        struct _mem_evidence allocation = {
                .address = new_ptr,
                .size    = new_sz,
                .file    = file,
                .line    = line,
                .type    = ENN_ALLOCATION,
                .time    = *localtime(&(time_t) { time(NULL) })
        };

        vector_push_back(_internal_memory_evidence, allocation);
        vector_push_back(_internal_memory_evidence, deallocation);

        DEBUG_UNTRACE();
        return new_ptr;
}

void debug_mem_free(void* ptr, const char* file, i32 line) {
        DEBUG_TRACE();

        struct _mem_evidence deallocation = {
                .address = ptr,
                .size    = 0,
                .file    = file,
                .line    = line,
                .type    = ENN_DEALLOCATION,
                .time    = *localtime(&(time_t) { time(NULL) })
        };

        free(ptr);


        vector_push_back(_internal_memory_evidence, deallocation);
        DEBUG_UNTRACE();
}

void debug_mem_evidence_dump(void) {
        DEBUG_TRACE();

        i32 i                   = 0;
        i32 to_be_written       = 0;
        for (i = _internal_memory_evidence.start; i < _internal_memory_evidence.end; ++i) {
                if (_internal_memory_evidence.data[i].type == ENN_ALLOCATION) {
                        if ((sizeof ENN_LOG_TIME_FORMAT) * 2 + _internal_memory_logs._buff_len >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_MEMORY_LOGS();
                        _internal_memory_logs._buff_len += strftime(_internal_memory_logs._buff + _internal_memory_logs._buff_len, (sizeof ENN_LOG_TIME_FORMAT) * 2, ENN_LOG_TIME_FORMAT, &_internal_memory_evidence.data[i].time);

                        to_be_written = snprintf(NULL, 0, "Alloc %zuB\t-> %p | %s:%" PRIi32 "\n", _internal_memory_evidence.data[i].size, 
                                        _internal_memory_evidence.data[i].address,
                                        _internal_memory_evidence.data[i].file,
                                        _internal_memory_evidence.data[i].line);
                        if (to_be_written + _internal_memory_logs._buff_len >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_MEMORY_LOGS();
                        _internal_memory_logs._buff_len += sprintf(_internal_memory_logs._buff + _internal_memory_logs._buff_len,
                                        "Alloc %zuB\t-> %p | %s:%" PRIi32 "\n", _internal_memory_evidence.data[i].size,
                                        _internal_memory_evidence.data[i].address,
                                        _internal_memory_evidence.data[i].file,
                                        _internal_memory_evidence.data[i].line);
                }


        }
        for (i = _internal_memory_evidence.start; i < _internal_memory_evidence.end; ++i) {
                if (_internal_memory_evidence.data[i].type == ENN_DEALLOCATION) {
                        if ((sizeof ENN_LOG_TIME_FORMAT) * 2 + _internal_memory_logs._buff_len >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_MEMORY_LOGS();
                        _internal_memory_logs._buff_len += strftime(_internal_memory_logs._buff + _internal_memory_logs._buff_len, (sizeof ENN_LOG_TIME_FORMAT) * 2, ENN_LOG_TIME_FORMAT, &_internal_memory_evidence.data[i].time);

                        to_be_written = snprintf(NULL, 0, "DeAlloc\t-> %p | %s:%" PRIi32 "\n",
                                        _internal_memory_evidence.data[i].address,
                                        _internal_memory_evidence.data[i].file,
                                        _internal_memory_evidence.data[i].line);
                        if (to_be_written + _internal_memory_logs._buff_len >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_MEMORY_LOGS();
                        _internal_memory_logs._buff_len += sprintf(_internal_memory_logs._buff + _internal_memory_logs._buff_len,
                                        "DeAlloc\t-> %p | %s:%" PRIi32 "\n",
                                        _internal_memory_evidence.data[i].address,
                                        _internal_memory_evidence.data[i].file,
                                        _internal_memory_evidence.data[i].line);
                }
        }

        i64 total_mem = 0;
        for (i = _internal_memory_evidence.start; i < _internal_memory_evidence.end; ++i)
                if (_internal_memory_evidence.data[i].type == ENN_ALLOCATION) total_mem += _internal_memory_evidence.data[i].size;

        i64 dealloc_mem = 0;
        i32 j = 0;
        for (i = _internal_memory_evidence.start; i < _internal_memory_evidence.end; ++i) {
                if (_internal_memory_evidence.data[i].type == ENN_DEALLOCATION) {
                        for (j = _internal_memory_evidence.start; j < _internal_memory_evidence.end; ++j)
                                if (_internal_memory_evidence.data[j].type == ENN_ALLOCATION && _internal_memory_evidence.data[i].address == _internal_memory_evidence.data[j].address) {
                                        _internal_memory_evidence.data[j].type = ENN_HANDLED;
                                        _internal_memory_evidence.data[i].type = ENN_HANDLED;
                                        dealloc_mem += _internal_memory_evidence.data[j].size;
                                        break;
                                }
                }
        }


        to_be_written = snprintf(NULL, 0, "\nTotal Memory Usage: %" PRIi64 " B, %lf KiB, %lf MiB\n", total_mem, BiK(total_mem), BiM(total_mem));
        if (to_be_written + _internal_memory_logs._buff_len >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_MEMORY_LOGS();
        _internal_memory_logs._buff_len += sprintf(_internal_memory_logs._buff + _internal_memory_logs._buff_len, "\nTotal Memory Usage: %" PRIi64 " B, %lf KiB, %lf MiB\n", total_mem, BiK(total_mem), BiM(total_mem));

        to_be_written = snprintf(NULL, 0, "Deallocated Memory: %" PRIi64 " B, %lf KiB, %lf MiB\n", dealloc_mem, BiK(dealloc_mem), BiM(dealloc_mem));
        if (to_be_written + _internal_memory_logs._buff_len >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_MEMORY_LOGS();
        _internal_memory_logs._buff_len += sprintf(_internal_memory_logs._buff + _internal_memory_logs._buff_len, "Deallocated Memory: %" PRIi64 " B, %lf KiB, %lf MiB\n", dealloc_mem, BiK(dealloc_mem), BiM(dealloc_mem));

        to_be_written = snprintf(NULL, 0, "Remaining Memory (Leaks): %" PRIi64 " B, %lf KiB, %lf MiB\n", total_mem - dealloc_mem, BiK(total_mem - dealloc_mem), BiM(total_mem - dealloc_mem));
        if (to_be_written + _internal_memory_logs._buff_len >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_MEMORY_LOGS();
        _internal_memory_logs._buff_len += sprintf(_internal_memory_logs._buff + _internal_memory_logs._buff_len, "Remaining Memory (Leaks): %" PRIi64 " B, %lf KiB, %lf MiB\n", total_mem - dealloc_mem, BiK(total_mem - dealloc_mem), BiM(total_mem - dealloc_mem));

        to_be_written = snprintf(NULL, 0, "\nRemaining memory allocations:\n");
        if (to_be_written + _internal_memory_logs._buff_len >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_MEMORY_LOGS();
        _internal_memory_logs._buff_len += sprintf(_internal_memory_logs._buff + _internal_memory_logs._buff_len, "\nRemaining memory allocations:\n");

        for (i = _internal_memory_evidence.start; i < _internal_memory_evidence.end; ++i) {
                if (_internal_memory_evidence.data[i].type != ENN_ALLOCATION) continue;

                to_be_written = snprintf(NULL, 0, "%zuB\t-> %p | %s:%" PRIi32 "\n", _internal_memory_evidence.data[i].size,
                                                                                                                               _internal_memory_evidence.data[i].address,
                                                                                                                               _internal_memory_evidence.data[i].file,
                                                                                                                               _internal_memory_evidence.data[i].line);

                if (to_be_written + _internal_memory_logs._buff_len >= ENN_MAX_LOG_BUFF_SIZE) DEBUG_DUMP_MEMORY_LOGS();
                _internal_memory_logs._buff_len += sprintf(_internal_memory_logs._buff + _internal_memory_logs._buff_len,
                                                  "%zuB\t-> %p | %s:%" PRIi32 "\n", _internal_memory_evidence.data[i].size,
                                                                                                                               _internal_memory_evidence.data[i].address,
                                                                                                                               _internal_memory_evidence.data[i].file,
                                                                                                                               _internal_memory_evidence.data[i].line);
        }

        vector_destroy(_internal_memory_evidence);
        DEBUG_UNTRACE();
}

#endif
