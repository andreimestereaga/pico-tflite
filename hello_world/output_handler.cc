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

#include "output_handler.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

volatile int iX = 0;
volatile int iY = 0;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void HandleOutput(tflite::ErrorReporter* error_reporter, float x_value,
                  float y_value) {
  // Log the current X and Y values
  //TF_LITE_REPORT_ERROR(error_reporter, "x_value: %f, y_value: %f\n",
   //                    static_cast<double>(x_value),
    //                   static_cast<double>(y_value));
    iX = static_cast<int>(x_value * 100);
    iY = static_cast<int>(y_value * 1000);
    int led_offset = map(iX, 0, 596, 0,3);
    TF_LITE_REPORT_ERROR(error_reporter, "LED: %d, x_value: %d, y_value: %d\n", led_offset, iX, iY);
    int scale = iY + 1109;
    
    pwm_set_gpio_level(PWM1_LED_PIN + led_offset,  scale*scale);

}
