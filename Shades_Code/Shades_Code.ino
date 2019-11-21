//This code is placed under the MIT license
//Copyright (c) 2019 Albert Barber
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

//code intended to run on esp8266, Wemos D1 mini
//requires lastest version of adafruit neopixel library (use the library manager)
#include <PixelStrip.h>
#include "segmentDefs.h"

//pin connections
#define PIXEL_PIN      D8
#define BUTTON_1       D6
#define BUTTON_2       D7
#define BUTTON_3       D4
#define BAT_KEEP_ALIVE D5
//below vars are placeholders for possible bluetooth and mic board connections
#define MIC_IN         A0
#define RX_BT          D1
#define TX_BT          D2

//effects control vars
byte effectIndex = 0; //number of effect that's currently active
const byte numEffects = 39; //number of current effects - 1
boolean effectRota = true; //effects rotation on / off flag
boolean effectsStop = false; //stop all effects flag
boolean direct = true; //use for setting direction of effects
boolean breakCurrentEffect = false; //flag for breaking out of effects that use multiple sub effects

//macro for implementing break for effects with multiple sub effects
#define breakEffectCheck() ({ \
    if( (breakCurrentEffect) ){ \
      (breakCurrentEffect) = false; \
      break; \
    } \
  })

//brightness vars
byte brightnessIndex = 1; //initial brightness, index of brightnessLevels array
//brightness levels array, max is 255, but 100 should be bright enough for amost all cases
//!!WARNING brightness is directly tied to power consumption, the max current per led is 60ma, this is for white at 255 brightness
//if you actually run all the leds at max, the glasses will draw 4.75 amps, this will overwhelm most power banks, and is beyond the rating of the jst connectors
const byte brightnessLevels[] = { 10, 20, 30, 50, 70, 100 };
const byte numBrightnessLevels = SIZE( brightnessLevels );

//time settings for battery keep alive
//if using something other than an ESP8266, you'll need to adjust the timer code to work with whatever timers your chip has
//power banks shutdown unless a minimum current is pulled within a certain time period
//The BAT_KEEP_ALIVE pin is connected to a NPN transistor, which, when triggered, allows about 200ma to pass through a resistor
//the pin is triggered by an ISR based on the esp8266's timer1 internal clock
//the below variables set the times for triggering the transistor, adjust them as need for your power bank
//each tick is 0.2us, so ticks Needed = ( desired Time (in sec) ) / .0000002 (this depends on the clock freq used during ISR setup)
const uint32_t BAT_KEEP_ALIVE_FREQ = 25000000; //5 sec; how often the keep alive is activated
const uint32_t BAT_KEEP_ALIVE_INTERV = 1000000; //0.2 sec; how long the keep alive stays active for
const uint32_t BAT_KEEP_ALIVE_START_DELAY = 1000000; //0.2 sec; inital delay for starting the keep alive cycle
//(more than enough time for the setup function to finish)
//Strip definitions
const uint16_t stripLength = 76;
const uint8_t stripType = NEO_GRB + NEO_KHZ800;
PixelStrip strip = PixelStrip(stripLength, PIXEL_PIN, stripType);

//Define some colors we'll use frequently
const uint32_t white =    strip.Color(255, 255, 255);
const uint32_t UCLAGold = strip.Color(254, 187, 54);
const uint32_t UCLABlue = strip.Color(83, 104, 149);
const uint32_t off =      strip.Color( 0, 0, 0 );
const uint32_t red =      strip.Color(255, 0, 0);
const uint32_t orange =   strip.Color(255, 43, 0);
const uint32_t ltOrange = strip.Color(255, 143, 0);
const uint32_t yellow =   strip.Color(255, 255, 0);
const uint32_t green =    strip.Color(0, 128, 0);
const uint32_t blue =     strip.Color(0, 0, 255);
const uint32_t indigo =   strip.Color( 75, 0, 130);
const uint32_t violet =   strip.Color(238, 130, 238);
const uint32_t purple =   strip.Color(123, 7, 197);
const uint32_t pink =     strip.Color(225, 0, 127);

