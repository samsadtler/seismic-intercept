PRODUCT_ID(12296);
PRODUCT_VERSION(1);

int valve = D2;
int pump = D5;
int pump2 = D3;
int ledIndicator = D7;
int pumpState = LOW;
bool processing = false;

unsigned long startMillis = 0;
float interval = 0;
int ledState = LOW;
int count = 20;
float magintudes[20] = {0};

int dataParse(String incomingData);

void setup()
{
    Particle.function("data", dataParse); //initialize function to recieve data

    Serial.begin(9600);
    pinMode(ledIndicator, OUTPUT); // led on particle for tests only
    pinMode(pump, OUTPUT);         // pump 1
    pinMode(pump2, OUTPUT);        // pump 2
    pinMode(valve, OUTPUT);        // valve 1 && 2

    digitalWrite(valve, HIGH); // close valves
}
void loop()
{
    if (count < 20)
    {
        if (startMillis == 0 && processing != true)
        {
            startMillis = millis();
        }

        if ((millis() - startMillis) <= magintudes[count])
        {
            pumpState = HIGH;
            digitalWrite(pump, pumpState);
            digitalWrite(pump2, pumpState);
            digitalWrite(ledIndicator, pumpState);
        }
        else
        {

            processing = true;
            startMillis = 0;
            pumpState = LOW;

            digitalWrite(pump, pumpState);
            digitalWrite(pump2, pumpState);
            digitalWrite(ledIndicator, pumpState);

            processing = removeDisplay(magintudes, count); 
        }
    };
}

// void dataDisplay(unsigned long interval, unsigned long startMillis)
// {

//     if ((millis() - startMillis) <= interval)
//     {
//         pumpState = HIGH;
//     }
//     else
//     {
//         processing = true;
//         startMillis = 0;
//         pumpState = LOW;
//         count = arrayRemover(magintudes, count);
//         processing = removeDisplay(magintudes[count]);
//         Serial.printlnf("count after interval %i ", count);
//     }
// }

// Deflate / open valve. the rate should be controlled by manual valve or tube length
bool removeDisplay(float array[], int counter) 
{
    float interval = array[counter];
    unsigned long start = millis();
    Serial.println("release start");
    // interval is .5 X the inflation time
    while (interval * .5 >= millis() - start)
    {
        digitalWrite(valve, LOW);
    };

    digitalWrite(valve, HIGH);
    Serial.println("release done");
    count = arrayRemover(array, counter);
    return false;
}

int dataParse(String incomingData)
{
    int keyIndex = incomingData.lastIndexOf('"');
    int substringIndex = incomingData.indexOf('n');
    int stringLength = incomingData.length();
    String duration = incomingData.substring(substringIndex + 1, stringLength);
    String magnitude = incomingData.substring(keyIndex + 1, substringIndex);
    float dur = duration.toFloat();
    float mag = magnitude.toFloat();

    Serial.printlnf("NEW mag and duration data data: %f, %f ", mag, dur);

    count = arrayAdder(mag, magintudes, count);

    Serial.printlnf("New Count %i", count);

    return String(magnitude + "-" + duration).toFloat();
}

int arrayAdder(float value, float array[], int counter)
{
    --counter;
    array[counter] = value;
    return counter;
}

int arrayRemover(float array[], int counter)
{
    Serial.printlnf("Mag value to remove from array buffer %f ", array[counter]);

    // removeDisplay(array[counter]);
    array[counter] = 0;
    return ++counter;
}