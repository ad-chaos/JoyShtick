cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)
project(joyshtick)

pico_sdk_init()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(joyshtick src/main.c src/usb_descriptors.c)
add_executable(jreader read_joystick.c)

pico_enable_stdio_usb(jreader 1)
pico_enable_stdio_uart(jreader 0)

target_include_directories(joyshtick PUBLIC include/)

target_link_libraries(joyshtick PUBLIC pico_stdlib pico_unique_id tinyusb_device tinyusb_board)
target_link_libraries(jreader PUBLIC pico_stdlib hardware_adc)

pico_add_extra_outputs(joyshtick)
pico_add_extra_outputs(jreader)
