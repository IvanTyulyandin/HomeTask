script.system("echo 2 > /sys/class/misc/l3g42xxd/odr_selection");

var pi = 3.1415926535897931
var gyro = brick.gyroscope();
var readGyro = brick.gyroscope().read;
var calibrateTime = 30000;
var middleError = 0; // will be calculated in calc_bias_error

function calc_bias_error(){
	var myTime = 0;
	var sumOfAngles = 0;
	while (myTime <= calibrateTime / 30) {
		sumOfAngles += gyro.read()[2];
		myTime ++;
		script.wait(30);
	}
	// 8.75 * 10 * 3 == 87.5 * 3
	return sumOfAngles * 8.75 / myTime; //mdps
}

function calc_readGyro5(tilt) {
	var angle_speed = gyro.read()[2] * 8.75; //mdps
	tilt = tilt * 180 / pi;
	tilt += (angle_speed - middleError) * 0.05 / 1000;
	tilt = tilt * pi / 180;
	if (tilt > 3.14) {
		tilt = - 3.14 + (tilt - 3.14);
	}
	return tilt;
}

{
	gyro.calibrate(calibrateTime);
	script.wait(calibrateTime + 1000);
	middleError = calc_bias_error();
	
	brick.motor(M3).setPower(70);
	brick.motor(M4).setPower(-70);
	var startTime = new Date();
	var tilt = readGyro()[5] / 100;
	var myTilt = 0;
	
	print("--------------------------------------------------------------------------");
	print(middleError, " == middle error--------------------- ;");
	while (new Date - startTime < 3000) {
		tilt = readGyro()[5] / 100;
		myTilt = calc_readGyro5(myTilt);
		print(tilt.toFixed(2), " ", myTilt.toFixed(2), "DIFF = ", (tilt - myTilt));
	}
}
