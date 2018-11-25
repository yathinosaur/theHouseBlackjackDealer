
task main()
{
	for (int card = 0; card < 52; card++)
	{
	motor[motorB] = -25;
	nMotorEncoder[motorB] = 0;
	while(nMotorEncoder[motorB] > -5)
	{}
	motor[motorB] = 0;
	wait1Msec(1000);
}}
