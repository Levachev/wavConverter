
#include "func.h"
#include "Process.h"
#include "Instructions.h"


Factory factory;
std::vector<instr> instructions;

int main(int argc, char* argv[])
{
    factory_store(factory);

    std::string config_file_name;
    std::string output_file_name;
    std::vector<std::string> name_of_input_files;

    if(!parsing_input(argc, argv, config_file_name, output_file_name, name_of_input_files))
    {
        return 0;
    }

    Instructions i;
    if(!(i.get_instructions(config_file_name, factory, instructions)))
    {
        return 0;
    }

    Process process;
    if(!process.run(instructions, name_of_input_files, factory, output_file_name))
    {
        std::cerr << "Error! Something wrong, use key \"-h\", to help" << std::endl;
    }

    return 0;
}