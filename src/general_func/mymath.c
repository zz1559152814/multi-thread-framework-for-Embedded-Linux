
/* Includes ------------------------------------------------------------------*/
#include "../../include/includes.h"

double Max(double a,double b)
{
	if(a>b)
		return a;
	else
		return b;
}

double MaxFour(double a,double b,double c,double d)
{
	double maxtemp1,maxtemp2;
	maxtemp1=Max(a,b);
	maxtemp2=Max(c,d);
	return Max(maxtemp1,maxtemp2);
}
 
double GetLength(struct Point a,struct Point b)
{
	double length;
	double dx,dy;
	dx = (a.x-b.x)*(a.x-b.x);
	dy = (a.y-b.y)*(a.y-b.y);
	length = sqrt(dx+dy);
	return length;
}

double GetLineSlope(struct Point a,struct Point b)
{
	double slope;
	if(abs(a.x-b.x) <= 0.02)
		slope=pi/2;
	else
		slope = (b.y-a.y)/(b.x-a.x);
	
	return slope;
}

double GetLineAngle(struct Point sta,struct Point end)
{
	double radian;
	if(abs(sta.x-end.x) <= 0.02)
	{
		if(sta.y>end.y) radian=-pi/2;
		else radian=pi/2;
	}
	else
	{
		radian = atan((end.y-sta.y)/(end.x-sta.x));
		
		//atan值域为第四和第一象限，所以要通过判断分离出二、三象限的部分
		if(sta.x>end.x && sta.y<end.y) radian+=pi;
		else if(sta.x>end.x && sta.y>end.y) radian-=pi;
	}
	
	return radian;
}

double GetDis_P2L(struct Point p,double a,double b,double c)
{
	return abs(a*p.x+b*p.y+c)/(sqrt(a*a+b*b));
}

double GetDis_P2L_PP(struct Point p,struct Point p1,struct Point p2)
{
	double a,b,c;
	a=p2.y-p1.y;
	b=p1.x-p2.x;
	c=p2.x*p1.y-p1.x*p2.y;
	
	return GetDis_P2L(p,a,b,c);
}

double GetDis_P2L_PK(struct Point p,struct Point p0,double k)
{
	double a,b,c;
	a=k;
	b=-1;
	c=-k*p0.x+p0.y;
	
	return GetDis_P2L(p,a,b,c);
}
struct Point GetFoot_P2L(struct Point p,double a,double b,double c)
{
	struct Point foot;
	foot.x=(b*b*p.x-a*b*p.y-a*c)/(a*a+b*b);
	foot.y=(a*a*p.y-a*b*p.x-b*c)/(a*a+b*b);
	
	return foot;
}

struct Point GetFoot_P2L_PP(struct Point p,struct Point p1,struct Point p2)
{
	struct Point foot;
	double a,b,c;
	a=p2.y-p1.y;
	b=p1.x-p2.x;
	c=p2.x*p1.y-p1.x*p2.y;
	
	foot = GetFoot_P2L(p,a,b,c);
	return foot;
}

struct Point GetFoot_P2L_PK(struct Point p,struct Point p0,double k)
{
	struct Point foot;
	double a,b,c;
	a=k;
	b=-1;
	c=-k*p0.x+p0.y;
	
	foot = GetFoot_P2L(p,a,b,c);
	return foot;
}

/**********************************END OF FILE*********************************/