const uint32_t rainbowdashBody = strip.Color(130, 185, 226); //178,231,254,
const uint32_t rainbowdashHair1 = strip.Color(110, 46, 145); //purple
const uint32_t rainbowdashHair2 = strip.Color(54, 174, 218); //teal
const uint32_t rainbowdashHair3 = strip.Color(120, 212, 96); //green
const uint32_t rainbowdashHair4 = strip.Color(255, 254, 188); //yellow
const uint32_t rainbowdashHair5 = strip.Color(236, 116, 70); //orange
const uint32_t rainbowdashHair6 = strip.Color(229, 61, 84); //pink red

//define pallet array, contains 32bit representations of all colors used in patterns
uint32_t pallet[9] = { off, white, UCLAGold, UCLABlue, blue, yellow, red, green, purple };
//                   { -0-, --1--, ---2----, ----3---, -4--, ---5--, -6-, --7--, --8-- }

//pallet to match typical fairy light colors
uint32_t christmasPallet[5] = { red, blue, green, yellow, purple };

uint32_t rainbowdashPallet[] = { rainbowdashBody, rainbowdashHair1 , rainbowdashHair2, rainbowdashHair3, rainbowdashHair4, rainbowdashHair5, rainbowdashHair6 };
byte rainbowdashPattern2[] = {  6, 1, 2, 5, 4, 3, 0 };

//byte wavepattern[]  = { 6, 1 };
//byte wavepattern2[] = { 5, 4 };
//byte wavepattern3[] = { 6, 7 };
//byte wavepattern4[] = { 8, 7 };
//byte christmasWavePattern[5] = { 0, 1, 2, 3, 4};

//below are spin patterns
//these are for use with the colorSpin function, which draws a pattern across segments (using the longest segment as the pattern basis)
//and then moves it along the longest segment
//you can make your own, their structure is the following
//(note that you'll have to understand how segments work in my library, you can find notes on segments in segmentSet.h)
//a spin pattern is comprised of two parts combined into one 1d array
// { 0, 1,   1, 2, 0, 0, 0, }
//  part 1       part 2
//part 1 indicates the leds on the longest segment that part 2 is to be drawn on (doesn't inculde the last index, so 1 above wouldn't be drawn)
//part 2 is the pattern of colors for each segment, made up of indeces for at pallet (ie 1 represents the second color in the pallet)

//this is confusing so I'll give an example
//for the glasses, using lenseRowSegments (each row of the glasses is a segment, excluding the center 6 leds so that all the rows are the same length)
//part 1 above indicates that, from the 0th led of the first segment, up to (not including) the 1st led, the color pattern of the segments is part 2
//the result is that the first led of each row are colored (using pallet above) row 0 -> white, row 1 -> uclaGold, row 2 -> off,  row 3 -> off, row 4 -> off
//note that leds indexs in part 1 are always defined using the longest segment, because colorSpin will map the pattern (part 1) onto shorter segments
//this works well on most led arrangments, but not so much on the glasses
//which is why I'm using lenseRowSegments, so that all segments are equal length, and the mapping is one to one

//as below, spin patterns can have multiple pattern sections
//you define the matrix as: spinPatternName[(5 + 2) * 8] -> [ (number of segments + 2) * number of rows in pattern ]
//you can also make patterns repeat using colorSpin, so you just have to define a single complete pattern (like I do with spinPatternWave)

//for rowLenseSegments
//two color sine wave pattern
byte spinPatternWave[(5 + 2) * 8] = {
  0, 1,   1, 2, 0, 0, 0,
  1, 2,   0, 1, 2, 0, 0,
  2, 3,   0, 0, 1, 2, 0,
  3, 4,   0 , 0, 0, 1, 2,
  4, 5,   0, 0, 0, 1, 2,
  5, 6,   0, 0, 1, 2, 0,
  6, 7,   0, 1, 2, 0, 0,
  7, 8,   1, 2, 0, 0, 0,
};

