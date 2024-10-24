import paho.mqtt.client as mqtt

Broker="3.106.224.216"
Topic="Humidifier"

client=mqtt.Client()

def send_command(command):
	try:
		client.connect(Broker,1883,60)
		client.publish(Topic,command)
		print(f"Sent command:{command}")
	except Exception as e:
		print(f"Error sending command: {e}")
	finally:	
		client.disconnect()
	
while True:
	command=input("Enter command (on/off/auto/read):").strip().lower()
	if command in ["on","off","auto","read"]:
		send_command(command)
	else:
		print("Invalid command.Please enter 'on','off','auto','read'.")
