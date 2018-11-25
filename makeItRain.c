
task main()
{
	wait1Msec(2000);
	motor[motorA] = -100;
	wait1Msec(7000);
	motor[motorA] = 0;
}
