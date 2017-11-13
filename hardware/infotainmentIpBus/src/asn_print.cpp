
#include <cstdarg>
#include <cstdint>
#include <cstdio>

extern "C" {
#include "pl/asn_base/asn_base.h"
}

// Keeps track of the current indentation level
static thread_local int g_indent = 0;

// This function will increase or decrease the current indentation level
int ASN_PrintSetIndent(int i) {
    g_indent += i;
    if (g_indent < 0) {  // Do not allow negative indentation
        g_indent = 0;
    }
    return 0;
}

// This function will print the given input to stdout
int ASN_Print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    return 0;
}

// This function will print the given input to stdout and with indentation
int ASN_PrintIndented(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf("%*c", g_indent, ' ');
    vprintf(format, args);
    va_end(args);
    return 0;
}
