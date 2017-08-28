var:0

input:0

output:1
internal_data[4]

CAN_input:1				//you don't need to change this part
0:0x7FD:1
car_output[SPEED]

CAN_output:0

code:
{
	internal_data[4] = car_output[SPEED];

	previous_data += 1;
	printf("%f, %f, %f, %f\n", car_output[TRACK_TYPE], car_output[TRACK_RADIUS], car_output[TRACK_FRICTION], car_output[TRACK_NUM]);
	printf("%f\n", previous_data);
	return previous_data;
}
