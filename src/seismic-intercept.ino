#include <google-maps-device-locator.h>

SerialLogHandler logHandler;
GoogleMapsDeviceLocator locator;
int vibe = D2;
int ledIndicator = D7;

float mag;
float dis;
unsigned long previousMillis; // will store last time LED was updated
long interval;
int ledState = LOW;
bool dataRecieved;

void onLocationReceived(const char *event, const char *data);
void onErrorReceived(const char *event, const char *data);
void handler(const char *topic, const char *data);
void locationCallback(double lat, double lon, double accuracy);

int dataParse(String incomingData);
int getLatestLocation(String extra);
int vibrationParrtern(int length, int strength);

void setup()
{
    Particle.function("data", dataParse);
    Particle.function("location", getLatestLocation);
    Particle.subscribe("particle/device/ip", handler, MY_DEVICES);
    Particle.publish("particle/device/ip", PRIVATE);

    Serial.begin(9600);
    pinMode(ledIndicator, OUTPUT);
    Serial.write("hello");
    // Particle.subscribe("hook-response/get_location", onLocationReceived, MY_DEVICES);
    // Particle.subscribe("hook-error/get_location", onErrorReceived, MY_DEVICES);
    locator.publishLocation();

    digitalWrite(ledIndicator, HIGH);
    delay(2000);
    digitalWrite(ledIndicator, LOW);
}

void loop()
{
}

void handler(const char *topic, const char *data)
{
    Serial.println("received " + String(topic) + ": " + String(data));
}

void locationCallback(double lat, double lon, double accuracy)
{
    Serial.print("outgoing Data: ");
    Serial.println(lat);
    Particle.variable("lat", lat);
    Particle.variable("lon", lon);
    Particle.variable("accuracy", accuracy);
}

int getLatestLocation(String extra)
{
    // const char *wifiScan();
    locator.publishLocation();
    Serial.println("location requested");
    return 0;
}

int dataParse(String incomingData)
{
    Serial.write("incoming Data: ");
    Serial.println(incomingData);
    int keyIndex = incomingData.lastIndexOf('"');
    int substringIndex = incomingData.indexOf('n');
    int stringLength = incomingData.length();
    String distance = incomingData.substring(substringIndex + 1, stringLength);
    String magnitude = incomingData.substring(keyIndex + 1, substringIndex);
    float dis = distance.toFloat();
    float mag = magnitude.toFloat();

    Serial.printlnf("split incoming data: %s, %s ", magnitude, distance);
    Serial.print("slipt string : ");
    Serial.print(magnitude);
    Serial.print("  ---  ");
    Serial.println(distance);
    Serial.printlnf("vibration data: %f, %f ", mag, dis);

    interval = mag;
    unsigned long currentMillis = millis();
    Serial.printlnf("currentMillis: %f", currentMillis);
    // analogWrite(vibe, dis);
    delay(interval);
    digitalWrite(ledIndicator, dis);
    // analogWrite(vibe, 255);
    ledState = HIGH;
    Serial.write("vibe off");
    Serial.println();
    return String(magnitude + distance).toFloat();
}

void onLocationReceived(const char *event, const char *data)
{
    if ((data == NULL) || (strlen(data) == 0))
    {
        return;
    }

    Serial.println("got location back " + String(data));

    char one[32], two[32], three[32];
    float latitude, longitude, uncertainty;
    //sscanf(someString, "%f,%f,%f", &latitude, &longitude, &altitude);
    sscanf(data, "%[^,],%[^,],%[^,]", one, two, three);

    latitude = String(one).toFloat();
    longitude = String(two).toFloat();
    uncertainty = String(three).toFloat();

    if (latitude == 0)
    {
        Serial.println("nevermind");
        return;
    }

    //lets stop asking for location updates (so we don't burn a ton of data)
    //TODO: we could check cell tower id every few minutes, and request a location update
    // when that changes.

    Serial.println(String::format("Parsed into %f, %f, %f ", latitude, longitude, uncertainty));
    Serial.println(String::format("You're on a map! https://www.google.com/maps/place/%f,%f", latitude, longitude));

    String dataJson = String("{") + "\"lat\":" + String::format("%f", latitude) + ",\"lng\":" + String::format("%f", longitude) + ",\"unc\":" + String::format("%f", uncertainty) + "}";

    Particle.publish("current_location", dataJson, 60, PRIVATE);

    //gotLocation = true;
}