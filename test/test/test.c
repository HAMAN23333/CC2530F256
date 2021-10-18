/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------测试文件-----------------------------------------------------------------*/
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

void main()
{
  halBoardInit();
  ConfigRf_Init();
  while(1)
  {
    if(basicRfPacketIsReady())
    {
      len=basicRfReceive(receive,strlen(receive),NULL);
      if(receive[0]=='g'&&receive[1]=='t')
      {
       halLedSet(1); 
      }
    }
  }
}