#pragma once

#include "json.hpp"
#include <fstream>
#include <iostream>


class IConfiguration
{
    public:
        IConfiguration(const std::string& json) :
        // Initialization of values must be with (), 
        // json is otherwise set inside array [json]
            values(nlohmann::json::parse(json))
        {}
        IConfiguration(std::ifstream& file) :
            values(nlohmann::json::parse(file))
        {}
        IConfiguration(nlohmann::basic_json<> json) :
            values(json)
        {}

        void MergeConfig(const IConfiguration conf) 
        { 
            values.merge_patch(conf.values); 
        }

        auto operator[](const char* key) const { return values[key]; }
        friend std::ostream & operator<<( std::ostream &output, const IConfiguration &c ) 
        { 
            output << c.values;
            return output;            
        }
    private:
        nlohmann::basic_json<> values;
};