//for rowLensesHalvesSegments, we want rows seperated, so we can move the arrows in opposite directions
//two color arrows, moving horizontally
//if you want vertical arrows, you have to use columnSegments
byte spinPatternArrows[(10 + 2) * 5] = {
  0, 1,   1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
  1, 2,   2, 1, 0, 1, 2, 2, 1, 0, 1, 2,
  2, 3,   0, 2, 1, 2, 0, 0, 2, 1, 2, 0,
  3, 4,   0, 0, 2, 0, 0, 0, 0, 2, 0, 0,
  4, 5,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

//for columnSegments, an arrow in each lense
byte spinPatternArrowsVert[(17 + 2) * 5] = {
  0, 1,   2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2,
  1, 2,   1, 2, 0, 0, 0, 2, 1, 0, 0, 0, 1, 2, 0, 0, 0, 2, 1,
  2, 3,   0, 1, 2, 0, 2, 1, 0, 0, 0, 0, 0, 1, 2, 0, 2, 1, 0,
  3, 4,   0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0,
  4, 5,   0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
};

//for rowLenseSegments, USA flag
byte spinPatternUSflag[(5 + 2) * 4] = {
  0, 1,   4, 1, 4, 6, 1,
  1, 2,   1, 4, 1, 6, 1,
  2, 3,   4, 1, 4, 6, 1,
  3, 8,   1, 6, 1, 6, 1,
};

//for rowLenseSegments, St George's cross (can't get blue right for Union Jack)
byte spinPatternStGeorgeCross[(5 + 2) * 3] = {
  0, 3,   1, 1, 6, 1, 1,
  3, 4,   6, 6, 6, 6, 6,
  4, 7,   1, 1, 6, 1, 1,
};

//storage for pallets we'll generate on the fly
uint32_t tempRandPallet[5]; //pallet for random colors (length sets number of colors used by randomColors effect)
uint32_t tempTwinklePallet[2]; //used for twinkle effect, we are only twinkling two colors at a time

//battery keep alive ISR
//(pulses a NPM transistor at regular intervals to pull current from the power bank, preventing it from shutting down due to low current draw)
//when called, if BatKeepAliveOn is true, we turn off the transistor, and set the timer delay to the BAT_KEEP_ALIVE_FREQ
//otherwise, we turn on the transistor and set the timer delay to BAT_KEEP_ALIVE_INTERV
//ie we switch between truning the transistor on and off, while waiting either BAT_KEEP_ALIVE_FREQ or BAT_KEEP_ALIVE_INTERV
//before turing it on/off again
//if using something other than an ESP8266, you'll need to adjust this code to work with whatever timers your chip has
boolean BatKeepAliveOn = false;
void ICACHE_RAM_ATTR BatKeepAlive_ISR() {
  if (BatKeepAliveOn) {
    digitalWrite(BAT_KEEP_ALIVE, LOW); //turn off transistor
    timer1_write(BAT_KEEP_ALIVE_FREQ); //set next trigger time
  } else {
    digitalWrite(BAT_KEEP_ALIVE, HIGH); //turn on transistor
    timer1_write(BAT_KEEP_ALIVE_INTERV); //set next trigger time
  }
  BatKeepAliveOn = !BatKeepAliveOn;
}

//triggered by button 1, stops the current pattern, and switches to the next one, wrapping if needed
//if button 2 is also being held, turn effects on / off
void ICACHE_RAM_ATTR nextEffect() {
  strip.pixelStripStopPattern = true; //stop the current pattern
  breakCurrentEffect = true; //set flag to break out of current case statement
  //if button 2 is being held, stop/start the effect cycle
  //otherwise increase the index to start the next effect
  if ( digitalRead(BUTTON_2) == LOW) {
    effectsStop = !effectsStop;
  }
}

//triggered by button 2, turns effect rotation on / off
//(if rotation is off, the current effect will be repeated continuously)
//if button 1 is also being held, turn effects on / off
void ICACHE_RAM_ATTR effectRotaToggle() {
  if ( digitalRead(BUTTON_1) == LOW) {
    effectsStop = !effectsStop;
    strip.pixelStripStopPattern = true;
  } else {
    effectRota = !effectRota;
  }
}

//triggered by button 3, sets the strip brightness to the next
//brightness level in the brightnessLevels array (wrapping to the start if needed)
void ICACHE_RAM_ATTR brightnessAdjust() {
  brightnessIndex = (brightnessIndex + 1) % numBrightnessLevels;
  strip.setBrightness( brightnessLevels[ brightnessIndex ] );
}

//increments the effect index (wrapping if needed)
void incrementEffectIndex() {
  effectIndex = (effectIndex + 1) % numEffects;
}

void setup() {
  //initalize the led strip, and set the starting brightness
  strip.begin();
  strip.setBrightness( brightnessLevels[brightnessIndex] );

  //configure the button and bat keep alive pins
  pinMode(BAT_KEEP_ALIVE, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  //because of the way my library currently works, effects occupy the processor until they end
  //so to break out of an effect, or change sytem values, we need to use interrupts
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), nextEffect, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), effectRotaToggle, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_3), brightnessAdjust, FALLING);

  //attach an timer interrupt for keeping the power bank alive, and start it
  //if using somkething other than an esp8266, you'll need to adjust this code to work with the timers on your chip
  timer1_attachInterrupt(BatKeepAlive_ISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE); //TIM_DIV16 ==> each tick is 0.2us
  timer1_write(BAT_KEEP_ALIVE_START_DELAY); //start the ISR timer

  strip.show();
  //Serial.begin(115200);

  randomSeed(ESP.getCycleCount()); //generate a random seed
  //fill in our random pallet
  strip.genRandPallet( tempRandPallet, SIZE(tempRandPallet) );
}

