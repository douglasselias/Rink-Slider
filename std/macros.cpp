#pragma once

/// @todo: is these really necessary?

#define declare_array(type, name, size) \
  u64 name##_size = size; \
  type name[size] = {}

#define for_each(name) \
  for(u64 i = 0; i < name##_size; i++)

#define for_each_index(name, index) \
  for(u64 index = 0; index < name##_size; index++)

#define declare_array2d(type, name, w, h) \
  u64 name##_width = w; \
  u64 name##_height = h; \
  type name[w][h] = {}

#define for_each_width(name, index) \
  for(u64 index = 0; index < name##_width; index++)

#define for_each_height(name, index) \
  for(u64 index = 0; index < name##_height; index++)