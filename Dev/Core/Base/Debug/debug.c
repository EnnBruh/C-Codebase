#include "Base/base_internal.h"

struct _Log _internal_general_logs;
#ifdef ENN_DEBUG_MODE
        struct _CallStack _internal_call_stack;
#endif

void debug_init(void) {
        DEBUG_TRACE();

        char buff[(sizeof ENN_APP_DIRECTORY) + (sizeof ENN_LOGS_PATH) + (sizeof ENN_MEMORY_LOG_FILENAME_FORMAT) * 2]; 
        i32 buff_size = sprintf(buff, ENN_APP_DIRECTORY ENN_LOGS_PATH);

        struct tm current_time = *localtime(&(time_t) { time(NULL) });
        strftime(buff + buff_size, (sizeof ENN_GENERAL_LOG_FILENAME_FORMAT) * 2, ENN_GENERAL_LOG_FILENAME_FORMAT, &current_time);

        _internal_general_logs._stream  = fopen(buff, "wb");
#ifdef ENN_DEBUG_MODE
        strftime(buff + buff_size, (sizeof ENN_MEMORY_LOG_FILENAME_FORMAT) * 2, ENN_MEMORY_LOG_FILENAME_FORMAT, &current_time);
        _internal_memory_logs._stream   = fopen(buff, "wb");
#endif
        DEBUG_UNTRACE();
}

void debug_term(void) {
        DEBUG_TRACE();
        DEBUG_DUMP_GENERAL_LOGS();
        fclose(_internal_general_logs._stream);
#ifdef ENN_DEBUG_MODE
        debug_mem_evidence_dump();
        DEBUG_DUMP_MEMORY_LOGS();
        fclose(_internal_memory_logs._stream);
#endif
        DEBUG_UNTRACE();
}
