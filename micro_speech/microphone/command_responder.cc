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

#include "command_responder.h"

#include "tensorflow/lite/micro/micro_log.h"

#define BLINK_LED

#ifdef BLINK_LED
#include "pico/cyw43_arch.h"
#endif


// The default implementation writes out the name of the recognized command
// to the error console. Real applications will want to take some custom
// action instead, and should implement their own versions of this function.
void RespondToCommand(int32_t current_time, const char* found_command,
                      uint8_t score, bool is_new_command) {

#ifdef BLINK_LED
  static int init_led = 0;
  if(!init_led) {
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
    }
    init_led = 1;
  }
#endif

  if (is_new_command) {
   // MicroPrintf("Heard %s (%d) @%dms", found_command, score, current_time);
  
#ifdef BLINK_LED
  if (found_command == "yes"){
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
  }
  else if (found_command == "no") {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
  }
#endif
  }
}
