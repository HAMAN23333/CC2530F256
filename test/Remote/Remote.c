/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------遥控器-----------------------------------------------------------------*/
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
#include "hal_uart.h"

uint8 auto_1[128]="ae";//自动功能
uint8 sw[128]="gt";//道闸开关

#define KEY1 P1_2
#define KEY2 P1_6//外接一个

static basicRfCfg_t basicRfConfig;

void ConfigRf_Init(void)
{
  basicRfConfig.panId = 0x0001;
  basicRfConfig.channel = 24;
  basicRfConfig.myAddr = 0x0002;
  basicRfConfig.ackRequest = TRUE;
  while(basicRfInit(&basicRfConfig)==FAILED);
  basicRfReceiveOn();
}

void check_key()//检查遥控按钮状态
{
  if(KEY1==0)
  {
    halMcuWaitMs(1);
    if(KEY1==0)//执行道闸开关
    {
      while(KEY1==0)
      {
        halLedSet(2);
        basicRfSendPacket(0x0001,sw,2);
        halMcuWaitMs(250);
      }
      halLedClear(2);
    }
  }
  if(KEY2==0)
  {
    halMcuWaitMs(1);
    if(KEY2==0)//改变道闸自动打开功能开关
    {
      while(KEY2==0)
      {
        halLedSet(1);
        basicRfSendPacket(0x0001,auto_1,2);
        halMcuWaitMs(250);
      }
      halLedClear(1);
    }
  }
}

void main()
{
  halBoardInit();
  ConfigRf_Init();
  while(1)
  {
    check_key();//检查遥控按钮状态
  }
}