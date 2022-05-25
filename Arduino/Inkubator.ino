// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLmuoIqwvV"
#define BLYNK_DEVICE_NAME           "Inkubator"
#define BLYNK_AUTH_TOKEN            "ATwHK3jngwasc1R7Y7gmHGNpm0dmbJsK"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DeltaDeltaDelta";
char pass[] = "labalabalabi";

BlynkTimer timer;

//DHT11
#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float Humid, TempC, TempF;

//SetPoint Value
float SetPointC = 38.9;

//Relay
const int RELAY = D1;

//Set Point Control
BLYNK_WRITE(V5)
{
  // Set incoming value from pin V5 to a variable
  float value = param.asFloat();

  // Update state
  Blynk.virtualWrite(V5, value);
  SetPointC = value;
}

//Fungsi Detik
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

//Fungsi ketika membaca nilai suhu dari LM35
void BacaTemperature()
{
  Humid = dht.readHumidity();
  TempC = dht.readTemperature();
  TempF = dht.readTemperature(true);
  Blynk.virtualWrite(V4, TempC);
  Blynk.virtualWrite(V3, Humid);
  //Serial.println(TempC);
  //Serial.println(analogValue);
  //Serial.println(millivolts);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY,OUTPUT);

  digitalWrite(RELAY,LOW);

  dht.begin();

  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V5, SetPointC);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  timer.setInterval(2000L, BacaTemperature);
}

void loop()
{
  Blynk.run();
  timer.run(); 
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  if(TempC > SetPointC)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(RELAY,HIGH);        //Turn Off Heater
    Blynk.virtualWrite(V6, 0);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELAY,LOW);       //Turn On Heater
    Blynk.virtualWrite(V6, 1);
  }
}
