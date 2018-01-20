/*mini2440_buttons_my.c*/
/*后面加了_my*/

/*按键驱动程序*/

/*mini2440所用到的按键资源*/
/**************************************************/
/* 按键          对应的IO寄存器     对应的中断引脚*/
/* K1             GPG0                 EINT8      */
/* K2             GPG3                 EINT11     */
/* K3             GPG5                 EINT13     */
/* K4             GPG6                 EINT14     */
/* K5             GPG7                 EINT15     */
/* K6             GPG11                EINT19     */
/**************************************************/

/*要搞清楚谁是输入*/
/*在这里,按键控制对应的中断引脚,从而控制对应的IO寄存器*/
/*相当于信息从外面输入*/
/*我们要做的是根据对应的输入信息,来采取相应的响应动作*/
/*这就达到了中断响应的目的*/
/*其核心就是要检测*/
/*那么,该如何去检测呢?*/
/*通过什么来检测呢?*/

/*如何得知一个设备究竟用到哪些资源呢?*/
/*这是个非常重要的问题*/
/*我想应该看具体的电路原理图*/
/*只有看图,才能了解具体的电路连接情况*/
/*从而得知设备所需的硬件资源*/
/*厂商的原理图通常给的都比较详细*/

/*引用的头文件*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/gpio.h>

/*定义宏*/
#define BUTTON_MAJOR 221 /*主设备号,本来是232,我改为221*/
#define DEVICE_NAME "buttons_my" /*设备名,本来是buttons,我加上了_my*/
/*定义按钮中断的描述结构体*/
/*由它把按钮中断的信息综合起来*/
/*各个成员表示什么意思?*/
struct button_irq_desc
{
  int irq; /*中断号*//*中断号唯一表示一个中断*/
  int pin; /*中断控制的寄存器*/
  /*该寄存器的值由中断引脚设置*/
  /*我们希望从该寄存器读出控制信息*/
  int pin_setting; /*中断的引脚*/
  /*该引脚的电平由按键来控制*/
  /*从而最终我们由按键控制了寄存器的值*/
  int number; /*编号*/
  char *name; /*名称*/
};

/*指定6个按键的信息*/

static struct button_irq_desc button_irqs [] =
{
  {IRQ_EINT8,S3C2410_GPG0,S3C2410_GPG0_EINT8,0,"KEY1"}, /*K1*/
  {IRQ_EINT11,S3C2410_GPG3,S3C2410_GPG3_EINT11,1,"KEY2"}, /*K2*/
  {IRQ_EINT13,S3C2410_GPG5,S3C2410_GPG5_EINT13,2,"KEY3"}, /*K3*/
  {IRQ_EINT14,S3C2410_GPG6,S3C2410_GPG6_EINT14,3,"KEY4"}, /*K4*/
  {IRQ_EINT15,S3C2410_GPG7,S3C2410_GPG7_EINT15,4,"KEY5"}, /*K5*/
  {IRQ_EINT19,S3C2410_GPG11,S3C2410_GPG11_EINT19,5,"KEY6"}, /*K6*/
}

/*这样,资源就组织起来了*/
/*事实上,在这里我们不仅组织起了硬件资源*/
/*我们也把一定的软件资源也糅合进去了*/
/*像中断号*/

/*key_values数组*/
/*存放各个按键在发生中断情况下的值*/
/*volatile是什么意思呢?*/
/*这个数组是我们存放按键操作结果的,因此非常重要*/

static volatile int key_values [] = {0,0,0,0,0,0};
/*宏DECLARE_WAIT_QUEUE_HEAD(),是干什么的呢?*/
/*该宏应该是创建了一个等待队列*/
/*等待队列,是进程调度的一种重要方法*/
/*等待队列也很有意思,button_waitq,表示按键等待的队列*/
/*就是说,按键一按下,就会激活其等待队列里的进程,来做相应的处理*/
/*因此,按键的等待队列,或者说中断所设置的等待队列,*/
/*是中断处理中非常重要的资源,它大大扩展了中断处理的能力*/
static DECLARE_WAIT_QUEUE_HEAD(button_waitq); /*button_waitq是什么呢?*/
/*应该是等待队列的名称*/

/*key_values数组中是否有数据的标志,0表示无数据可读,1表示有数据可读*/

static volatile int ev_press = 0; /*初始为0*/

