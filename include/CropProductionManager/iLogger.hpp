#pragma once

#include <iostream>

namespace CropProductionManager::Log
{
    // TODO: Att field to set at construction, whether to write to file on destruction (for fast threads)
    // or on log call (for long running threads like Program and WebServer)
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