/* 
 * File:   CommandLineParser.cpp
 * Author: jayanga
 * 
 * Created on July 12, 2013, 10:36 PM
 */

#include <CommandLineParser.h>

CommandLineParser* CommandLineParser::_instance = NULL;

CommandLineParser::CommandLineParser() :
_parameters()
{
}

CommandLineParser::CommandLineParser(const CommandLineParser& orig)
{
}

CommandLineParser::~CommandLineParser()
{
}

CommandLineParser& CommandLineParser::GetInstance()
{
    if (_instance == NULL)
    {
        _instance = new CommandLineParser();
          
        CommandLineParser::_instance->SetParameterValue("--mode", "s");
        CommandLineParser::_instance->SetParameterValue("--count", "1000");
        CommandLineParser::_instance->SetParameterValue("--threads", "1");
    }

    return *_instance;
}

void CommandLineParser::Initialize(int32_t count, char** parameters)
{
    for (int32_t index = 0; index < count; index++)
    {
        std::string param = parameters[index];

        uint32_t pos = param.find("=");
        if (pos != std::string::npos)
        {
            std::string param_name = param.substr(0, pos);
            std::string param_value = param.substr(pos + 1);

            _parameters[param_name] = param_value;

            //std::cout << "ParamName:" << param_name << ", ParamValue:" << param_value << std::endl;
        }
    }
}

std::string CommandLineParser::GetParameterValue(std::string parameter_name)
{
    std::map<std::string, std::string>::iterator ite = _parameters.find(parameter_name);
    if (ite != _parameters.end())
    {
        return ite->second;
    }
    return "";
}

void CommandLineParser::SetParameterValue(std::string parameter_name, std::string parameter_value)
{
    _parameters[parameter_name] = parameter_value;
}