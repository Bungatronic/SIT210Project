// Include library for DHT22 temperature & humidity sensor
#include <Adafruit_DHT.h>

#define DHTPIN  A2              // DHT22 sensor pin - Analogue 2
#define DHTTYPE DHT22           // DHT 22  (AM2302)
#define RED     D2              // Red LED pin - Digital 2
#define GREEN   D3              // Green LED pin - Digital 3
#define FAN     D7              // Fan pin - Digital 7 (output to NPN transistor)

DHT dht(DHTPIN, DHTTYPE);       // Initialise DHT sensor

float humThreshold = 65 ;       // Default threshold for humidity to turn on/off fan   
float hum = 0;                  // Monitored humidity variable - intialised to 0
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
    
    publishHumidity();
    fanControl();

    // Delay 10 seconds
    delay(10000); 
}

// Handler for subscription to changes of humidity threshold 
// updates humThreshold variable with new value
void humChangeHandler(const char *event, const char *data) {
    float update = atof(data); 
    
    if (update >= 0 && update <= 100){ 
        humThreshold = update;
    }
}

// Checks if reading from sensor is NaN value to publish it to thingspeak.com
void publishHumidity(){
    if (isnan(temp)){
    }
    else{
        hum = temp;
        Particle.publish("humidity", String(hum), PRIVATE);
    }  
}

// Check whether to turn the fan on or off depending on humidity
void fanControl(){
    if(hum >= humThreshold){
        turnFanOn();
    } else {
        turnFanOff();
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