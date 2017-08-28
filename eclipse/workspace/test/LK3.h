var:1
float steering;

input:0

output:2
internal_data[5]
internal_data[6]

CAN_input:0

CAN_output:1
10:0x7FE:1
steering STEER

code:
{
    float trackWidth = internal_data[0];
    float distance = internal_data[1];
    float track_angle = internal_data[2];
    float yaw = internal_data[3];
    float speed = internal_data[4];

    float trackType = car_output[TRACK_TYPE];
    float trackRadius = car_output[TRACK_RADIUS];
    float trackFriction = car_output[TRACK_FRICTION];

    float nextTrackType = car_output[TRACK_TYPE2];
	float nextTrackRadius = car_output[TRACK_RADIUS2];
	float nextTrackFriction = car_output[TRACK_FRICTION2];

	float twoNextTrackType = car_output[TRACK_TYPE3];

    if (trackWidth == 0.0 || trackWidth > 10000) {
		steering = 0.0;
    	internal_data[5] = 0.0;
        internal_data[6] = 0.0;
        return 0;
    } else {
    	float targetSteering = 0.0;
    	float targetAccel = 0.0;
		float targetBrake = 0.0;

		float angle = track_angle - yaw;

		float pi = 3.141592;
		while (angle > pi)
			angle -= 2.0*pi;
		while (angle < -pi)
			angle += 2.0*pi;

		float absoluteAngle = angle > 0 ? angle : -angle;
		float absoluteDistance = distance > 0 ? distance : -distance;

		float targetSpeed = 90.0;

		float frictionWeight = 1.0;
		if (trackFriction >= 1.0) {
			frictionWeight = frictionWeight + (trackFriction - 1.0) * 1.5;
		} else {
			frictionWeight = frictionWeight + (trackFriction - 1.0) * 2.0;
		}

		float radiusWeight = 1.0;
		if (trackRadius <= 101) {
			radiusWeight = 0.7;
		} else if (trackRadius <= 201) {
			radiusWeight = 0.8;
		} else if (trackRadius > 0) {
			radiusWeight = 0.9;
		} else {
			radiusWeight = 1.0;
		}

		float widthWeight = 1.0;
		if (trackWidth <= 11) {
			widthWeight = 0.9;
		}

		float trackTypeWeight = 1.0;
		if (trackType == 3 && nextTrackType == 3 && twoNextTrackType == 3 && absoluteDistance < 0.5 && absoluteAngle <= pi/8) {
			if (trackFriction > 1.09) {
				trackTypeWeight = 1.5;
			} else {
				trackTypeWeight = 1.1;
			}
		} else if (trackType != nextTrackType || trackType != twoNextTrackType) {
			trackTypeWeight = 0.4;
		}

		targetSpeed = 80 * frictionWeight * radiusWeight * widthWeight * trackTypeWeight;

		if (targetSpeed > 120.0) {
			targetSpeed = 120.0;
		}

		// calculate steering value
		float cornering = trackRadius * pi / 360 * 0.12;
		float stabilize = 0.04 * distance;

		if (trackType != nextTrackType) {
			cornering = cornering * 0.8;
			stabilize = stabilize * 1.2;
		} else  if ((trackType == 1 && distance < 0) || (trackType == 2 && distance > 0)) {
			cornering = cornering / (absoluteDistance * 0.5 + 1);
			stabilize = stabilize * 1;
		} else {
			cornering = cornering;
			stabilize = stabilize;
		}
		if (trackType == 1) {
			targetSteering = angle - cornering - stabilize;
		} else if (trackType == 2) {
			targetSteering = angle + cornering - stabilize;
		} else {
			targetSteering = angle - stabilize;
		}

		// calculate accel value
		float speedDiff = targetSpeed - speed;
		// float remainedDistance = trackWidth - absoluteDistance * 2;
		targetAccel = speedDiff / 10.0;
		if (targetAccel > 1.0) {
			targetAccel = 1.0;
			targetBrake = 0.0;
		} else if (targetAccel < -1.0) {
			targetAccel = 0.0;
			targetBrake = -targetAccel;
		}

		if (speed < 0) {
			targetSteering = -targetSteering;
		}
		steering = targetSteering;
		internal_data[5] = targetAccel;
		internal_data[6] = targetBrake;

		printf("----------------------\n");
		printf("trackType: %f\n", trackType);
		printf("trackRadius: %f\n", trackRadius);
		printf("tractFriction: %f\n", trackFriction);
		printf("trackWidth: %f\n", trackWidth);
		printf("distance: %f\n", distance);
		printf("targetSpeed: %f\n", targetSpeed);
		printf("targetSteering: %f\n", targetSteering);
    }

    return 0;
}
