script.system("echo 2 > /sys/class/misc/l3g42xxd/odr_selection");

var __interpretation_started_timestamp__;
var gyro = brick.gyroscope();
var tilt = gyro.read()[5] / 100;
var pi = 3.1415926535897931;
var middleError = 0;
var calibrateTime = 20000;


gyro.calibrate(calibrateTime);

var calc_bias_error = function() {
	var myTime = 0;
	var sumOfAngles = 0;
	while (myTime <= calibrateTime / 10) {
		sumOfAngles += gyro.read()[2];
		myTime ++;
		script.wait(10);
	}
	// 8.75 * 10 == 
	middleError = sumOfAngles * 87.5 / calibrateTime; //mdps
	print(middleError, " == middle error---------------------");
}

var calc_gyro5 = function() {
	var angle_speed = gyro.read()[2] * 8.75; //mdps
	tilt = tilt * 180 / pi;
	tilt += (angle_speed - middleError) * 0.05 / 1000; 
	tilt = tilt * pi / 180;
}

var main = function()
{
	__interpretation_started_timestamp__ = Date.now();
	calc_bias_error();
	print("-------------------------------------------");
	tilt = gyro.read()[5] / 100;
	while (true) {
		calc_gyro5();
		print(gyro.read()[5] / 100, " / ", tilt);
		print();
		script.wait(50);
	}
	return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
