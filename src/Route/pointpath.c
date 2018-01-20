
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "math.h" 

#define Acc_Limit_Enable_new 0

extern struct Path_Point PointRoute[];
int Point_NowNum;
int Point_StartNum;
int Point_EndNum;
extern int RunTime;
extern u8 WalkGround;
/*
 * ������: SetPointPath
 * ��  ��: ��ʼ���ܵ���·��
 * ��  ��: -sta_num ·����ʼ�ĵ�
 *		   -end_num ·�������ĵ�
 * ��  ��: ��
 * ��  ��: �ⲿ����
 */
void SetPointPath(int sta_num,int end_num)
{
	Point_StartNum = sta_num;
	Point_EndNum = end_num;
	Point_NowNum = sta_num;
	PID_Clear();
	RouteFinish=1;//1��ʾ·��δ������0��ʾ·������
	TIM2->CNT = 0;
}

void GoPointPath(void)
{
	int count_scope=9;				//������Χ
	int count_delta=0;
	int i;
	
	double phi;									//
	double speed,speed_x,speed_y;				//������������ٶ�����ٶ�
	double speed_robot_x,speed_robot_y;			//ת��������������ķ��ٶ�
	
	double error_angle,error_dis;				//�Ƕ������������
// 	double Vout_A=0;							//PID������Ƕ����ֵ
// 	double Vout_D=0;							//PID������������ֵ
	double v_err_x,v_err_y;						//PID�����ٶȵķ���
	
#if Acc_Limit_Enable_new
	static struct GPS_State last_GPS;
	double delta_s;
	double delta_a;
	double robot_speed;
	double w;
	double max_acc = 200;
	double max_acc_w = 50;
#endif	
	
	//������һ��Ŀ���
	for(i=0;i<count_scope;i++)
	{
		if((Point_NowNum+i) >= Point_EndNum) break;
		if(GetLength(PointRoute[Point_NowNum+i+1].position,GPS.position)
			<= GetLength(PointRoute[Point_NowNum+i].position,GPS.position))
			count_delta = i+1;
	}
//	if((Point_NowNum+count_delta)<=Point_EndNum)
//	{
//		if(GetLength(PointRoute[Point_NowNum+count_delta+1].position,GPS.position)
//			<= GetLength(PointRoute[Point_NowNum+count_delta-1].position,GPS.position))
//			Point_NowNum += count_delta;
//		else
//			Point_NowNum += count_delta-1;
//	}
	
	Point_NowNum += count_delta;
	
	//���ٶ���x,y�����ϵķ���
	speed = PointRoute[Point_NowNum].speed;
    
//if(WalkGround)
//{
//    speed = speed/2.0;
//}
	/****************************�Լ��ٶȽ�������(��ʵ���ٶ����Ƹ��ٶ�)******************************/
	
#if Acc_Limit_Enable_new
	
	delta_s = GetLength(last_GPS.position,GPS.position);
	robot_speed = delta_s/((TIM2->CNT)/1000000.0);
	robot_speed = robot_speed/1.3;
	
	delta_a = GPS.radian - last_GPS.radian;
	w = delta_a/((TIM2->CNT)/1000000.0);
	
	TIM2->CNT = 0;
	last_GPS = GPS;
	
	if(speed-robot_speed > max_acc)
	{
		speed = robot_speed + max_acc;
	}
	else if(speed-robot_speed < -max_acc)
	{
		speed = robot_speed - max_acc;
	}
#endif
	
	speed_x = speed*cos(PointRoute[Point_NowNum].speeddir);
	speed_y = speed*sin(PointRoute[Point_NowNum].speeddir);
	
	//�Ƕ����;������
	error_angle = PointRoute[Point_NowNum].selfdir-GPS.radian;
	error_dis = (GPS.position.x-PointRoute[Point_NowNum].position.x)*sin(PointRoute[Point_NowNum].speeddir)
				-(GPS.position.y-PointRoute[Point_NowNum].position.y)*cos(PointRoute[Point_NowNum].speeddir);
	
	if(error_angle>pi)
		error_angle-=2*pi;
	if(error_angle<-pi)
		error_angle+=2*pi;
	
	if(Route_Num==2||Route_Num==3)
	{
		if(CamFlag==1)
		{
			error_angle=Cam_Angle[Cam_Circle_Line]*pi/180.0-cam_angle;
			
			if(error_angle>pi)
				error_angle=error_angle-2*pi;
			if(error_angle<-pi)
				error_angle=error_angle+2*pi;

			Vout[0] = 1000*AnglePID(error_angle,Pid_List[12]);
			Vout[1] = DistancePID(error_dis,Pid_List[13]);
		}
		else
		{
			Vout[0] = 1000*AnglePID(error_angle,Pid_List[6]);
			Vout[1] = DistancePID(error_dis,Pid_List[7]);
		}
	}
	else if(Route_Num==5)
	{
	   if(CamFlag==1)
	   {
			Vout[0] = 1000*AnglePID(error_angle,Pid_List[12]);
			Vout[1] = DistancePID(error_dis,Pid_List[7]);
	   }
	   else
	   {
			Vout[0] = 1000*AnglePID(error_angle,Pid_List[6]);
			Vout[1] = DistancePID(error_dis,Pid_List[7]);
	   }
	}
	else 
	{
		Vout[0] = 1000*AnglePID(error_angle,Pid_List[6]);
		Vout[1] = DistancePID(error_dis,Pid_List[7]);
	}

	if(Vout[0]>300)
		Vout[0]=300;
	if(Vout[0]<-300)
		Vout[0]=-300;
	
//#if Acc_Limit_Enable_new
//	if(Vout[0]-w > max_acc_w)
//	{
//		Vout[0] = w + max_acc_w;
//	}
//	else if(Vout[0]-w < -max_acc_w)
//	{
//		Vout[0] = w - max_acc_w;
//	}
//#endif
	
	//PID�����ٶȵķ���
	v_err_x=-(Vout[1]*sin(PointRoute[Point_NowNum].speeddir));
	v_err_y= (Vout[1]*cos(PointRoute[Point_NowNum].speeddir));

	if(PID_flag==0)
	{
		v_err_x=0;
		v_err_y=0;
	}
	
	//���ϵ�������
	speed_x += v_err_x;
	speed_y += v_err_y;
	
	//������������ٶ�ת��Ϊ�����˾ֲ�������ٶ�
	phi = GPS.radian;
	speed_robot_x =  speed_x*cos(phi) + speed_y*sin(phi);
	speed_robot_y = -speed_x*sin(phi) + speed_y*cos(phi);
	
	//Ϊ���ָ��ٶ�
	
	Speed_X = speed_robot_x;
	Speed_Y = speed_robot_y;
	Speed_Rotation = Vout[0];
	
	SD_Printf_MQ(&fil_pid, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;\r\n",speed,GPS.position.x,GPS.position.y,GPS.radian,PointRoute[Point_NowNum].position.x,PointRoute[Point_NowNum].position.y,PointRoute[Point_NowNum].selfdir,error_angle,error_dis,Vout[0]);
	
	//LCD��ʾ
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("Now:");
	LCD_WriteInt(Point_NowNum);

	LCD_SetXY(8,0);
	LCD_Printf("R_N:%d R_F:%d",Route_Num,RouteFinish);
	
	LCD_SetXY(0,1);
	LCD_WriteString("A:");
	LCD_WriteDouble(GPS.radian/pi*180);
	
	LCD_SetXY(0,2);
	LCD_WriteString("X:");
	LCD_WriteDouble(GPS.position.x);
	
	LCD_SetXY(0,3);
	LCD_WriteString("Y:");
	LCD_WriteDouble(GPS.position.y);
	
	LCD_SetXY(0,4);
	LCD_Printf("e_a:%.2f",error_angle/pi*180);
	
	LCD_SetXY(10,4);
	LCD_Printf("e_d:%.2f",error_dis);
	
	LCD_SetXY(0,5);
	LCD_WriteString("Vout_A:");
	LCD_WriteDouble(Vout[0]);
	
	LCD_SetXY(0,6);
	LCD_WriteString("Vout_D:");
	LCD_WriteDouble(Vout[1]);
	
	LCD_SetXY(0,7);
	LCD_Printf("Time:%d min %d s",RunTime*5/1000/60,RunTime*5/1000%60);
	
}


/**********************************END OF FILE*********************************/
