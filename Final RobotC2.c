const int RGBTOL = 5;
long colorsRGB [11][3] = {{72, 15, 12}, {0, 0, 0},{81, 69, 6},{38, 14, 10}, {69, 17, 6},{75, 27, 6}, {55, 73, 8},{9, 40, 24}, {15, 41, 6},{7, 9, 3},
	{0,0,0}};
int Position [5] = {120, 0, 30, 60, 90};
int playerPoints[5] = {0,0,0,0,0};
bool aceTrack[5] = {false, false, false, false, false};

void rotate(int player)
{
	int degree = 0;
	degree = Position [player];
	motor[motorD] = 15;
	while (nMotorEncoder[motorD] < degree)
	{}
	motor[motorD] = -15;
	while (nMotorEncoder[motorD] > degree)
	{}
	motor[motorD] = 0;
	wait1Msec(1000);
}

void aceShift (int player)
{
	if (playerPoints[player] > 21 && aceTrack[player])
	{
		playerPoints[player] -= 10;
		aceTrack [player] = false;
	}
			displayBigTextLine(3,"1                   ");
			displayBigTextLine(5,"player: %d : %d", player, playerPoints[player]);
}

int colorValue (int red, int blue, int green)
{
	int index = -1;
	for (int color = 0; color < 11; color ++)
	{
		if (fabs(colorsRGB[color][0] - red) <= RGBTOL && fabs(colorsRGB[color][1] - blue) <= RGBTOL && fabs(colorsRGB[color][2] - green) <= RGBTOL)
			index = color;
	}
	return index + 1;
}

void calibration ()
{
	displayBigTextLine(1, "Calibrating");
	for (int index = 0; index < 11; index++)
	{

		long redValue = 0, blueValue = 0, greenValue = 0;
		long redSum = 0;
		long blueSum = 0;
		long greenSum = 0;
		for (int i = 0; i < 5; i++)
		{
			getColorRGB(S2, redValue, blueValue, greenValue);
			getColorRGB(S2, redValue, blueValue, greenValue);
			redSum += redValue;
			blueSum += blueValue;
			greenSum += greenValue;
		}
		colorsRGB[index][0] = redSum/5;
		colorsRGB[index][1] = blueSum/5;
		colorsRGB[index][2] = greenSum/5;

		wait1Msec(500);
		nMotorEncoder[motorA] = 0;
		motor[motorA] = -100;
		while (fabs(nMotorEncoder[motorA]) < 27)
		{}
		motor[motorA] = 10;
		wait1Msec(50);
		motor[motorA] = 0;
		wait1Msec(50);
		motor[motorA] = 5;
		wait1Msec(500);
		motor[motorA] = 0;
		wait1Msec(2000);
	}
	eraseDisplay();
	displayBigTextLine(5," Press Enter");
	while (!getButtonPress(buttonEnter))
	{}
	while (getButtonPress(buttonEnter))
	{}
}

void dealCard (int player)
{
	long redValue = 0, blueValue = 0, greenValue = 0;
	int points = 0;
	do{
		getColorRGB(S2, redValue, blueValue, greenValue);
		getColorRGB(S2, redValue, blueValue, greenValue);
		points = colorValue (redValue, blueValue, greenValue);
		playerPoints[player] += points;
		if (points == 11)
			aceTrack[player] = true;
		if(points == 0)
		{
			displayBigTextLine(3, "DISCARD CARD");
		}
		else
		{
			displayBigTextLine(3,"%d                   ", points);
			displayBigTextLine(5,"player: %d : %d", player, playerPoints[player]);
		}
		nMotorEncoder[motorA] = 0;
		motor[motorA] = -100;
		while (fabs(nMotorEncoder[motorA]) < 27)
		{}
		motor[motorA] = 10;
		wait1Msec(50);
		motor[motorA] = 0;
		wait1Msec(50);
		motor[motorA] = 5;
		wait1Msec(500);
		motor[motorA] = 0;
		wait1Msec(2000);
	}while(points == 0);
}

void startGame(int numPlayer)
{
	for(int turn = 1; turn <= numPlayer; turn++)
	{
		rotate(turn);
		dealCard (turn);
		wait1Msec(1000);
		dealCard (turn);
	}
	rotate(0);
	dealCard (0);
	wait1Msec(1000);
	dealCard (0);
}

int menu()
{
	displayString(5, "Would you like a game of");
	displayBigTextLine(7, "BLACKJACK");

	while (!getButtonPress(buttonEnter))
	{}
	while (getButtonPress(buttonEnter))
	{}

	eraseDisplay();
	displayString(3, "How many players do you have?");
	displayString(5, "Use left or right arrow to ");
	displayString(6, "select the number of players");

	int numPlayer = 1;

	while(!getButtonPress(buttonEnter))
	{
		displayBigTextLine (8,"   %i", numPlayer);
		if (getButtonPress(buttonLeft))
		{
			numPlayer --;
			if (numPlayer <= 1)
				numPlayer = 1;
			while(getButtonPress(buttonLeft))
			{}
		}
		else if (getButtonPress(buttonRight))
		{
			numPlayer ++;
			if (numPlayer >= 4)
				numPlayer = 4;
			while(getButtonPress(buttonRight))
			{}
		}
	}

	while(getButtonPress(buttonEnter))
	{}

	eraseDisplay();
	displayBigTextLine(7, "Game in Process");

	return numPlayer;
}

void configureSensors ()
{
	SensorType[S1] = sensorEV3_Touch;
	SensorType[S2] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S2] = modeEV3Color_Color;
	wait1Msec(50);
	long redValue = 0, greenValue = 0, blueValue = 0;
	getColorRGB(S2, redValue, greenValue, blueValue);
	nMotorEncoder[motorD] = 0;
}

void dealerDraw ()
{
	while (playerPoints[0] < 17)
	{
		rotate (0);
		dealCard (0);
		aceShift (0);
	}
}

bool detWinner (int player)
{
	if(playerPoints[player] > 21)
		return false;
	if(playerPoints[player] > playerPoints[0] || playerPoints[0] > 21)
		return true;
	return false;
}

void additionalCard(int numPlayer)
{
	for (int player = 1; player <= numPlayer; player++)
	{
		rotate (player);
		while(SensorValue[S3] == 0 && playerPoints[player] <= 21)
		{
			if (SensorValue[S1] == 1)
			{
				dealCard (player);
				aceShift (player);
			}
		}
	}
}

void disPlayer(int numPlayer)
{
	eraseDisplay();
	int line = 0;
	for (int player = 1; player <= numPlayer; player++)
	{
		bool win = false;
		win = detWinner (player);
		line += 3;
		if (win)
			displayString(line, "Player %d WON", player);
		else
			displayString(line, "Player %d LOST", player);
	}
}

void waitForTouch ()
{
	displayBigTextLine(1, "Hit me to start");
	while(SensorValue[S1] == 0)
	{}
	while(SensorValue[S1] == 1)
	{}
	eraseDisplay();
}

task main()
{
	configureSensors();
	waitForTouch();
	calibration();
	eraseDisplay();
	int numPlayer = 0;
	numPlayer = menu();
	startGame(numPlayer);
	additionalCard(numPlayer);
	dealerDraw();
	disPlayer(numPlayer);
	wait1Msec(30000);
}
