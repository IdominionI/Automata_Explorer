#pragma once

#include <glm/glm.hpp>

#include "Source/Application/vwa_application_parameters.h"

//enum class cubic_grid_value_data_type_enum { Boolean, Float, Integer, Double, nan };
typedef grid_value_data_type_enum cubic_grid_value_data_type_enum;

typedef int		   cubic_surface_index_data_type;
typedef glm::ivec3 cubic_surface_vec_data_type;   //Note Change this to appropriate glm vec data type as required to match cubic_surface_index_data_type eg for cubic_surface_index_data_type of int64 have glm::i64vec2

#define MAX_PEMISSIBLE_CUBIC_GRID_SIZE INT_MAX   //Note Change this to appropriate glm vec data type as required to match cubic_surface_index_data_type

#define MIN_PERMISSIBLE_CUBIC_SIZE 0.0001f

// Following for Hex values of type integer
#define INVALID_CUBIC_VALUE		   INT_MIN
#define INVALID_CUBIC_INT_DATA_VALUE INT_MIN
#define MIN_CUBIC_VALUE              INT_MIN+1
#define MAX_CUBIC_VALUE              INT_MAX

#define DEFAULT_ACTIVE_CUBIC_VALUE   0
#define DEFAULT_INACTIVE_CUBIC_VALUE INT_MIN