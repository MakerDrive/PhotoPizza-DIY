 
#define IR_BIT_LENGTH 32    // number of bits sent by IR remote
#define FirstLastBit 15     // divide 32 bits into two 15 bit chunks for integer variables. Ignore center two bits. they are all the same.
#define BIT_1 1500          // Binary 1 threshold (Microseconds)
#define BIT_0 450           // Binary 0 threshold (Microseconds)
#define BIT_START 4000      // Start bit threshold (Microseconds)
 
#define IR_PIN 2            // IR Sensor pin
 
void setup() {
  pinMode(IR_PIN, INPUT);
  Serial.begin(9600);
}
 
void loop() {
  int key = get_ir_key();
  do_response(key);
  delay(130);                  // 2 cycle delay to cancel duplicate keypresses
}
 
/*
  wait for a keypress from the IR remote, and return the
  integer mapping of that key (e.g. power button on remote returns 
  the integer 1429)
*/
 
int get_ir_key() 
{
  int pulse[IR_BIT_LENGTH];
  int bits[IR_BIT_LENGTH];
 
  do {} //Wait for a start bit
  while(pulseIn(IR_PIN, HIGH) < BIT_START);
 
  read_pulse(pulse);
  pulse_to_bits(pulse, bits);
  RemoteVerify(bits);
  return bits_to_int(bits);
}
 
 
/*
  use pulseIn to receive IR pulses from the remote.
  Record the length of these pulses (in ms) in an array
*/
 
void read_pulse(int pulse[])
{
  for (int i = 0; i < IR_BIT_LENGTH; i++)
  {
    pulse[i] = pulseIn(IR_PIN, HIGH);
  }
}
 
/*
  IR pulses encode binary "0" as a short pulse, and binary "1"
  as a long pulse.  Given an array containing pulse lengths,
  convert this to an array containing binary values
*/
 
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
      Serial.println("Error");
    }
  }
}
 
/*
  check returns proper first 14 check bits
*/
 
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
 
 
/*
  convert an array of binary values to a single base-10 integer
*/
 
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
 
 
/* 
  respond to specific remote-control keys with different behaviors
*/
 
void do_response(int key)
{  
   
  
  switch (key)
  {
    /*case 32640:  // turns on UUT power
      Serial.println("POWER");
      break;
 
    case 32385:  // FUNC/STOP turns off UUT power
      Serial.println("FUNC/STOP");
      break;
 
    case 32130:  // |<< ReTest failed Test
      Serial.println("|<<");
      break;
 
    case 32002:  // >|| Test
      Serial.println(">||");
      break;
 
    case 31875:  // >>| perform selected test number
      Serial.println(">>|");
      break;
 
    case 32512:  // VOL+ turns on individual test beeper
      Serial.println("VOL+");
      break;
 
    case 31492:  // VOL- turns off individual test beeper
      Serial.println("VOL-");
      break;
 
    case 31620:  // v scroll down tests
      Serial.println("v");
      break;
 
    case 31365:  // ^ scroll up tests
      Serial.println("^");
      break;
 
    case 30982:  // EQ negative tests internal setup
      Serial.println("EQ");
      break;
 
    case 30855:  // ST/REPT Positive tests Select Test and Repeat Test
    Serial.println("ST/REPT");
      break;
 
    case 31110:  // 0
      Serial.println("0");
      break;
 
    case 30600:  // 1
      Serial.println("1");
      break;
 
    case 30472:  // 2
      Serial.println("2");
      break;
 
    case 30345:  // 3
      Serial.println("3");
      break;
 
    case 30090:  // 4
      Serial.println("4");
      break;
 
    case 29962:  // 5
      Serial.println("5");
      break;
 
    case 29835:  // 6
      Serial.println("6");
      break;
 
    case 29580:  // 7
      Serial.println("7");
      break;
 
    case 29452:  // 8
      Serial.println("8");
      break;
 
    case 29325:  // 9
      Serial.println("9");
      break;
      */
    default:
      {
        Serial.print("Key ");
        Serial.print(key);
        Serial.println("");
      }
    break;
  }
}
