
/* Includes ------------------------------------------------------------------*/
#include "../../include/includes.h"

#define Acc_Limit_Enable 0
#define Max_Acc 50
#define RATE 0.56
#define PI 3.1415926

extern void prepare(void);
extern void hit1(void);
//下面的坐标系是机器人的局部坐标系
double Speed_X;				// mm/s
double Speed_Y;				// mm/s
double Speed_Rotation;		// (rad/s)/100
double ACC_X_1=350; //450
double ACC_Y_1=350; //350
double ACC_X_2=600; //550
double ACC_Y_2=600; //420

extern double coeff_x,coeff_y,coeff_r;
int x_sign=1.0,y_sign=1.0;

static double robo_L = 636.6;
static double robo_W = 636.2;
static double wheel_r = 76.0;
double coeff_x,coeff_y,coeff_r;

double motor_speed=0;
double speed_rate = 30;
double Speed_Limit = 1000;
double speed_x_zx,speed_y_zx;
struct Point get_realspe_robot(void);
//void SetSpeed(double fl_speed,double fr_speed,double bl_speed,double br_speed)

	
double fl_speed;
double fr_speed;
double bl_speed;
double br_speed;
void SetSpeed(double speed_x,double speed_y,double speed_rotation)
{
	int key_value;
	
	double fl_delta;
	double fr_delta;
	double bl_delta;
	double br_delta;
	
	double real_fl_speed;
	double real_fr_speed;
	double real_bl_speed;
	double real_br_speed;
	
	static double fl_last=0;	//储存四轮的速度
	static double fr_last=0;
	static double bl_last=0;
	static double br_last=0;
	

	double delta_max;			//速度变化量最大值
	double PWM_Max;				//生成的PWM后对应的速度最大值
	
	double PWM_FL,PWM_FR,PWM_BL,PWM_BR;
/*******************anti-slip************************/	
	
	coeff_x = coeff_y = 1/wheel_r/(2*PI)*60;
	coeff_r = (robo_L+robo_W)/2/wheel_r/(2*PI)*60;

/****************************通过雅克比矩阵把速度分解到各个轮子******************************/
	fl_speed = coeff_y*speed_y + coeff_x*speed_x - coeff_r*speed_rotation/100 ;
	fr_speed = coeff_y*speed_y - coeff_x*speed_x + coeff_r*speed_rotation/100 ;
	bl_speed = coeff_y*speed_y - coeff_x*speed_x - coeff_r*speed_rotation/100 ;
	br_speed = coeff_y*speed_y + coeff_x*speed_x + coeff_r*speed_rotation/100 ;
	/****************************对加速度进行限制(以理论速度限制赋速度)******************************/
	
	Send_Data1.senddata.wheelmotor.fl_speed = fl_speed;
	Send_Data1.senddata.wheelmotor.bl_speed = bl_speed;
	Send_Data1.senddata.wheelmotor.fr_speed = fr_speed;
	Send_Data1.senddata.wheelmotor.br_speed = br_speed;
}
/**********************************END OF FILE*********************************/
// void SetSpeed2(double speed_x,double speed_y,double speed_rotation)
// {
// 	rt_uint32_t key_value;
// 	double replace;
// 	double temp_realspe_x,temp_realspe_y;
// 	double fl_speed;
// 	double fr_speed;
// 	double bl_speed;
// 	double br_speed;
	
// 	double fl_delta;
// 	double fr_delta;
// 	double bl_delta;
// 	double br_delta;
	
// 	double real_fl_speed;
// 	double real_fr_speed;
// 	double real_bl_speed;
// 	double real_br_speed;
	
// 	static double fl_last=0;	//储存四轮的速度
// 	static double fr_last=0;
// 	static double bl_last=0;
// 	static double br_last=0;
	
// 	double delta_max;			//速度变化量最大值
// 	double PWM_Max;				//生成的PWM后对应的速度最大值
	
// 	double PWM_FL,PWM_FR,PWM_BL,PWM_BR;
// /*******************anti-slip************************/	
// 	double speed_x_rep,speed_y_rep,speed_rep;
// 	double rate_x,rate_y;
// 	double acc_x_final,acc_y_final,dec_y_final,dec_x_final;
// 	struct Point realspe_robot;
// 	double angle;
// 	int i;
// 	double go_angle,change_x,change_y;
// 	realspe_robot=get_realspe_robot();
// 	if(speed_x<0)
// 		x_sign=-1;
// 	else
// 		x_sign=1;
	
// 	if(speed_y<0)
// 		y_sign=-1;
// 	else
// 		y_sign=1;
// 	coeff_x = coeff_y = 1/wheel_r/(2*pi)*60;
// 	coeff_r = (robo_L+robo_W)/2/wheel_r/(2*pi)*60;
	
// 	speed_x=speed_x_rep;
// 	speed_y=speed_y_rep;
// 	speed_x_zx=speed_x;
// 	speed_y_zx=speed_y;

// 	fl_speed = coeff_y*speed_y + coeff_x*speed_x - coeff_r*speed_rotation/100 ;
// 	fr_speed = coeff_y*speed_y - coeff_x*speed_x + coeff_r*speed_rotation/100 ;
// 	bl_speed = coeff_y*speed_y - coeff_x*speed_x - coeff_r*speed_rotation/100 ;
// 	br_speed = coeff_y*speed_y + coeff_x*speed_x + coeff_r*speed_rotation/100 ;

// }
// // void keep_test(void)
// // {
// // 	struct PID keep_angle_pid={-0.7,800000,0.3};
// // 	struct Point end_point={1303,-2973};
// // 	//struct Point end_point={0,0};
// 	double aim_angle=-25.5*pi/180;
// 	double speed_max;
// 	int x_f,y_f,a_f;
// 	double error_angle,Vout_A;
// 	double zhuan;
// 	SetKeep(end_point,0,2000); 
// 	while(1)
// 	{
// 		Keep();
// 		LCD_Clear();
// 		LCD_SetXY(0,0);
// 		LCD_WriteString("X:");
// 		LCD_WriteDouble(GPS.position.x);
		
// 		LCD_SetXY(0,1);
// 		LCD_WriteString("Y:");
// 		LCD_WriteDouble(GPS.position.y);
// 		if(GetLength(GPS.position,end_point) < 30)
// 		{
// 			SPEED_STOP;
// 			break;
// 		}
// 		Delay_ms(5);
// 	}
// 	while(1)
// 	{
// 		error_angle = aim_angle-GPS.radian;
	
// 		if(error_angle>pi)
// 			error_angle-=2*pi;
// 		if(error_angle<-pi)
// 			error_angle+=2*pi;
	
// 		Vout_A = 1000*AnglePID(error_angle,keep_angle_pid);			
// 		zhuan=Vout_A;

// 		LCD_Clear();
// 		LCD_SetXY(0,0);
// 		LCD_WriteString("R:");
// 		LCD_WriteDouble(GPS.radian);
		
// 		if(zhuan>300)
// 			zhuan= 300;		
// 		if(zhuan<-300)
// 			zhuan= -300;
		
// 		SetSpeed(0,0,zhuan);
// 		if(fabs(error_angle)<=4*pi/180)
// 			break;
// 		Delay_ms(5);
// 	}
// 	PID_Clear();
// 	aim_r=aim_angle;
// 	SetSpeed(0,0,0);
// }	