/*中断服务程序buttons_interrupt()的申明*/
/*即当检测到有中断时,就会执行该中断服务程序*/
/*那么如何检测到有中断发生呢?*/
/*并且中断发生了,知道发生了什么样的中断呢?*/
/*中断有很多种,该中断服务程序究竟该服务于哪一个中断呢?*/
/*显然,要把中断号与中断服务程序联结起来,构成一个整体*/
/*这个工作可以在open函数里做*/

/*参数irq---中断号*/
/*中断服务程序应该是与中断号一一对应的*/
/*对应于某个中断号的中断一发生,就会调用该中断号对应的服务程序*/
/*那么,检测中断的发生,就成了先决条件*/
/*参数dev_id ---具体是哪一个按钮*/

static irqreturn_t buttons_interrupt(int irq,void *dev_id);

/*mini2440_buttons_open()函数申明*/
/*驱动函数open调用的具体函数*/
/*由open函数具体实现硬件的初始化工作*/
/*以及软件的初始化工作*/
/*为我们的键盘设备的运行创造好环境*/

static int mini2440_buttons_open(struct inode *inode,struct file *file);

/*mini2440_buttons_close()函数的申明*/
/*release调用的具体函数*/
/*设备软件环境的拆卸*/
/*具体就是中断的释放工作*/
/*因为中断资源,也是系统宝贵的资源,所以不用的时候,要释放*/

static int mini2440_buttons_close(struct inode *inode,struct file *file);

/*mini2440_buttons_read()函数的申明*/
/*read调用的具体函数*/
/*由它读取键盘输入的结果*/
/*实质上就是读取key_values数组的值*/
/*它完成了键盘作为输入设备的核心功能*/
/*数组是否可读,要根据标志位ev_press来判断*/
/*如果数组可读,则读取数据到用户buffer中*/
/*如果数组不可读,则进程进入等待队列,等待到数组可读为止*/
/*等待队列机制,是中断管理中常用到的机制*/
/*因为有些进程经常需要等待某一事件的发生*/

static int mini2440_buttons_read(struct file *filp,char __user *buff,size_t count,loff_t *offp);
/*注意__user,指的是用户空间*/
/*即要把键盘的输入结果读取到用户空间去*/

/*mini2440_buttons_poll()函数的申明*/
/*poll调用的具体函数*/
/*poll实质上是select的调用函数*/
/*如果有按键数据,则select会立刻返回*/
/*如果没有按键数据,则等待*/
/*实质上这是键盘等待输入的机制*/

static unsigned int mini2440_buttons_poll(struct file *file,struct poll_table_struct *wait);


/*file_operations结构体*/
/*驱动函数的设置*/
/*分别将前面的驱动函数设置进来*/

static struct file_operations mini2440_buttons_fops =
{
    .owner = THIS_MODULE,

    .open = mini2440_buttons_open, /*open()*/

    .release = mini2440_buttons_close, /*release()*/

    .read = mini2440_buttons_read, /*read()*/

    .poll = mini2440_buttons_poll /*poll()*/
};

/*mini2440_buttons_init()函数的申明*/
/*module_init调用的具体函数*/
/*模块创建时的初始化函数*/
/*主要做的工作是注册设备和创建设备*/
/*而具体的硬件初始化工作,它可以不做*/
/*而把它留给fops里的函数来做*/

static int __init mini2440_buttons_init(void);

/*mini2440_buttons_exit()函数的申明*/
/*模块卸载时的扫尾工作*/
/*主要是设备的卸载工作*/

static void __exit mini2440_buttons_exit(void);

/*模块创建时的入口点*/

module_init(mini2440_buttons_init);
module_exit(mini2440_buttons_exit);
MODULE_AUTHOR("http://www.arm9.net "); /*驱动程序的作者*/
MODULE_DESCRIPTION("S3C2410/S3C2440 BUTTON Driver"); /*描述信息*/
MODULE_LICENSE("GPL"); /*遵循的协议*/

