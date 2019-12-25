/*
  Definitions of all the CAN IDs for each message given by the ECU.
  Refer to the Documentation in the Q20 GitHub for the data contained in each
  message data frame.
*/

// Mask Definition
// The upper 4 digits of the ECU CAN IDs need to be masked off
// Do Bitwise AND on the MASK and the defined ID
#define ID_MASK 0x0000FFFF
/* #define MASKED 1 */
// ECU CAN IDs
#ifdef MASKED

#define PE1 0x0000F048
#define PE2 0x0000F148
#define PE3 0x0000F248
#define PE4 0x0000F348
#define PE5 0x0000F448
#define PE6 0x0000F548
#define PE7 0x0000F648
#define PE8 0x0000F748
#define PE9 0x0000F848
#define PE10 0x0000F948

#define PE11 0x0000FA48
#define PE12 0x0000FB48
#define PE13 0x0000FC48
#define PE14 0x0000FD48
#define PE15 0x0000FE48
#define PE16 0x0000D048

#else

#define PE1 0x0CFFF048
#define PE2 0x0CFFF148
#define PE3 0x0CFFF248
#define PE4 0x0CFFF348
#define PE5 0x0CFFF448
#define PE6 0x0CFFF548
#define PE7 0x0CFFF648
#define PE8 0x0CFFF748
#define PE9 0x0CFFF848
#define PE10 0x0CFFF948

#define PE11 0x0CFFFA48
#define PE12 0x0CFFFB48
#define PE13 0x0CFFFC48
#define PE14 0x0CFFFD48
#define PE15 0x0CFFFE48
#define PE16 0x0CFFD048

#endif
