// Include library for DHT22 Temperature sensor
#include <Adafruit_DHT.h>

#define DHTPIN  A2              // Define pin we are connected to - Analogue 2
#define DHTTYPE DHT22           // DHT 22  (AM2302)
#define RED     D2              // Red LED
#define GREEN   D3              // Green LED
#define FAN     D7              // Fan power

DHT dht(DHTPIN, DHTTYPE);       // Initialise DHT sensor

float humThreshold = 65 ;       // Default threshold for humidity to turn on/off fan   
float hum = 0;                  // Humidity variable - intialised to 0
float temp;                     // Temporary variable used to check if reading is NaN

void setup() {
    Serial.begin(9600);
    dht.begin();
    
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(FAN, OUTPUT);
}


void loop() {
    // Read humidity into temp variable
    temp = dht.getHumidity();

    // Check for an update to humidity threshold from web GUI
    Particle.subscribe("change_humidity", humChangeHandler);
    
    // If reading from sensor is NaN value, do nothing. Otherwise, publish it to thingspeak.com
    if (isnan(temp)){
    }
    else{
        hum = temp;
        Particle.publish("humidity", String(hum), PRIVATE);
    }    
    
    // If monitored humidity is above threshold, turn on red LED and fan, turn off Green LED
    // Otherwise, turn off fan and red LED, turn on green LED
    if(hum >= humThreshold){
        turnFanOn();
    } else {
        turnFanOff();
    }
    
    Serial.printlnf("Humidity threshold: %f ", humThreshold);
    // Delay 10 seconds
    delay(10000); 
}

// Handler for subscription to changes of humidity threshold - updates humThreshold variable with new value
void humChangeHandler(const char *event, const char *data) {
    float update = atof(data);
    
    if (update >= 0 && update <= 100){
        humThreshold = update;
    }
    
}

// Turns on fan and red LED, turn green LED off
void turnFanOn(){
    digitalWrite(GREEN, LOW);   
    digitalWrite(RED, HIGH);    
    digitalWrite(FAN, HIGH);
}

// Turn off fan and red LED, turn green LED on
void turnFanOff(){
    digitalWrite(GREEN, HIGH);   
    digitalWrite(RED, LOW);    
    digitalWrite(FAN, LOW);
}




























