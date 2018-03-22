#ifndef CURRENTCONTROL_H
#define CURRENTCONTROL_H


#define NUMSAMPS 100

volatile int Waveform[NUMSAMPS];

volatile int j = 0;
volatile int currentvalue[NUMSAMPS];
volatile int currentcounts = 0;
volatile int currentma = 0;


volatile float kpcurrenttemp=0, kicurrenttemp=0;
volatile float kpcurrent=0, kicurrent=0;
volatile float errorcurrentint = 0;
volatile float errorcurrent = 0;


volatile float ucurrent = 0;
volatile float ucurrentnew = 0;

void getcurrentcounts(void);            //Function to get the current counts
void getcurrentma(void);                //Function to get the current in milliamperes
void readcurrentgain(void);             //Function to read the Kp and Ki values for the current control
void writecurrentgain(void);            //Function to write the Kp and Ki values for the current control
void makeWaveform(void);                //Function to make the reference Waveform
void sendcurrentdata(void);             //Function to send the data to Matlab for plotting the graph

#endif
