#include <Wire.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN 13
#define ADDRESS 0x40
#define LED_TYPE UCS1903B
#define COLOR_ORDER RGB
#define NUM_LEDS 5
#define BRIGHTNESS 255/1
#define FRAMES_PER_SECOND 60
#define HUE_OFFSET 4

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
byte start_led;
byte end_led;
bool rbcw = false;
bool cyclecw = false;
bool rainbow = false;
bool cycle = false;
bool individual = true;
bool rainbowLEDS[NUM_LEDS];
uint8_t gHue = 0;
int ledId = 0;
Circle circleInfo;

void setup() {
    Wire.begin(ADDRESS);
    Wire.onReceive(on_receive_message);

    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    CRGB rgbVals = CRGB(0, 0, 0);
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

    if (rainbow) {
        hsv2rgb_rainbow(CHSV(gHue, 255, 255), rgbVals);
        for (int j = 0; j < NUM_LEDS; ++j) {
            leds[j] += rgbVals;
        }
        gHue++;
    }

    if (cycle) {
        hsv2rgb_rainbow(CHSV(circleInfo.hue, circleInfo.saturation, 255), rgbVals);
        leds[ledId] = rgbVals;
        EVERY_N_MILLISECONDS(250)
        {
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

    LedStatus *packetType = (LedStatus*) buffer;

    if (packetType->lightingType == LightingType::SPECIFIC) {
        rainbow = false;
        if (packetType->colorType == ColorType::HSV) {
            unsigned char ledBuffer[sizeof(SpecificColor)];
            for (int i = 0; Wire.available(); i++) {
                ledBuffer[i] = Wire.read();
            }
            SpecificColor *command = (SpecificColor*) ledBuffer;
            rainbowLEDS[command->ledId] = false;
            hsv[command->ledId] = CHSV(command->hue, command->saturation, command->value);
        } else if (packetType->colorType == ColorType::RAINBOW) {
            unsigned char ledBuffer[sizeof(SpecificRainbow)];
            for (int i = 0; Wire.available(); i++) {
                ledBuffer[i] = Wire.read();
            }
            SpecificRainbow *command = (SpecificRainbow*) ledBuffer;
            hsv[command->ledId] = CHSV(command->startHue, 255, 255);
            rainbowLEDS[command->ledId] = true;
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
            circleInfo.startId = command->startId;
            circleInfo.endId = command->endId;
            circleInfo.cw = command->cw;
            circleInfo.hue = command->hue;
            circleInfo.saturation = command->saturation;
            cycle = true;
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
            for (int i = 0; i < NUM_LEDS; ++i) {
                hsv[i] = CHSV(command->hue, command->saturation, command->value);
            }
        }
    }
}
