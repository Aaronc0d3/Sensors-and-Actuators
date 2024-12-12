#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 4
int count = 0;
float lastTemperature = -1000;  // Initialize with a value that is unlikely to be a real temperature

void setup(){
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  Serial.println("Temperature (C),Temperature (K),Temperature(F), Seconds");
}

void loop(){
  int chk;
  float Kel = 273.15 + DHT.temperature;
  int Far = DHT.temperature * 1.8 + 32;

  chk = DHT.read(DHT11_PIN);    // READ DATA
  switch (chk){
    case DHTLIB_OK:  
                
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }

  // Only print the temperature if it has changed
  if (DHT.temperature != lastTemperature) {
    Serial.print(DHT.temperature, 1);
    Serial.print(",\t");
    Serial.print("\t");
    Serial.print(Kel, 1);
    Serial.print(",\t");
    Serial.print("\t");
    Serial.print(Far, 1);
    Serial.print("\t");
    Serial.print("\t");
    Serial.println(count);
    lastTemperature = DHT.temperature;  // Update last recorded temperature
  }

  delay(1000);
  count++;
}
