#pragma once

#include <glm/glm.hpp>

enum class hex_grid_value_data_type_enum { Boolean, Float, Integer, Double, nan };

typedef int		   hex_surface_index_data_type;
typedef glm::ivec2 hex_surface_vec_data_type;   //Note Change this to appropriate glm vec data type as required to match hex_surface_index_data_type eg for hex_surface_index_data_type of int64 have glm::i64vec2

#define MAX_PEMISSIBLE_HEX_GRID_SIZE INT_MAX   //Note Change this to appropriate glm vec data type as required to match hex_surface_index_data_type

#define MIN_PERMISSIBLE_HEX_SIZE 0.0001f

// Following for Hex values of type integer
#define INVALID_HEX_VALUE	   INT_MIN
#define INVALID_INT_DATA_VALUE INT_MIN
#define MIN_HEX_VALUE          INT_MIN+1
#define MAX_HEX_VALUE          INT_MAX

#define DEFAULT_ACTIVE_HEX_VALUE   0
#define DEFAULT_INACTIVE_HEX_VALUE INT_MIN

// Following for hex values of type XXXXX