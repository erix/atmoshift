#include <ShiftBrite.h>

#define clockpin 13 // CI
#define enablepin 10 // EI
#define latchpin 9 // LI
#define datapin 11 // DI


#define MAX_STEP 1535
//number of modules in the chain
#define NumLEDs 5 
//order is reversed, first byte is last module in the chain
//0=BL
//1=TLC
//2=TL
//3=TR
//4=TRC
//5=BR
byte channelorder[NumLEDs]={4,3,2,1,0};
int gammatable[]={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,4,4,4,5,5,6,6,6,7,7,8,8,9,10,10,11,12,12,13,14,14,15,16,17,18,19,19,20,21,22,23,24,26,27,28,29,30,31,33,34,35,36,38,39,41,42,44,45,47,48,50,52,53,55,57,58,60,62,64,66,68,70,72,74,76,78,80,82,85,87,89,92,94,96,99,101,104,106,109,112,114,117,120,123,125,128,131,134,137,140,143,146,149,153,156,159,162,166,169,172,176,179,183,187,190,194,198,201,205,209,213,217,221,225,229,233,237,241,246,250,254,258,263,267,272,276,281,286,290,295,300,305,310,314,319,324,329,335,340,345,350,355,361,366,372,377,383,388,394,400,405,411,417,423,429,435,441,447,453,459,465,472,478,484,491,497,504,510,517,524,530,537,544,551,558,565,572,579,586,593,601,608,615,623,630,638,645,653,661,668,676,684,692,700,708,716,724,732,740,749,757,765,774,782,791,800,808,817,826,835,844,853,862,871,880,889,898,908,917,926,936,945,955,965,974,984,994,1004,1014,1023};


// this array contains the shiftbrite string
int LEDChannels[NumLEDs * 3] = {0};

// serial buffer for incomming ATMO command
int incomingatmo[255];

ShiftBrite sb(datapin, latchpin, enablepin, clockpin, NumLEDs);

void setup()
{
	Serial.begin(38400);	
	sb.sendColour(1023, 150, 0);
}

// reads the ATMO RGB array from serial
bool ReadAtmo(int *command) {
	int buffer=0;
	int channels;
	bool retval=false;

	if (command == NULL) return false;

	buffer = Serial.read();
	if(buffer==0xff)
	{
		while(Serial.available()==0){ }
		buffer=Serial.read();
		if(buffer==0x00)
		{
			while(Serial.available()==0){ }
			buffer=Serial.read();
			if(buffer==0x00)
			{
				while(Serial.available()==0){ }
				channels = Serial.read(); // how many channels we have
				//Serial.println(channels);
				for(int i = 0; i < channels; i++)
				{					
					while(Serial.available()==0){ }
					command[i]=Serial.read();
				}
				retval=true;
			}
		}
	}
	return retval;
}

// converts the RGB array to shiftbrite array
void BuildLEDArray(int *rgbArray)
{
	for (int channel = 0; channel<NumLEDs; channel++)
	{           
		LEDChannels[channel * 3 + 0]=gammatable[rgbArray[(channelorder[channel]*3)+0]]; //red
		LEDChannels[channel * 3 + 1]=gammatable[rgbArray[(channelorder[channel]*3)+1]]; //green
		LEDChannels[channel * 3 + 2]=gammatable[rgbArray[(channelorder[channel]*3)+2]]; //blue             
	}
}

void loop()
{
	// check the serial

	if(Serial.available()>0)
	{
		if (ReadAtmo(incomingatmo)) {
			BuildLEDArray(incomingatmo);
			sb.sendColour(LEDChannels);
		}
	} 
	else
	{
		delay(50);
	}
}
