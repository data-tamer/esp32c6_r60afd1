#include "main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <stdio.h>

// ตัวแปรสำหรับ debouncing และ fall alarm optimization
static volatile uint32_t g_fall_alarm_timestamp = 0;
static volatile bool g_fall_alarm_debounce = false;
static volatile uint8_t g_fall_alarm_count = 0;
static volatile bool g_fall_alarm_triggered = false;

#define FALL_ALARM_DEBOUNCE_MS 50    // Debounce time 50ms
#define FALL_ALARM_MIN_INTERVAL_MS 1000  // Minimum interval between alarms 1 second
#define FALL_ALARM_CONFIRMATION_COUNT 2   // Need 2 consecutive triggers to confirm
#define FALL_ALARM_GPIO     GPIO_NUM_18  // D2 (GPIO18)

// ฟังก์ชันสำหรับ reset debounce flag (เรียกจาก main task)
void reset_fall_alarm_debounce(void) {
    g_fall_alarm_debounce = false;
}

// ฟังก์ชันสำหรับ reset fall alarm trigger
void reset_fall_alarm_trigger(void) {
    g_fall_alarm_triggered = false;
}

// ฟังก์ชันสำหรับตรวจสอบ fall alarm status
bool get_fall_alarm_status(void) {
    return g_fall_alarm_triggered;
}

// Optimized Fall Alarm ISR Handler
void IRAM_ATTR fall_alarm_isr_handler_optimized(void* arg) {
    // Get current timestamp for debouncing
    uint32_t current_time = esp_timer_get_time() / 1000; // Convert to milliseconds
    
    // Check if we're in debounce period
    if (g_fall_alarm_debounce) {
        return; // Ignore if still in debounce period
    }
    
    // Check minimum interval between alarms
    if ((current_time - g_fall_alarm_timestamp) < FALL_ALARM_MIN_INTERVAL_MS) {
        return; // Ignore if too soon after last alarm
    }
    
    // Check GPIO level and set fall alarm accordingly
    int level = gpio_get_level(FALL_ALARM_GPIO);
    if (level == 1) {
        g_fall_alarm_count++;
        
        // Only trigger if we get enough consecutive confirmations
        if (g_fall_alarm_count >= FALL_ALARM_CONFIRMATION_COUNT) {
            g_fall_alarm_triggered = true; // Set fall alarm
            g_fall_alarm_timestamp = current_time;
            g_fall_alarm_count = 0; // Reset counter
            
            // Note: printf() should not be used in ISR context, but for debugging we'll use it
            // In production, you should use a different method to signal the main task
            printf("[FALL ALARM OPTIMIZED] ตรวจจับการล้ม! (GPIO%d) - Timestamp: %lu\n", FALL_ALARM_GPIO, current_time);
        }
    } else {
        // Reset counter if level is low
        g_fall_alarm_count = 0;
    }
    
    // Set debounce flag
    g_fall_alarm_debounce = true;
    
    // Clear the interrupt flag
    gpio_intr_disable(FALL_ALARM_GPIO);
    gpio_intr_enable(FALL_ALARM_GPIO);
}

// Task สำหรับ reset debounce flag เป็นระยะ
void fall_alarm_debounce_task(void *arg) {
    while (1) {
        reset_fall_alarm_debounce();
        vTaskDelay(pdMS_TO_TICKS(FALL_ALARM_DEBOUNCE_MS));
    }
}

// ฟังก์ชันสำหรับตั้งค่า fall alarm GPIO พร้อม optimization
esp_err_t init_fall_alarm_optimized(void) {
    // ตั้งค่า GPIO สำหรับ Fall Alarm พร้อม interrupt
    gpio_config_t fall_alarm_io_conf = {
        .pin_bit_mask = (1ULL << FALL_ALARM_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE, // Trigger ขอบขาขึ้น
    };
    esp_err_t ret = gpio_config(&fall_alarm_io_conf);
    if (ret != ESP_OK) {
        printf("[ERROR] Failed to configure fall alarm GPIO: %s\n", esp_err_to_name(ret));
        return ret;
    }

    // เพิ่ม handler สำหรับ Fall Alarm ISR
    ret = gpio_isr_handler_add(FALL_ALARM_GPIO, fall_alarm_isr_handler_optimized, NULL);
    if (ret != ESP_OK) {
        printf("[ERROR] Failed to add fall alarm ISR handler: %s\n", esp_err_to_name(ret));
        return ret;
    } else {
        printf("[SUCCESS] Optimized fall alarm ISR handler registered for GPIO%d\n", FALL_ALARM_GPIO);
    }

    // สร้าง task สำหรับ reset debounce flag
    xTaskCreate(fall_alarm_debounce_task, "fall_alarm_debounce", 2048, NULL, 5, NULL);
    
    return ESP_OK;
}

// Test function to manually trigger fall alarm (for debugging)
void test_fall_alarm_trigger_optimized(void) {
    printf("[TEST OPTIMIZED] Manually triggering fall alarm...\n");
    g_fall_alarm_triggered = true;
    g_fall_alarm_timestamp = esp_timer_get_time() / 1000;
    printf("[TEST OPTIMIZED] g_fall_alarm_triggered set to: %d\n", g_fall_alarm_triggered);
} 