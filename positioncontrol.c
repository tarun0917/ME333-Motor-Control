#include "positioncontrol.h"
#include "currentcontrol.h"
#include "encoder.h"
#include "NU32.h"
#include <stdio.h>
#include "utilities.h"


void readpositiongain(void){

  NU32_ReadUART3(buffer,BUF_SIZE);
  sscanf(buffer, "%f %f %f", &kpmotortemp,&kdmotortemp, &kimotortemp);
  __builtin_disable_interrupts();
  errorcurrentint=0;
  kpmotor=kpmotortemp;
  kimotor=kimotortemp;
  kdmotor = kdmotortemp;
  __builtin_enable_interrupts();

}

void writepositiongain(void){
  sprintf(buffer, "%f %f %f\r\n", kpmotor, kdmotor, kimotor);
  NU32_WriteUART3(buffer);
}


void readuserangle(void){
__builtin_disable_interrupts();
NU32_ReadUART3(buffer, BUF_SIZE);
sscanf(buffer, "%f", &userangle);
errorangleint = 0;
setMODE(HOLD);
__builtin_enable_interrupts();
}

void readstep(void){
  NU32_ReadUART3(buffer, BUF_SIZE);
  sscanf(buffer,"%d",&tracklength);
  for(j=0;j<tracklength;j++)
  {
    trackuserangle[j]= 0;
    trackmotorangle[j]=0;

  }
  for(j=0;j<tracklength;j++)
  {
    NU32_ReadUART3(buffer, BUF_SIZE);
    sscanf(buffer,"%f",&trackuserangle[j]);
  }
}

void readcubic(void)
{
  NU32_ReadUART3(buffer, BUF_SIZE);
  sscanf(buffer,"%d",&tracklength);
  for(j=0;j<tracklength;j++)
  {
    trackuserangle[j]= 0;
    trackmotorangle[j]=0;

  }
  for(j=0;j<tracklength;j++)
  {
    NU32_ReadUART3(buffer, BUF_SIZE);
    sscanf(buffer,"%f",&trackuserangle[j]);
  }
}

void sendmotordata(void){

  sprintf(buffer,"%d\n",tracklength);
  NU32_WriteUART3(buffer);
  for(j=0;j<tracklength;j++)
  {
    sprintf(buffer, "%f %f\r\n", trackuserangle[j] , trackmotorangle[j]);
    NU32_WriteUART3(buffer);
  }


}