//!! If you want to change the main loop code, please read all the comments below and in the loop !!
//To remove an effect, simply change its case # to anything greater than the total number of effects (999 for ex)
//if you want to know about certain effects, please see comments in the library for said effect
//if you want to know how segments work, please see comments in segmentSet.h

//The main loop of the program, works as follows:
//if effectsStop is true, effects are "off", so we won't try to start the next effect
//otherwise we jump to the effect matching the effectIndex value using a switch statment
//we also "clean up" a bit by reseting direct to true and the breakCurrentEffect flag to false
//if we don't find an effect with the effectIndex, we increment the effectIndex until we do
//while an effect is running, button inputs can either lock the effect or skip to the next effect
//if we lock the effect (set effectRota to false), we do not increment effectIndex when the effect ends, essentially restarting the effect (with new colors if they're randomly choosen)
//if the effect is skipped, we set strip.pixelStripStopPattern and breakCurrentEffect to true
//strip.pixelStripStopPattern will stop the current effect, and breakCurrentEffect will break out of the current switch statement if needed (the switch case has more than one effect)
//once the effect ends (either naturally or from a button press), we incremented effectIndex (as long as effectRota is set true)
//and jump to the top of the main loop

void loop() {

  if (!effectsStop) { //if effectsStop is true, we won't display any effect
    direct = true;
    breakCurrentEffect = false;
    //switch statment contains all effects
    //I'm not going to comment each one, as they're hard to describe
    //if an case has a loop, it generally means the effect will by run multiple times in diff directions
    //these will contain breakEffectCheck(); which will breakout of the case if the effect is skipped by button input
    //segmentName.flipSegDirectionEvery(1, true) means that the direction of the names segments will be reversed
    //this is used to change the direction of colorSpin effects
    
    switch (effectIndex) { //select the next effect based on the effectIndex
      case 0:
        strip.segGradientCycle(columnSegments, rainbowdashPattern2, SIZE(rainbowdashPattern2), rainbowdashPallet, SIZE( rainbowdashPallet ), 5, 100, true, 120);
        break;
      case 1:
        strip.rainbowCycle(20, 3);
        break;
      case 2:
        strip.genRandPallet( tempRandPallet, SIZE(tempRandPallet) );
        strip.randomColorSet(off, true, tempRandPallet, SIZE(tempRandPallet), 100, 5, 15000);
        break;
      case 3:
        strip.randomColorSet(off, true, christmasPallet, SIZE(christmasPallet), 100, 5, 15000);
        break;
      case 4:
        strip.shooterSeg( columnSegments, pallet, 3, 0, 8, 1, 1, 2, 4, true, false, 70, 100);
        break;
      case 5:
        strip.shooterSeg( lenseSegments, pallet, 4, 0, 5, 4, 1, 3, 4, true, false, 70, 100);
        break;
      case 6:
        strip.shooterSeg( rowHalvesSegments, pallet, 3, 0, 10, 1, 1, 3, 4, true, false, 65, 100);
        break;
      case 7:
        strip.shooterSeg( rowHalvesSegments, pallet, 3, 0, 10, 1, 1, 2, 2, true, false, 65, 100);
        break;
      case 8:
        strip.sparkSeg( rowLenseSegments, 10, 1, 1, 0, 2, 0, 1, false, 100, 80 ); //segmented rainbow sparks, blank Bg
        break;
      case 9:
        strip.sparkSeg( rowLenseSegments, 20, 1, 1, white, 1, 0, 5, false, 200, 80 ); //radial rainbow Bg, white sparks
        break;
      case 10:
        for (int i = 0; i < 2; i++) {
          breakEffectCheck();
          strip.segGradientCycleRand(columnSegments, 3, 7, 200, direct, 1, 80);
          direct = !direct;
        }
        break;
      case 11:
        strip.rainbowWave( columnSegments, 100, true, 8, 20);
        break;
      case 12:
        for (int i = 0; i < 2; i++) {
          breakEffectCheck();
          strip.segGradientCycleRand(rowSegments, 3 , 5, 200, direct, 1, 100);
          direct = !direct;
        }
        break;
      case 13:
        strip.genRandPallet( tempRandPallet, SIZE(tempRandPallet) );
        tempRandPallet[0] = 0; //background color for spin patterns (off)
        for (int i = 0; i < 2; i++) {
          rowLensesHalvesSegments.flipSegDirectionEvery(1, true); //reverse the direction of each segment of rowLensesHalvesSegments
          breakEffectCheck();
          strip.colorSpin( rowLensesHalvesSegments, spinPatternArrows, SIZE(spinPatternArrows), tempRandPallet, 0, 1, true, 100, 110 );
        }
        break;
      case 14:
        strip.genRandPallet( tempRandPallet, SIZE(tempRandPallet) );
        tempRandPallet[0] = 0; //background color for spin patterns (off)
        for (int i = 0; i < 2; i++) {
          columnSegments.flipSegDirectionEvery(1, true);
          breakEffectCheck();
          strip.colorSpin( columnSegments, spinPatternArrowsVert, SIZE(spinPatternArrowsVert), tempRandPallet, 0, 1, true, 100, 110 );
        }
        break;
      case 15:
        strip.genRandPallet( tempRandPallet, SIZE(tempRandPallet) );
        tempRandPallet[0] = 0; //background color for spin patterns (off)
        for (int i = 0; i < 2; i++) {
          rowLenseSegments.flipSegDirectionEvery(1, true);
          breakEffectCheck();
          strip.colorSpin( rowLenseSegments, spinPatternWave, SIZE(spinPatternWave), tempRandPallet, 0, 1, true, 100, 100 );
        }
        break;
      case 16:
        for (int i = 0; i < 2; i++) {
          breakEffectCheck();
          strip.randomWaves( columnSegments, 3, 2, 2, 50, direct, 7, 20);
          direct = !direct;
        }
        break;
      case 17:
        for (int i = 0; i < 4; i++) {
          breakEffectCheck();
          strip.colorWipeRandomSeg( rowLensesHalvesSegments, 2, 2, 0, 130, direct, false, true);
          breakEffectCheck();
          strip.colorWipeSeg(rowLensesHalvesSegments, 0, 0, 130, !direct, false, true);
          direct = !direct;
        }
        break;
      case 18:
        for (int i = 0; i < 4; i++) {
          breakEffectCheck();
          strip.colorWipeRainbowSeg(rowLensesHalvesSegments, 120, 0, true, false, false, true);
          breakEffectCheck();
          strip.colorWipeSeg(rowLensesHalvesSegments, 0, 0, 130, false, false, true);
        }
        break;
      case 19:
        strip.colorSpin( rowLenseSegments, spinPatternUSflag, SIZE(spinPatternUSflag), pallet, 0, 1, false, 100, 100 );
        break;
      case 20:
        strip.colorSpin( rowLenseSegments, spinPatternStGeorgeCross, SIZE(spinPatternStGeorgeCross), pallet, 0, 1, false, 100, 100 );
        break;
      case 21:
        strip.colorSpinSimple( lenseSegments, 5, 0, 0, 4, -1, 4, 0, 1, 24 * 7, 80 );
        break;
      case 22:
        strip.colorSpinSimple( lenseSegments, 3, 0, 0, 7, -1, 7, 0, 1, 24 * 7, 60 );
        break;
      case 23:
        strip.colorSpinSimple( lenseSegments, 1, 0, 0, 7, 3, 7, 0, 2, 24 * 7, 60 );
        break;
      case 24:
        strip.segGradientCycleRand(lenseSegments, 4, 7, 200, false, 2, 80);
        break;
      case 25:
        strip.patternSweepRand( 12, white, -1, 0, 0, true, 0, 1, 60, 320 );
        break;
      case 26:
        strip.simpleStreamerRand( 5, 0, 7, 0, 0, true, 100, 70);
        break;
      case 27:
        strip.gradientCycleRand( 4, 8, 24 * 7, true, 90);
        break;
      case 28:
        strip.colorSpinSimple( rowLenseSegments, 1, 0, 0, 7, 1, 0, 0, 2, 24 * 5, 60 ); //rainbow half
        break;
      case 29:
        strip.colorSpinSimple( rowLensesHalvesSegments, 1, 0, 0, 3, 1, 0, 0, 2, 24 * 5, 70 );
        break;
      case 30:
      //twinkle four times, each with a different pair of colors
        for (int i = 0; i < 4; i++) {
          if (i == 0) {
            tempTwinklePallet[0] = red;
            tempTwinklePallet[1] = white;
          } else if (i == 1) {
            tempTwinklePallet[0] = blue;
            tempTwinklePallet[1] = yellow;
          } else if (i == 2) {
            tempTwinklePallet[0] = red;
            tempTwinklePallet[1] = green;
          } else if (i == 3) {
            tempTwinklePallet[0] = purple;
            tempTwinklePallet[1] = green;
          }
          breakEffectCheck();
          strip.twinkleSet(0, tempTwinklePallet, SIZE(tempTwinklePallet), 2, 60, 28, 8000);
        }
        break;
      case 31:
        strip.twinkleSet(0, christmasPallet, SIZE(christmasPallet), 2, 60, 15, 10000);
        break;
      case 32:
      //rainbow() only does one full cycle, we'll do 3 to extend its duration
        for (int i = 0; i < 3; i++) {
          breakEffectCheck();
          strip.rainbow(35);
        }
        break;
      case 33:
        for (int i = 0; i < 3; i++) {
          breakEffectCheck();
          strip.patternSweepRand( 4, -1, 0, 1, 5, true, 0, 1, 60, 24 * 8 );
        }
        break;
      case 34:
        for (int i = 0; i < 2; i++) {
          breakEffectCheck();
          strip.patternSweepRainbowRand( 2, 0, 1, 8, true, 0, 60, 24 * 8);
        }
        break;
      case 35:
        strip.colorSpinSimple( rowLenseSegments, 1, white, -1, 1, -1, 4, 0, 1, 24 * 7, 100 ); //white strips on rainbow Bg
        break;
      case 36:
        for (int i = 0; i < 4; i++) {
          tempRandPallet[0] = RC();
          for (int j = 0; j < 2; j++) {
            breakEffectCheck();
            strip.singleWave( columnSegments, tempRandPallet[0], 0, 1, columnSegments.numSegs - 1, direct, 3, 20);
            direct = !direct;
          }
        }
        break;
      case 37:
        for (int i = 0; i < 4; i++) {
          tempRandPallet[0] = RC();
          for (int j = 0; j < 2; j++) {
            breakEffectCheck();
            strip.singleWave( rowSegments, tempRandPallet[0], 0, 1, rowSegments.numSegs - 1, direct, 10, 20);
            direct = !direct;
          }
        }
        break;
      case 38:
        strip.randomColors(-1, false, white, 70, 5, 15000);
        break;
      case 39:
        for (int i = 0; i < 3; i++) {
          breakEffectCheck();
          strip.patternSweepRepeatRand(3, 0, 0, 2, 3, false, false, 0, 0, 1, 70, 200 );
        }
        break;
      default:
        //if we don't find an effect for the current effectIndex, we'll move to the next effect
        //this is for when an effect is set to always be skipped
        incrementEffectIndex();
        break;
    }
    //if effectRota is true we'll advance to the effect index
    if (effectRota) {
      incrementEffectIndex();
    }
    strip.stripOff(); //clear the strip for the next effect
  }

  //un-used effects, you can try them if you want

  //strip.theaterChaseRainbow(off.color32, 70, 4, 2);
  //strip.crossFadeCycleRand(2, 10, 40, 20);
  //strip.randomWaves( lenseSegments, 2, 1, 0, 30, true, 14, 20);
  //strip.rainbowSingleWave( rowSegments, 1, 50, 2, 8, 30);
  //strip.shooterSeg( columnSegments, pallet, 3, 0, 10, 1, 1, 2, 2, true, false, 70, 100); //rainbow falling down w/ trails
  //strip.waves( columnSegments, rainbowdashPallet, SIZE( rainbowdashPallet ), rainbowdashPattern2, SIZE(rainbowdashPattern2), 15, false, 30, 20);
  //strip.patternSweepRand( 3, 0, RC(), 0, 0, false, 5, 1, 50, 320 ); blank LEDs, random BG, no trails, scanner, eyesize 5, color mode 1
  //strip.waves( columnSegments, pallet, SIZE( pallet ), wavepattern, SIZE(wavepattern), 15, false, 30, 20);
  //strip.waves( columnSegments, pallet, SIZE( pallet ), wavepattern2, SIZE(wavepattern2), 15, false, 30, 20);
  //strip.waves( columnSegments, pallet, SIZE( pallet ), wavepattern3, SIZE(wavepattern3), 15, false, 30, 20);
  //strip.waves( columnSegments, pallet, SIZE( pallet ), wavepattern4, SIZE(wavepattern4), 15, false, 30, 20);
  //strip.waves( columnSegments, christmasPallet, SIZE( christmasPallet ), christmasWavePattern, SIZE(christmasWavePattern), 20, true, 30, 20);
  //  for (int j = 0; j < 256; j++) {
  //    strip.randomColors(off.color32, true, strip.Wheel(j & 255), 50, 8, 110);
  //  }
}

//a quick shortening of the random color function, just to reduce the pattern function calls more readable
uint32_t RC() {
  return strip.randColor();
}
