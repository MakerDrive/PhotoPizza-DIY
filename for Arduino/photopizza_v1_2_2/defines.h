#ifndef defines_h
#define defines_h
//// Modes
#define MENU_MODE 1
#define EDIT_MODE 2
#define EXEC_MODE 3

////  PARAMS
#define SPEED_MIN 0
#define SPEED_MAX 4300
#define SPEED_STEP 50

#define ROT_MIN 0
#define ROT_MAX 200000000  
#define ROT_STEP 1000

#define ACC_MIN 0
#define ACC_MAX 5000
#define ACC_STEP 100

#define NUM_PROGRAMS 4
#define PRESET_SIZE 14

#define CW -1  //clockwise
#define CCW 1  //counterclockwise

// parameter num
#define SPEEED 0
#define ROT 1
#define ACC 2
#define DIR 3

/////////Ir buttons map
#define BTN_POWER      23842         // pos 0 0
#define BTN_VOL_U      24480         // pos 0 1
#define BTN_FUNC       23587         // pos 0 2

#define BTN_RW         31747         // pos 1 0
#define BTN_PLAY       29962         // pos 1 1
#define BTN_FW         31492         // pos 1 2

#define BTN_CH_D       23970         // pos 2 0
#define BTN_VOL_D      29452         // pos 2 1
#define BTN_CH_U       24097         // pos 2 2


#define BTN_0          29835         // pos 3 0
#define BTN_EQ         30982         // pos 3 1
#define BTN_ST         30855         // pos 3 2

#define BTN_1          31110         // pos 4 0
#define BTN_2          29580         // pos 4 1
#define BTN_3          20655         // pos 4 2
#define BTN_4          31620         // pos 5 0
#define BTN_5          29070         // pos 5 1
#define BTN_6          21165         // pos 5 2
#define BTN_7          24225         // pos 6 0
#define BTN_8          22185         // pos 6 1
#define BTN_9          23205         // pos 6 2

///////// LCD buttons map
#define btnRIGHT  100
#define btnUP     101
#define btnDOWN   102
#define btnLEFT   103
#define btnSELECT 104
#define btnNONE   105

#endif
