# SBS_ESP: Smart Battery System for ESP

This library allows an ESP8266 (or ESP32) to read data from Smart Battery System (SBS) compliant batteries, typically found in laptops, drones, and other portable devices.

This version has been heavily optimized for microcontroller memory and adapted for modern workshop use!

##  Hardware Setup (Pinout)

Most laptop batteries use a standard SMBus/I2C interface. You will typically find 5 to 9 pins on the battery connector. The most common standard is:
1. **VCC (+) / BAT+**: Usually multiple pins tied together at one end.
2. **GND (-) / BAT-**: Usually multiple pins tied together at the other end.
3. **SCL (Clock)**: SMBus Clock pin.
4. **SDA (Data)**: SMBus Data pin.
5. **System Present (SysPres/T)**: Often needs to be connected to GND to wake up the battery and enable reading or discharging.

**ESP8266 Connection (Default WebSerial Example):**
* ESP `D1` -> Battery `SCL`
* ESP `D2` -> Battery `SDA`
* ESP `GND` -> Battery `GND`
* *Note: Do NOT connect the battery VCC directly to the ESP unless you are using a proper step-down regulator, as laptop batteries are often 11.1V to 14.8V!*

> ** WARNING**: Laptop batteries can deliver extremely high currents. Be careful not to short the pins while probing (it happens to me)!

##  Workshop Web App (No Installation Required!)

We've included a modern, beautiful **Web App** that connects directly to your ESP8266 via USB (using the Web Serial API). No screen or WiFi setup needed!

### How to use:
1. Open the `examples/WebSerial/WebSerial.ino` sketch in Arduino IDE and flash it to your ESP8266.
2. Open `examples/WebSerial/webapp.html` in a modern browser (like Google Chrome or Edge).
3. Click **Connect to ESP USB**, select your ESP's COM port, and instantly see a beautiful dashboard with your battery's Health, SOC, Voltage, Current, and Manufacturer info!

## Core Library Note

To use this library reliably, you may need to reduce the I2C clock speed to that of SMBus. The library does this automatically by calling:
```cpp
Wire.setClockStretchLimit(35000);
Wire.setClock(40000);
```


## TODOs