/**********************mini2440_buttons_init()***********************/
static int __init mini2440_buttons_init(void)
{
  int ret; /*设备注册的返回值*/
  
  /*注册设备驱动程序*/
  /*设备号,设备名,和驱动函数*/
  ret = register_chrdev(BUTTON_MAJOR,DEVICE_NAME,&mini2440_buttons_fops);

  /*对注册失败的处理*/
  if(ret < 0)
  {
  printk(DEVICE_NAME " can't register major number\n");
  return ret;
  }

  /*创建设备*/
  /*devfs_mk_cdev()函数是内核态的设备创建函数*/
  /*而mknod是用户态的设备创建函数*/
  devfs_mk_cdev(MKDEV(BUTTON_MAJOR,0),S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP,DEVICE_NAME);
  printk(DEVICE_NAME " initialized\n");

  return 0;
}


/******************mini2440_buttons_exit()****************************/
static void __exit mini2440_buttons_exit(void)
{
/*移除设备*/
devfs_remove(DEVICE_NAME);

/*注消设备驱动*/
unregister_chrdev(BUTTON_MAJOR,DEVICE_NAME);
}

/*****************mini2440_buttons_open()******************************/
static int mini2440_buttons_open(struct inode *inode,struct file *file)
{
  int i;    /*循环变量,因为有6个按钮*/
  int err; /*中断注册函数的返回值*/

  /*对每个按钮分别处理,用for循环来做*/
  /*具体地是要联结寄存器和相应的引脚*/
  /*联结中断号和相应的中断服务程序*/
  /*这一步类似于前面所说的驱动的注册*/
  /*我们可以成功称作中断的注册*/
  for(i = 0;i < sizeof(button_irqs)/sizeof(button_irqs[0]);i++)
  {
    /*寄存器与中断引脚的联结*/
    s3c2410_gpio_cfgpin(button_irqs[i].pin,button_irqs[i].pin_setting);

    /*中断的注册*/
    /*request_irq()函数*/
    /*要注意其输入参数*/
    /*&button_irqs[i]是该中断享有的资源*/
    /*会被传入buttons_interrupt,进行处理*/
    err = request_irq(button_irqs[i].irq,buttons_interrupt,NULL,button_irqs[i].name,(void *)&button_irqs[i]);

    /*中断类型的设置*/
    /*set_irq_type()函数*/
    /*IRQT_BOTHEDGE的中断类型代表什么样的中断呢?*/
    set_irq_type(button_irqs[i].irq,IRQT_BOTHEDGE);

    /*注册失败的处理*/
    if(err)
      break; /*跳出循环*/
  }

  /*若有一个按钮中断注册失败*/
  /*则还需把前面注册成功的中断给拆了*/
  if(err)
  {
    i--; /*回到前面一个按钮的处理*/
    for(;i >=0; i--) /*依此拆除*/
    {
    /*使中断不起作用*/
      disable_irq(button_irqs[i].irq);

      /*释放中断资源*/
      free_irq(button_irqs[i].irq,(void *)&button_irqs[i]);
    }
    return -EBUSY; /*中断注册没成功的最终的返回值*/
  }

  return 0; /*正常返回*/
}


/**************************buttons_interrupt()*****************************/
/*此中断服务程序,在每中断一次,就要对key_values数组设一下值*/
/*并对数组可读标志位ev_press设一下值*/
/*并唤醒在等待队列里的进程*/
/*这是中断处理经常要做的事情*/
/*在这里,等待队列button_waitq里经常等待的进程是数组的读取进程*/
/*就是说,读取进程在没有读到数据的时候就一直在等待,等待按键的输入*/
/*读取进程在等待,并不代表所有进程在等待,其它进程该干啥干啥去*/

