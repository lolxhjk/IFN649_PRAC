import paho.mqtt.client as mqtt
import serial
import time
import csv
import os

#initialize BT connection
try:
	ser=serial.Serial("/dev/rfcomm0",9600,timeout=1)
	print("Connected to Bluetooth module.")
except serial.SerialException as e:
	print(f"Error:{e}")
	exit(1)

#initialize csv file
csv_file="sensor_data.csv"
if not os.path.exists(csv_file):
	
	with open(csv_file,mode='w',newline='') as file:
		writer=csv.writer(file)
		writer.writerow(["Timestamp","Humidity(%)","Temperature(C)"])


def on_connect(client,userdata,flags,rc):
	print("Connected to MQTT")
	print("Connect returned result:"+str(rc))
	client.subscribe("Humidifier")
	
def on_message(client,userdata,msg):
	command = msg.payload.decode().strip()
	print(f"Received MQTT message: {command}")
	ser.write((command+'\n').encode())
	
def read_data():
    """Read data from Bluetooth and save to CSV."""
    data = ser.readline().decode().strip()
    if data:
        print(f"Received: {data}")

        # Parse the received data (expecting "Humidity: X%, Temp: Y C")
        try:
            # Adjust splitting logic for your specific data format
            # Example: 'Humidity: 69.00% Temperature: 20.50C 68.90F Heat index: 20.41C 68.73F'
            parts = data.split(" ")  # Split by space
            print(f"Split data: {parts}")  # Debug the split data

            # Extract the specific humidity and temperature values
            humidity = float(parts[1].replace("%", ""))  # Extract '69.00'
            temperature = float(parts[4].replace("C", ""))  # Extract '20.50'

            # Get current timestamp
            timestamp = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())

            # Save the data to the CSV file
            with open(csv_file, mode='a', newline='') as file:
                writer = csv.writer(file)
                writer.writerow([timestamp, humidity, temperature])

            print(f"Data saved: {timestamp}, {humidity}%, {temperature}C")
        except (IndexError, ValueError) as e:
            print(f"Error parsing data: {e}")
	
client=mqtt.Client()
client.on_connect=on_connect
client.on_message=on_message

client.connect("3.106.224.216",1883,60)
client.loop_start()

try:
	while True:
		read_data()
		time.sleep(20)
except KeyboardInterrupt:
	print("Exiting...")
finally:
	ser.close()
	client.loop_stop()
	client.disconnect()
	
	
	
	
	
 
