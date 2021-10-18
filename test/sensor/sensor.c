#include "iocc2530.h"
#include "hal_defs.h"

void adc_init()
{
  P0SEL |= 0x03;
  P0DIR &= ~0x03;
}

uint16 adc_fanshe()//获取反射值
{
  return P0_1;
}

uint16 adc_duishe()//获取对射值
{
  return P0_0;
}