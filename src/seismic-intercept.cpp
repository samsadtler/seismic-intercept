/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/Sam/Development/seismic-intercept/src/seismic-intercept.ino"
void setup();
void dataDisplay(int interval, int mag);
#line 1 "/Users/Sam/Development/seismic-intercept/src/seismic-intercept.ino"
int valve = D2;
int pump = D5;
int ledIndicator = D7;

long interval;
int ledState = LOW;

int dataParse(String incomingData);

void setup()
{
    Particle.function("data", dataParse);

    Serial.begin(9600);
    pinMode(ledIndicator, OUTPUT);
    pinMode(pump, OUTPUT);
    pinMode(valve, OUTPUT);
}

int dataParse(String incomingData)
{
    Serial.write("incoming Data: ");
    Serial.println(incomingData);
    int keyIndex = incomingData.lastIndexOf('"');
    int substringIndex = incomingData.indexOf('n');
    int stringLength = incomingData.length();
    String duration = incomingData.substring(substringIndex + 1, stringLength);
    String magnitude = incomingData.substring(keyIndex + 1, substringIndex);
    float dur = duration.toFloat();
    float mag = magnitude.toFloat();

    Serial.print("split incoming data: \n");
    Serial.println(magnitude);
    Serial.print("  ---  ");
    Serial.println(duration);
    Serial.printlnf("mag and duration data data: %f, %f ", mag, dur);

    dataDisplay(dur, mag);

    return String(magnitude + duration).toFloat();
}

void dataDisplay(int interval, int mag)
{
    unsigned long startMillis = millis();
    digitalWrite(valve, HIGH);
    digitalWrite(pump, HIGH);
    digitalWrite(ledIndicator, HIGH);
    bool valveOpen = false;

    while (interval >= (millis() - startMillis))
    {
        if (mag < (millis() - startMillis))
        {
            digitalWrite(valve, LOW);
            if (!valveOpen)
            {
                Serial.println("valve open");
                valveOpen = true;
            }
        }
    }

    digitalWrite(valve, LOW);
    digitalWrite(pump, LOW);
    digitalWrite(ledIndicator, LOW);
    Serial.println("pump off and valve closed");
}
