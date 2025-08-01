#include "fall_alarm_improvements.h"
#include "main.h"
#include <stdio.h>

// ตัวอย่างการใช้งาน Fall Alarm Improvements

// ตัวอย่าง 1: การเริ่มต้น Fall Alarm ที่ปรับปรุงแล้ว
void example_init_fall_alarm(void) {
    printf("=== ตัวอย่างการเริ่มต้น Fall Alarm ที่ปรับปรุงแล้ว ===\n");
    
    // เริ่มต้น fall alarm ที่ปรับปรุงแล้ว
    esp_err_t ret = init_fall_alarm_improved();
    if (ret == ESP_OK) {
        printf("[SUCCESS] Fall alarm initialized successfully\n");
    } else {
        printf("[ERROR] Failed to initialize fall alarm\n");
    }
}

// ตัวอย่าง 2: การปรับแต่ง sensitivity
void example_adjust_sensitivity(void) {
    printf("=== ตัวอย่างการปรับแต่ง Sensitivity ===\n");
    
    // ปรับแต่ง sensitivity เป็น Fast (level 1)
    adjust_fall_alarm_sensitivity(1);
    
    // หรือปรับแต่งเป็น Very Fast (level 0) สำหรับการทดสอบ
    // adjust_fall_alarm_sensitivity(0);
}

// ตัวอย่าง 3: การตรวจสอบ fall alarm status
void example_check_status(void) {
    printf("=== ตัวอย่างการตรวจสอบ Status ===\n");
    
    bool status = get_fall_alarm_status();
    printf("Fall Alarm Status: %s\n", status ? "TRIGGERED" : "NORMAL");
    
    // ตรวจสอบ global variable ด้วย
    printf("Global Fall Alarm: %s\n", g_fall_alarm ? "TRIGGERED" : "NORMAL");
}

// ตัวอย่าง 4: การทดสอบ fall alarm
void example_test_fall_alarm(void) {
    printf("=== ตัวอย่างการทดสอบ Fall Alarm ===\n");
    
    // ทดสอบ fall alarm
    test_fall_alarm_trigger_improved();
    
    // ตรวจสอบผลลัพธ์
    example_check_status();
}

// ตัวอย่าง 5: การ reset fall alarm
void example_reset_fall_alarm(void) {
    printf("=== ตัวอย่างการ Reset Fall Alarm ===\n");
    
    // Reset fall alarm trigger
    reset_fall_alarm_trigger();
    
    // ตรวจสอบผลลัพธ์
    example_check_status();
}

// ตัวอย่าง 6: การ monitor fall alarm แบบต่อเนื่อง
void example_monitor_fall_alarm(void) {
    printf("=== ตัวอย่างการ Monitor Fall Alarm แบบต่อเนื่อง ===\n");
    
    for (int i = 0; i < 10; i++) {
        bool status = get_fall_alarm_status();
        printf("Monitor [%d]: Fall Alarm = %s\n", i, status ? "TRIGGERED" : "NORMAL");
        
        // รอ 1 วินาที
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ตัวอย่าง 7: การใช้งานใน app_main (ตัวอย่างการแทนที่โค้ดเดิม)
void example_app_main_integration(void) {
    printf("=== ตัวอย่างการ Integrate ใน app_main ===\n");
    
    // แทนที่โค้ดเดิมใน app_main():
    /*
    // โค้ดเดิม:
    gpio_config_t fall_alarm_io_conf = {
        .pin_bit_mask = (1ULL << FALL_ALARM_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    gpio_config(&fall_alarm_io_conf);
    ret = gpio_isr_handler_add(FALL_ALARM_GPIO, fall_alarm_isr_handler, NULL);
    */
    
    // โค้ดใหม่:
    esp_err_t ret = init_fall_alarm_improved();
    if (ret != ESP_OK) {
        printf("[ERROR] Failed to initialize improved fall alarm\n");
    } else {
        printf("[SUCCESS] Improved fall alarm initialized\n");
        
        // ปรับแต่ง sensitivity (ถ้าต้องการ)
        adjust_fall_alarm_sensitivity(1); // Fast mode
    }
}

// ตัวอย่าง 8: การ debug fall alarm
void example_debug_fall_alarm(void) {
    printf("=== ตัวอย่างการ Debug Fall Alarm ===\n");
    
    // ตรวจสอบ GPIO level
    int gpio_level = gpio_get_level(GPIO_NUM_18);
    printf("GPIO18 Level: %d\n", gpio_level);
    
    // ตรวจสอบ fall alarm status
    example_check_status();
    
    // ทดสอบ fall alarm
    printf("Testing fall alarm...\n");
    test_fall_alarm_trigger_improved();
    
    // ตรวจสอบผลลัพธ์
    example_check_status();
    
    // Reset fall alarm
    printf("Resetting fall alarm...\n");
    reset_fall_alarm_trigger();
    
    // ตรวจสอบผลลัพธ์
    example_check_status();
}

// ตัวอย่าง 9: การใช้งานแบบ complete workflow
void example_complete_workflow(void) {
    printf("=== ตัวอย่าง Complete Workflow ===\n");
    
    // 1. เริ่มต้น fall alarm
    example_init_fall_alarm();
    
    // 2. ปรับแต่ง sensitivity
    example_adjust_sensitivity();
    
    // 3. Monitor fall alarm
    example_monitor_fall_alarm();
    
    // 4. ทดสอบ fall alarm
    example_test_fall_alarm();
    
    // 5. Reset fall alarm
    example_reset_fall_alarm();
    
    printf("Complete workflow finished\n");
}

// ฟังก์ชันหลักสำหรับทดสอบ
void run_fall_alarm_examples(void) {
    printf("\n=== Fall Alarm Improvements Examples ===\n");
    
    // รันตัวอย่างต่างๆ
    example_init_fall_alarm();
    example_adjust_sensitivity();
    example_check_status();
    example_test_fall_alarm();
    example_reset_fall_alarm();
    example_monitor_fall_alarm();
    example_app_main_integration();
    example_debug_fall_alarm();
    example_complete_workflow();
    
    printf("\n=== All Examples Completed ===\n");
} 