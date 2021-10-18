/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------主控-----------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_board.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_led.h"
#include "hal_adc.h"
#include "hal_clock.h"
#include "hal_mcu.h"
#include "TIMER.h"
#include "util.h"
#include "sensor.h"
#include <stdio.h>
#include <string.h>

uint8 receive[128];//接收数据缓存
uint16 len;//接受数据长度
uint8 auto_1=1;//道闸自动打开功能开关，默认打开
uint16 value;//接受对射数据

#define KEY1 P1_2//按钮
#define sw P1_3//道闸开关控制
#define alarm P1_4//警报灯

static basicRfCfg_t basicRfConfig;

void ConfigRf_Init(void)//网络初始化
{
  basicRfConfig.panId = 0x0001;
  basicRfConfig.channel = 24;
  basicRfConfig.myAddr = 0x0001;
  basicRfConfig.ackRequest = TRUE;
  while(basicRfInit(&basicRfConfig)==FAILED);
  basicRfReceiveOn();
}

void change_sw()//改变道闸开关状态
{
  sw=1;
  alarm=1;//打开道闸
  halMcuWaitMs(10000);//等待10秒，可能已通过
  while(adc_fanshe()==0);//如果没有通过，道闸继续保持打开
  sw=0;
  alarm=0;//关闭道闸
}

void check_signal()
{
  if(receive[0]=='a'&&receive[1]=='e')//道闸自动打开功能开关
  {
    if(auto_1==1)
    {
     auto_1=0;
     halLedClear(1);
    }
    else if(auto_1==0)
    {
      auto_1=1;
      halLedSet(1);
    }
    halMcuWaitMs(1000);
  }
  else if(receive[0]=='g'&&receive[1]=='t')
  {
    halLedSet(2);
    change_sw();
    halLedClear(2);//遥控控制道闸会显示LED灯
  }
}

void check_duishe()
{
  int a=1;
  do{
    value=adc_duishe();
    if(value==1)
    {
      a=16; //有车辆通过直接跳过while执行下面的判断
    }
    halMcuWaitMs(1);
    a++;
    }while(a<16);//循环判断有无车通过红外对射
  if(value==1)
  {
    change_sw();
  }
}

void main()
{
  halBoardInit();
  ConfigRf_Init();
  adc_init();
  halLedSet(1);//用于指示道闸自动功能有无开启
  sw=0;
  alarm=0;//以上为初始化
  while(1)
  {
    if(KEY1==0)//判断按钮有没有按下
    {
      halMcuWaitMs(1);
      if(KEY1==0)
      {
        change_sw();//执行道闸开关命令
      }
    }
    else if(basicRfPacketIsReady())
    {
      len=basicRfReceive(receive,2,NULL);
      check_signal();
    }
    if(auto_1==1)
    {
      check_duishe();//执行对射监测命令
    }
  }
}