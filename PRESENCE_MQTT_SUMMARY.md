# สรุปการทำงานของระบบ Presence และ MQTT

## 1. การเชื่อมต่อ Presence กับ UART

### การรับข้อมูลจาก UART
- **ฟังก์ชัน**: `uart_read_task()`
- **การประมวลผล**: เมื่อได้รับข้อมูล presence จาก UART (control = 0x80, command = 0x01)
- **การอัปเดต**: ตัวแปร `g_presence` จะถูกอัปเดตตามข้อมูลที่ได้รับ
- **Debug Output**: `printf("Parsed Presence: %d\n", g_presence);`

### โครงสร้างข้อมูล UART
```
Frame Header: 0x53 0x59
Control: 0x80 (Presence Report)
Command: 0x01
Payload: 1 byte (0x00 = ไม่มีคน, 0x01 = มีคน)
Checksum: 1 byte
Frame Tail: 0x54 0x43
```

## 2. การส่งข้อมูลไปยัง MQTT

### ความถี่ในการส่ง
- **เดิม**: ทุก 60 วินาที
- **ปัจจุบัน**: ทุก 15 วินาที (แก้ไขแล้ว)

### ฟังก์ชันที่เกี่ยวข้อง
- **mqtt_publish_task()**: ส่งข้อมูลทุก 15 วินาที
- **mqtt_publish_live_data()**: ส่งข้อมูล live ไปยัง MQTT
- **get_live_json_payload_str()**: สร้าง JSON payload

### MQTT Topic
- **Topic**: `R60AFD1/live`
- **Format**: JSON
- **ข้อมูลที่ส่ง**:
  ```json
  {
    "device_id": "device_id",
    "device_type": "R60AFD1",
    "presence": true/false,
    "fall_alarm": true/false,
    "stay_still_alarm": true/false,
    "movement_state": 0/1/2,
    "body_movement_param": 0-255,
    "heartbeat": 0-255,
    "trajectory_x": -32768 to 32767,
    "trajectory_y": -32768 to 32767,
    "total_height_count": 0-65535,
    "height_prop_0_0_5": 0-255,
    "height_prop_0_5_1": 0-255,
    "height_prop_1_1_5": 0-255,
    "height_prop_1_5_2": 0-255,
    "non_presence_time": 0-4294967295
  }
  ```

## 3. การเริ่มต้นระบบ

### ใน app_main()
1. **เริ่มต้น UART**: `init_uart()`
2. **สร้าง UART Task**: `xTaskCreate(uart_read_task, ...)`
3. **สร้าง MQTT Task**: `xTaskCreate(mqtt_publish_task, ...)`
4. **ตั้งค่า MQTT Topic**: `snprintf(mqtt_topic_live, ...)`

### GPIO Configuration
- **PRESENCE_INPUT_GPIO**: GPIO_NUM_1 (D1)
- **FALL_ALARM_GPIO**: GPIO_NUM_18 (D2)

## 4. การทำงานของระบบ

### ขั้นตอนการทำงาน
1. **เริ่มต้น**: ระบบเริ่มต้น UART และ MQTT
2. **รับข้อมูล**: UART task อ่านข้อมูลจาก sensor
3. **ประมวลผล**: แยกแยะข้อมูล presence และอัปเดตตัวแปร
4. **ส่งข้อมูล**: MQTT task ส่งข้อมูลทุก 15 วินาที
5. **แสดงผล**: ข้อมูลจะถูกส่งไปยัง MQTT broker

### การ Debug
- **UART Debug**: `printf("Parsed Presence: %d\n", g_presence);`
- **MQTT Debug**: `printf("Published live data to %s\n", mqtt_topic_live);`

## 5. การทดสอบ

### การตรวจสอบการทำงาน
1. **ตรวจสอบ UART**: ดู log ที่แสดง "Parsed Presence"
2. **ตรวจสอบ MQTT**: ดู log ที่แสดง "Published live data"
3. **ตรวจสอบ Topic**: ใช้ MQTT client เชื่อมต่อ topic `R60AFD1/live`

### การแก้ไขปัญหา
- **UART ไม่ทำงาน**: ตรวจสอบการเชื่อมต่อและ baud rate
- **MQTT ไม่ส่ง**: ตรวจสอบการเชื่อมต่อ WiFi และ MQTT broker
- **ข้อมูลไม่ถูกต้อง**: ตรวจสอบ frame format และ checksum

## 6. การปรับแต่ง

### การเปลี่ยนความถี่การส่ง
แก้ไขในฟังก์ชัน `mqtt_publish_task()`:
```c
vTaskDelay(pdMS_TO_TICKS(15000)); // 15 วินาที
```

### การเพิ่มข้อมูลใหม่
เพิ่มในฟังก์ชัน `get_live_json_payload_str()`:
```c
cJSON_AddBoolToObject(json, "new_field", new_value);
``` 