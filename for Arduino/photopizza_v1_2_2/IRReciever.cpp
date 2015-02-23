// From IRrecvDemo

#include "IRReciever.h"

#include <Arduino.h>

extern volatile boolean start_interrupt;

/*
  use pulseIn to receive IR pulses from the remote.
 Record the length of these pulses (in ms) in an array
 */

static void read_pulse(int pulse[]);

/*
  IR pulses encode binary "0" as a short pulse, and binary "1"
 as a long pulse.  Given an array containing pulse lengths,
 convert this to an array containing binary values
 */

static void pulse_to_bits(int pulse[], int bits[]);

/*
  check returns proper first 14 check bits
 */

static void RemoteVerify(int bits[]);

/*
  convert an array of binary values to a single base-10 integer
 */

static int bits_to_int(int bits[]);

///////////  IR RECEIVER
int get_key() 
{

  //lcd.setCursor(15,0);
  //lcd.print("%");
  int pulse[IR_BIT_LENGTH];
  int bits[IR_BIT_LENGTH];
  int rez = btnNONE;
  start_interrupt = false;
  do {

    //rez = read_LCD_buttons(); //TODO: restore this!
  } //Wait for a start bit
  while(pulseIn(IR_PIN, HIGH,100000) < BIT_START && rez == btnNONE);
  if(rez != btnNONE){
    return rez;
  }
  read_pulse(pulse);
  pulse_to_bits(pulse, bits);
  RemoteVerify(bits);
  rez = bits_to_int(bits);
  //lcd.setCursor(15,0);
  //lcd.print("*");
  return rez;
}

void read_pulse(int pulse[])
{
  for (int i = 0; i < IR_BIT_LENGTH; i++)
  {
    pulse[i] = pulseIn(IR_PIN, HIGH);
  }
}

void pulse_to_bits(int pulse[], int bits[])
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
      //  Serial.println("Error");
    }
  }
}

void RemoteVerify(int bits[])
{
  int result = 0;
  int seed = 1;

  //Convert bits to integer
  for(int i = 0 ; i < (FirstLastBit) ; i++) 
  {       
    if(bits[i] == 1) 
    {
      result += seed;
    }

    seed *= 2;
  }

}


int bits_to_int(int bits[])
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



