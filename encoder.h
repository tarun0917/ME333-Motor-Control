#ifndef ENCODER_H
#define ENCODER_H


#define BUF_SIZE 200
char buffer[BUF_SIZE];
volatile int encoderdegree = 0;
static int encoder_command(int read);

void encoder_init(void);                       //Function to initialse the encoder
int encoder_counts(void);                      //Function to return the encoder count
int encoder_reset(void);                       //Function to reset the count
void encoder_degree(void);                     //Function to get encoder reading in degrees

#endif
