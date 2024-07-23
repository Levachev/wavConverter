#pragma once
#include "Factory.h"
#include <iostream>
#include "InsertConverter.h"
#include "MixConverter.h"
#include "MuteConverter.h"

bool parsing_input(int argc, char *argv[], std::string &file_name, std::string &output_file_name, std::vector<std::string> &name_of_input_files);

void factory_store(Factory &factory);