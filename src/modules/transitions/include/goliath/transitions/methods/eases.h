#pragma once

/**
 * @file eases.h
 * @author Group 7 - Informatica
 */

 /**
  * @namespace goliath::transitions::methods
  * @brief Contains methods for transitioning a phase
  */

namespace goliath::transitions::methods {
    double easeInQuad(double value);
    double easeOutQuad(double value);
    double easeInOutQuad(double value);
    double easeInCubic(double value);
    double easeOutCubic(double value);
    double easeInOutCubic(double value);
    double easeInQuart(double value);
    double easeOutQuart(double value);
    double easeInOutQuart(double value);
    double easeInQuint(double value);
    double easeOutQuint(double value);
    double easeInOutQuint(double value);

    double linear(double value);
}
