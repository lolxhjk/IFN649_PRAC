import paho.mqtt.publish as publish

publish.single("LEDPIN","LED_ON", hostname="server-IP-address")
print("Done")
