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

#pragma once
#include <stdint.h>
#include "SimpleStepper.h"

// convinience class for hoop positioning
// ... no acceleration needed?! just move as fast as possible to the next position
// 
class Mover
{
public:
    Mover();

    // mm and mm/s
    void moveTo(float x, float y, uint32_t stepsMachine, float speed);

    // setting the current position ... wherever the hoop currently is
    void setPosition(float x, float y);

    // enable/disable Stepperdriver
    void enable();
    void disable();

    bool isRunning() const;

    // has to be called frequently ... fast !!!
    void run();

private:
    SimpleStepper _stepperX;
    SimpleStepper _stepperY;
    SimpleStepper _machineMotor;
};
