#ifndef input_h
#define input_h
#include <array>
#include "utils.h"

typedef unsigned short ushort;
typedef float data;

const data coordinate1 = 3.14;
const data coordinate2 = 3.14;
const data speed1 = 0;
const data speed2 = 0;
const data frequency_pendulum = 1;
const data g = 10;
const data l1 = 1;
const data l2 = 1;
const data m1 = 1;
const data m2 = 1;
const data k_friction_linear = 2;
const data k_friction_cube = 0;
const data force = 1;
const data frequency_force = 1;
const data time = 60;
const data step = 0.01;
const ushort points = 10000;
HeunMethod<data, 4> method;
Solver<data, 4> solver;

#endif
