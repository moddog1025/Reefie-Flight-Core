import serial
import serial.tools.list_ports
import time
import csv
import threading
import os

def find_reefie_port():
    """
    Scan available COM ports and attempt to connect to Reefie on COM5.
    If COM5 is found but doesn't respond, it keeps retrying until a connection is made.
    """
    while True:
        ports = list(serial.tools.list_ports.comports())
        port_com5 = None
        
        for port_info in ports:
            port = port_info.device
            if port == "COM5":  # Prioritize COM5
                port_com5 = port
                break  # Stop searching once COM5 is found
        
        if port_com5:
            print(f"Trying COM5...")
            try:
                ser = serial.Serial(port_com5, 9600, timeout=1)
                ser.reset_input_buffer()
                
                # Send handshake command
                ser.write(b'x00x00\n')
                time.sleep(0.5)
                response = ser.readline().decode('utf-8', errors="replace").strip()
                
                print(f"Response from COM5: {response}")
                if response == "Connected":
                    ser.close()
                    print(f"Found Reefie device on COM5!")
                    return port_com5
                else:
                    print("No response, retrying in 2 seconds...")
                    ser.close()
                    time.sleep(2)  # Retry delay
            except Exception as e:
                print(f"Error on COM5: {e}. Retrying in 2 seconds...")
                time.sleep(2)
        else:
            print("COM5 not found. Scanning for ports...")
            time.sleep(2)  # Keep scanning until COM5 appears

def read_serial_output(ser):
    """
    Continuously reads from the serial monitor and prints Reefie's responses.
    Runs in a separate thread to avoid blocking data transmission.
    """
    while ser.is_open:
        try:
            if ser.in_waiting > 0:
                response = ser.readline().decode('utf-8', errors="replace").strip()
                if response:  # Only print if there's actual data
                    print(f"Reefie: {response}")
        except Exception as e:
            print(f"Error reading from Reefie: {e}")
            break

def send_csv_data(port, csv_filename):
    """
    Sends data from a CSV file to Reefie at 20Hz.
    Each message follows the format: x05x01 ["altitude value"]
    """
    if not os.path.exists(csv_filename):
        print(f"Error: CSV file not found at '{csv_filename}'")
        return

    try:
        ser = serial.Serial(port, 9600, timeout=1)
        time.sleep(1)  # Allow connection to establish

        # Start a thread to read Reefie's serial output in real-time
        serial_thread = threading.Thread(target=read_serial_output, args=(ser,), daemon=True)
        serial_thread.start()

        # Send preparation command (x05x00)
        print("Sending prep command (x05x00)...")
        ser.write(b'x05x00\n')
        response = ser.readline().decode('utf-8', errors="replace").strip()
        print(response)

        # Read CSV and send values at 20Hz
        try:
            with open(csv_filename, 'r') as csvfile:
                csv_reader = csv.reader(csvfile)
                for row in csv_reader:
                    if not row:
                        continue  # Skip empty lines
                    altitude_value = row[0].strip()  # Extract altitude (assuming 1st column)
                    command_string = f'x05x01 ["{altitude_value}"]\n'  # Format with square brackets
                    ser.write(command_string.encode('utf-8'))

                    time.sleep(1 / 20)  # 20Hz transmission rate
        except Exception as e:
            print(f"Error reading CSV file: {e}")

        print("Finished sending data.")
        ser.close()

    except Exception as e:
        print(f"Error communicating with Reefie: {e}")

# Main execution
csv_filename = r"C:\Users\Cooper\dev\Projects\Current\Reefie Development\Reefie Flight Desk\.venv\altitude_data.csv"
port = find_reefie_port()

if port:
    send_csv_data(port, csv_filename)
else:
    print("Reefie device not found.")
