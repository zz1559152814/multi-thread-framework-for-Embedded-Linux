
struct WheelMotor
{
    float fl_speed;
    float fr_speed;
    float bl_speed;
    float br_speed;
};

struct Point 
{
    float x;
    float y;
};

struct GPS_State
{
    struct Point position;  //
    double radian;          //»¡¶È
};

struct  HandleData
{
    int HandleData_X;
    int HandleData_Y;
    int HandleData_R;
    int Buttons;
};

struct RecvData
{
    struct GPS_State    GPS;
    struct HandleData   handlevaule;
};

union uRecvData
{
    struct  RecvData recvdata;
    char    buff[sizeof(struct RecvData*)];
};

struct SendData
{
    int     Switch_8;
    float   motor[3];
    struct  WheelMotor  wheelmotor;
};

union uSendData
{
    struct  SendData    senddata;
    char    buff[sizeof(struct SendData*)];
};

struct AllData 
{
    struct RecvData recvdata;
    struct SendData senddata;
};

union sSenddata
{
    struct  AllData     netsend;
    char    buffer[1024];
};