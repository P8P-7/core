#include <goliath/servo/repositories/wing_state.h>
#include <cmath>
#include <stdexcept>
#include <limits>

using namespace goliath::servo;

WingState::WingState(const size_t &numberOfSectors)
        : numberOfSectors(numberOfSectors), angle(0), sector(0), speed(0), mode(ServoMode::JOINT),
          isTrackingLost(true) {}

WingState::WingState(const WingState &other)
        : numberOfSectors(other.numberOfSectors), angle(other.angle), sector(other.sector), speed(other.speed),
          mode(other.mode), isTrackingLost(true) {}

double WingState::getWorldAngle() const {
    return sector * 360.0 / getNumberOfSectors() + angle / getNumberOfSectors();
}

double WingState::getAngle() const {
    return angle;
}

size_t WingState::getSector() const {
    return sector;
}

size_t WingState::getNumberOfSectors() const {
    return numberOfSectors;
}

double WingState::getRatio() const {
    return 1.0 / numberOfSectors;
}

void WingState::setWorldAngle(double newAngle) {
    angle = getAngleFromWorldAngle(newAngle);
    sector = getSectorFromWorldAngle(newAngle);
}

void WingState::reset(double newAngle) {
    angle = newAngle;
    sector = 0;

    isTrackingLost = false;
}

bool WingState::hasLostTracking() const {
    return isTrackingLost;
}

void WingState::setMode(ServoMode newMode) {
    isTrackingLost = mode != newMode;

    if (isTrackingLost) {
        angle = 0;
        sector = 0;
    }

    mode = newMode;
}

ServoMode WingState::getMode() const {
    return mode;
}

void WingState::setAngle(double newAngle) {
    angle = newAngle;
}

void WingState::setSector(size_t newSector) {
    sector = newSector;
}

double WingState::getAngleFromWorldAngle(double angle) const {
    if (angle < 0 || angle > 360) {
        throw std::out_of_range("Angle out of bounds [0, 360]");
    }

    return (std::fmod(angle, 360.0 / getNumberOfSectors())) * getNumberOfSectors();
}

size_t WingState::getSectorFromWorldAngle(double angle) const {
    if (angle < 0 || angle > 360) {
        throw std::out_of_range("Angle out of bounds [0, 360]");
    }

    auto sector = static_cast<size_t>(std::floor(angle * getNumberOfSectors() / 360));
    return sector >= getNumberOfSectors() ? 0 : sector;
}

Direction WingState::getDirectionFromWorldAngle(double angle, double threshold) const {
    return std::abs(angle - getWorldAngle()) <= threshold ? Direction::CLOCKWISE : Direction::COUNTER_CLOCKWISE;
}

std::uint16_t WingState::getSpeed() const {
    return speed;
}

void WingState::setSpeed(std::uint16_t newSpeed) {
    speed = newSpeed;
}

size_t WingState::getAdjacentSector(size_t sector, servo::Direction direction) const {
    if (direction == servo::Direction::CLOCKWISE) {
        if (sector >= 1) {
            return sector - 1;
        }

        return getNumberOfSectors() - 1;
    }

    return sector + 1 >= getNumberOfSectors() ? 0 : sector + 1;
}

size_t WingState::getNextSector(servo::Direction direction) const {
    return getAdjacentSector(getSector(), direction);
}