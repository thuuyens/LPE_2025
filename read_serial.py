import serial
import time

# Open the serial port - adjust the port name as needed
# For Windows, this might be 'COM3', 'COM4', etc.
# For Linux, it might be '/dev/ttyUSB0'
# For Mac, it might be '/dev/cu.usbserial-XXXX'
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

print("Connected to", ser.name)
print("Waiting for messages from ESP8266...")

try:
    while True:
        # Read a line from the serial port
        line = ser.readline()
        
        # If we received data, decode and print it
        if line:
            decoded_line = line.decode('utf-8', errors='replace').strip()
            print(decoded_line)

        time.sleep(0.1)
            
except KeyboardInterrupt:
    print("\nScript terminated by user")
except Exception as e:
    print("Error:", e)
finally:
    # Always close the port when done
    ser.close()
    print("Serial port closed")
