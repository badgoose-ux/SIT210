#define waterThreshold   700
#define inputPin         A0
#define emailInterval    1800000

int thresholdMet = false;
unsigned long lastTextTime = 0;

void setup() {
    pinMode(inputPin, INPUT);
}

void loop() {
    if (analogRead(inputPin) < waterThreshold) {
        thresholdMet = true;
        
        unsigned long now = millis();
        
        if (now - lastTextTime >= emailInterval) {
            Particle.publish("Your plant needs watering!");
        }
    }
    
    else {
        thresholdMet = false;
    }
}