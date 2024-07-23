#pragma once
#include "Instructions.h"
#include <array>
const size_t BUFFER_OF_HEADER_SIZE=44;

class Process
{
    char header[BUFFER_OF_HEADER_SIZE];
    bool read_data(std::ifstream &current_stream, std::array<int16_t, BUFFER_OF_DATA_SIZE> &buffer_of_data);
    bool write_data(std::ofstream &current_stream, std::array<int16_t, BUFFER_OF_DATA_SIZE> &buffer_of_data);
    bool read_header(std::ifstream &current_stream);
    bool write_header(std::ofstream &output_stream);
    bool assign_data(std::ifstream &input_stream, std::ofstream &output_stream);
public:
    bool run(std::vector<instr> &instructions, std::vector<std::string> &name_of_input_files, const Factory &factory, std::string &output_file_name);
};