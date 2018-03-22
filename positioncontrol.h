#ifndef POSITIONCONTROL_H
#define POSITIONCONTROL_H

volatile float kpmotortemp = 0, kdmotortemp = 0, kimotortemp=0;
volatile float kpmotor=0, kimotor = 0 ,kdmotor =  0;
volatile float userangle = 0;
volatile float errorangleint = 0;
volatile float trackmotorangle[3000], trackuserangle[3000];
volatile int tracklength=0;
volatile float umotor = 0;

volatile float errorangle = 0;
volatile float prevangle = 0;
volatile float motorangle = 0;

volatile int trackcount =0;

volatile float umotortrack = 0, trackerrorangle=0;

void readpositiongain(void);
void writepositiongain(void);
void readuserangle(void);
void readstep(void);
void readcubic(void);
void sendmotordata(void);
#endif
