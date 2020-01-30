#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


char auth[] = "token";

char ssid[] = "ssid";
char pass[] = "password";


#define DHTPIN 2       //NODE MCU PIN D4
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

boolean stateled=0;
boolean prevStateled=0;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  pinMode(13,OUTPUT); // NODEMCU PIN D7

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  
  timer.setInterval(300L, checkledstate);
}

BLYNK_WRITE(V0)
{
    if (param.asInt()){       
        digitalWrite(13, HIGH); Blynk.virtualWrite(V13,255); Blynk.notify("led ACTIVADO");
    } else {
        digitalWrite(13, LOW);  Blynk.virtualWrite(V13,0);
    }
}


void loop()
{
  if (Blynk.connected())
  {
    Blynk.run();
  }
   timer.run();  
}


BLYNK_CONNECTED()
{
  Blynk.syncAll();
  
}
void checkledstate()
{
  
  stateled=digitalRead(13);
  if (stateled!=prevStateled)
  {
    if (stateled==0) Blynk.virtualWrite(V13,0); Blynk.notify("led ACTIVADO");
    if (stateled==1) Blynk.virtualWrite(V13,255); 
  }
  prevStateled=stateled;
}
