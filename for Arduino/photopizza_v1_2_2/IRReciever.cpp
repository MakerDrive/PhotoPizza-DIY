// From IRrecvDemo

#include "IRReciever.h"

#include <Arduino.h>

///////////  IR
#define IR_BIT_LENGTH 32    // number of bits sent by IR remote
#define FirstLastBit 15     // divide 32 bits into two 15 bit chunks for integer variables. Ignore center two bits. they are all the same.
#define BIT_1 1500          // Binary 1 threshold (Microseconds)
#define BIT_0 450           // Binary 0 threshold (Microseconds)
#define BIT_START 4000      // Start bit threshold (Microseconds)

#define IR_PIN 2            // IR Sensor pin

#define IR_TIMEO 100000L    //

/*
  use pulseIn to receive IR pulses from the remote.
 Record the length of these pulses (in ms) in an array
 */

static void read_pulse(volatile int pulse[]);

/*
  IR pulses encode binary "0" as a short pulse, and binary "1"
 as a long pulse.  Given an array containing pulse lengths,
 convert this to an array containing binary values
 */

static bool pulse_to_bits(volatile int pulse[], volatile int bits[]);

/*
  convert an array of binary values to a single base-10 integer
 */

static int bits_to_int(volatile int bits[]);

static volatile int pulse[IR_BIT_LENGTH];
static volatile int bits[IR_BIT_LENGTH];
static volatile char arrPos = 0;
static volatile unsigned long lastPulseTime = 0;
static volatile bool ready = false;
static volatile bool reading = false;

static void Ir_interrupt();

///////////  IR RECEIVER
void IrDump(){
  for(char i = 0; i < IR_BIT_LENGTH; i++){
    Serial.println((String)(int)i + ": " + pulse[i] );
  }
}

int Ir_getKey(){
  int key = 0;
  if(ready){
    //Serial.println("RDY");
    if(pulse_to_bits(pulse, bits)){
      //IrDump();
      key = bits_to_int(bits);
      Serial.println((String) "IR ReadCode: " + key);
    }else
      Serial.println("IR ReadCode: Parsing error");
    arrPos = 0;
    ready = false;
  }

  return key;
}

void Ir_init(){
  pinMode(IR_PIN, INPUT_PULLUP);
  attachInterrupt(0, Ir_interrupt, CHANGE);
  interrupts();
}

void Ir_interrupt(){
  if(ready){ //skip all frames if current key is not read yet
    //TODO: inc stat. frame overrun
    //Serial.println("BSY (IRQ)");
    return;
  }
  unsigned long pulseTime = micros();
  int state = digitalRead(IR_PIN);

  if(!reading){ //receiving new packet
    if(arrPos == 0 && state){
      //Serial.println("FE");
      //TODO: inc Sat frame start error;
      lastPulseTime = pulseTime;
      return;
    }
    reading = true;
    lastPulseTime = pulseTime;
    return;
  }

  if(lastPulseTime > pulseTime){
    //Serial.println("OVR (IRQ)");
    //TODO: inc stat. TimeOverflow, reset frame or calc correct value
    arrPos = 0;
    lastPulseTime = pulseTime;
    return;
  }

  unsigned long timeo = pulseTime - lastPulseTime;
  if(reading && ( timeo > IR_TIMEO)){
    //Serial.println((String) "Timeo: " + timeo);
    //TODO: stat timeo increase
    reading = false;
    arrPos = 0;
    lastPulseTime = pulseTime;
    return;
  }

  if(arrPos == 0 && state){ //skip start bit
    lastPulseTime = pulseTime;
    return;
  }

  if(reading && state == 1){ //do not count time of low pulses
    lastPulseTime = pulseTime;
    return;
  }

  pulse[arrPos] = pulseTime - lastPulseTime;

  if(pulse[arrPos] < BIT_START) //skip long impulses
    ++arrPos;

  if(arrPos > IR_BIT_LENGTH){
    //Serial.println("LEN");
    ready = true;
    reading = false;
    //TODO: inc stat frames recieved, process data
    arrPos = 0;
  }
  lastPulseTime = pulseTime;
}

void read_pulse(volatile int pulse[])
{
  for (int i = 0; i < IR_BIT_LENGTH; i++)
  {
    pulse[i] = pulseIn(IR_PIN, HIGH);
  }
}

bool pulse_to_bits(volatile int pulse[], volatile int bits[])
{

  for(int i = 0; i < IR_BIT_LENGTH; i++) 
  {

    if(pulse[i] > BIT_1) //is it a 1?
    {
      bits[i] = 1;
    }  
    else if(pulse[i] > BIT_0) //is it a 0?
    {
      bits[i] = 0;
    } 
    else //data is invalid...
    {
      return false;
    }
  }
  return true;
}

int bits_to_int(volatile int bits[])
{
  int result = 0;
  int seed = 1;

  //Convert bits to integer
  for(int i = (IR_BIT_LENGTH-FirstLastBit) ; i < IR_BIT_LENGTH ; i++) 
  {       
    if(bits[i] == 1) 
    {
      result += seed;
    }   
    seed *= 2;
  }
  return result;
}



