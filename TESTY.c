//LONGCHEN NIU and Yathindra Shivakumar
#include "PC_FileIO.c"

void writeRGB(TFileHandle fout, string paper, long redValue, long greenValue, long blueValue)
{
	writeTextPC(fout, paper);
	writeCharPC(fout, ' ');
	writeLongPC(fout, redValue);
	writeCharPC(fout, ' ');
	writeLongPC(fout, greenValue);
	writeCharPC(fout, ' ');
	writeLongPC(fout, blueValue);
	writeEndlPC(fout);
}


task main ()
{
	TFileHandle fout;
	bool okay = openWritePC(fout, "colourRGB3.txt");
	SensorType[S2] = sensorEV3_Color;
	wait1Msec (50);
	SensorMode[S2] = modeEV3Color_Color;
	wait1Msec(50);
	string paperColor = "bob ";
	long redValue = 0, greenValue = 0, blueValue = 0;
	getColorRGB(S2, redValue, greenValue, blueValue);
	while (getButtonPress(buttonLeft) == 0 && getButtonPress(buttonRight) == 0 && getButtonPress(buttonEnter) == 0)
	{}
	while (getButtonPress(buttonEnter) == 0)
	{
		if (getButtonPress(buttonLeft) == 1)
		{
			while(getButtonPress(buttonLeft) == 1)
			{}
			paperColor = "pink";
			getColorRGB(S2, redValue, greenValue, blueValue);
			writeRGB(fout, paperColor, redValue, greenValue, blueValue);
		}
		else if (getButtonPress(buttonRight) == 1)
		{
			while(getButtonPress(buttonRight) == 1)
			{}
			paperColor = "orange";
			getColorRGB(S2, redValue, greenValue, blueValue);
			writeRGB(fout, paperColor, redValue, greenValue, blueValue);
		}
	}
	closeFilePC(fout);
}
