#include <Wire.h>

#define DEBUG
#define ADDRESS 0x30
#define CHANGE_DIRECTION_THRESHOLD 0

enum class motor_direction: byte {
  FORWARDS = 0,
  BACKWARDS = 1,
  FREE = 2,
  LOCKED = 3
};
struct motor {
  char forwards_pin;
  char backwards_pin;
  char pwm_pin;

  long time_of_direction_change = 0;
  motor_direction direction = motor_direction::FORWARDS;
  motor_direction target_direction = motor_direction::FORWARDS;
  byte speed = 0;
  byte target_speed = 0;

  motor() { }
  motor(byte forwards_pin, byte backwards_pin, byte pwm_pin) : forwards_pin(forwards_pin), 
                                                               backwards_pin(backwards_pin), 
                                                               pwm_pin(pwm_pin) { }
};
struct motor_status {
  byte motor_id;
  motor_direction direction;
  byte speed;
};

const byte number_of_motors = 1;
motor motors[number_of_motors];

void setup() {
  Wire.begin(ADDRESS);
  Wire.onRequest(on_request_status);
  Wire.onReceive(on_receive_message);

#ifdef DEBUG
  Serial.begin(115200);
  Serial.print("Started listening on 0x");
  Serial.println(ADDRESS, HEX);
#endif

  setup_motors();
}

void loop() {
  for (int motor_id = 0; motor_id < number_of_motors; motor_id++) {
    motor& m = motors[motor_id];

    if (m.direction != m.target_direction) {
      if (m.speed != 0 && m.time_of_direction_change == 0) {
#ifdef DEBUG
        Serial.print("Motor ");
        Serial.print(motor_id);
        Serial.print(" has changed direction (");
        Serial.print((int)m.direction, DEC);
        Serial.print(" -> ");
        Serial.print((int)m.target_direction, DEC);
        Serial.print("), speed: (");
        Serial.print(m.speed, DEC);
        Serial.print(" -> ");
        Serial.print(m.target_speed, DEC);
        Serial.print("). Stalling engine at ");
        Serial.println(millis());
#endif

        analogWrite(m.pwm_pin, 0);
        m.time_of_direction_change = millis();
      } else if (m.speed == 0 || (millis() - m.time_of_direction_change) > CHANGE_DIRECTION_THRESHOLD) {
#ifdef DEBUG
        Serial.print("Motor ");
        Serial.print(motor_id);
        Serial.print(" has been stalled. Changing direction at ");
        Serial.print(millis());
        Serial.println();
#endif

        m.direction = m.target_direction;
        m.time_of_direction_change = 0;
      }
    } else {
      set_direction(m, m.direction);
      m.speed = m.target_speed;
      analogWrite(m.pwm_pin, m.speed);
    }
  }
}

void setup_motors() {
  motors[0] = motor(
    8, // forwards_pin / linksom
    7, // backwards_pin / rechtsom
    3 // pwm_pin
  );

  for (int i = 0; i < number_of_motors; i++) {
    pinMode(motors[i].forwards_pin, OUTPUT);
    pinMode(motors[i].backwards_pin, OUTPUT);
  }
}

void set_direction(const motor &motor, const motor_direction &direction) {
  bool forwards = HIGH;
  bool backwards = HIGH;
  
  switch (direction) {
    case motor_direction::FORWARDS:
      forwards = HIGH;
      backwards = LOW;
    break;
    case motor_direction::BACKWARDS:
      forwards = LOW;
      backwards = HIGH;
    break;
    case motor_direction::FREE:
      forwards = backwards = LOW;
    break;
    case motor_direction::LOCKED:
      forwards = backwards = HIGH;
    break;
    default:
      return;
  }
  
  digitalWrite(motor.forwards_pin, forwards);
  digitalWrite(motor.backwards_pin, backwards);
}

void clear_wire() {
  while (Wire.available() > 0)
    Wire.read();
}

void on_request_status() {
#ifdef DEBUG
  Serial.println("Received a status request");
#endif
  Wire.write(number_of_motors);
  
  for (byte i = 0; i < number_of_motors; i++) {
    const motor& m = motors[i];
    motor_status status{ i, m.direction, m.speed }; // TODO: read speed from analog pin
#ifdef DEBUG
  Serial.println("Writing motor status: ");
  Serial.print("- id: ");
  Serial.println(status.motor_id, DEC);
  Serial.print("- direction: ");
  Serial.println((char)status.direction, DEC);
  Serial.print("- speed: ");
  Serial.println(status.speed, DEC);
#endif

    Wire.write((char*)&status, sizeof(motor_status));
  }
}

void on_receive_message(int length) {
  if (Wire.available() != sizeof(motor_status)) {
#ifdef DEBUG
    Serial.print("Received message was of an invalid size. (");
    Serial.print(length, DEC);
    Serial.println(")");
#endif

    clear_wire();
    return;
  }  

  unsigned char buffer[sizeof(motor_status)];
  for (int i = 0; Wire.available(); i++) {
    buffer[i] = Wire.read();
  }
  motor_status *command = (motor_status*)buffer;

#ifdef DEBUG
  Serial.println("Received: ");
  Serial.print("- id: ");
  Serial.println(command->motor_id, DEC);
  Serial.print("- direction: ");
  Serial.println((char)command->direction, DEC);
  Serial.print("- speed: ");
  Serial.println(command->speed, DEC);
#endif

  if (command->motor_id > number_of_motors - 1) {
#ifdef DEBUG
  Serial.println("Received command was invalid. Discarding command.");
#endif
    return;
  }

  motor& m = motors[command->motor_id];
  if (m.time_of_direction_change > 0 && m.target_direction != command->direction) {
    m.time_of_direction_change = 0;
  }
  m.target_direction = command->direction;
  m.target_speed = command->speed;
}

