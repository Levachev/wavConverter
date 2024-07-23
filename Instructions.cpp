#include "Instructions.h"

namespace
{
void message( )
{
  std::cerr << "Error! Something wrong, use key \"-h\", to help" << std::endl;
}

bool is_converter(const Factory &factory, std::string str)
{
    return factory.can_create(str);
}

bool is_digit(std::string tmp)
{
  if(tmp[0]=='0' && tmp.size()>1)
  {
    return false;
  }
  if(tmp.size()==0)
  {
    return false;
  }
  for ( auto i : tmp )
  {
    if ( !(i >= '0' && i <= '9'))
    {
      return false;
    }
  }
  return true;
}

bool is_nubmer(std::string str)
{
    if(str[0]!='$')
    {
        return false;
    }
    str=str.substr(1);
    return is_digit(str);
}
bool have_comment(std::ifstream &stream, std::string &word)
{
    bool result=true;
    if(word.find('#')!=0)
    {
        word=word.substr(0, word.find('#'));
        result=false;
    }
    else
    {
        word="";
    }
    char x='#';
    while(x!='\n' && !stream.eof())
    {
        stream.get(x);
    }
    return result;
}
}

bool Instructions::get_instructions(std::string config_file_name, const Factory &factory, std::vector<instr> &result)
{
	std::string word; 
	std::ifstream stream;
	
    stream.open(config_file_name, std::ios::in);
    if(!stream.is_open())
    {
    	message();
        return false;
    }
    bool flag_of_read=true;

    stream>>word;
    if(word.find('#')!=std::string::npos)
    {
        have_comment(stream, word);
        std::cout<<word<<std::endl;
    }
    while(!stream.eof())
    {
        if(word=="")
        {
        }
        else if(word=="mute")
        {
            std::string converter_name=word;
            int32_t begin_time=0;
            int32_t end_time=-1;

            stream>>word;
            if(word.find('#')!=std::string::npos)
            {
                have_comment(stream, word);
            }
            if(word=="")
            {
                flag_of_read=false;
            }
            else if(is_digit(word))
            {
                begin_time=atoi(word.c_str());
            }
            else if(is_converter(factory, word))
            {
                flag_of_read=false;
            }
            else
            {
                message();
                return false;
            }

            if(stream.eof())
            {
                begin_time=0;
                flag_of_read=false;
            }
            if(flag_of_read)
            {
                stream>>word;
                if(word.find('#')!=std::string::npos)
                {
                    if(have_comment(stream, word))
                    {
                        stream>>word;
                    }
                }
                
                if(word=="")
                {
                    flag_of_read=false;
                }
                else if(is_digit(word))
                {
                    end_time=atoi(word.c_str());
                }
                else if(is_converter(factory, word))
                {
                    flag_of_read=false;
                }
                else
                {
                    message();
                    return false;
                }
            }
            if(stream.eof())
            {
                end_time=-1;
                flag_of_read=false;
            }
            instr tmp;
            tmp.converter_name=converter_name;
            tmp.number=-1;
            tmp.begin_time=begin_time;
            tmp.end_time=end_time;
            result.push_back(tmp);
        }
        else if(word=="mix" || word=="insert")
        {
        	std::string converter_name=word;
            int32_t begin_time=0;
            int32_t end_time=-1;
            int32_t number=0;

            stream>>word;
            if(word.find('#')!=std::string::npos)
            {
                if(have_comment(stream, word))
                {
                    stream>>word;
                }
            }
            
            if(is_nubmer(word))
            {
                word=word.substr(1);
                number=atoi(word.c_str());
            }
            else
            {
                message();
                return false;
            }

            if(flag_of_read)
            {
                stream>>word;
                if(word.find('#')!=std::string::npos)
                {
                    if(have_comment(stream, word))
                    {
                        stream>>word;
                    }
                }
                if(word=="")
                {
                    flag_of_read=false;
                }
                else if(is_digit(word))
                {
                    begin_time=atoi(word.c_str());
                }
                else if(is_converter(factory, word))
                {
                    flag_of_read=false;
                }
                else
                {
                    message();
                    return false;
                }
            }
            if(stream.eof())
            {
                begin_time=0;
                flag_of_read=false;
            }
            if(flag_of_read)
            {
                stream>>word;
                if(word.find('#')!=std::string::npos)
                {
                    if(have_comment(stream, word))
                    {
                        stream>>word;
                    }
                }
                if(word=="")
                {
                    flag_of_read=false;
                }
                else if(is_digit(word))
                {
                    end_time=atoi(word.c_str());
                }
                else if(is_converter(factory, word))
                {
                    flag_of_read=false;
                }
                else
                {
                    message();
                    return false;
                }
            }
            if(stream.eof())
            {
                end_time=-1;
                flag_of_read=false;
            }
            instr tmp;
            tmp.converter_name=converter_name;
            tmp.number=number;
            tmp.begin_time=begin_time;
            tmp.end_time=end_time;
            result.push_back(tmp);
        }
        else
        {
            message();
            return false;
        }
        if(flag_of_read)
        {
            stream>>word;
            if(word.find('#')!=std::string::npos)
            {
                have_comment(stream, word);
            }
            
        }
        flag_of_read=true;
    }
    return true;
}