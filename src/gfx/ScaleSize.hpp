#pragma once
//#define SIMPLE_SIZE

#ifdef SIMPLE_SIZE
#define SCALE_SIZE 1
#define SCALE_DIVIDE 2
#else
// default: scaled 200%
#define SCALE_SIZE 2
#define SCALE_DIVIDE 1
#endif
