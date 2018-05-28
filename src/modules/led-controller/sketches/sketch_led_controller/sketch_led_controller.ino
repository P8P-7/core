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

enum class lightingType : uint8_t {
    SPECIFIC = 0,
    ALL = 1,
    CIRCLE = 2,
};

enum class colourType : uint8_t {
    HSV = 0,
    RAINBOW = 1,
};

struct led_status {
    lightingType lightingType;
    colourType colourType;
};

struct specific_colour {
    uint8_t led_id;
    uint8_t hue;
    uint8_t saturation;
    uint8_t value;
};

struct all_leds {
    uint8_t hue;
    uint8_t saturation;
    uint8_t value;
    bool rainbow;
};

struct specific_rainbow {
    uint8_t led_id;
    uint8_t start_hue;
};

struct circle {
    uint8_t start_id;
    uint8_t end_id;
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
int led_id = 0;
circle circleInfo;

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
        leds[led_id] = rgbVals;
        EVERY_N_MILLISECONDS(250)
        {
            if (circleInfo.cw) {
                if (led_id < circleInfo.end_id) {
                    led_id++;
                } else {
                    led_id = circleInfo.start_id;
                }
            } else {
                if (led_id > circleInfo.start_id) {
                    led_id--;
                } else {
                    led_id = circleInfo.end_id;
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
    if (Wire.available() <= sizeof(led_status)) {

        clear_wire();
        return;
    }

    unsigned char buffer[sizeof(led_status)];

    buffer[0] = Wire.read();
    buffer[1] = Wire.read();

    led_status *packetType = (led_status *) buffer;

    if (packetType->lightingType == lightingType::SPECIFIC) {
        rainbow = false;
        if (packetType->colourType == colourType::HSV) {
            unsigned char ledBuffer[sizeof(specific_colour)];
            for (int i = 0; Wire.available(); i++) {
                ledBuffer[i] = Wire.read();
            }
            specific_colour *command = (specific_colour *) ledBuffer;
            rainbowLEDS[command->led_id] = false;
            hsv[command->led_id] = CHSV(command->hue, command->saturation, command->value);
        } else if (packetType->colourType == colourType::RAINBOW) {
            unsigned char ledBuffer[sizeof(specific_rainbow)];
            for (int i = 0; Wire.available(); i++) {
                ledBuffer[i] = Wire.read();
            }
            specific_rainbow *command = (specific_rainbow *) ledBuffer;
            hsv[command->led_id] = CHSV(command->start_hue, 255, 255);
            rainbowLEDS[command->led_id] = true;
        }
    } else if (packetType->lightingType == lightingType::CIRCLE) {
        rainbow = false;
        unsigned char ledBuffer[sizeof(circle)];
        for (int i = 0; Wire.available(); i++) {
            ledBuffer[i] = Wire.read();
        }
        circle *command = (circle *) ledBuffer;

        if (command->start_id == command->end_id) {
            cycle = false;
        } else {
            circleInfo.start_id = command->start_id;
            circleInfo.end_id = command->end_id;
            circleInfo.cw = command->cw;
            circleInfo.hue = command->hue;
            circleInfo.saturation = command->saturation;
            cycle = true;
        }
    } else if (packetType->lightingType == lightingType::ALL) {
        unsigned char ledBuffer[sizeof(all_leds)];
        for (int i = 0; Wire.available(); i++) {
            ledBuffer[i] = Wire.read();
        }
        all_leds *command = (all_leds *) ledBuffer;
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
