#ifndef _SPEED_TEST_H
#define _SPEED_TEST_H


typedef struct
{
	double speed_x;
	double speed_y;
	double speed_rotation;
}Speed_Data;

void test_speed(void);
void go_stright(void);
void pid_set_A(void);
void my_goline(Speed_Data speed_mes);
extern int GOLINEFLAG;
extern struct PID pid_goline_A;
extern struct PID pid_goline_D_x;
void pid_set_D_x(void);
#endif
