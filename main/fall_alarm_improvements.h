#ifndef FALL_ALARM_IMPROVEMENTS_H
#define FALL_ALARM_IMPROVEMENTS_H

#include <stdbool.h>
#include "esp_err.h"

// Function prototypes for improved fall alarm
void reset_fall_alarm_debounce(void);
void reset_fall_alarm_trigger(void);
bool get_fall_alarm_status(void);
void IRAM_ATTR fall_alarm_isr_handler_improved(void* arg);
void fall_alarm_debounce_task_improved(void *arg);
esp_err_t init_fall_alarm_improved(void);
void test_fall_alarm_trigger_improved(void);
void adjust_fall_alarm_sensitivity(uint8_t sensitivity_level);

// Configuration constants for improved fall alarm
#define FALL_ALARM_DEBOUNCE_MS 30    // ลด debounce time เป็น 30ms (ไวขึ้น)
#define FALL_ALARM_MIN_INTERVAL_MS 500  // ลด minimum interval เป็น 500ms (ไวขึ้น)
#define FALL_ALARM_CONFIRMATION_COUNT 1   // ลดเป็น 1 trigger (ไวขึ้น)

#endif // FALL_ALARM_IMPROVEMENTS_H 