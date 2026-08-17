#pragma once

#include <glm/glm.hpp>

#include "Source/Application/vwa_application_parameters.h"

//enum class cart2D_grid_value_data_type_enum { Boolean, Float, Integer, Double, nan };
typedef grid_value_data_type_enum cart2D_grid_value_data_type_enum;

typedef int		   cart2D_surface_index_data_type;
typedef glm::ivec2 cart2D_surface_vec_data_type;   //Note Change this to appropriate glm vec data type as required to match cart2D_surface_index_data_type eg for cart2D_surface_index_data_type of int64 have glm::i64vec2

#define MAX_PEMISSIBLE_CART2D_GRID_SIZE INT_MAX   //Note Change this to appropriate glm vec data type as required to match cart2D_surface_index_data_type

#define MIN_PERMISSIBLE_CART2D_SIZE 0.0001f

// Following for Hex values of type integer
#define INVALID_CART2D_VALUE		   INT_MIN
#define INVALID_CART2D_INT_DATA_VALUE INT_MIN
#define MIN_CART2D_VALUE              INT_MIN+1
#define MAX_CART2D_VALUE              INT_MAX

#define DEFAULT_ACTIVE_CART2D_VALUE   0
#define DEFAULT_INACTIVE_CART2D_VALUE INT_MIN

// Following for cart2D values of type XXXXX