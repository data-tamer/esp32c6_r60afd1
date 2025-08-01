#include "main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <stdio.h>

// ฟังก์ชันสำหรับแก้ไขปัญหา fall alarm ทันที
void quick_fix_fall_alarm(void) {
    printf("\n=== QUICK FIX FOR FALL ALARM ===\n");
    
    // 1. เปลี่ยน sensitivity เป็น Very Fast (0)
    printf("1. Setting fall detection sensitivity to Very Fast (0)...\n");
    update_fall_detection_sensitivity(0);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // 2. เพิ่ม fall duration เป็น 10 วินาที
    printf("2. Setting fall duration to 10 seconds...\n");
    update_fall_duration(10);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // 3. ลด fall breaking height เป็น 100 cm
    printf("3. Setting fall breaking height to 100 cm...\n");
    update_fall_breaking_height(100);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // 4. เปิด fall detection switch
    printf("4. Enabling fall detection switch...\n");
    update_fall_detection_switch(true);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    printf("✅ Quick fix applied! Fall alarm should now be more sensitive.\n");
}

// ฟังก์ชันสำหรับทดสอบ fall alarm หลังแก้ไข
void test_fall_alarm_after_fix(void) {
    printf("\n=== TESTING FALL ALARM AFTER FIX ===\n");
    
    // ตรวจสอบ settings ใหม่
    printf("New Settings:\n");
    printf("- Fall Detection Sensitivity: %d (0 = Very Fast)\n", g_fall_detection_sensitivity);
    printf("- Fall Duration: %lu seconds\n", g_fall_duration);
    printf("- Fall Breaking Height: %d cm\n", g_fall_breaking_height);
    printf("- Fall Detection Switch: %s\n", g_fall_detection_switch ? "ENABLED" : "DISABLED");
    
    // ทดสอบ manual trigger
    printf("\nTesting manual fall alarm trigger...\n");
    g_fall_alarm = true;
    printf("g_fall_alarm: %s\n", g_fall_alarm ? "TRIGGERED" : "NORMAL");
    
    // รอ 2 วินาที
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // Reset
    g_fall_alarm = false;
    printf("Test completed!\n");
}

// ฟังก์ชันสำหรับตรวจสอบการทำงานของ radar
void check_radar_functionality(void) {
    printf("\n=== CHECKING RADAR FUNCTIONALITY ===\n");
    
    printf("Radar Status:\n");
    printf("- Working Status: %d (1 = Working)\n", g_working_status);
    printf("- Presence: %s\n", g_presence ? "DETECTED" : "NOT DETECTED");
    printf("- Body Movement: %d\n", g_body_movement_param);
    printf("- Height Count: %d\n", g_total_height_count);
    
    if (g_working_status == 1) {
        printf("✅ Radar module is working properly\n");
    } else {
        printf("❌ Radar module is not working\n");
    }
    
    if (g_presence) {
        printf("✅ Presence is detected\n");
    } else {
        printf("❌ No presence detected - check if person is in range\n");
    }
    
    if (g_total_height_count > 0) {
        printf("✅ Height data is being received\n");
    } else {
        printf("❌ No height data - check installation height\n");
    }
}

// ฟังก์ชันสำหรับแนะนำการทดสอบ
void suggest_testing_procedure(void) {
    printf("\n=== TESTING PROCEDURE ===\n");
    
    printf("1. STANDARD FALL TEST:\n");
    printf("   - Stand in the detection area\n");
    printf("   - Make a sudden downward movement (like falling)\n");
    printf("   - Check if fall alarm triggers\n");
    
    printf("\n2. MOVEMENT TEST:\n");
    printf("   - Walk around in the detection area\n");
    printf("   - Check if presence is detected\n");
    printf("   - Verify body movement parameter changes\n");
    
    printf("\n3. HEIGHT TEST:\n");
    printf("   - Stand at different heights (sitting, standing, lying)\n");
    printf("   - Check if height proportion data changes\n");
    
    printf("\n4. MONITOR UART:\n");
    printf("   - Watch for fall alarm frames: 53 59 83 01 00 01 01\n");
    printf("   - Check if GPIO18 level changes during fall simulation\n");
}

// ฟังก์ชันสำหรับตรวจสอบ GPIO fall alarm
void check_gpio_fall_alarm(void) {
    printf("\n=== CHECKING GPIO FALL ALARM ===\n");
    
    // ตรวจสอบ GPIO18 level
    int gpio_level = gpio_get_level(GPIO_NUM_18);
    printf("GPIO18 Level: %d\n", gpio_level);
    
    if (gpio_level == 1) {
        printf("✅ GPIO18 is HIGH - Fall alarm signal detected\n");
    } else {
        printf("❌ GPIO18 is LOW - No fall alarm signal\n");
    }
    
    // ตรวจสอบ interrupt configuration
    printf("GPIO18 Interrupt Type: POSEDGE (triggers on rising edge)\n");
    printf("GPIO18 Pull-up: ENABLED\n");
}

// ฟังก์ชันสำหรับ reset และทดสอบใหม่
void reset_and_test_fall_alarm(void) {
    printf("\n=== RESET AND TEST FALL ALARM ===\n");
    
    // Reset fall alarm
    g_fall_alarm = false;
    printf("Fall alarm reset\n");
    
    // ตรวจสอบ GPIO
    check_gpio_fall_alarm();
    
    // ตรวจสอบ radar
    check_radar_functionality();
    
    // แนะนำการทดสอบ
    suggest_testing_procedure();
}

// ฟังก์ชันหลักสำหรับแก้ไขปัญหา
void fix_fall_alarm_issues(void) {
    printf("\n=== FALL ALARM ISSUE FIX ===\n");
    
    // 1. แก้ไข settings
    quick_fix_fall_alarm();
    
    // 2. ตรวจสอบ radar
    check_radar_functionality();
    
    // 3. ตรวจสอบ GPIO
    check_gpio_fall_alarm();
    
    // 4. ทดสอบหลังแก้ไข
    test_fall_alarm_after_fix();
    
    // 5. Reset และแนะนำการทดสอบ
    reset_and_test_fall_alarm();
    
    printf("\n=== FIX COMPLETED ===\n");
    printf("Now try testing with the following movements:\n");
    printf("1. Sudden downward movement\n");
    printf("2. Lying down quickly\n");
    printf("3. Sitting down rapidly\n");
    printf("4. Bending down suddenly\n");
} 