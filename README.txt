![image alt](https://github.com/yotsaphatlee-2518/esp32c6_r60afd1/blob/72b2e01502f1b66e1d26aa8ff6a367f5b1eb60b6/esp32c6.jpg)

# Set up the ESP-IDF environment
. $HOME/esp/esp-idf/export.sh

idf.py build

idf.py flash -p /dev/ttyACM0

idf.py monitor

idf.py -p /dev/ttyACM0 -b 115200 monitor



# Set up the ESP-IDF environment
. $HOME/esp/esp-idf/export.sh

# For ESP32-C6 chip, specify the chip type in the build command
idf.py set-target esp32c6
idf.py build

# Flash the firmware to the ESP32-C6
idf.py -p /dev/ttyACM0 flash

# Monitor the device output
idf.py -p /dev/ttyACM0 monitor

# Alternative monitor command with specific baud rate
idf.py -p /dev/ttyACM0 -b 115200 monitor
