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
#include "CommandBuffer.h"
#include "Mover.h"

Mover g_hoopMover;
CommandBuffer<20> g_commandBuffer;

bool wait = false;
bool isCommandComplete{};

int serialData = -1; // means ready to read

void setup()
{
  Serial.begin(9600);
}

void loop()
{

  isCommandComplete = false;
  if ((serialData != -1) && !g_commandBuffer.isFull())
  {
    isCommandComplete = g_commandBuffer.push(serialData);
    serialData = -1; // reset
  }

  if (g_commandBuffer.isFull() && !wait)
  {
    // this is the case, when a command has completed and the buffer got full
    putchar('!'); // signal buffer full
    wait = true;
  }
  else if (!g_commandBuffer.isFull() && wait && (serialData == -1))
  {
    // this is the case when the buffer was previosly full and the caller is waiting for sending more data.
    // but before we let the caller send more data ... read the pending data (if any) from the serial buffer.
    putchar('+'); // signal there is space left
    wait = false;
  }
  else if (isCommandComplete)
  {
    putchar('>'); // normal ACK
  }

  if (!g_hoopMover.isRunning() && !g_commandBuffer.isEmpty())
  {
    auto &command = g_commandBuffer.pop();
    switch (command.command)
    {
    case Command::move:
      g_hoopMover.moveTo(command.var0, command.var1, command.var3, command.var4);
      break;
    case Command::disable_steppers:
      g_hoopMover.disable();
      putchar('<'); // signal we are finished
      break;
    case Command::enable_steppers:
      g_hoopMover.enable();
      break;
    default:
      break;
    }
  }
  g_hoopMover.run();
}