#ifndef input_h
#define input_h
#include <array>
#include "utils.h"

typedef unsigned short ushort;
typedef float data;

const data coordinate = 0;
const data speed = 0;
const data frequency_pendulum = 1;
const data k_friction_linear = 1;
const data k_friction_cube = 0;
const data force = 0.01;
const data frequency_force = 1;
const data time = 100;
const data step = 0.01;
const ushort points = 1000;
HeunMethod<data, 2> method;
Solver<data, 2> solver;

#endif
