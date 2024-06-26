#include <libtock-sync/display/screen.h>
#include <libtock/sensors/touch.h>
#include <libtock/tock.h>
#include <lvgl/lvgl.h>

#include "lvgl_driver.h"

static lv_disp_draw_buf_t disp_buf;
static lv_disp_drv_t disp_drv;
static lv_disp_t* display_device;

static lv_indev_drv_t indev_drv;
static lv_indev_t* touch_input_device;

static int touch_status = LIBTOCK_TOUCH_STATUS_UNSTARTED;
static uint16_t touch_x = 0, touch_y = 0;

static int buffer_size = 0;
static uint8_t* buffer;

/* screen driver */
static void screen_lvgl_driver(lv_disp_drv_t* disp, const lv_area_t* area,
                               __attribute__ ((unused)) lv_color_t* color_p) {
  int32_t x, y;
  x = area->x1;
  y = area->y1;
  int w = area->x2 - area->x1 + 1;
  int h = area->y2 - area->y1 + 1;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_write(buffer, buffer_size, (w * h) * sizeof(lv_color_t));

  lv_disp_flush_ready(disp);           /* Indicate you are ready with the flushing*/
}

static void touch_event(int status, uint16_t x, uint16_t y) {
  touch_status = status;
  touch_x      = x;
  touch_y      = y;
}

static void my_input_read(__attribute__((unused)) lv_indev_drv_t* drv, lv_indev_data_t* data) {
  if (touch_status == LIBTOCK_TOUCH_STATUS_PRESSED || touch_status == LIBTOCK_TOUCH_STATUS_MOVED) {
    data->point.x = touch_x;
    data->point.y = touch_y;
    data->state   = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}



int lvgl_driver_init(int buffer_lines) {
  uint32_t width, height;
  int error = libtock_screen_get_resolution(&width, &height);
  if (error != RETURNCODE_SUCCESS) return error;

  buffer_size = width * buffer_lines * sizeof(lv_color_t);
  error       = libtock_screen_buffer_init(buffer_size, &buffer);
  if (error != RETURNCODE_SUCCESS) return error;

  /* share the frame buffer with littlevgl */
  lv_color_t* buf = (lv_color_t*) buffer;

  /* initialize littlevgl */
  lv_init();
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = screen_lvgl_driver;
  disp_drv.hor_res  = width;
  disp_drv.ver_res  = height;
  lv_disp_draw_buf_init(&disp_buf, buf, NULL, width * buffer_lines);
  disp_drv.draw_buf = &disp_buf;
  display_device    = lv_disp_drv_register(&disp_drv);

  int touches;
  if (libtock_touch_get_number_of_touches(&touches) == RETURNCODE_SUCCESS && touches >= 1) {
    libtock_touch_enable_single_touch(touch_event);
    lv_indev_drv_init(&indev_drv);
    indev_drv.type     = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb  = my_input_read;
    touch_input_device = lv_indev_drv_register(&indev_drv);
  }

  return RETURNCODE_SUCCESS;
}

void lvgl_driver_event(int millis) {
  lv_tick_inc(millis);
  lv_task_handler();
}
