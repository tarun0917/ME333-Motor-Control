#include "currentcontrol.h"
#include "encoder.h"
#include "NU32.h"
#include <stdio.h>


void getcurrentcounts(void){
  currentcounts = adc_sample_convert(7);
  sprintf(buffer,"%d\r\n", currentcounts);
  NU32_WriteUART3(buffer);
}

void getcurrentma(void){
  currentma = 2*adc_sample_convert(7) - 1023;
  sprintf(buffer,"%d\r\n", currentma);
  NU32_WriteUART3(buffer);
}
  void readcurrentgain(void){

  NU32_ReadUART3(buffer,BUF_SIZE);
  sscanf(buffer, "%f %f", &kpcurrenttemp, &kicurrenttemp);
  __builtin_disable_interrupts();
  errorcurrentint=0;
  kpcurrent=kpcurrenttemp;
  kicurrent=kicurrenttemp;
  __builtin_enable_interrupts();

}

void writecurrentgain(void){
  sprintf(buffer, "%f %f\r\n", kpcurrent, kicurrent);
  NU32_WriteUART3(buffer);
}

void makeWaveform(){
int i = 0, center = 0, A = 200;
  for (i = 0; i < NUMSAMPS; ++i)
  {
    if (i<NUMSAMPS/4)
    {
      Waveform[i]=center+A;
    }
    else if(i<NUMSAMPS/2 && i>NUMSAMPS/4)
    {
      Waveform[i]=center-A;
    }
    else if(i<3*NUMSAMPS/4 && i>NUMSAMPS/2)
    {
      Waveform[i]=center+A;
    }
    else
    {
      Waveform[i]=center-A;
    }
  }
}

void sendcurrentdata(void){
  sprintf(buffer, "%d\n", NUMSAMPS);
  NU32_WriteUART3(buffer);
  for (j = 0; j < NUMSAMPS; j++)
  {
    sprintf(buffer, "%d %d\n",Waveform[j],currentvalue[j]);
    NU32_WriteUART3(buffer);
  }
}
