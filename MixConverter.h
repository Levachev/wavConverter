#pragma once
#include "IConverter.h"

class MixConverter : public IConverter
{
 public:
	void convert(std::array<int16_t, BUFFER_OF_DATA_SIZE> &buffer_of_current_data, std::array<int16_t, BUFFER_OF_DATA_SIZE> &buffer_of_secondary_data) override;
};