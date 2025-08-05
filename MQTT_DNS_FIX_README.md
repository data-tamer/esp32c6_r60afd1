# การแก้ไขปัญหา MQTT DNS Resolution

## ปัญหาที่พบ
```
E (1618619) esp-tls: couldn't get hostname for :prod-connect.datatamer.ai: getaddrinfo() returns 202, addrinfo=0x0
E (1618629) esp-tls: Failed to open new connection
E (1618629) transport_base: Failed to open a new connection
E (1618639) mqtt_client: Error transport connect
I (1618639) mqtt_client: MQTT Error occurred
I (1618649) mqtt_client: MQTT Disconnected from broker
```

## สาเหตุของปัญหา
1. **DNS Resolution Failed**: ESP32 ไม่สามารถแก้ไขชื่อโดเมน `prod-connect.datatamer.ai` เป็น IP address ได้
2. **WiFi Connection Issues**: อาจมีปัญหากับการเชื่อมต่อ WiFi หรือการตั้งค่า DNS
3. **Network Configuration**: ไม่มีการตั้งค่า DNS servers ที่เหมาะสม

## การแก้ไขที่ทำ

### 1. เพิ่มการตั้งค่า DNS Servers
```c
// ตั้งค่า DNS servers (Google DNS และ Cloudflare DNS)
IP_ADDR4(&primary_dns, 8, 8, 8, 8);      // Google DNS
IP_ADDR4(&secondary_dns, 1, 1, 1, 1);    // Cloudflare DNS

dns_info.ip.u_addr.ip4.addr = primary_dns.u_addr.ip4.addr;
dns_info.ip.type = ESP_IPADDR_TYPE_V4;
esp_netif_set_dns_info(sta_netif, ESP_NETIF_DNS_MAIN, &dns_info);

dns_info.ip.u_addr.ip4.addr = secondary_dns.u_addr.ip4.addr;
esp_netif_set_dns_info(sta_netif, ESP_NETIF_DNS_BACKUP, &dns_info);
```

### 2. เพิ่มการทดสอบ DNS ก่อนเชื่อมต่อ MQTT
```c
// เพิ่มการทดสอบ DNS ก่อนเชื่อมต่อ MQTT
ESP_LOGI("MQTT", "Testing DNS resolution before MQTT connection...");
struct hostent *he = gethostbyname("prod-connect.datatamer.ai");
if (he != NULL) {
    ESP_LOGI("MQTT", "DNS resolution successful, starting MQTT connection...");
    esp_mqtt_client_start(mqtt_client);
} else {
    ESP_LOGE("MQTT", "DNS resolution failed, cannot connect to MQTT broker");
    // ลองใช้ DNS อื่น
    ESP_LOGI("MQTT", "Trying alternative DNS resolution...");
    vTaskDelay(pdMS_TO_TICKS(2000)); // รอ 2 วินาที
    he = gethostbyname("prod-connect.datatamer.ai");
    if (he != NULL) {
        ESP_LOGI("MQTT", "Alternative DNS resolution successful, starting MQTT connection...");
        esp_mqtt_client_start(mqtt_client);
    } else {
        ESP_LOGE("MQTT", "All DNS resolution attempts failed");
    }
}
```

### 3. เพิ่มการตรวจสอบการเชื่อมต่อ WiFi
```c
// ตรวจสอบการเชื่อมต่อ WiFi ก่อน
wifi_ap_record_t ap_info;
if (esp_wifi_sta_get_ap_info(&ap_info) != ESP_OK) {
    ESP_LOGE("MQTT", "WiFi not connected, cannot initialize MQTT");
    return;
}
ESP_LOGI("MQTT", "WiFi connected to: %s", ap_info.ssid);
```

### 4. เพิ่มการจัดการข้อผิดพลาดที่ดีขึ้น
```c
case MQTT_EVENT_ERROR:
    ESP_LOGE(MQTT_TAG, "MQTT Error occurred");
    // เพิ่มการ retry หลังจากข้อผิดพลาด
    vTaskDelay(pdMS_TO_TICKS(5000)); // รอ 5 วินาที
    ESP_LOGI(MQTT_TAG, "Attempting to reconnect to MQTT broker...");
    esp_mqtt_client_stop(mqtt_client);
    vTaskDelay(pdMS_TO_TICKS(1000)); // รอ 1 วินาที
    esp_mqtt_client_start(mqtt_client);
    break;
```

### 5. เพิ่ม Include Headers ที่จำเป็น
```c
#include "lwip/dns.h"   // เพิ่มสำหรับ DNS functions
#include "lwip/netdb.h" // เพิ่มสำหรับ gethostbyname
#include "esp_netif.h"  // เพิ่มสำหรับ network interface functions
```

## การทดสอบ
หลังจากแก้ไขแล้ว ให้ทดสอบดังนี้:

1. **ตรวจสอบ WiFi Connection**:
   ```
   I (xxxx) wifi_connect: Connected to AP SSID:Home_2.4G
   I (xxxx) wifi_connect: Got IP:192.168.x.x
   ```

2. **ตรวจสอบ DNS Resolution**:
   ```
   I (xxxx) wifi_connect: DNS resolution successful for prod-connect.datatamer.ai
   I (xxxx) wifi_connect: Resolved IP: xxx.xxx.xxx.xxx
   ```

3. **ตรวจสอบ MQTT Connection**:
   ```
   I (xxxx) MQTT: DNS resolution successful, starting MQTT connection...
   I (xxxx) mqtt_client: MQTT Connected to broker
   ```

## ข้อแนะนำเพิ่มเติม

### 1. ตรวจสอบการตั้งค่า WiFi
- ตรวจสอบว่า WiFi SSID และ Password ถูกต้อง
- ตรวจสอบว่า WiFi router มีการตั้งค่า DNS ที่เหมาะสม

### 2. ตรวจสอบ Network Connectivity
- ทดสอบ ping ไปยัง Google DNS (8.8.8.8)
- ทดสอบ nslookup สำหรับ prod-connect.datatamer.ai

### 3. ตรวจสอบ Certificate
- ตรวจสอบว่า certificate file ถูกต้องและไม่หมดอายุ
- ตรวจสอบว่า certificate ตรงกับ domain ที่ใช้

### 4. การ Debug เพิ่มเติม
หากยังมีปัญหา ให้เพิ่ม log level:
```c
esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
esp_log_level_set("mbedtls", ESP_LOG_VERBOSE);
```

## สรุป
การแก้ไขนี้จะช่วยให้ ESP32 สามารถ:
1. ตั้งค่า DNS servers ที่เหมาะสม
2. ทดสอบ DNS resolution ก่อนเชื่อมต่อ MQTT
3. จัดการข้อผิดพลาดได้ดีขึ้น
4. มีการ retry mechanism เมื่อเกิดข้อผิดพลาด 