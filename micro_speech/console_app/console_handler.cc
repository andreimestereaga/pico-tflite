#include "console_handler.h"
#ifndef TARGET_X86
#include "pico/stdlib.h"
#include "tusb.h"
#endif

void console_usb_handler()
{
    int16_t rc = getchar_timeout_us(10);

    if (rc != PICO_ERROR_TIMEOUT)
    {
    //print it back to console
    printf("%c", (char)rc);
    if((char)rc == 0x0D)
        printf("%c>>", 0x0A);
    }

}