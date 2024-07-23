#include "Process.h"

namespace
{
int16_t to_int16_t(uint8_t v1, uint8_t v2)
{
    int16_t tmp = (((uint16_t)(v1)) << 8) | (uint16_t)(v2);
    return tmp;
}
size_t get_full_len_of_seconds(std::ifstream &stream)
{
    stream.seekg(BUFFER_OF_HEADER_SIZE, std::ios_base::end);
    size_t full_len_of_bytes=stream.tellg();
    size_t full_len_of_second=((full_len_of_bytes-BUFFER_OF_HEADER_SIZE)/2)/BUFFER_OF_DATA_SIZE;
    stream.seekg(0, std::ios_base::beg);
    return full_len_of_second;
}
}

bool Process::read_header(std::ifstream &current_stream)
{
    current_stream.read(header, BUFFER_OF_HEADER_SIZE);
    if(!current_stream)
    {
        return false;
    }
    return true;
}

bool Process::write_header(std::ofstream &output_stream)
{
    output_stream.write(header, BUFFER_OF_HEADER_SIZE);
    if(!output_stream)
    {
        return false;
    }
    return true;
}

bool Process::read_data(std::ifstream &current_stream, std::array<int16_t, BUFFER_OF_DATA_SIZE> &buffer_of_data)
{
    char fisrt_part=0, second_part=0;
    int16_t value=0;

    for(int32_t i=0;i<BUFFER_OF_DATA_SIZE;i++)
    {
        current_stream.get(fisrt_part);
        if(!current_stream)
        {
            return false;
        }
        current_stream.get(second_part);
        if(!current_stream)
        {
            return false;
        }
        value=to_int16_t(second_part, fisrt_part);
        buffer_of_data[i]=value;
    }
    return true;
}

bool Process::write_data(std::ofstream &current_stream, std::array<int16_t, BUFFER_OF_DATA_SIZE> &buffer_of_data)
{
    for(int32_t i=0;i<BUFFER_OF_DATA_SIZE;i++)
    {
        uint16_t t=buffer_of_data[i];
        uint16_t tmp = (t<<8)>>8;
        char val=char(tmp);
        current_stream.put(val);
        if(!current_stream)
        {
            return false;
        }

        t=buffer_of_data[i];

        tmp = t>>8;
        val=char(tmp);
        current_stream.put(val);
        if(!current_stream)
        {
            return false;
        }
    }
    return true;
}

bool Process::assign_data(std::ifstream &input_stream, std::ofstream &output_stream)
{
    std::array<int16_t, BUFFER_OF_DATA_SIZE> buffer;
    
    size_t len_data_of_seconds=get_full_len_of_seconds(input_stream);

    input_stream.seekg(BUFFER_OF_HEADER_SIZE, std::ios_base::beg);
    output_stream.seekp(BUFFER_OF_HEADER_SIZE, std::ios_base::beg);

    for(int32_t i=0;i<len_data_of_seconds;i++)
    {
        if(!read_data(input_stream, buffer))
        {
            return false;
        }
        if(!write_data(output_stream, buffer))
        {
            return false;
        }
    }
    return true;
}

bool Process::run(std::vector<instr> &instructions, std::vector<std::string> &name_of_input_files, const Factory &factory, std::string &output_file_name)
{
    std::ifstream current_stream(name_of_input_files[0]);
    if(!current_stream.is_open())
    {
        return false;
    }
    std::ofstream output_stream(output_file_name);
    if(!output_stream.is_open())
    {
        return false;
    }

    size_t full_len_of_second_of_current_stream=get_full_len_of_seconds(current_stream);

    std::array<int16_t, BUFFER_OF_DATA_SIZE> buffer_of_current_data;
    std::array<int16_t, BUFFER_OF_DATA_SIZE> buffer_of_secondary_data;

    if(!read_header(current_stream))
    {
        return false;
    }
    if(!write_header(output_stream))
    {
        return false;
    }

    assign_data(current_stream, output_stream);


    for(int32_t i=0;i<instructions.size();i++)
    {
        if(instructions[i].converter_name=="mix" || instructions[i].converter_name=="insert")
        {
            std::ifstream secondary_stream(name_of_input_files[instructions[i].number-1]);

            size_t full_len_of_second_of_secondary_stream=get_full_len_of_seconds(secondary_stream);

            secondary_stream.seekg(BUFFER_OF_HEADER_SIZE+instructions[i].begin_time*2*BUFFER_OF_DATA_SIZE, std::ios_base::beg);
            current_stream.seekg(BUFFER_OF_HEADER_SIZE+instructions[i].begin_time*2*BUFFER_OF_DATA_SIZE, std::ios_base::beg);
            output_stream.seekp(BUFFER_OF_HEADER_SIZE+instructions[i].begin_time*2*BUFFER_OF_DATA_SIZE, std::ios_base::beg);

            auto strategy = factory.create(instructions[i].converter_name);
            if(instructions[i].end_time==-1)
            {
                instructions[i].end_time=full_len_of_second_of_current_stream;
            }

            for(int32_t j=instructions[i].begin_time; j<instructions[i].end_time && j<full_len_of_second_of_secondary_stream;j++)
            {
                if(!read_data(current_stream, buffer_of_current_data))
                {
                    return false;
                }
                if(!read_data(secondary_stream, buffer_of_secondary_data))
                {
                    return false;
                }
                strategy->convert(buffer_of_current_data, buffer_of_secondary_data);
                if(!write_data(output_stream, buffer_of_current_data))
                {
                    return false;
                }
            }
        }
        else
        {
            current_stream.seekg(BUFFER_OF_HEADER_SIZE+instructions[i].begin_time*2*BUFFER_OF_DATA_SIZE, std::ios_base::beg);
            output_stream.seekp(BUFFER_OF_HEADER_SIZE+instructions[i].begin_time*2*BUFFER_OF_DATA_SIZE, std::ios_base::beg);

            auto strategy = factory.create(instructions[i].converter_name);
            if(instructions[i].end_time==-1)
            {
                instructions[i].end_time=full_len_of_second_of_current_stream;
            }
            for(int32_t j=instructions[i].begin_time; j<instructions[i].end_time;j++)
            {
                if(!read_data(current_stream, buffer_of_current_data))
                {
                    return false;
                }
                strategy->convert(buffer_of_current_data, buffer_of_current_data);
                if(!write_data(output_stream, buffer_of_current_data))
                {
                    return false;
                }
            }
        }

    }
    return true;
}
