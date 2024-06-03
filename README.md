# A USB-HID joystick

This project implements a USB-HID device, a joystick using a rpi-pico.

# Build

    git clone https://github.com/ad-chaos/JoyShtick/
    cd JoyShtick
    make

All the build artefacts will be available in the `build/` directory, of interest should be the uf2 file
if you have picotool installed, you can run `make flash` and load the uf2 file to the board.

# TODO
An HID gamepad can have (according to the provided descriptor in tinyusb) two joysticks, a hat and a lot of buttons.
Now even though the rp2040 has got a 4 channel adc, only 2 are available for GPIO, it would be nice to buy a separate 4 channel adc and use two joysticks, I have the ads115, need to get around to it.
