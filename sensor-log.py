import serial

with serial.Serial("/dev/ttyACM0", 115200) as ser, open("dump.csv", 'w') as csv:
    csv.write("x,y\n")
    try:
        while True:
            x, y = ser.readline().decode().split()
            print(x, y)
            csv.write(f"{x},{y}\n")
    except KeyboardInterrupt:
        print("Bye!")
