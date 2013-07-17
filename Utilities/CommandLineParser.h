/* 
 * File:   CommandLineParser.h
 * Author: jayanga
 *
 * Created on July 12, 2013, 10:36 PM
 */

#pragma once

#include <map>
#include <string>
#include <stdint.h>

class CommandLineParser
{
public:
    virtual ~CommandLineParser();

    static CommandLineParser& GetInstance();

    void Initialize(int32_t count, char** parameters);

    std::string GetParameterValue(std::string parameter_name);

    void SetParameterValue(std::string parameter_name, std::string parameter_value);

private:
    CommandLineParser();
    CommandLineParser(const CommandLineParser& orig);

    static CommandLineParser* _instance;
    std::map<std::string, std::string> _parameters;
};


