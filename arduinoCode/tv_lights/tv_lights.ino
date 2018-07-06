#include "FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2811

//#define COLOR_ORDER GRB
//#define NUM_LEDS    8

#define COLOR_ORDER RGB
#define NUM_LEDS    50

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          25
#define FRAMES_PER_SECOND  120

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);
  //Serial.println("Starting...");
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {all_off, all_on, all_purple, all_main_color, rainbow_solid_with_glitter, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

int incomingByte = 0;
int mode = 0;
int item = 0;
int value = 0;

uint8_t mainColorHue = 185;

int mainAddGlitter = 1;
uint8_t mainGlitterHue = 0;
fract8 mainGlitterPercent = 80;


void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    mode = Serial.read();
    if (mode=='?') {
      Serial.print('r');
    }else if (mode=='s'){
      item = Serial.parseInt();
      value = Serial.parseInt();
      if (item == 1) { //1 is presets
        gCurrentPatternNumber = value;
        Serial.print(1);
      }else if (item == 2) { //2 is mainColorHue
        mainColorHue = value;
      }else if (item == 3) { //3 is mainAddGlitter
        mainAddGlitter = value;
      }else if (item == 4) { //4 is mainGlitterHue
        mainGlitterHue = value;
      }else if (item == 5) { //5 is mainGlitterPercent
        mainGlitterPercent = value;
      }else if (item == 6) { //6 is brightness
        FastLED.setBrightness(value);
      }
    }
  }

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 40 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  //gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void addGlitterSpecial( fract8 chanceOfGlitter, uint8_t GlitterHue) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] = CHSV(GlitterHue,255,255);
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void all_on() {
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(gHue, 255, 255);
  }
}

void all_off() {
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(0, 0, 0);
  }
}

void all_purple() {
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(185, 255, 255);
  }
  addGlitterSpecial(80, 0);
}

void all_main_color() {
  fill_solid(leds, NUM_LEDS, CHSV(mainColorHue, 255, 255));
  if (mainAddGlitter==1) {
    addGlitterSpecial(mainGlitterPercent, mainGlitterHue);
  }
}

void rainbow_solid_with_glitter() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, mainColorHue, 256/NUM_LEDS);
  if( random8() < mainGlitterPercent) {
    leds[ random16(NUM_LEDS) ] = CRGB::White;
  }
}
