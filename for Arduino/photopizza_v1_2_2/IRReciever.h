/*
 * IRReciever.h
 *
 *  Created on: 23 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_IRRECIEVER_H_
#define PHOTOPIZZA_V1_2_2_IRRECIEVER_H_


#include "defines.h"

int get_key();
void Ir_interrupt();
void Ir_process();

///////////  IR
#define IR_BIT_LENGTH 32    // number of bits sent by IR remote
#define FirstLastBit 15     // divide 32 bits into two 15 bit chunks for integer variables. Ignore center two bits. they are all the same.
#define BIT_1 1500          // Binary 1 threshold (Microseconds)
#define BIT_0 450           // Binary 0 threshold (Microseconds)
#define BIT_START 4000      // Start bit threshold (Microseconds)

#define IR_PIN 2            // IR Sensor pin


#endif /* PHOTOPIZZA_V1_2_2_IRRECIEVER_H_ */
