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
        auto operator[](const char* key) { return values[key]; }
    private:
        nlohmann::basic_json<> values;
};