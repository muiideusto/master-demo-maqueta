#include "Stepper.h"

Stepper::Stepper(PinName step_pin, PinName dir_pin, PinName enable_pin)
    : step(step_pin), dir(dir_pin), enable(enable_pin) {
  step = 0;
  dir = 0;
  enable = 0;
}

/**
 *
 */
void Stepper::enableStepper(void) {
  enable = 1;
  wait_us(100);
}

/**
 *
 */
void Stepper::disableStepper(void) {
  enable = 0;
  wait_us(100);
}

/**
 *
 */
void Stepper::doStep(int direcction) {
  dir = direcction;
  step = 1;
  wait_us(100);
  step = 0;
  wait_us(900);
}

/**
 *
 */
void Stepper::doStepForward() { doStep(1); }

/**
 *
 */
void Stepper::doStepBackward() { doStep(0); }

/**
 *
 */
void Stepper::doStepsForward(int steps) {}

/**
 *
 */
void Stepper::doStepsBackward(int steps) {}

/**
 *
 */
void Stepper::goForward() {}

/**
 *
 */
void Stepper::goBackward() {}

/**
 *
 */
void Stepper::doSteps(int direcction, int steps) {}

/**
 *
 */
void Stepper::go(int direcction) {}
