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

#include "Arduino.h"
#include "SimpleStepper.h"
#include "limits.h"

SimpleStepper::SimpleStepper(int enablePin,
                             int dirPin,
                             int stepPin,
                             uint32_t stepsPerM) : _enablePin(enablePin),
                                                   _dirPin(dirPin),
                                                   _stepPin(stepPin),
                                                   _stepsPerM(stepsPerM)
{
    pinMode(_enablePin, OUTPUT);
    digitalWrite(_enablePin, 1); // disable is default

    pinMode(_dirPin, OUTPUT);
    digitalWrite(_dirPin, 0);

    pinMode(_stepPin, OUTPUT);
    digitalWrite(_stepPin, 0);
}

void SimpleStepper::enable() const
{
    digitalWrite(_enablePin, 0);
}

void SimpleStepper::disable() const
{
    digitalWrite(_enablePin, 1);
}

uint32_t SimpleStepper::stepsToGo() const
{
    return _stepsToGo;
}

void SimpleStepper::moveAbs(float pos)
{
    int32_t posInStepsFromZero{(pos * (float)_stepsPerM) / 1000.0};
    if (posInStepsFromZero > _position)
    {
        _stepsToGo = posInStepsFromZero - _position;
        _direction = direction::pos;
    }
    else if (posInStepsFromZero < _position)
    {
        _stepsToGo = _position - posInStepsFromZero;
        _direction = direction::neg;
    }
    else
    {
        _stepsToGo = 0;
    }
    digitalWrite(_dirPin, (_direction == direction::pos) ? 1 : 0);
}

void SimpleStepper::moveSteps(uint32_t steps, direction dir)
{
    _position = 0;
    _direction = dir;
    _stepsToGo = steps;
}

void SimpleStepper::setSpeed(float speed)
{
    _speed = (speed * (float)_stepsPerM) / 1000; // ticks/sec
    _stepInterval = 1000000 / _speed;
}

void SimpleStepper::setPosition(float pos)
{
    _position = (pos * (float)_stepsPerM) / 1000.0;
}

void SimpleStepper::run()
{
    if (_stepsToGo == 0)
        return;

    auto current = micros();

    if(current < _lastStepTime)
    {
        // overflow
        _lastStepTime -= ULONG_MAX;
    }

    if ((current - _lastStepTime) >= _stepInterval)
    {
        // step !!!
        digitalWrite(_stepPin, 1);
        delayMicroseconds(2);
        digitalWrite(_stepPin, 0);
        switch (_direction)
        {
        case direction::neg:
            --_position;
            break;
        default:
            ++_position;
            break;
        }
        --_stepsToGo;
        _lastStepTime = current;
    }
}