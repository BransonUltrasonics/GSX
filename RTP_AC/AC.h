#pragma once
#include "ACStateMachine.h"
#include <time.h>
#include <taskLib.h>
#include "fioLib.h"
#include <usrFsLib.h>
#include "Servo.h"


#define TASK_SERVO_IDX 0
#define TASK_CONTROL_IDX 1
#define TASK_PDO_IDX 2
#define TASK_SDO_IDX 3
#define TASK_SIZE 4

#define ENCODER_RES 10//encoder pulses per µm
#define DEFAULT_SPEED 55 //mm/s
#define DEFAULT_ACCELERATION 500 //mm/s²
#define FULL_STROKE 120000 //µm
#define MAX_PART_CONTACT 120000 //µm
#define SEARCH_SPEED 15 //mm/s
#define PART_CONTACT_RATIO 4
#define MAX_PART_CONTACT_COMPRESSION 900 //µm
#define MIN_STROKE 5000 //µm
#define ACCEL_WINDOW (MIN_STROKE * 2 / 3) //this portion of the downstroke is ignored for part contact detection due to the jerk caused by acceleration
#define MAX_FORCE 2500 //N
#define MAX_COMPRESSION 12000 //µm
#define MAX_SERVO_ERROR 1000 //µm


class AC {
public:
	AC();
	~AC();
	static ACStateMachine stateMachine;
	static Servo servo;
};

