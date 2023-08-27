# embot-firmware

Firmware for a DIY embroidery machine.

## Hardware
* A simple DIY plotter
* A simple grbl board for aruino nano with 3 step sticks
* A DIY adapter board for replacing the arduino nano with a raspberry pi pico

## Dependencies
The only dependency is the [pico-sdk](https://github.com/raspberrypi/pico-sdk).

1. Install CMake (at least version 3.13), and GCC cross compiler
   ```
   sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi
   ```
2. Clone the pico-sdk
   ```
   git clone https://github.com/raspberrypi/pico-sdk
   ``` 
3. Build this project
   ```
   git clone https://github.com/tschabo/embot-firmware
   cd embot-firmware
   mkdir build && cd build
   cmake -DPICO_SDK_PATH=<path to pico-sdk> ..
   make
   ```