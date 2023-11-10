
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#define MOTOR_PWM  PB3
#define MOTOR_DIR  PB5
#define MOTOR_EN   PB6




typedef enum{
	MOTOR_ON_CW,
	MOTOR_ON_CCW,
	MOTOR_OFF
	}MOTOR_MODES;




void motor_state(MOTOR_MODES mode);





#endif /* DC_MOTOR_H_ */