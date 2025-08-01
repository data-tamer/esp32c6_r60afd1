#include "main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <stdio.h>

// ฟังก์ชันสำหรับ debug fall alarm
void debug_fall_alarm_settings(void) {
    printf("\n=== DEBUG FALL ALARM SETTINGS ===\n");
    printf("Current Settings:\n");
    printf("- Fall Detection Sensitivity: %d\n", g_fall_detection_sensitivity);
    printf("- Fall Duration: %lu seconds\n", g_fall_duration);
    printf("- Fall Breaking Height: %d cm\n", g_fall_breaking_height);
    printf("- Fall Detection Switch: %s\n", g_fall_detection_switch ? "ENABLED" : "DISABLED");
    printf("- Installation Height: %d cm\n", g_installation_height);
    printf("- Current g_fall_alarm: %s\n", g_fall_alarm ? "TRIGGERED" : "NORMAL");
    
    // ตรวจสอบ GPIO level
    int gpio_level = gpio_get_level(GPIO_NUM_18);
    printf("- GPIO18 Level: %d\n", gpio_level);
}

// ฟังก์ชันสำหรับปรับแต่ง fall alarm settings ให้ไวขึ้น
void optimize_fall_alarm_settings(void) {
    printf("\n=== OPTIMIZING FALL ALARM SETTINGS ===\n");
    
    // 1. เพิ่ม sensitivity เป็น Very Fast
    printf("Setting fall detection sensitivity to Very Fast (0)...\n");
    update_fall_detection_sensitivity(0);
    
    // 2. เพิ่ม fall duration
    printf("Setting fall duration to 10 seconds...\n");
    update_fall_duration(10);
    
    // 3. ลด fall breaking height
    printf("Setting fall breaking height to 100 cm...\n");
    update_fall_breaking_height(100);
    
    // 4. ตรวจสอบ fall detection switch
    if (!g_fall_detection_switch) {
        printf("Enabling fall detection switch...\n");
        update_fall_detection_switch(true);
    }
    
    printf("Settings optimized for faster fall detection!\n");
}

// ฟังก์ชันสำหรับทดสอบ fall alarm แบบ manual
void test_fall_alarm_manual(void) {
    printf("\n=== MANUAL FALL ALARM TEST ===\n");
    
    // ตรวจสอบ settings ปัจจุบัน
    debug_fall_alarm_settings();
    
    // ทดสอบโดยการ set g_fall_alarm เป็น true
    printf("Manually triggering fall alarm...\n");
    g_fall_alarm = true;
    
    // ตรวจสอบผลลัพธ์
    printf("g_fall_alarm after manual trigger: %s\n", g_fall_alarm ? "TRIGGERED" : "NORMAL");
    
    // รอ 3 วินาที
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    // Reset fall alarm
    printf("Resetting fall alarm...\n");
    g_fall_alarm = false;
    
    printf("Manual test completed!\n");
}

// ฟังก์ชันสำหรับตรวจสอบ UART data
void debug_uart_fall_alarm(void) {
    printf("\n=== DEBUG UART FALL ALARM ===\n");
    printf("Looking for fall alarm data in UART stream...\n");
    printf("Expected UART frame for fall alarm:\n");
    printf("- Control: 0x83\n");
    printf("- Command: 0x01\n");
    printf("- Payload: 0x01 (fall detected)\n");
    printf("If you don't see this frame, the radar module is not detecting falls.\n");
}

// ฟังก์ชันสำหรับตรวจสอบ radar module
void debug_radar_module(void) {
    printf("\n=== DEBUG RADAR MODULE ===\n");
    printf("Radar Module Status:\n");
    printf("- Working Status: %d\n", g_working_status);
    printf("- Presence: %s\n", g_presence ? "DETECTED" : "NOT DETECTED");
    printf("- Body Movement Param: %d\n", g_body_movement_param);
    printf("- Total Height Count: %d\n", g_total_height_count);
    
    if (g_total_height_count == 0) {
        printf("⚠️  WARNING: Radar module is not detecting any height data!\n");
        printf("This could be due to:\n");
        printf("1. Radar module not properly installed\n");
        printf("2. Installation height too high/low\n");
        printf("3. Radar module malfunction\n");
        printf("4. No person in detection area\n");
    }
}

// ฟังก์ชันสำหรับแนะนำการแก้ไข
void suggest_fall_alarm_fixes(void) {
    printf("\n=== SUGGESTED FIXES ===\n");
    
    printf("1. IMMEDIATE FIXES:\n");
    printf("   - Change fall_detection_sensitivity from 3 to 0 (Very Fast)\n");
    printf("   - Increase fall_duration from 5 to 10 seconds\n");
    printf("   - Reduce fall_breaking_height from 150 to 100 cm\n");
    
    printf("\n2. HARDWARE CHECKS:\n");
    printf("   - Verify radar module is properly connected\n");
    printf("   - Check installation height (currently 240 cm)\n");
    printf("   - Ensure person is within detection range\n");
    printf("   - Test with different movements\n");
    
    printf("\n3. SOFTWARE CHECKS:\n");
    printf("   - Verify fall_detection_switch is enabled\n");
    printf("   - Check if UART is receiving fall alarm frames\n");
    printf("   - Monitor GPIO18 for hardware fall alarm signal\n");
    
    printf("\n4. TESTING STEPS:\n");
    printf("   - Stand in detection area\n");
    printf("   - Make sudden downward movement\n");
    printf("   - Check if presence is detected\n");
    printf("   - Monitor UART for fall alarm frames\n");
}

// ฟังก์ชันหลักสำหรับ debug
void run_fall_alarm_debug(void) {
    printf("\n=== FALL ALARM DEBUG SESSION ===\n");
    
    // 1. Debug current settings
    debug_fall_alarm_settings();
    
    // 2. Debug radar module
    debug_radar_module();
    
    // 3. Debug UART fall alarm
    debug_uart_fall_alarm();
    
    // 4. Suggest fixes
    suggest_fall_alarm_fixes();
    
    // 5. Optimize settings
    optimize_fall_alarm_settings();
    
    // 6. Manual test
    test_fall_alarm_manual();
    
    printf("\n=== DEBUG SESSION COMPLETED ===\n");
} 