#pragma once

#include <vector>

namespace CropProductionManager::Api
{
    template<typename T, typename TCore>
    class IApi
    {
    protected:

    public:
        virtual std::vector<T> Get() const = 0;
        virtual T Get(const int id) const = 0;
        virtual T Post(const T&) = 0;
        virtual void Put(const T&) = 0;
        virtual void Remove(int id) = 0;
    };
}