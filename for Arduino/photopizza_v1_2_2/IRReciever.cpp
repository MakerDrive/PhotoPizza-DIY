// From IRrecvDemo

#include "IRReciever.h"

#include <Arduino.h>

extern volatile boolean start_interrupt;

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
  check returns proper first 14 check bits
 */

//static void RemoteVerify(int bits[]);

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

///////////  IR RECEIVER
void IrDump(){
  for(char i = 0; i < IR_BIT_LENGTH; i++){
    Serial.println((String)(int)i + ": " + pulse[i] );
  }
}

int get_key()
{

  //lcd.setCursor(15,0);
  //lcd.print("%");


  int rez = btnNONE;
  start_interrupt = false;
  //do {

    //rez = read_LCD_buttons(); //TODO: restore this!
  //} //Wait for a start bit
  if(pulseIn(IR_PIN, HIGH,100000) > BIT_START)
    return btnNONE;

  if(rez != btnNONE){
    return rez;
  }
  read_pulse(pulse);
  IrDump();
  if(!pulse_to_bits(pulse, bits))
    return btnNONE;
  //RemoteVerify(bits);
  rez = bits_to_int(bits);
  return rez;
}

void Ir_process(){

  cli(); //TODO: correctly set/reset IRQ

  if(ready){
    Serial.println("RDY");
    if(pulse_to_bits(pulse, bits)){
      IrDump();
      Serial.println((String) "ReadCode: " + bits_to_int(bits));
    }else
      Serial.println("ReadCode: Parsing error");
    arrPos = 0;
    ready = false;
  }

  sei();
}

void Ir_interrupt(){
  if(ready){
    Serial.println("BSY (IRQ)");
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

  if(pulseTime - lastPulseTime > 100000L){
    //Serial.println("Timeo");
    reading = false;
    ready = false;
    arrPos = 0;
    return;
  }

  if(arrPos == 0 && state){
    lastPulseTime = pulseTime;
    return;
  }

  if(reading && state == 1){
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
    //reading = false;
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



