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

struct Command
{
    enum
    {
        none = 0,
        move = 'm',
        enable_steppers = 'e',
        disable_steppers = 'd',
    } command{none};
    float var0{};
    float var1{};
    uint32_t var3{};
    float var4 {};
};

template <uint8_t SIZE>
class CBuffer
{
public:
    bool push_back(char c)
    {
        if (full())
            return false;
        _buffer[_size] = c;
        ++_size;
        return true;
    }

    void clear()
    {
        for (int i = 0; i < SIZE; i++)
        {
            _buffer[i] = 0;
        }
        _size = 0;
    }

    uint8_t size() const
    {
        return _size;
    }

    bool full() const
    {
        return _size == SIZE;
    }

    char *data()
    {
        return &_buffer[0];
    }

private:
    char _buffer[SIZE]{};
    uint8_t _size{};
};

class Parser
{
public:
    Parser();

    // feeding bytewise ... returns Command if the Parser found a complete Command otherwise nullptr
    Command* push(char c);

private:
    enum
    {
        findStart,
        evalCommand,
        findX,
        findY,
        findMachineTicks,
        findSpeed,
    } _state{findStart};

    CBuffer<50> _scratch_buffer{};
    Command _currentCommandBuffer{};
    void parseCommand(char command);
    bool parseFloat(char floatPart, float &thePlaceToPut); // returns true if finished parsing
    bool parseUInt(char uintPart, uint32_t &thePlaceToPut); // returns true if finished parsing
    bool _finished{false};
};