static irqreturn_t buttons_interrupt(int irq,void *dev_id)
{
    /*button_irq_desc结构体变量*/
    /*对传入的资源进行处理*/
    struct button_irq_desc *button_irqs = (struct button_irq_desc *)dev_id;

    /*获取寄存器的值*/
    /*这一步至关重要*/
    /*s3c2410_gpio_getpin()函数直接获取寄存器的值*/
    /*要注意,按一下按钮，会发生两次中断*/
    /*即按下是一次中断，放开又是一次中断*/
    int up = s3c2410_gpio_getpin(button_irqs->pin);

    /*通过电路原理图,可以知道没按下的时候,中断引脚应该是高电平*/
    /*从而寄存器的值应该是1*/
    /*变量取up也是有意义的,表示默认状态是弹起的状态*/
    /*当按下按钮的状态下,寄存器的值就应该是0*/
    /*下面对up的值进行处理*/
    /*即是要把数据经过一定的变换存入key_values数组中*/

    /*如果是弹起的状态*/
    /*那么就要在key_values数组的相应位存入很大的一个值*/
    /*同时又要能从值里辨别出是哪个按键*/
    if(up)   
       key_values[button_irqs->number] = (button_irqs->number + 1) + 0x80;
    /*比如K1键开启的状态下,key_values[0]被置为(0+1)+0x80,即为129*/

    /*如果按键是闭合的状态*/
    /*那么就要在key_values数组的相应位存入一个很小的数*/
    /*同时又要能从值中辨别出是哪个键*/
    else 
       key_values[button_irqs->number] = (button_irqs->number + 1);
    /*比如K1键闭合,则key_values[0]被置为(0+1),即为1*/

    /*对数组可读标志位进行设置*/
    ev_press = 1; /*表示数组已经可读了*/

    /*唤醒休眠的进程?*/
    /*button_waitq队列里存放有相应的处理进程*/
    /*如读取数组的值的进程*/
    /*要注意wake_up_interruptible()这些函数的用法*/
    wake_up_interruptible(&button_waitq);

    /*返回*/

    return IRQ_RETVAL(IRQ_HANDLED); /*?*/
}

/**********************mini2440_buttons_close()*****************************/

static int mini2440_buttons_close(struct inode *inode,struct file *file)
{
int i; /*循环变量,要操作好几个按键*/

/*for循环,对各个按键依此释放中断*/

for(i = 0;i < sizeof(button_irqs)/sizeof(button_irqs[0]);i++)
{
    /*使中断失效*/
    disable_irq(button_irqs[i].irq);

    /*释放资源*/

    free_irq(button_irqs[i].irq,(void *)&button_irqs[i]);
}

/*返回*/

return 0;
}


/**********************mini2440_buttons_read()***************************/

/*要注意,该read函数,只读取一次中断的值,而不是连续地读入*/
/*要做到连续地读入,则需要做一个循环,不断地调用该read函数,但那不是驱动程序里该做的事情*/

static int mini2440_buttons_read(struct file *filp,char __user *buff,size_t count,loff_t *offp)
{
  unsigned long err;   /*copy_to_user()函数的返回值*/

    /*如果key_values 数组里没有值,则会此进程会休眠*/
    /*一直到中断来临之后,中断服务程序会唤醒此休眠进程从而继续读取值*/
    /*key_values数组里有没有值,是靠ev_press标志位来判断的*/
    /*有值,就是1,无值,就是0*/

    /*进程等待队列的机制,是进程调度的一种方法*/

    if(!ev_press) /*标志位为0,即无数据时*/
    {
      if(filp->f_flags & O_NONBLOCK) /*??*/
        return -EAGAIN;

      /*进程休眠,放进button_waitq等待队列*/
      /*这里,把ev_press标志位设成了休眠进程的标志位了?*/
      /*这是为了便于利用poll_wait函数*/
      /*也就是利于select函数*/
      else 
        wait_event_interruptible(button_waitq,ev_press);
      /*在中断处理函数中,此进程会被唤醒*/
      /*唤醒前,ev_press 已被置1了*/
      /*唤醒后的执行点从这里开始*/
    }

    /*下面就是标志位为1,即有数据可读的的处理情况*/

    /*那就开始往用户空间读数据呗*/

    err = copy_to_user(buff,(const void *)key_values,min(sizeof(key_values),count));
    /*copy_to_user()函数的使用*/

    /*对key_values数组清零*/

    memset((void *)key_values,0,sizeof(key_values));

    /*对标志位置0*/
    /*表示读取过了*/

    ev_press = 0;

    /*对err的处理*/

    if(err) /*读取错误*/
      return -EFAULT;
    else /*读取正确*/
      /*则返回读取到的字节数*/
      return min(sizeof(key_values),count);
}

/************************mini2440_buttons_poll()***********************/

static unsigned int mini2440_buttons_poll(struct file *file,struct poll_table_struct *wait)
{
    unsigned int mask = 0; /* */

    /*poll_wait()函数*/
    /*会监测进程队列button_waitq里的进程*/
    /*例如,如果mini2440_button_read所在的进程的标志位ev_press置为1了*/
    /*那么就不会再等待了*/
    /*这实质上就是select函数的运行机制*/

    poll_wait(file,&button_waitq,wait);

    if(ev_press)
    mask |= POLLIN | POLLRDNORM; /*??*/

    return mask;
}
