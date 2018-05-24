#include <Wire.h>

#define DEBUG
#define ADDRESS 0x30
#define CHANGE_DIRECTION_TIME_SPAN 1500
#define MAX_SPEED 255

int calculateTimeNeededForSpeedTransition(int maxTime, int maxDistance, int distance);
int interpolateSpeed(int prevSpeed, int nextSpeed, int startTime, int endTime, int currentTime);

enum class motor_direction: byte {
  FORWARDS = 0,
  BACKWARDS = 1,
  FREE = 2,
  LOCKED = 3
};
struct transition {
  int from = 0;
  int to = 0;

  long timeStarted = 0;
  long timeNeeded = 0;

  int clamp(int value) {
    int min = max(from, to);
    int max = min(from, to);
    return max(max, min(min, value));
  }

  int at(long currentTime) {
    return clamp(interpolateSpeed(from, to, timeStarted, timeStarted + timeNeeded, currentTime));
  }
  int current() {
    return at(millis());
  }
  void start(int length) {
    timeStarted = millis();
    timeNeeded = length;
  }
  void stop() {
    timeStarted = 0;
  }
  bool isStarted() {
    return timeStarted > 0;
  }
  bool isFinished() {
    return millis() > timeStarted + timeNeeded;
  }
};
struct motor {
  char forwards_pin;
  char backwards_pin;
  char pwm_pin;
  
  motor_direction direction = motor_direction::FORWARDS;
  byte speed = 0;
  bool isStalling = false;
  transition speedTransition;
  
  volatile long time_of_direction_change = 0;
  volatile motor_direction target_direction = motor_direction::FORWARDS;
  volatile byte target_speed = 0;

  motor() { }
  motor(byte forwards_pin, byte backwards_pin, byte pwm_pin) : forwards_pin(forwards_pin), 
                                                               backwards_pin(backwards_pin), 
                                                               pwm_pin(pwm_pin) { }
  bool hasChangedDirection() {
    return direction != target_direction;
  }
  bool isTransitioningDirection() {
    return time_of_direction_change > 0;
  }
  
  bool hasChangedSpeed() {
    return speed != target_speed;
  }
  bool isTransitioningSpeed() {
    return speedTransition.isStarted() && !speedTransition.isFinished();
  }
  void updateSpeed() {
    if (speedTransition.isStarted()) {
      speed = speedTransition.current();
      if (speed == speedTransition.to) {
        speedTransition.stop();
      }
    }
  }
  void startTransitionSpeedTo(int target) {
    speedTransition.from = speed;
    speedTransition.to = target;
    speedTransition.start(calculateTimeNeededForSpeedTransition(CHANGE_DIRECTION_TIME_SPAN, MAX_SPEED, abs(speedTransition.to - speedTransition.from)));
  }
  
  bool needsTransitioning() {
    return hasChangedSpeed() || hasChangedDirection();
  }
  bool isTransitioning() {
    return isTransitioningDirection() || isTransitioningSpeed();
  }
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
  Serial.begin(9600);
  Serial.print("Started listening on 0x");
  Serial.println(ADDRESS, HEX);
#endif
  
  setup_motors();
}

void loop() {
  for (int motor_id = 0; motor_id < number_of_motors; motor_id++) {
    motor& m = motors[motor_id];

    if (!m.needsTransitioning() && !m.isTransitioning()) {
      continue;
    }

    if (m.isStalling && !m.hasChangedDirection()) {
      m.isStalling = false;
    }
    
    if (!m.isStalling) {
      if (m.hasChangedDirection()) {
        m.isStalling = true;
        m.startTransitionSpeedTo(0);
      } else if (!m.isStalling && m.hasChangedSpeed() && m.speedTransition.to != m.target_speed) {
        m.startTransitionSpeedTo(m.target_speed);
      }
    }
    m.updateSpeed();

    if (m.speed == 0) {
      m.isStalling = false;
      m.direction = m.target_direction;
    }
    
    analogWrite(m.pwm_pin, m.speed);
    set_direction(m, m.direction);
  }
}

int interpolateSpeed(int prevSpeed, int nextSpeed, int startTime, int endTime, int currentTime) {
  int dy = nextSpeed - prevSpeed;
  int dx = endTime - startTime;

  double slope = (double)dy / dx;
  
  return ceil(prevSpeed + slope * (currentTime - startTime));
}

int calculateTimeNeededForSpeedTransition(int maxTime, int maxDistance, int distance) {
  double slope = (double)maxTime / maxDistance;
  
  return ceil(slope * distance);
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
  while (Wire.available() > 0) {
    Wire.read();
  }
}

void on_request_status() {
#ifdef DEBUG
  Serial.println("Received a status request");
#endif
  Wire.write(number_of_motors);
  
  for (byte i = 0; i < number_of_motors; i++) {
    const motor& m = motors[i];
    motor_status status{ i, m.direction, m.speed };
    
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
  if (length % sizeof(motor_status) != 0) {
#ifdef DEBUG
    Serial.print("Received message was of an invalid size. (");
    Serial.print(length, DEC);
    Serial.println(")");
#endif

    clear_wire();
    return;
  }
  
  unsigned char buffer[length];
  for (int i = 0; Wire.available(); i++) {
    buffer[i] = Wire.read();
  }

  int number_of_commands = length / sizeof(motor_status);
  motor_status *commands = (motor_status*)buffer;
#ifdef DEBUG
  Serial.print("Received ");
  Serial.print(number_of_commands);
  Serial.println(" commands");
#endif  
  for (int i = 0; i < number_of_commands; i++) {
    motor_status &command = commands[i];
#ifdef DEBUG

    Serial.println("Received: ");
    Serial.print("- id: ");
    Serial.println(command.motor_id, DEC);
    Serial.print("- direction: ");
    Serial.println((char)command.direction, DEC);
    Serial.print("- speed: ");
    Serial.println(command.speed, DEC);
#endif
  
    if (command.motor_id > number_of_motors - 1) {
#ifdef DEBUG
    Serial.println("Received command was invalid. Discarding command.");
#endif
      clear_wire();
      return;
    }
    
    motor& m = motors[command.motor_id];

    m.target_direction = command.direction;
    m.target_speed = command.speed;
  }
}

