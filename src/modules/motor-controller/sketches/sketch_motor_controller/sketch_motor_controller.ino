#include <Wire.h>

#define ADDRESS 0x30
#define CHANGE_DIRECTION_TIME_SPAN 100
#define MAX_SPEED 255

int calculateTimeNeededForSpeedTransition(int maxTime, int maxDistance, int distance);

int interpolateSpeed(int prevSpeed, int nextSpeed, int startTime, int endTime, int currentTime);

enum class MotorDirection : byte {
    FORWARDS = 0,
    BACKWARDS = 1,
    FREE = 2,
    LOCKED = 3
};

struct Transition {
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

struct Motor {
    char forwardsPin;
    char backwardsPin;
    char pwmPin;

    MotorDirection direction = MotorDirection::FORWARDS;
    byte speed = 0;
    bool isStalling = false;
    Transition speedTransition;

    volatile long timeOfDirectionChange = 0;
    volatile MotorDirection targetDirection = MotorDirection::FORWARDS;
    volatile byte targetSpeed = 0;

    Motor() {}

    Motor(byte forwardsPin, byte backwardsPin, byte pwmPin)
        : forwardsPin(forwardsPin), backwardsPin(backwardsPin), pwmPin(pwmPin) {
    }

    bool hasChangedDirection() {
        return direction != targetDirection;
    }

    bool isTransitioningDirection() {
        return timeOfDirectionChange > 0;
    }

    bool hasChangedSpeed() {
        return speed != targetSpeed;
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
        int timeNeeded = calculateTimeNeededForSpeedTransition(CHANGE_DIRECTION_TIME_SPAN, MAX_SPEED,
                                                               abs(speedTransition.to - speedTransition.from));

        if (speedTransition.isStarted() && !speedTransition.isFinished()) {
            speedTransition.timeNeeded = timeNeeded;
        } else {
            speedTransition.start(timeNeeded);
        }
    }

    bool needsTransitioning() {
        return hasChangedSpeed() || hasChangedDirection();
    }

    bool isTransitioning() {
        return isTransitioningDirection() || isTransitioningSpeed();
    }
};

struct MotorStatus {
    byte motorId;
    MotorDirection direction;
    byte speed;
};

const byte numberOfMotors = 4;
Motor motors[numberOfMotors];

void setup() {
    Wire.begin(ADDRESS);
    Wire.onRequest(onRequestStatus);
    Wire.onReceive(onReceiveMessage);

#ifdef DEBUG
    Serial.begin(9600);
    Serial.print("Started listening on 0x");
    Serial.println(ADDRESS, HEX);
#endif

    setupMotors();
}

void loop() {
    for (int motorId = 0; motorId < numberOfMotors; motorId++) {
        Motor& m = motors[motorId];

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
            } else if (m.hasChangedSpeed() && m.speedTransition.to != m.targetSpeed) {
                m.startTransitionSpeedTo(m.targetSpeed);
            }
        }
        m.updateSpeed();

        if (m.speed == 0) {
            m.isStalling = false;
            m.direction = m.targetDirection;
        }

        analogWrite(m.pwmPin, m.speed);
        setDirection(m, m.direction);
    }
}

int interpolateSpeed(int prevSpeed, int nextSpeed, int startTime, int endTime, int currentTime) {
    int dy = nextSpeed - prevSpeed;
    int dx = endTime - startTime;

    double slope = (double) dy / dx;

    return ceil(prevSpeed + slope * (currentTime - startTime));
}

int calculateTimeNeededForSpeedTransition(int maxTime, int maxDistance, int distance) {
    double slope = (double) maxTime / maxDistance;

    return ceil(slope * distance);
}

void setupMotors() {
    motors[0] = Motor(
        8, // forwardsPin / linksom
        7, // backwardsPin / rechtsom
        10 // pwmPin
    );
    motors[1] = Motor(
      13,
      12,
      11
    );
    motors[2] = Motor(
      2,
      4,
      3
    );
    motors[3] = Motor(
      5,
      6,
      9
    );

    for (int i = 0; i < numberOfMotors; i++) {
        pinMode(motors[i].forwardsPin, OUTPUT);
        pinMode(motors[i].backwardsPin, OUTPUT);
    }
}

void setDirection(const Motor& motor, const MotorDirection& direction) {
    bool forwards = HIGH;
    bool backwards = HIGH;

    switch (direction) {
        case MotorDirection::FORWARDS:
            forwards = HIGH;
            backwards = LOW;
            break;
        case MotorDirection::BACKWARDS:
            forwards = LOW;
            backwards = HIGH;
            break;
        case MotorDirection::FREE:
            forwards = backwards = LOW;
            break;
        case MotorDirection::LOCKED:
            forwards = backwards = HIGH;
            break;
        default:
            return;
    }

    digitalWrite(motor.forwardsPin, forwards);
    digitalWrite(motor.backwardsPin, backwards);
}

void clearWire() {
    while (Wire.available() > 0) {
        Wire.read();
    }
}

void onRequestStatus() {
#ifdef DEBUG
    Serial.println("Received a status request");
#endif
    Wire.write(numberOfMotors);

    for (byte i = 0; i < numberOfMotors; i++) {
        const Motor& m = motors[i];
        MotorStatus status{i, m.direction, m.speed};

#ifdef DEBUG
        Serial.println("Writing motor status: ");
        Serial.print("- id: ");
        Serial.println(status.motorId, DEC);
        Serial.print("- direction: ");
        Serial.println((char) status.direction, DEC);
        Serial.print("- speed: ");
        Serial.println(status.speed, DEC);
#endif

        Wire.write((char*) &status, sizeof(MotorStatus));
    }
}

void onReceiveMessage(int length) {
    if (length % sizeof(MotorStatus) != 0) {
#ifdef DEBUG
        Serial.print("Received message was of an invalid size. (");
        Serial.print(length, DEC);
        Serial.println(")");
#endif

        clearWire();
        return;
    }

    uint8_t buffer[length];
    for (int i = 0; Wire.available(); i++) {
        buffer[i] = Wire.read();
    }

    int numberOfCommands = length / sizeof(MotorStatus);
    MotorStatus* commands = (MotorStatus*) buffer;
#ifdef DEBUG
    Serial.print("Received ");
    Serial.print(numberOfCommands);
    Serial.println(" commands");
#endif
    for (int i = 0; i < numberOfCommands; i++) {
        MotorStatus& command = commands[i];

#ifdef DEBUG
        Serial.println("Received: ");
        Serial.print("- id: ");
        Serial.println(command.motorId, DEC);
        Serial.print("- direction: ");
        Serial.println((char) command.direction, DEC);
        Serial.print("- speed: ");
        Serial.println(command.speed, DEC);
#endif

        if (command.motorId > numberOfMotors - 1) {
#ifdef DEBUG
            Serial.println("Received command was invalid. Discarding command.");
#endif
            clearWire();
            return;
        }

        Motor& m = motors[command.motorId];

        m.targetDirection = command.direction;
        m.targetSpeed = command.speed;
    }
}

