#include "core.h"

signed main() {
        DEBUG_TRACE();
        core_init();
        core_run();
        core_term();
        DEBUG_UNTRACE();
        return 0;
}