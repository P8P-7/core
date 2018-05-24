#include <Wire.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN 13
#define ADDRESS 0x40
#define LED_TYPE UCS1903B
#define COLOR_ORDER RGB
#define NUM_LEDS 5
#define BRIGHTNESS 255/1
#define FRAMES_PER_SECOND 500

        uint8_t
gHue = 0;
uint8_t varValue = 255;
bool brighten = false;
bool rainbow = false;

struct led_status {
    byte led_id;
    byte hue;
    byte saturation;
    byte value;
};
CRGB leds[NUM_LEDS];

void setup() {
    Wire.begin(ADDRESS);
    Wire.onReceive(on_receive_message);

    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
    //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    FastLED.show();
    FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void clear_wire() {
    while (Wire.available() > 0)
        Wire.read();
}

void on_receive_message(int length) {
    if (Wire.available() != sizeof(led_status)) {
        clear_wire();
        return;
    }

    unsigned char buffer[sizeof(led_status)];
    for (int i = 0; Wire.available(); i++) {
        buffer[i] = Wire.read();
    }
    led_status *command = (led_status *) buffer;

    if (command->led_id > NUM_LEDS - 1) {
        return;
    }

    leds[command->led_id] = CHSV(command->hue, command->saturation, command->value);
}

