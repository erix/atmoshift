/*
	HughesyShiftBrite.cpp - A library to run shiftbrites
	Created by Ashley J. Hughes, 14 Jun 2010.
	ashley.hughes@me.com
	Based on a post found @ http://macetech.com/blog/node/54
*/

#include "ShiftBrite.h"

ShiftBrite::ShiftBrite()
{
	ShiftBrite(10,11,12,13);
}

ShiftBrite::ShiftBrite(int dataPin, int latchPin, int enablePin, int clockPin, int lights)
{
	_dPin = dataPin;
	_lPin = latchPin;
	_ePin = enablePin;
	_cPin = clockPin;
	_lights = lights;
	pinMode(_dPin, OUTPUT);
	pinMode(_lPin, OUTPUT);
	pinMode(_ePin, OUTPUT);
	pinMode(_cPin, OUTPUT);
	
	digitalWrite(_lPin, LOW);
	digitalWrite(_ePin, LOW);
}

/*
Sends the same color to all lights
*/
void ShiftBrite::sendColour(int r, int g, int b)
{
	for (int i = 0; i < _lights; i++)
	{
		_sendColour(r, g, b);		
	}
	_latch();

	for (int i = 0; i < _lights; i++)
	{
		_sendVoltage();	
	}
	_latch();	
}

/*
Sends the color for the lights given in the RGB array
*/
void ShiftBrite::sendColour(int *rgbArray)
{
	for (int i = 0; i < _lights; i++)
	{
		_sendColour(rgbArray[3*i + 0], rgbArray[3*i + 1], rgbArray[3*i + 2]);
	}
	_latch();

	for (int i = 0; i < _lights; i++)
	{
		_sendVoltage();	
	}
	_latch();
}

void ShiftBrite::_sendColour(int r, int g, int b)
{
	if (r <= 1023 && g <= 1023 && b <= 1023) 
	{
		_SB_CommandMode = B00;
		_SB_RedCommand = r;
		_SB_GreenCommand = g;
		_SB_BlueCommand = b;
		_SB_SendPacket();
	}
}

void ShiftBrite::_sendVoltage()
{
	_SB_CommandMode = B01;
	_SB_RedCommand = 127;
	_SB_GreenCommand = 127;
	_SB_BlueCommand = 127;
	_SB_SendPacket();	
}

void ShiftBrite::_SB_SendPacket() 
{
  _SB_CommandPacket = _SB_CommandMode & B11;
  _SB_CommandPacket = (_SB_CommandPacket << 10)  | (_SB_BlueCommand & 1023);
  _SB_CommandPacket = (_SB_CommandPacket << 10)  | (_SB_RedCommand & 1023);
  _SB_CommandPacket = (_SB_CommandPacket << 10)  | (_SB_GreenCommand & 1023);

  shiftOut(_dPin, _cPin, MSBFIRST, _SB_CommandPacket >> 24);
  shiftOut(_dPin, _cPin, MSBFIRST, _SB_CommandPacket >> 16);
  shiftOut(_dPin, _cPin, MSBFIRST, _SB_CommandPacket >> 8);
  shiftOut(_dPin, _cPin, MSBFIRST, _SB_CommandPacket);
}

void ShiftBrite::_latch()
{
  delayMicroseconds(15);
  digitalWrite(_lPin,HIGH);
  delayMicroseconds(15);
  digitalWrite(_lPin,LOW);
}
