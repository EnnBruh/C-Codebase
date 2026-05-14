#include "Optional/opt.h"

bool network_init() {
        DEBUG_TRACE();
#       if ENN_PLATFORM == ENN_WINDOWS
                WSADATA wsa;
                DEBUG_UNTRACE();
                return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
#       else
                DEBUG_UNTRACE();
                return true;
#       endif
}

void network_term() {
        DEBUG_TRACE();
#       if ENN_PLATFORM == ENN_WINDOWS
                WSACleanup();
#       endif
        DEBUG_UNTRACE();
}

void network_socket_close(Socket sock) {
        DEBUG_TRACE();
#       if ENN_PLATFORM == ENN_WINDOWS
                closesocket(sock);
#       else
                close(sock);
#       endif
        DEBUG_UNTRACE();
}
