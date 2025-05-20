// #include <SoftwareSerial.h>
#include <LD2450.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Define pins for SoftwareSerial connection to PC
#define DEBUG_TX D5  // Connect to RX of USB-to-Serial adapter
#define DEBUG_RX D6  // Connect to TX of USB-to-Serial adapter (optional)
#define MAX_DISTANCE 4000
#define MIN_DISTANCE 500
#define MAX_BRIGHTNESS 5
#define NUM_PIXELS 15
#define PIN 0

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
// Create a SoftwareSerial instance for PC debugging
// SoftwareSerial debugSerial(DEBUG_RX, DEBUG_TX);
LD2450 ld2450;
int lights_c = 0;

// Function to light up pixels with a green-yellow-red gradient at reduced brightness
void lightGradient(int numToLight) {
  // Clear all pixels first
  pixels.clear();
  
  // Ensure numToLight doesn't exceed the total number of pixels
  if (numToLight > NUM_PIXELS) {
    numToLight = NUM_PIXELS;
  }
  
  // Light up the specified number of pixels with gradient colors
  for(int i = 0; i < numToLight; i++) {
    // Calculate position in the gradient (0.0 to 1.0)
    float position = (float)i / (NUM_PIXELS - 1);
    
    // RGB values for the gradient
    uint8_t red, green, blue = 0;
    
    // Create gradient: green (0,MAX_BRIGHTNESS,0) -> yellow (MAX_BRIGHTNESS,MAX_BRIGHTNESS,0) -> red (MAX_BRIGHTNESS,0,0)
    if (position < 0.5) {
      // First half: green to yellow
      red = MAX_BRIGHTNESS * (position * 2);
      green = MAX_BRIGHTNESS;
    } else {
      // Second half: yellow to red
      red = MAX_BRIGHTNESS;
      green = MAX_BRIGHTNESS * (1.0 - (position - 0.5) * 2);
    }
    
    // Set pixel color
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  
  // Update the pixels
  pixels.show();
}

// Convert distance from mms to number of lights that should light up
int distanceToLights(uint16_t distance) {
  
  float ratio;

  if (distance > MAX_DISTANCE) {
    ratio = 1;
  } else if (distance < MIN_DISTANCE) {
    ratio = 0;
  } else {
    ratio = (float) (distance - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE);
  }

  return round((1 - ratio) * NUM_PIXELS);
}

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();
  // SETUP SENSOR USING HARDWARE SERIAL INTERFACE 1
  Serial.begin(57600);
  ld2450.begin(Serial, true);
  // ld2450.setNumberOfTargets(3);
  
  // Initialize SoftwareSerial for debug output
  // debugSerial.begin(9600);
  
  // Initial message
  // debugSerial.println("Init completed...");
}


void loop() {
  const int found_targets = ld2450.read();
  if (found_targets > 0){
    const LD2450::RadarTarget valid_target = ld2450.getTarget(0);
    // debugSerial.print("DISTANCE TO TARGET: ");
    // debugSerial.println(valid_target.distance); // see struct RadarTarget

    lights_c = distanceToLights(valid_target.distance);
    lightGradient(lights_c);
    // debugSerial.print("LIGHT_COUNT: ");
    // debugSerial.println(lights_c); // see struct RadarTarget
  }

  // delay(10);
}