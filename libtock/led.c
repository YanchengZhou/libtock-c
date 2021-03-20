#include "led.h"

int led_count(void) {
  syscall_return_t rval = command(DRIVER_NUM_LEDS, 0, 0, 0);
  if (rval.type == TOCK_SYSCALL_SUCCESS_U32) {
    return rval.data[0];
  } else {
    return tock_error_to_rcode(rval.data[0]);
  }
}

int led_on(int led_num) {
  syscall_return_t rval = command(DRIVER_NUM_LEDS, 1, led_num, 0);
  if (rval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(rval.data[0]);
  }
}

int led_off(int led_num) {
  syscall_return_t rval = command(DRIVER_NUM_LEDS, 2, led_num, 0);
  if (rval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(rval.data[0]);
  }
}

int led_toggle(int led_num) {
  syscall_return_t rval = command(DRIVER_NUM_LEDS, 3, led_num, 0);
  if (rval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(rval.data[0]);
  }
}
