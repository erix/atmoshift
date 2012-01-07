/*
	HughesyShiftBrite.h - A library to run shiftbrites
	Created by Ashley J. Hughes, 14 Jun 2010.
	ashley.hughes@me.com
	Based on a post found @ http://macetech.com/blog/node/54
*/

#ifndef ShiftBrite_h
#define ShiftBrite_h

#include "Arduino.h"

class ShiftBrite
{
public:
	ShiftBrite();
	ShiftBrite(int dataPin, int latchPin, int enablePin, int clockPin, int lights=1);
	void sendColour(int r, int g, int b);
	void sendColour(int *rgbArray);
private:
	int _SB_CommandMode;
	int _SB_RedCommand;
	int _SB_GreenCommand;
	int _SB_BlueCommand;
	int _dPin;
	int _lPin;
	int _ePin;
	int _cPin;
	int _lights;
	unsigned long _SB_CommandPacket;
	void _SB_SendPacket();
	void _sendVoltage();
	void _sendColour(int r, int g, int b);
	void _latch();
	
};

#endif
