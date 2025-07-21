// paciFIST studios. 2025. MIT License

#ifndef DEFINES_H
#define DEFINES_H

// Common Types --------------------------------------------------------------------------------------------------------

// use of LongLong (LL) indicates to most compilers that this is 64 bit.
// if the number is small enough, we can still assign it to an int32
#define Kibibytes(value) ((value) * 1024LL)
#define Mebibytes(value) (Kibibytes(value) * 1024LL)
#define Gibibytes(value) (Mebibytes(value) * 1024LL)
#define Tebibytes(value) (Gibibytes(value) * 1024LL)


#define TRUE 1
#define FALSE 0

// Constant Values -----------------------------------------------------------------------------------------------------


// The value of PI, in 32 bits
#define PI_32 = 3.14159265359f
// The value of Tau (2PI) in 32 bits
#define TAU_32 = 6.28318530718f 

// 2 / 65535
#define TWO_OVER_INT16 0.000030518f 


#endif //DEFINES_H
