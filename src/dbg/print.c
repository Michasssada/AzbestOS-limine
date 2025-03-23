//
// Created by Neko on 23.03.2025.
//

#include "print.h"

#include "runtime/nanoprintf.h"

#include <drivers/cpu/io.h>
#include <stdarg.h>

// fwd declaration
void dbg_puts(
    const char* msg);

void dbg_puts(const char *msg)
{
    for (; *msg != '\0'; msg++)
    {
        outb(0xE9, *msg);
    }
}
void dbg_print(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    // Format the log message.
    char message_buffer[1024];
    npf_vsnprintf(message_buffer, sizeof(message_buffer), fmt, args);
    dbg_puts(message_buffer);

    va_end(args);
}