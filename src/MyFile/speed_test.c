#include "includes.h"
#include "math.h"
#define SQR 1.41421356
#define PI 3.1415926
#include "speed_test.h"
#include "mecanum.h"
static	rt_uint32_t key_value;
//extern u8 spin_flag;
static double acc_vaule_x;
static double acc_vaule_y;
static int gotime=0;
static int times=1;
static int rotation_sign;
int GOLINEFLAG=0;
int delay_time;
FIL speed_file;
FIL encod_file;
FIL posit_file;
FIL x_mes;
FIL y_mes;
void acc_set(void);
void my_goline(Speed_Data speed_mes);
void flag_set(void);
void convert_get(void);
void test_speed(void);
/************************goline**********************************/
struct PID pid_goline_A={-0.5,800000,0.3};

struct PID pid_goline_D_x={2,800000,0};
struct PID pid_goline_D_y={2,800000,0};
double keepspeed1=2;
double keepspeed=150;
int pid_rate=1;

extern double rotation_zx;

/***************************斜测45度**********************************/
void test_speed(void)
{
    static int flag_speed=1;
	double angle;
	TCHAR *path1,*path2,*path3;
	Speed_Data speed_mes;
	acc_set();
    flag_set();
	times+=30;
	path1="speed .txt";
	path1[6]=(TCHAR)times;
	path2="encod .txt";
	path2[6]=(TCHAR)times;
	path3="posit .txt";
	path3[6]=(TCHAR)times;
	f_open (&speed_file, path1, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&x_mes, "x_mes.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&y_mes, "y_mes.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	SD_Printf_MQ(&speed_file,"x1      y1        x2      y2\r\n");
	gotime=0;
    while(flag_speed)
    {	
		gotime++;
		Speed_X=acc_vaule_x;
		Speed_Y=acc_vaule_y;
		Speed_Rotation=0;
		if(gotime==1)
			SetSpeed2(acc_vaule_x,acc_vaule_y,0);			

		gotime++;
//		SD_Printf_MQ(&speed_file,"%-8d%-10.4f%-8d%-10.4f\r\n",gotime,realspe,gotime,pow(Speed_X*Speed_X+Speed_X*Speed_X,0.5));
//		SD_Printf_MQ(&encod_file,"%-12.4f%-12.4f%-12.4f\r\n",Encoder[0].dis,Encoder[1].dis,Gyro.angle);
//		SD_Printf_MQ(&posit_file,"%-12.4f%-12.4f%-12.4f\r\n",GPS.position.x,GPS.position.y,GPS.radian);
 		SD_Printf_MQ(&speed_file,"%-12.4f%-12.4f\r\n",ACC_X_1,ACC_Y_1);
		SD_Printf_MQ(&x_mes,"%-12.4f%-12.4f%-8d%-12.4f\r\n",realspe_x,speed_x_zx,gotime,pid_goline_A.p);
		SD_Printf_MQ(&y_mes,"%-12.4f%-12.4f%-8d%-12.4f\r\n",realspe_y,speed_y_zx,gotime,pid_goline_A.p);
		
		gotime+=5;
		Delay_ms(5);
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
        {
			switch(key_value)
            {
				case key4:
					flag_speed=0;
				break;
            }
        }
        LCD_Clear();
        LCD_SetXY(0,0);
        LCD_WriteString("1.A:");
        LCD_WriteDouble(MOSI_GPS_ANG.f64_data);

        LCD_SetXY(0,1);
        LCD_WriteString("2.Speed_X:");
        LCD_WriteDouble(Speed_X);

        LCD_SetXY(0,2);
        LCD_WriteString("3.Speed_Y:");
        LCD_WriteDouble(Speed_Y);

        LCD_SetXY(0,3);
        LCD_WriteString("4.STOP");

        LCD_SetXY(0,4);
        LCD_WriteString("5.speed_set:");
        LCD_WriteDouble(acc_vaule_x==0? Speed_Y:Speed_X);

        LCD_SetXY(0,5);
        LCD_WriteString("6.speed_really:");
        LCD_WriteDouble(realspe);
    }
	SetSpeed2(0,0,0); 
    f_close(&speed_file);
	f_close(&x_mes);
	f_close(&y_mes);
	gotime=0;
	flag_speed=1;
	++times;	
}
void go_stright(void)
{
	Speed_Data speed_mes;
    static int flag_speed=1;
	char *path1,*path2,*path3;
	path1="speed .txt";
	path2="encod .txt";
	path3="posit .txt";
	f_open (&speed_file, "speed.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&x_mes, "GPS_mes.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&y_mes, "sign.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	f_open (&posit_file, path3, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	while(flag_speed)
    {
		SetSpeed(handle_speed_x,handle_speed_y,0);			
		
		gotime+=1;
 		SD_Printf_MQ(&speed_file,"%-12.4f%-12.4f\r\n",speed_x_zx,speed_y_zx);
		SD_Printf_MQ(&x_mes,"%-12.4f%-12.4f%-8d%-12.4f\r\n",GPS_mes[0],GPS_mes[5]);
		SD_Printf_MQ(&y_mes,"%-12.4f%-12.4f%-8d%-12.4f\r\n",x_sign,y_sign);
		
		Delay_ms(5);
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
        {
			switch(key_value)
            {
				case key4:
					flag_speed=0;
				break;
            }
        }
        LCD_Clear();
        LCD_SetXY(0,0);
        LCD_WriteString("1.speed_x:");
        LCD_WriteDouble(speed_x_zx);

        LCD_SetXY(0,1);
        LCD_WriteString("2.speed_y:");
		LCD_WriteDouble(speed_y_zx);

        LCD_SetXY(0,2);
        LCD_WriteString("3.GPS[0].x:");
		LCD_WriteDouble(GPS_mes[0].x);

        LCD_SetXY(0,3);
        LCD_WriteString("4.GPS[0].y");
		LCD_WriteDouble(GPS_mes[0].y);
		
        LCD_SetXY(0,4);
        LCD_WriteString("5.x_sign:");
        LCD_WriteDouble(x_sign);
		
        LCD_SetXY(0,5);
        LCD_WriteString("6.y_sign:");
        LCD_WriteDouble(y_sign);
    }
	SetSpeed(0,0,0); 
    f_close(&speed_file);
	f_close(&x_mes);
	f_close(&y_mes);
	gotime=0;
	flag_speed=1;
	++times;	
}

void acc_set(void)
{
    static struct Input_ListValue Values[]=
    {
        {&acc_vaule_x,	TYPE_FP64,	"acc_x"},
        {&acc_vaule_y,	TYPE_FP64,	"acc_y"},
				
    };
    LCD_Clear();
    GPS_Clear();
    PID_Clear();

    Input_List(Values,2);

    if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
}
void flag_set(void)
{
    static struct Input_ListValue Values[]=
    {
        {&GOLINEFLAG,	TYPE_INT8,	"GOLINEFLAG"},
    };

    LCD_Clear();
    GPS_Clear();
    PID_Clear();

    Input_List(Values,1);

    if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
}


void my_goline(Speed_Data speed_mes)
{
	static double aim_x,aim_y;
	
	static int count;
	double error_dis_x,error_dis_y,error_angle;         //错误信息
	double Vout_D_x=0,Vout_D_y=0;                       //pid系数
	double v_err_x,v_err_y;                             //经过调整的误差速度
	double temp_speed_x,temp_speed_y;                             //
	int pid_sign;
	static int x_off,y_off;
	static int xoff_flag,yoff_flag;
	static int only_x_goline,only_y_goline;
	double speed_x,speed_y;
	static double Rotation=0;
	temp_speed_x=speed_mes.speed_x;
	temp_speed_y=speed_mes.speed_y;
	
	speed_x_zx=speed_mes.speed_x;
	speed_y_zx=speed_mes.speed_y;
	/*****判断xon xoff yon yoff*****/
	if(speed_x_zx==0)
	{	
		if(xoff_flag!=0)
		{
			xoff_flag=0;
			x_off=0;
		}
		x_off++;
	}
	else 
		xoff_flag=1;
	
	if(speed_y_zx==0)
	{	
		if(yoff_flag!=0)
		{
			yoff_flag=0;
			y_off=0;
		}
		y_off++;
	}
	else 
		yoff_flag=1;

/*************使用上一次函数运行得到的only_x_goline，only_x_goline进行误差计算,
	这次计算并不是完全正确，只在X,Y两个方向都动的情况下是错误的错误会在下面纠正***************/
	if(only_x_goline&&only_y_goline)
	{
		aim_x=GPS.position.x;
		aim_y=GPS.position.y;
	}
	else if(only_y_goline&&only_x_goline!=0)
	{
		aim_y=GPS.position.y;
		aim_x=aim_x;
	}
	else if(only_x_goline&&only_y_goline!=0)
	{
		aim_x=GPS.position.x;
		aim_y=aim_y;
	}
	else if(only_x_goline==0&&only_y_goline==0)
	{
		aim_x=GPS.position.x;
		aim_y=GPS.position.y;
	}

/**********************判断此次运动的运动状态，作为下次执行此函数的依据，同时也排除了XY都动的情况下上面算法的错误***********/
	if(speed_x_zx==0)
	{
		if(speed_y_zx !=0||y_off<=100)
			only_y_goline=1;
		else
			only_y_goline=0;
	}
	else
	{
		aim_x=GPS.position.x;
	}

	if(speed_y_zx==0)
	{
		if(speed_x_zx!=0||x_off<=100)
			only_x_goline=1;
		else
			only_x_goline=0;
	}
	else
	{
		aim_y=GPS.position.y;
	}

	if(count==0)
		aim_r=MISO_GPS_ANG.f64_data;
	
/********get error information**********/
/*pid of position*/
		error_dis_x=aim_x-GPS.position.x;
		error_dis_y=aim_y-GPS.position.y;
		Vout_D_x = DistancePID(error_dis_x,pid_goline_D_y);
		Vout_D_y = DistancePID(error_dis_y,pid_goline_D_y);
		v_err_x = Vout_D_x;
		v_err_y = Vout_D_y;
		speed_mes.speed_x = v_err_x;
		speed_mes.speed_y = v_err_y;
/**************根据运动状态调节********************/
		if(only_y_goline&&only_x_goline)
		{
			SetSpeed2(temp_speed_y,temp_speed_y,speed_mes.speed_rotation);
			LCD_SetXY(0,7);
			LCD_WriteString("1:");
			LCD_WriteDouble(v_err_y);
		}
		else if(only_x_goline&&only_y_goline==0)
		{
			SetSpeed2(temp_speed_x,speed_mes.speed_y,speed_mes.speed_rotation);
			LCD_SetXY(0,7);
			LCD_WriteString("2:");
			LCD_WriteDouble(v_err_y);
		}
		else if(only_y_goline&&only_x_goline==0)
		{
			SetSpeed2(speed_mes.speed_x,temp_speed_y,speed_mes.speed_rotation);
			LCD_SetXY(0,7);
			LCD_WriteString("3:");
			LCD_WriteDouble(v_err_y);
		}

		else if(only_x_goline==0&&only_y_goline==0)
		{
			SetSpeed2(temp_speed_x,temp_speed_y,speed_mes.speed_rotation);
			LCD_SetXY(0,7);
			LCD_WriteString("4:");
			LCD_WriteDouble(v_err_y);
		}
		++count;
		if(count==4)
			count=1;

}
void pid_set_A(void)
{
    static struct Input_ListValue Values[]=
    {
        {&pid_goline_A.p,	TYPE_FP64,	"angle.p"},
        {&pid_goline_A.i,	TYPE_FP64, 	"angle.i"},
		{&pid_goline_A.d,   TYPE_FP64,	"angle.d"},		
				
    };

    LCD_Clear();
    GPS_Clear();
    PID_Clear();

    Input_List(Values,3);

    if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
}

void pid_set_D_y(void)
{
    static struct Input_ListValue Values[]=
    {
        {&pid_goline_D_y.p,	TYPE_FP64,	"dis.p"},
        {&pid_goline_D_y.i,	TYPE_FP64,	"dis.i"},
				{&pid_goline_D_y.d, TYPE_FP64,	"dis.d"},
				
    };

    LCD_Clear();
    GPS_Clear();
    PID_Clear();

    Input_List(Values,3);

    if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);

	}

void pid_set_D_x(void)
{
    static struct Input_ListValue Values[]=
    {
        {&pid_goline_D_x.p,	TYPE_FP64,	"dis.p"},
        {&pid_goline_D_x.i,	TYPE_FP64,	"dis.i"},  
		{&pid_goline_D_x.d,	TYPE_FP64,	"dis.d"},

    };

    LCD_Clear();
    GPS_Clear();
    PID_Clear();

    Input_List(Values,3);

    if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_FOREVER) == RT_EOK);
}

void convert_get(void)
{
	int flag_convert=1;
	LCD_Clear();

	while(flag_convert)
	{
		SetSpeed(0,0,60);
		if(rt_mb_recv(&Mb_Key, &key_value, RT_WAITING_NO) == RT_EOK)
        {
			switch(key_value)
            {
				case key4:
					flag_convert=0;
				break;
            }
        }
        LCD_SetXY(0,0);
        LCD_WriteString("1.A:");
        LCD_WriteDouble(Gyro.angle);

		LCD_SetXY(0,1);
		LCD_WriteString("Dis1:");
		LCD_WriteDouble(Encoder[0].dis);

		LCD_SetXY(0,2);
		LCD_WriteString("Dis2:");
		LCD_WriteDouble(Encoder[1].dis);
	}
}
