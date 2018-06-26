#include <Wire.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN 13
#define DATA_PIN_2 12
#define DATA_PIN_3 11
#define ACTIVE_PLACE 13
#define ADDRESS 0x40
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 60
#define BRIGHTNESS 255/1
#define FRAMES_PER_SECOND 60
#define HUE_OFFSET 2

enum class LightingType : uint8_t {
    SPECIFIC = 0,
    ALL = 1,
    CIRCLE = 2,
};

enum class ColorType : uint8_t {
    HSV = 0,
    RAINBOW = 1,
};

struct LedStatus {
    LightingType lightingType;
    ColorType colorType;
    uint8_t place;
};

struct SpecificColor {
    uint8_t ledId;
    uint8_t hue;
    uint8_t saturation;
    uint8_t value;
};

struct AllLeds {
    uint8_t hue;
    uint8_t saturation;
    uint8_t value;
    bool rainbow;
};

struct SpecificRainbow {
    uint8_t ledId;
    uint8_t startHue;
};

struct Circle {
    uint8_t startId;
    uint8_t endId;
    bool cw;
    uint8_t hue;
    uint8_t saturation;
};

CRGB leds[NUM_LEDS];
CHSV hsv[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CHSV hsv2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CHSV hsv3[NUM_LEDS];
byte start_led;
byte end_led;
bool rbcw = false;
bool cyclecw = false;
bool rainbow = false;
bool cycle = false;
bool cycle2 = false;
bool cycle3 = false;
bool individual = true;
bool rainbowLEDS[NUM_LEDS];
bool rainbowLEDS2[NUM_LEDS];
bool rainbowLEDS3[NUM_LEDS];
uint8_t gHue = 0;
int ledId = 0;
int ledId2 = 0;
int ledId3 = 0;
Circle circleInfo;
Circle circleInfo2;
Circle circleInfo3;

void setup() {
    Wire.begin(ADDRESS);
    Wire.onReceive(on_receive_message);

    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(UncorrectedColor).setTemperature(UncorrectedTemperature);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection(UncorrectedColor).setTemperature(UncorrectedTemperature);
    FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds3, NUM_LEDS).setCorrection(UncorrectedColor).setTemperature(UncorrectedTemperature);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    CRGB rgbVals = CRGB(0, 0, 0);
    CRGB rgbVals2 = CRGB(0, 0, 0);
    CRGB rgbVals3 = CRGB(0, 0, 0);
    for (int i = 0; i < NUM_LEDS; ++i) {
        if (individual) {
            if (rainbowLEDS[i]) {
                if (rbcw) {
                    hsv[i] = CHSV(hsv[i].h - HUE_OFFSET, hsv[i].s == 0 ? 255 : hsv[i].s,
                                  hsv[i].v == 0 ? 255 : hsv[i].s);
                } else {
                    hsv[i] = CHSV(hsv[i].h + HUE_OFFSET, hsv[i].s == 0 ? 255 : hsv[i].s,
                                  hsv[i].v == 0 ? 255 : hsv[i].s);
                }
            }
            leds[i] = hsv[i];
        } else {
            leds[i] = CHSV(0, 0, 0);
        }
    }

    for (int i = 0; i < NUM_LEDS; ++i) {
        if (individual) {
            if (rainbowLEDS2[i]) {
                if (rbcw) {
                    hsv2[i] = CHSV(hsv2[i].h - HUE_OFFSET, hsv2[i].s == 0 ? 255 : hsv2[i].s,
                                   hsv2[i].v == 0 ? 255 : hsv2[i].s);
                } else {
                    hsv2[i] = CHSV(hsv2[i].h + HUE_OFFSET, hsv2[i].s == 0 ? 255 : hsv2[i].s,
                                   hsv2[i].v == 0 ? 255 : hsv2[i].s);
                }
            }
            leds2[i] = hsv2[i];
        } else {
            leds2[i] = CHSV(0, 0, 0);
        }
    }

    for (int i = 0; i < NUM_LEDS; ++i) {
        if (individual) {
            if (rainbowLEDS3[i]) {
                if (rbcw) {
                    hsv3[i] = CHSV(hsv3[i].h - HUE_OFFSET, hsv3[i].s == 0 ? 255 : hsv3[i].s,
                                   hsv3[i].v == 0 ? 255 : hsv3[i].s);
                } else {
                    hsv3[i] = CHSV(hsv3[i].h + HUE_OFFSET, hsv3[i].s == 0 ? 255 : hsv3[i].s,
                                   hsv3[i].v == 0 ? 255 : hsv3[i].s);
                }
            }
            leds3[i] = hsv3[i];
        } else {
            leds3[i] = CHSV(0, 0, 0);
        }
    }

    if (rainbow) {
        hsv2rgb_rainbow(CHSV(gHue, 255, 255), rgbVals);
        for (int j = 0; j < NUM_LEDS; ++j) {
            leds[j] += rgbVals;
            leds2[j] += rgbVals;
            leds3[j] += rgbVals;
        }
        gHue++;
    }

    if (cycle) {
        hsv2rgb_rainbow(CHSV(circleInfo.hue, circleInfo.saturation, 255), rgbVals);
        if(ledId-1>=0){
            leds[ledId-1] = rgbVals;
        }
        else{
            leds[NUM_LEDS-1] = rgbVals;
        }
        leds[ledId] = rgbVals;
        if(ledId+1<=NUM_LEDS-1){
            leds[ledId+1] = rgbVals;
        }
        else{
            leds[0] = rgbVals;
        }
        EVERY_N_MILLISECONDS(250) {
            if (circleInfo.cw) {
                if (ledId < circleInfo.endId) {
                    ledId++;
                } else {
                    ledId = circleInfo.startId;
                }
            } else {
                if (ledId > circleInfo.startId) {
                    ledId--;
                } else {
                    ledId = circleInfo.endId;
                }
            }
        }
    }

    if (cycle2) {
        hsv2rgb_rainbow(CHSV(circleInfo2.hue, circleInfo2.saturation, 255), rgbVals);
        if(ledId2-1>=0){
            leds2[ledId-1] = rgbVals;
        }
        else{
            leds2[NUM_LEDS-1] = rgbVals;
        }
        leds2[ledId2] = rgbVals;
        if(ledId+1<=NUM_LEDS-1){
            leds2[ledId2+1] = rgbVals;
        }
        else{
            leds2[0] = rgbVals;
        }
        EVERY_N_MILLISECONDS(250) {
            if (circleInfo2.cw) {
                if (ledId2 < circleInfo2.endId) {
                    ledId2++;
                } else {
                    ledId2 = circleInfo2.startId;
                }
            } else {
                if (ledId2 > circleInfo2.startId) {
                    ledId2--;
                } else {
                    ledId2 = circleInfo2.endId;
                }
            }
        }
    }

    if (cycle3) {
        hsv2rgb_rainbow(CHSV(circleInfo3.hue, circleInfo3.saturation, 255), rgbVals);
        if(ledId3-1>=0){
            leds3[ledId-1] = rgbVals;
        }
        else{
            leds3[NUM_LEDS-1] = rgbVals;
        }
        leds3[ledId] = rgbVals;
        if(ledId3+1<=NUM_LEDS-1){
            leds3[ledId3+1] = rgbVals;
        }
        else{
            leds3[0] = rgbVals;
        }
        EVERY_N_MILLISECONDS(250) {
            if (circleInfo3.cw) {
                if (ledId3 < circleInfo3.endId) {
                    ledId3++;
                } else {
                    ledId3 = circleInfo3.startId;
                }
            } else {
                if (ledId3 > circleInfo3.startId) {
                    ledId3--;
                } else {
                    ledId3 = circleInfo3.endId;
                }
            }
        }
    }

    FastLED.show();
    FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void clear_wire() {
    while (Wire.available() > 0)
        Wire.read();
}

void on_receive_message(int length) {
    if (Wire.available() <= sizeof(LedStatus)) {

        clear_wire();
        return;
    }

    unsigned char buffer[sizeof(LedStatus)];

    buffer[0] = Wire.read();
    buffer[1] = Wire.read();
    buffer[2] = Wire.read();

    LedStatus *packetType = (LedStatus*) buffer;

    if (packetType->lightingType == LightingType::SPECIFIC) {
        rainbow = false;
        if (packetType->colorType == ColorType::HSV) {
            unsigned char ledBuffer[sizeof(SpecificColor)];
            for (int i = 0; Wire.available(); i++) {
                ledBuffer[i] = Wire.read();
            }

            SpecificColor* command = (SpecificColor*) ledBuffer;
            rainbowLEDS[command->ledId] = false;
            switch(packetType->place){
                case 0:
                    hsv[command->ledId] = CHSV(command->hue, command->saturation, command->value);
                    break;
                case 1:
                    hsv2[command->ledId] = CHSV(command->hue, command->saturation, command->value);
                    break;
                case 2:
                    hsv3[command->ledId] = CHSV(command->hue, command->saturation, command->value);
                    break;
            }
        } else if (packetType->colorType == ColorType::RAINBOW) {
            unsigned char ledBuffer[sizeof(SpecificRainbow)];
            for (int i = 0; Wire.available(); i++) {
                ledBuffer[i] = Wire.read();
            }
            SpecificRainbow* command = (SpecificRainbow*) ledBuffer;
            switch(packetType->place){
                case 0:
                    hsv[command->ledId] = CHSV(command->startHue, 255, 255);
                    rainbowLEDS[command->ledId] = true;
                    break;
                case 1:
                    hsv2[command->ledId] = CHSV(command->startHue, 255, 255);
                    rainbowLEDS2[command->ledId] = true;
                    break;
                case 2:
                    hsv3[command->ledId] = CHSV(command->startHue, 255, 255);
                    rainbowLEDS3[command->ledId] = true;
                    break;
            }
        }
    } else if (packetType->lightingType == LightingType::CIRCLE) {
        rainbow = false;
        unsigned char ledBuffer[sizeof(Circle)];
        for (int i = 0; Wire.available(); i++) {
            ledBuffer[i] = Wire.read();
        }
        Circle *command = (Circle*) ledBuffer;

        if (command->startId == command->endId) {
            cycle = false;
        } else {
            switch(packetType->place){
                case 0:
                    circleInfo.startId = command->startId;
                    circleInfo.endId = command->endId;
                    circleInfo.cw = command->cw;
                    circleInfo.hue = command->hue;
                    circleInfo.saturation = command->saturation;
                    cycle = true;
                    break;
                case 1:
                    circleInfo2.startId = command->startId;
                    circleInfo2.endId = command->endId;
                    circleInfo2.cw = command->cw;
                    circleInfo2.hue = command->hue;
                    circleInfo2.saturation = command->saturation;
                    cycle2 = true;
                    break;
                case 2:
                    circleInfo3.startId = command->startId;
                    circleInfo3.endId = command->endId;
                    circleInfo3.cw = command->cw;
                    circleInfo3.hue = command->hue;
                    circleInfo3.saturation = command->saturation;
                    cycle3 = true;
                    break;
            }
        }
    } else if (packetType->lightingType == LightingType::ALL) {
        unsigned char ledBuffer[sizeof(AllLeds)];
        for (int i = 0; Wire.available(); i++) {
            ledBuffer[i] = Wire.read();
        }
        AllLeds *command = (AllLeds*) ledBuffer;
        if (rainbow) {
            rainbow = true;
        } else {
            rainbow = false;
            switch(packetType->place){
                case 0:
                    for (int i = 0; i < NUM_LEDS; ++i) {
                        hsv[i] = CHSV(command->hue, command->saturation, command->value);
                    }
                    break;
                case 1:
                    for (int i = 0; i < NUM_LEDS; ++i) {
                        hsv2[i] = CHSV(command->hue, command->saturation, command->value);
                    }
                    break;
                case 2:
                    for (int i = 0; i < NUM_LEDS; ++i) {
                        hsv3[i] = CHSV(command->hue, command->saturation, command->value);
                    }
                    break;
            }
        }
    }
}
