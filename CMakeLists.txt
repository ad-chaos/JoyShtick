cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)
project(joyshtick)

pico_sdk_init()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(joyshtick
    src/main.c
    src/analog_stick.c
    src/usb_descriptors.c
)
target_include_directories(joyshtick PUBLIC include/common/ include/gamepad/)
target_link_libraries(joyshtick PUBLIC
    pico_stdlib
    pico_unique_id
    hardware_adc
    tinyusb_device
    tinyusb_board
)
pico_add_extra_outputs(joyshtick)

add_executable(jreader read_joystick.c src/analog_stick.c)
target_include_directories(jreader PUBLIC include/common/)
pico_enable_stdio_usb(jreader 1)
pico_enable_stdio_uart(jreader 0)
target_link_libraries(jreader PUBLIC
    hardware_adc
    pico_stdlib
)
pico_add_extra_outputs(jreader)
