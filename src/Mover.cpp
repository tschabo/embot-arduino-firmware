// embot-firmware - Firmware for a DIY embroidery machine
// Copyright (C) 2021 Sebastian Schmitt
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Mover.h"

// nano cnc shield
#define STEPPER_X_DIR_PIN 2
#define STEPPER_X_STEP_PIN 5
#define STEPPER_X_ENABLE_PIN 8

#define STEPPER_Y_DIR_PIN 3
#define STEPPER_Y_STEP_PIN 6
#define STEPPER_Y_ENABLE_PIN 8

#define STEPPER_MACHINE_DIR_PIN 4
#define STEPPER_MACHINE_STEP_PIN 7
#define STEPPER_MACHINE_ENABLE_PIN 8

Mover::Mover() : _stepperX(SimpleStepper(STEPPER_X_ENABLE_PIN, STEPPER_X_DIR_PIN, STEPPER_X_STEP_PIN, 50000)),
                 _stepperY(SimpleStepper(STEPPER_Y_ENABLE_PIN, STEPPER_Y_DIR_PIN, STEPPER_Y_STEP_PIN, 50000)),
                 _machineMotor(SimpleStepper(STEPPER_MACHINE_ENABLE_PIN, STEPPER_MACHINE_DIR_PIN, STEPPER_MACHINE_STEP_PIN, 50000))
{
    _stepperX.disable();
    _stepperY.disable();
    _machineMotor.disable();
}

void Mover::moveTo(float x, float y, uint32_t stepsMachine, float speed)
{
    // coreXY
    auto x_ = (x - y) * 2;
    auto y_ = (x + y) * 2;
    _stepperX.moveAbs(x_);
    _stepperY.moveAbs(y_);
    _machineMotor.moveSteps(stepsMachine, SimpleStepper::direction::neg);

    SimpleStepper *steppers[]{&_machineMotor, &_stepperX, &_stepperY};

    auto mostSteps = steppers[0];
    for (uint8_t c = 1; c < sizeof(steppers) / sizeof(SimpleStepper *); c++)
    {
        if (steppers[c]->stepsToGo() > mostSteps->stepsToGo())
            mostSteps = steppers[c];
    }
    mostSteps->setSpeed(speed);
    for (uint8_t c = 0; c < sizeof(steppers) / sizeof(SimpleStepper *); c++)
    {
        auto current = steppers[c];
        if (current == mostSteps)
            continue;
        current->setSpeed((float)current->stepsToGo() / (float)mostSteps->stepsToGo() * speed);
    }
}

void Mover::setPosition(float x, float y)
{
    _stepperX.setPosition(static_cast<long>(x));
    _stepperY.setPosition(static_cast<long>(y));
}

void Mover::enable()
{
    _stepperX.enable();
    _stepperY.enable();
    _machineMotor.enable();
}

void Mover::disable()
{
    _stepperX.disable();
    _stepperY.disable();
    _machineMotor.disable();
}

bool Mover::isRunning() const
{
    return _stepperX.stepsToGo() != 0 || _stepperY.stepsToGo() != 0 || _machineMotor.stepsToGo() != 0;
}

void Mover::run()
{
    if (!isRunning())
        return;

    _stepperX.run();
    _stepperY.run();
    _machineMotor.run();
}