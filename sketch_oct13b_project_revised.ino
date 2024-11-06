#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//Reads the humidity and temperature from the DHT11 sensor.
//Turns on the humidifier (via the relay) if the humidity is below 70%.
//Turns off the humidifier if the humidity is 70% or above.
//Optionally prints the readings to the serial monitor for debugging.

#define DHTPIN 21 //digital pin connected to the DHT sensor
#define DHTTYPE DHT11
#define LEDPIN 11
#define RELAYPIN 13
LiquidCrystal_I2C lcd(0x27,20,4);//Initialize I2C LCD
DHT dht(DHTPIN,DHTTYPE);//Initialize DHT sensor

bool manualOverride=false; 
bool relayState = LOW; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//for primary serial comm
  Serial1.begin(9600);//for BT comm
  
  pinMode(DHTPIN,INPUT);//set up DHT sensor
  dht.begin();

  pinMode(LEDPIN,OUTPUT);//set up LED pin  
  pinMode(RELAYPIN, OUTPUT);
  
  lcd.init();//set up I2C
  lcd.clear();         
  lcd.backlight();

//display startup message
  lcd.setCursor(0,0);//first row
  lcd.print("Smart Humidifier");
  lcd.setCursor(0,2);
  lcd.print("Initializing...");
  delay(2000);//wait for 2 seconds
  lcd.clear();
}

void loop() {

  //remote control via BT
    if (Serial1.available()>0){
      String str=Serial1.readString().substring(0);
      str.trim();

      Serial.println("Received command:["+ str+"]");

      if (str =="on"){
        manualOverride=true;
        relayState=HIGH;
        updateRelayAndLED();
        //digitalWrite(RELAYPIN,HIGH);
        //digitalWrite(LEDPIN,HIGH);
        Serial.println("MANUAL POWER ON");
      }else if(str =="off"){
        manualOverride = true;  // Enable manual mode
        relayState = LOW;
        updateRelayAndLED();
        //digitalWrite(RELAYPIN,LOW);
        //digitalWrite(LEDPIN,LOW);
        Serial.println("MANUAL POWER OFF");
      }else if(str=="auto"){
        manualOverride=false;
        Serial.println("Switched to Auto Mode");
      }else if (str=="read"){
        sendSensorReadings();
      }
    }

    if (!manualOverride) {      
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        // Automatic control logic                                
        if (humidity < 70) {
            relayState = HIGH;
            lcd.setCursor(0, 2);
            lcd.print("Humidifier: ON ");
        } else {
            relayState = LOW;
            lcd.setCursor(0, 2);
            lcd.print("Humidifier: OFF");
        }
        updateRelayAndLED();

        // Display sensor readings on the LCD
        lcd.setCursor(0, 0);
        lcd.print("Humidity: ");
        lcd.print(humidity);
        lcd.print("%");

        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print(temperature);
        lcd.print("C");

        // Print readings to serial monitor
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%, Temp: ");
        Serial.print(temperature);
        Serial.println("C");
    }

    delay(2000);  // Wait 2 seconds before the next reading
}
void updateRelayAndLED() {
    // Update the relay and LED states
    digitalWrite(RELAYPIN, relayState);
    digitalWrite(LEDPIN, relayState);
}


void sendSensorReadings(){
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);
   //print readings to BT serial
    Serial1.print(F(" Humidity: "));
    Serial1.print(h);
    Serial1.print(F("%  Temperature: "));
    Serial1.print(t);
    Serial1.print(F("C "));
    Serial1.print(f);
    Serial1.print(F("F  Heat index: "));
    Serial1.print(hic);
    Serial1.print(F("C "));
    Serial1.print(hif);
    Serial1.println(F("F"));
}
