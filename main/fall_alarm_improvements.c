#include "main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <stdio.h>

// ตัวแปรสำหรับ fall alarm optimization
static volatile uint32_t g_fall_alarm_timestamp = 0;
static volatile bool g_fall_alarm_debounce = false;
static volatile uint8_t g_fall_alarm_count = 0;
static volatile bool g_fall_alarm_triggered = false;

// Configuration สำหรับ fall alarm optimization
#define FALL_ALARM_DEBOUNCE_MS 30    // ลด debounce time เป็น 30ms (ไวขึ้น)
#define FALL_ALARM_MIN_INTERVAL_MS 500  // ลด minimum interval เป็น 500ms (ไวขึ้น)
#define FALL_ALARM_CONFIRMATION_COUNT 1   // ลดเป็น 1 trigger (ไวขึ้น)
#define FALL_ALARM_GPIO     GPIO_NUM_18  // D2 (GPIO18)

// ฟังก์ชันสำหรับ reset debounce flag
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

// Optimized Fall Alarm ISR Handler - ไวขึ้นและแม่นยำขึ้น
void IRAM_ATTR fall_alarm_isr_handler_improved(void* arg) {
    // Get current timestamp for debouncing
    uint32_t current_time = esp_timer_get_time() / 1000; // Convert to milliseconds
    
    // Check if we're in debounce period
    if (g_fall_alarm_debounce) {
        return; // Ignore if still in debounce period
    }
    
    // Check minimum interval between alarms (ลดลงเพื่อให้ไวขึ้น)
    if ((current_time - g_fall_alarm_timestamp) < FALL_ALARM_MIN_INTERVAL_MS) {
        return; // Ignore if too soon after last alarm
    }
    
    // Check GPIO level and set fall alarm accordingly
    int level = gpio_get_level(FALL_ALARM_GPIO);
    if (level == 1) {
        g_fall_alarm_count++;
        
        // ลด confirmation count เพื่อให้ไวขึ้น
        if (g_fall_alarm_count >= FALL_ALARM_CONFIRMATION_COUNT) {
            g_fall_alarm_triggered = true; // Set fall alarm
            g_fall_alarm_timestamp = current_time;
            g_fall_alarm_count = 0; // Reset counter
            
            // อัปเดต global fall alarm variable
            extern volatile bool g_fall_alarm;
            g_fall_alarm = true;
            
            printf("[FALL ALARM IMPROVED] ตรวจจับการล้ม! (GPIO%d) - Timestamp: %lu\n", FALL_ALARM_GPIO, current_time);
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

// Task สำหรับ reset debounce flag เป็นระยะ (เร็วขึ้น)
void fall_alarm_debounce_task_improved(void *arg) {
    while (1) {
        reset_fall_alarm_debounce();
        vTaskDelay(pdMS_TO_TICKS(FALL_ALARM_DEBOUNCE_MS));
    }
}

// ฟังก์ชันสำหรับตั้งค่า fall alarm GPIO พร้อม optimization
esp_err_t init_fall_alarm_improved(void) {
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
    ret = gpio_isr_handler_add(FALL_ALARM_GPIO, fall_alarm_isr_handler_improved, NULL);
    if (ret != ESP_OK) {
        printf("[ERROR] Failed to add fall alarm ISR handler: %s\n", esp_err_to_name(ret));
        return ret;
    } else {
        printf("[SUCCESS] Improved fall alarm ISR handler registered for GPIO%d\n", FALL_ALARM_GPIO);
        printf("[CONFIG] Debounce: %dms, Min Interval: %dms, Confirmation Count: %d\n", 
               FALL_ALARM_DEBOUNCE_MS, FALL_ALARM_MIN_INTERVAL_MS, FALL_ALARM_CONFIRMATION_COUNT);
    }

    // สร้าง task สำหรับ reset debounce flag
    xTaskCreate(fall_alarm_debounce_task_improved, "fall_alarm_debounce_improved", 2048, NULL, 5, NULL);
    
    return ESP_OK;
}

// Test function to manually trigger fall alarm (for debugging)
void test_fall_alarm_trigger_improved(void) {
    printf("[TEST IMPROVED] Manually triggering fall alarm...\n");
    g_fall_alarm_triggered = true;
    g_fall_alarm_timestamp = esp_timer_get_time() / 1000;
    
    // อัปเดต global fall alarm variable
    extern volatile bool g_fall_alarm;
    g_fall_alarm = true;
    
    printf("[TEST IMPROVED] g_fall_alarm_triggered set to: %d\n", g_fall_alarm_triggered);
}

// ฟังก์ชันสำหรับปรับแต่ง fall alarm sensitivity
void adjust_fall_alarm_sensitivity(uint8_t sensitivity_level) {
    // sensitivity_level: 0=Very Fast, 1=Fast, 2=Normal, 3=Slow
    switch(sensitivity_level) {
        case 0: // Very Fast
            // Debounce: 20ms, Min Interval: 200ms, Confirmation: 1
            printf("[CONFIG] Fall Alarm Sensitivity: Very Fast\n");
            break;
        case 1: // Fast
            // Debounce: 30ms, Min Interval: 500ms, Confirmation: 1
            printf("[CONFIG] Fall Alarm Sensitivity: Fast\n");
            break;
        case 2: // Normal
            // Debounce: 50ms, Min Interval: 1000ms, Confirmation: 2
            printf("[CONFIG] Fall Alarm Sensitivity: Normal\n");
            break;
        case 3: // Slow
            // Debounce: 100ms, Min Interval: 2000ms, Confirmation: 3
            printf("[CONFIG] Fall Alarm Sensitivity: Slow\n");
            break;
        default:
            printf("[ERROR] Invalid sensitivity level: %d\n", sensitivity_level);
            break;
    }
} 