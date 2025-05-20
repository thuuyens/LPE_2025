import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

print("Connected to", ser.name)
print("Waiting for messages...")

try:
    while True:
        line = ser.readline()
        
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
