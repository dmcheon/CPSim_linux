var:2
float accel;
float brake;

input:0

output:0

CAN_input:0

CAN_output:1
10:0x7FF:2
accel ACCEL
brake BRAKE

code:
{
	float speed = internal_data[0];
    brake = 0.0;
    if(speed < 40.0)
        accel = 1.0;
    else if(speed < 60.0)
        accel = 0.5;
    else if(speed < 80.0)
        accel = 0.2;
    else
        accel = 0.0;
}
