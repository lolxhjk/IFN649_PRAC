import paho.mqtt.client as mqtt 

def on_connect(client,userdata,flags,rc):
    print("Connected to MQTT")
    print("Connect returned result:"+str(rc))
    client.subscribe("LEDPIN")

def on_message(client,userdata,msg):
    print(msg.topic+""+str(msg.payload))

client=mqtt.Client()
client.on_connect=on_connect
client.on_message=on_message
client.connect("server-ip-address",1883,60)
client.loop_forever()
