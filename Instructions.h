#pragma once
#include "StructOfInstructions.h"
#include "Factory.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class Instructions
{
public:
	bool get_instructions(std::string config_file_name, const Factory &factory, std::vector<instr> &result);
};