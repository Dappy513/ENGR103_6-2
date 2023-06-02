#include <AsyncDelay.h>
#include <Adafruit_CircuitPlayground.h>

int value;
int volume;
int capacitance;
int brightness;
int temp;
bool switchVal = 0;
AsyncDelay delay_2s;

volatile bool switchFlag = 0;
int switchPin = 7;

void setup()
{
  CircuitPlayground.begin();
  Serial.begin(9600);
  while(!Serial); // Pause program till serial opens
  Serial.println("Starting sensors");
  attachInterrupt(digitalPinToInterrupt(switchPin), onOff, CHANGE);
  delay_2s.start(2000, AsyncDelay::MILLIS);
  //pinMode(switchPin, INPUT_PULLUP);
  switchVal = digitalRead(switchPin);
}

void loop()
{
  //Add a debounce here for the switch
  //Serial.println(switchFlag);
  if(switchFlag)
  {
    Serial.println("Interrupt");
    delay(5);
    switchVal = digitalRead(switchPin);
    switchFlag = 0;
  }

  if(delay_2s.isExpired()) // this is our data collection every 2 seconds
  {
    Serial.println(switchVal);
    int temp = CircuitPlayground.temperatureF(); //measure sensors
    int brightness = CircuitPlayground.lightSensor(); // measure sensors
    int capacitance = CircuitPlayground.readCap(6);
    int maptemp = map(temp, 75, 95, 0, 9);
    int mapcapacitance = map(capacitance, 0, 1016, 0, 255);
    int mapbright = map(brightness, 0, 600, 440, 1760);
    for(int i=0; i < 10; i++)
    {
      CircuitPlayground.setPixelColor(i, 0, 0, 0);
    }
    for(int i=0; i < maptemp; i++)
    {
      CircuitPlayground.setPixelColor(i, 255-mapcapacitance, mapcapacitance, 0);
    }

    if(!switchVal)
    {
      // I need the sound queues
      // this section will turn off at night
      Serial.println(mapbright);
      CircuitPlayground.playTone(mapbright, 100);
    }

     delay_2s.repeat();
  }
}

void onOff()
{
  switchFlag = 1;
}