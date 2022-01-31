#pragma once

#include <iostream>

namespace CropProductionManager::Log
{

    class ILogger
    {
    public:
        void LogDebug(const std::string& message)
        {
            log("Debug", message);
        }

    private:
        void log(const std::string& level, const std::string& message)
        {
            std::cout << level << ":" << message << '\n';
        }
    };
}