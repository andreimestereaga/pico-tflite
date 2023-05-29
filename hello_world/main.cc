/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#include "main_functions.h"
#include "output_handler.h"

// add blinky example as well with this just to make sure the board is properly working
#define BLINK_LED

#ifdef BLINK_LED
#include "pico/cyw43_arch.h"

#endif

void console_usb_handler();

// This is the default main used on systems that have the standard C entry
// point. Other devices (for example FreeRTOS or ESP32) that have different
// requirements for entry code (like an app_main function) should specialize
// this main.cc file in a target-specific subfolder.
int main(int argc, char* argv[]) {
  stdio_init_all();
  while (!tud_cdc_connected()) {
      tight_loop_contents();
  }
#ifdef BLINK_LED
  if (cyw43_arch_init()) {
      printf("Wi-Fi init failed");
      return -1;
  }
  int blink = 0;

  // Tell the LED pin that the PWM is in charge of its value.
  gpio_set_function(PWM1_LED_PIN, GPIO_FUNC_PWM);
  gpio_set_function(PWM2_LED_PIN, GPIO_FUNC_PWM);
  gpio_set_function(PWM3_LED_PIN, GPIO_FUNC_PWM);
  gpio_set_function(PWM4_LED_PIN, GPIO_FUNC_PWM);
  // Figure out which slice we just connected to the LED pin
  uint slice_num1 = pwm_gpio_to_slice_num(PWM1_LED_PIN);
  uint slice_num2 = pwm_gpio_to_slice_num(PWM2_LED_PIN);
  uint slice_num3 = pwm_gpio_to_slice_num(PWM3_LED_PIN);
  uint slice_num4 = pwm_gpio_to_slice_num(PWM4_LED_PIN);

  // Get some sensible defaults for the slice configuration. By default, the
  // counter is allowed to wrap over its maximum range (0 to 2**16-1)
  pwm_config config = pwm_get_default_config();
  // Set divider, reduces counter clock to sysclock/this value
  pwm_config_set_clkdiv(&config, 4.f);
  // Load the configuration into our PWM slice, and set it running.
  pwm_init(slice_num1, &config, true);
  pwm_init(slice_num2, &config, true);
  pwm_init(slice_num3, &config, true);
  pwm_init(slice_num4, &config, true);

  // Everything after this point happens in the PWM interrupt handler, so we
  // can twiddle our thumbs

#endif

  setup();
  printf("Hello world\n");
  while (true) {
    loop();
#ifdef BLINK_LED
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, blink);
    blink ^= 1;
    sleep_ms(25);
#endif
    console_usb_handler();
  }
}


void console_usb_handler()
{
  int16_t rc = getchar_timeout_us(10);

    if (rc != PICO_ERROR_TIMEOUT)
    {
      //print it back to console
      printf("%c", (char)rc);
    }
    else{
      printf("USB read: error RX, timeout \n");
    }
}