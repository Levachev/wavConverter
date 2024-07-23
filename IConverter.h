#pragma once
#include <vector>
#include <cstdint>
#include <array>

const size_t BUFFER_OF_DATA_SIZE=44100;


class IConverter
{
 public:
  virtual ~IConverter( ) = default;

  virtual void convert(std::array<int16_t, BUFFER_OF_DATA_SIZE> &buffer_of_current_data, std::array<int16_t, BUFFER_OF_DATA_SIZE> &buffer_of_secondary_data) = 0;
};