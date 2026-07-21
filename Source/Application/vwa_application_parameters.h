#pragma once

//#define INVALID_INT_DATA_VALUE INT_MIN

enum class grid_value_data_type_enum { Boolean, Float, Integer, Double, nan };
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))