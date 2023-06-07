#include "console_handler.h"
#ifndef TARGET_X86
#include "pico/stdlib.h"
#include "tusb.h"
#endif
#include <string.h>
 
 #define WAIT_CMD        0
 #define STORE_CMD      1
 #define PARAM_LEFT     2
 #define PARAM_RIGHT    3
 #define PARAM_STORE    4
 #define CMD_HANDLER    5
 #define LED_HANDLER    6

 #define MAX_CMD        15



void console_usb_handler()
{
    int16_t rc = getchar_timeout_us(10);
    static int is_init = 0;
    static char buffer[MAX_CMD+1] = {0};
    static int idx = 0;
    static int state = WAIT_CMD;
    static int LED_NO = 0;

    if(!is_init)
    {
        printf(">>");
        is_init = 1;
    }

    if (rc != PICO_ERROR_TIMEOUT)
    {
        //debug 
        //if((char)rc != 0x0D)
        //     printf("read:%c state:%d \n", (char)rc,state);
        // else
        //     printf("read:CR state:%d \n",state);

        switch((char)state)
        {
            
            case WAIT_CMD:
                if((char)rc == '/')
                    state = STORE_CMD;
                
                break;

            case STORE_CMD:
                if((char)rc == '/')
                {
                    state = STORE_CMD;
                    idx = 0;
                }
                else if(idx >= MAX_CMD)
                {
                    printf("\n>>Invalid string length, maximum is 15 \n>>");
                    idx = 0;
                    LED_NO = 0;
                    state = WAIT_CMD;
                }
                else if( ((char)rc >='a' && (char)rc <='z') || ((char)rc == '_') )
                {
                    buffer[idx] = (char)rc;
                    idx++;
                }
                else if((char)rc == '[')
                {
                    state = PARAM_STORE;
                }
                else if((char)rc == 0X0D)
                    state = CMD_HANDLER;
                else
                    state = WAIT_CMD;

                break;

            case PARAM_STORE:
                if((char)rc >='1'&& (char)rc <= '4' )
                {
                    LED_NO = (char)rc - '0';
                }
                else if((char)rc == ']')
                {
                    state = PARAM_RIGHT;
                }
                else
                {
                    printf("\n>>Invalid command!\n>>");
                    idx = 0;
                    LED_NO = 0;
                    state = WAIT_CMD;
                }
            break;

            case PARAM_RIGHT:
                if((char)rc == 0x0D)
                    state = LED_HANDLER;
                else
                {
                    printf("Invalid command!\n");
                    idx = 0;
                    state = WAIT_CMD;
                }
            break;

            case CMD_HANDLER:
                buffer[idx] = '\0';
               // printf("DBG input: %s \n", buffer);
                if(strcmp(buffer, "get_sdata") == 0)
                {
                    printf("DBG:get_sdata received!\n");
                }

                idx = 0;
                printf(">>");
               
                if((char)rc == '/')
                    state = STORE_CMD;
                else
                    state = WAIT_CMD;

            break;
            
            case LED_HANDLER:
            
                if(strcmp(buffer, "set_led") == 0)
                {
                    printf("DBG:set_led received! Turn on LED: %d\n", LED_NO);
                }
                else if(strcmp(buffer, "clear_led") == 0)
                {
                    printf("DBG:clear_led received! Turn off LED: %d\n", LED_NO);
                }

                idx = 0;
                printf(">>");
               
                if((char)rc == '/')
                    state = STORE_CMD;
                else
                    state = WAIT_CMD; 
            
            break;

        }
        
        if((char)rc == 0x0D)
            printf("\n>>");
        else 
            printf("%c", (char)rc);
       

    }

}