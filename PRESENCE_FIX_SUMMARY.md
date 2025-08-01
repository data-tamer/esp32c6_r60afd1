# สรุปการแก้ไขปัญหา Presence

## ปัญหาที่พบ
จาก log ที่แสดง ระบบไม่ได้รับข้อมูล presence จาก UART เลย ทำให้ `g_presence` เป็น `false` ตลอดเวลา

## การแก้ไขที่ทำ

### 1. เพิ่มการอัปเดต Presence จากข้อมูลอื่นๆ
เนื่องจาก sensor อาจจะไม่ได้ส่งข้อมูล presence โดยตรง แต่ส่งข้อมูลอื่นๆ ที่สามารถใช้เป็น indicator ได้:

#### จาก Movement State (control = 0x80, command = 0x02)
```c
if (movement_value > 0) {
    g_presence = true;
    printf("Updated Presence from Movement State: %d (presence: true)\n", movement_value);
} else {
    g_presence = false;
    printf("Updated Presence from Movement State: %d (presence: false)\n", movement_value);
}
```

#### จาก Body Movement Parameter (control = 0x80, command = 0x03)
```c
if (body_movement_value > 0) {
    g_presence = true;
    printf("Updated Presence from Body Movement: %d (presence: true)\n", body_movement_value);
} else {
    g_presence = false;
    printf("Updated Presence from Body Movement: %d (presence: false)\n", body_movement_value);
}
```

#### จาก Height Count (control = 0x83, command = 0x0E)
```c
if (total > 0) {
    g_presence = true;
    printf("Updated Presence from Height Count: %d (presence: true)\n", total);
} else {
    g_presence = false;
    printf("Updated Presence from Height Count: %d (presence: false)\n", total);
}
```

### 2. เพิ่มการส่งคำสั่งขอข้อมูล Presence
เพิ่มฟังก์ชันและ task สำหรับส่งคำสั่งขอข้อมูล presence:

```c
void send_presence_query(void) {
    printf("Sent Presence Query\n");
    send_query(0x80, 0x01, 0x00); // ขอข้อมูล presence
}

void presence_query_task(void *arg)
{
    while(1) {
        send_presence_query();
        vTaskDelay(pdMS_TO_TICKS(5000)); // ส่งคำสั่งทุก 5 วินาที
    }
}
```

### 3. เพิ่มการ Debug
- เพิ่มการแสดง control และ command ใน unknown frame
- เพิ่มการแสดงสถานะ presence ในทุกครั้งที่ส่ง MQTT
- เพิ่มการ debug สำหรับข้อมูลที่ได้รับจาก UART

### 4. การทำงานใหม่
1. **ส่งคำสั่งขอข้อมูล**: `presence_query_task` ส่งคำสั่งขอข้อมูล presence ทุก 5 วินาที
2. **อัปเดตจากข้อมูลอื่น**: หากไม่ได้รับข้อมูล presence โดยตรง จะใช้ข้อมูล movement, body movement, หรือ height count แทน
3. **แสดงผล**: ส่งข้อมูล presence ไปยัง MQTT ทุก 15 วินาที
4. **Debug**: แสดงสถานะ presence ในทุกครั้งที่ส่งข้อมูล

## การทดสอบ
หลังจากแก้ไขแล้ว ให้ตรวจสอบ log ต่อไปนี้:

### 1. การส่งคำสั่ง
```
Sent Presence Query
```

### 2. การอัปเดต Presence
```
Updated Presence from Movement State: 1 (presence: true)
Updated Presence from Body Movement: 1 (presence: true)
Updated Presence from Height Count: 5 (presence: true)
```

### 3. การส่ง MQTT
```
[MQTT DEBUG] Current presence status: TRUE
Published live data to R60AFD1/live
```

## การตรวจสอบผลลัพธ์
1. **ตรวจสอบ Log**: ดูว่ามีการอัปเดต presence หรือไม่
2. **ตรวจสอบ MQTT**: ดูข้อมูล JSON ที่ส่งไปยัง topic `R60AFD1/live`
3. **ตรวจสอบ Sensor**: ตรวจสอบว่า sensor ทำงานถูกต้องหรือไม่

## หมายเหตุ
- หาก sensor ไม่ส่งข้อมูล presence โดยตรง ระบบจะใช้ข้อมูลอื่นๆ เป็น indicator
- การส่งคำสั่งขอข้อมูล presence จะช่วยให้ sensor ส่งข้อมูลกลับมา
- หากยังไม่ได้รับข้อมูล presence อาจต้องตรวจสอบการเชื่อมต่อ UART หรือการตั้งค่า sensor 