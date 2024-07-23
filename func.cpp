#include "func.h"

namespace
{
void help()
{
    std::cout<<"The program starts like this:"<<std::endl;
    std::cout<<"> ./audio -c <config.txt> <output.wav> <input1.wav> <input2.wav> …"<<std::endl<<std::endl;
    std::cout<<"\t<config.txt> - configuration file, rule of input config file:"<<std::endl;
    std::cout<<"\t\tThere are 3 commands: mute, mix, insert. if the command to mix "<<std::endl;
    std::cout<<"\t\tis entered, then the next parameter should be the number of the "<<std::endl;
    std::cout<<"\t\tfile with which the mix will be. After that, two numbers are"<<std::endl;
    std::cout<<"\t\tentered - the beginning and end of the interval to which the"<<std::endl;
    std::cout<<"\t\tconverter will be applied. If 1 number is entered, then this"<<std::endl;
    std::cout<<"\t\tis a pointer to the beginning of the interval, and the end of"<<std::endl;
    std::cout<<"\t\tthe interval is the end of the file. If no number is entered,"<<std::endl;
    std::cout<<"\t\tthen the beginning and end of the interval is the beginning and"<<std::endl;
    std::cout<<"\t\tend of the file."<<std::endl;
    std::cout<<"\t\tFor the rest of the commands, everything is the same, only the "<<std::endl;
    std::cout<<"\t\tnumber of the secondary file is not entered"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"\tDescription of each converter: "<<std::endl;
    std::cout<<"\t\tmix or insert: accordingly, it mixes or insert the current track with the track entered with the"<<std::endl;
    std::cout<<"\t\tnumber i, in the interval starting from begin time to end time "<<std::endl;
    std::cout<<"\t\tmute: mutes the current rack in the interval from begin time to "<<std::endl;
    std::cout<<"\t\tend time"<<std::endl;
    std::cout<<"\t<output.wav> - output file"<<std::endl;
    std::cout<<"\t<input'i'.wav> - input file with number 'i'"<<std::endl;
}
bool is_header(std::string file_name)
{
    FILE* wav_file = fopen(file_name.c_str(), "r");
    if(wav_file==NULL)
    {
        fclose(wav_file);
        return false;
    }
    std::array<char, 44> buffer;
    for(int32_t i=0;i<44;i++)
    {
        fread(&buffer[i], 1, 1, wav_file);
    }
    fclose(wav_file);
    if(!(buffer[0]=='R' && buffer[1]=='I' && buffer[2]=='F' && buffer[3]=='F'))
    {
        return false;
    }
    if(!(buffer[8]=='W' && buffer[9]=='A' && buffer[10]=='V' && buffer[11]=='E'))
    {
        return false;
    }
    if(!(buffer[12]=='f' && buffer[13]=='m' && buffer[14]=='t'))
    {
        return false;
    }
    if(!(buffer[16]==16))
    {
        return false;
    }
    if(!(buffer[20]==1))
    {
        return false;
    }
    if(!(buffer[22]==1))
    {
        return false;
    }
    if(!(buffer[24]==68 && buffer[25]==-84))
    {
        return false;
    }
    if(!(buffer[34]==16))
    {
        return false;
    }
    return true;
}
bool is_wav(std::string file_name)
{
    if(!is_header(file_name))
    {
        return false;
    }
    if(file_name[file_name.size()-1]=='v' && file_name[file_name.size()-2]=='a' && file_name[file_name.size()-3]=='w')
    {
        return true;
    }
    return false;
}
}

bool parsing_input(int argc, char *argv[], std::string &config_file_name, std::string &output_file_name, std::vector<std::string> &name_of_input_files)
{
    if(argc==1)
    {
        std::cerr << "Error! Something wrong, use key \"-h\", to help" << std::endl;
        return false;
    }
    std::string tmp=argv[1];
    if(argc==2 && tmp=="-h")
    {
        help();
        return false;
    }
    if(tmp!="-c")
    {
        std::cerr<<"Invalid key used! use key \"-h\", to help!"<<std::endl;
        return false;
    }
    config_file_name=argv[2];
    output_file_name=argv[3];
    for(int i=4;i<argc;i++)
    {
        tmp=argv[i];
        if(!is_wav(tmp))
        {
            std::cerr<<"Not wav file! use key \"-h\", to help"<<std::endl;
            return false;
        }
        name_of_input_files.push_back(argv[i]);
    }
    return true; 
}

void factory_store(Factory &factory)
{
    factory.register_creator("mix", std::make_shared<Creator<MixConverter>>());
    factory.register_creator("insert", std::make_shared<Creator<InsertConverter>>());
    factory.register_creator("mute", std::make_shared<Creator<MuteConverter>>());
}