#ifndef FALL_ALARM_OPTIMIZED_H
#define FALL_ALARM_OPTIMIZED_H

#include <stdbool.h>
#include "esp_err.h"

// Function prototypes for optimized fall alarm
void reset_fall_alarm_debounce(void);
void reset_fall_alarm_trigger(void);
bool get_fall_alarm_status(void);
void IRAM_ATTR fall_alarm_isr_handler_optimized(void* arg);
void fall_alarm_debounce_task(void *arg);
esp_err_t init_fall_alarm_optimized(void);
void test_fall_alarm_trigger_optimized(void);

// Configuration constants
#define FALL_ALARM_DEBOUNCE_MS 50    // Debounce time 50ms
#define FALL_ALARM_MIN_INTERVAL_MS 1000  // Minimum interval between alarms 1 second
#define FALL_ALARM_CONFIRMATION_COUNT 2   // Need 2 consecutive triggers to confirm

#endif // FALL_ALARM_OPTIMIZED_H 