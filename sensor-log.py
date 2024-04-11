import serial
import platform

if platform.system() == 'Darwin':
    serial_device = "/dev/tty.usbmodem101"
else:
    serial_device = "/dev/ttyACM0"

with serial.Serial(serial_device, 115200) as ser:
    try:
        while True:
            print(*ser.readline().decode().split())
    except KeyboardInterrupt:
        print("Exit")
