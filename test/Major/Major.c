/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------����-----------------------------------------------------------------*/
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

uint8 receive[128];//�������ݻ���
uint16 len;//�������ݳ���
uint8 auto_1=1;//��բ�Զ��򿪹��ܿ��أ�Ĭ�ϴ�
uint16 value;//���ܶ�������

#define KEY1 P1_2//��ť
#define sw P1_3//��բ���ؿ���
#define alarm P1_4//������

static basicRfCfg_t basicRfConfig;

void ConfigRf_Init(void)//�����ʼ��
{
  basicRfConfig.panId = 0x0001;
  basicRfConfig.channel = 24;
  basicRfConfig.myAddr = 0x0001;
  basicRfConfig.ackRequest = TRUE;
  while(basicRfInit(&basicRfConfig)==FAILED);
  basicRfReceiveOn();
}

void change_sw()//�ı��բ����״̬
{
  sw=1;
  alarm=1;//�򿪵�բ
  halMcuWaitMs(10000);//�ȴ�10�룬������ͨ��
  while(adc_fanshe()==0);//���û��ͨ������բ�������ִ�
  sw=0;
  alarm=0;//�رյ�բ
}

void check_signal()
{
  if(receive[0]=='a'&&receive[1]=='e')//��բ�Զ��򿪹��ܿ���
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
    halLedClear(2);//ң�ؿ��Ƶ�բ����ʾLED��
  }
}

void check_duishe()
{
  int a=1;
  do{
    value=adc_duishe();
    if(value==1)
    {
      a=16; //�г���ͨ��ֱ������whileִ��������ж�
    }
    halMcuWaitMs(1);
    a++;
    }while(a<16);//ѭ���ж����޳�ͨ���������
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
  halLedSet(1);//����ָʾ��բ�Զ��������޿���
  sw=0;
  alarm=0;//����Ϊ��ʼ��
  while(1)
  {
    if(KEY1==0)//�жϰ�ť��û�а���
    {
      halMcuWaitMs(1);
      if(KEY1==0)
      {
        change_sw();//ִ�е�բ��������
      }
    }
    else if(basicRfPacketIsReady())
    {
      len=basicRfReceive(receive,2,NULL);
      check_signal();
    }
    if(auto_1==1)
    {
      check_duishe();//ִ�ж���������
    }
  }
}