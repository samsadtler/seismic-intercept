
SerialLogHandler logHandler;
int pump = D2;
int ledIndicator = D7;

float mag;
float dis;
unsigned long previousMillis; // will store last time LED was updated
long interval;
int ledState = LOW;
bool dataRecieved;

void onErrorReceived(const char *event, const char *data);
void handler(const char *topic, const char *data);

int dataParse(String incomingData);
int vibrationParrtern(int length, int strength);

void setup()
{
    Particle.function("data", dataParse);

    Serial.begin(9600);
    pinMode(ledIndicator, OUTPUT);
    pinMode(pump, OUTPUT);
    Serial.write("hello");
}

void loop()
{
}

void handler(const char *topic, const char *data)
{
    Serial.println("received " + String(topic) + ": " + String(data));
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
    digitalWrite(pump, HIGH);
    digitalWrite(ledIndicator, HIGH);
    delay(mag * 10);
    digitalWrite(pump, LOW);
    digitalWrite(ledIndicator, LOW);

    Serial.write("vibe off");
    Serial.println();
    return String(magnitude + distance).toFloat();
}
