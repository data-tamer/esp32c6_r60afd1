#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>

// Frame constants
#define FRAME_HEADER0 0x53
#define FRAME_HEADER1 0x59
#define FRAME_TAIL0 0x54
#define FRAME_TAIL1 0x43
#define PRODUCT_STR_LEN 32

// --- Global State Variables ---
// These variables hold the most recent state reported by the radar.
// They are defined in main.c and declared here as 'extern' to be accessible by other files.

// Live Data (change frequently)
extern volatile bool g_presence;
extern volatile bool g_fall_alarm;
extern volatile bool g_stay_still_alarm;
extern volatile uint8_t g_movement_state;
extern volatile uint8_t g_body_movement_param;
extern volatile uint8_t g_heartbeat;
extern volatile int16_t g_traj_x;
extern volatile int16_t g_traj_y;
extern volatile uint16_t g_total_height_count;
extern volatile uint8_t g_height_prop_0_0_5;
extern volatile uint8_t g_height_prop_0_5_1;
extern volatile uint8_t g_height_prop_1_1_5;
extern volatile uint8_t g_height_prop_1_5_2;
extern volatile uint32_t g_non_presence_time;
extern volatile uint32_t g_height_accumulation_time;

// Settings (configuration parameters, change infrequently)
extern volatile uint8_t g_working_status;
extern volatile uint8_t g_scenario;
extern volatile int16_t g_installation_angle_x;
extern volatile int16_t g_installation_angle_y;
extern volatile int16_t g_installation_angle_z;
extern volatile uint16_t g_installation_height;
extern volatile uint8_t g_fall_detection_sensitivity;
extern volatile uint32_t g_fall_duration;
extern volatile uint16_t g_fall_breaking_height;
extern volatile uint16_t g_sitting_still_distance;
extern volatile uint16_t g_moving_distance;
extern volatile uint32_t g_operating_time;
extern volatile bool g_stay_still_switch;
extern volatile uint32_t g_stay_still_duration;
extern volatile bool g_fall_detection_switch;

// Product Information
extern char g_product_model[PRODUCT_STR_LEN];
extern char g_product_id[PRODUCT_STR_LEN];
extern char g_hardware_model[PRODUCT_STR_LEN];
extern char g_firmware_version[PRODUCT_STR_LEN];

// Function Prototypes from main.c that might be needed elsewhere
const char* movement_state_str(uint8_t state);
void mqtt_publish_live_data(void);


#endif // MAIN_H
